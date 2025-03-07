%% meta 1
for i = 1:10
    numero = randi([0,49]);
    name = strcat(num2str(i-1),'_37_',num2str(numero),'.wav');
    [amp,freq] = audioread(name);
    ampMax = max(abs(amp));  
    amp = amp/ampMax;
    silencio = find(amp>ampMax * 0.15,1,'First');
    amp = amp(silencio:end);
    amp(end:48000) = 0;
    tempo = (0:length(amp)-1)/freq;
    figure(1);
    subplot(5,2,i);
    plot(tempo,amp);
    title(i-1);
    xlabel('Tempo (s)');
end
ampMaxima=zeros(10,50);
ampMinima=zeros(10,50);
desvP=zeros(10,50);
razaoAmp=zeros(10,50);
eTotal=zeros(10,50);
eIntervalos=zeros(10,50);
eDiferencas=zeros(10,50);
for i = 1:10
    for j = 1:50
        name = strcat(num2str(i-1),'_37_',num2str(j-1),'.wav');
        [amp,freq] = audioread(name);
        ampMax = max(amp); 
        ampMin = abs(min(amp));  
        amp = amp/ampMax;
        silencio = find(amp>ampMax * 0.15,1,'First');
        amp = amp(silencio:end);
        amp(end:48000) = 0;
        eTotal(i,j) = sum(abs(amp).^2);
        desvP(i,j) = std(amp);
        ampMaxima(i,j) = max(amp);
        ampMinima(i,j)= abs(min(amp)); 
        razaoAmp(i,j) = ampMax/ampMin;
        intervalo = 0.1;
        num_amostras_intervalo = round(intervalo*length(amp));
        for k = 1:10
            amp_int = amp(k:k+1);
            num_intervalos = floor(length(amp)/num_amostras_intervalo);
            eIntervalos(k,j) = sum(amp_int.^2);
        end
        %calcular a energia de tempo em tempo e nao a total; 
        dif_amp = diff(amp); 
        eDiferencas(i, j) = sum(dif_amp.^2);
    end
    figure(2);
    subplot(5,2,i);
    boxplot(eIntervalos','PlotStyle', 'compact');
    title(i-1);
    xlabel('Tempo (s)')

end
figure(3)
subplot(3,2,1)
boxplot(razaoAmp', 'PlotStyle', 'compact')
title('razaoAmp')

subplot(3,1,2)
boxplot(ampMinima','PlotStyle', 'compact')
title('ampMinima')

subplot(3,1,3)
boxplot(eTotal','PlotStyle', 'compact')
title('eTotal')

%% meta 2
SEF = zeros(1,10); 
percentagemSEF = 0.9;
media_espectral = [];

for i = 1:10
    matriz = [];
    
    for j = 1:50
        name = strcat(num2str(i-1),'_37_',num2str(j-1),'.wav');
        [amp, freq] = audioread(name);
        
        % Normalize amplitude
        ampMax = max(amp); 
        ampMin = abs(min(amp));  
        amp = amp / ampMax;
        
        % Remove silence (where amplitude is less than 15% of max)
        silence = find(amp > ampMax * 0.15, 1, 'first');
        amp = amp(silence:end);
        amp(end:48000) = 0;  % Pad to 48000 samples
        
        % Calculate necessary variables
        N = numel(amp);
        Ts = 1 / freq;
        T0 = N * Ts;
        ws = 2 * pi / Ts;
        w0 = 2 * pi / T0;

        % Apply Blackman window
        blackmanWindow = blackman(length(amp));
        ampBlackman = amp .* blackmanWindow;
        X_blackman = fft(ampBlackman);

        % Apply Hamming window
        hammingWindow = hamming(length(amp));
        ampHamming = amp .* hammingWindow;
        X_hamming = fft(ampHamming);

        % Apply Hann window
        hannWindow = hann(length(amp));
        ampHann = amp .* hannWindow;
        X_hann = fft(ampHann);

        % Compute FFT of original signal
        X = fft(amp);
        X = fftshift(X);
        
        % Threshold small values to zero
        X(abs(X) < 0.001) = 0;

        % Determine frequency range
        if mod(N, 2) == 0
            w = -ws/2 : ws/N : ws/2 - ws/N;
            Omega = -pi : 2*pi/N : pi - 2*pi/N;
        else
            w = -ws/2 + ws/N/2 : ws/N : ws/2 - ws/N/2;
            Omega = -pi + pi/N : 2*pi/N : pi - pi/N;
        end
        
        % Calculate variables
        W = w / w0;
        X = abs(X) / N;
        f0 = find(W == 0);
        X(1:f0-1) = [];
        W(1:f0-1) = [];
        
        matriz = [matriz; X'];
    end

    % Compute median and quartiles
    mediana = quantile(matriz, 0.5);
    q1 = quantile(matriz, 0.25);
    q3 = quantile(matriz, 0.75);

    % Maximum spectral peaks (position and amplitude)
    ampMaxima = max(q3);
    lim = find(q3 >= 0.5 * ampMaxima);
    media_espectral = [media_espectral, mean(lim)];

    % Plot median and spectral quartiles
    figure(4);
    subplot(5, 2, i);
    title(i-1)
    hold on
    plot(W, mediana, 'r');
    plot(W, q1, 'y'); 
    plot(W, q3, 'b');
    hold off
    legend({'Mediana', 'Q25', 'Q75'});

    % Spectral Energy Flux (SEF)
    f = freq * (0:(length(amp)/2)) / length(amp);
    somaCumulativa = cumsum(mediana);
    energiaTotal = somaCumulativa(end);
    indiceSEF = find(somaCumulativa >= percentagemSEF * energiaTotal, 1, 'first');
    SEF(i) = f(indiceSEF);
    
    % Plot positions
    figure(5);
    plot(i-1, lim, 'r.')
    title('Posições')
    hold on

    % Plot maximum spectral peaks of third quartile
    figure(6);
    title('Máximos espectrais do terceiro quartil')
    plot(i-1, q3(lim), 'b.')
    hold on
end

% Plot SEF MEDIANA
figure(7)
boxplot(SEF)
title('SEF MEDIANA')

% Plot median spectral mean
figure(8);
plot(media_espectral, 'r.')
title('Média espectral mediana')
hold off

%% meta 3
for i = 1:10
    freq_P=[];
    amp_media = zeros(48000,1);  %criar matriz para media da amplitude
    potencia_media = []; 
    tries = [];
    matriz = [];
    media = [];
    freq_peak = [];
    for j = 1:50
        %calcular e normalizar os valores da amp outra vez
        name = strcat(num2str(i-1),'_37_',num2str(j-1),'.wav');
        [amp,freq] = audioread(name);
        ampMax = max(amp); 
        ampMin = abs(min(amp));  
        amp = amp/ampMax;
        silencio = find(amp>ampMax * 0.15,1,'First');
        amp = amp(silencio:end);
        amp(end:48000) = 0;
        potencia_total = 0;
        Tframe = 0.01; % largura da janela de análise em s
        Toverlap = 0.002; % sobreposiçao das janelas em s
        Nframe = round(Tframe*freq); % número de amostras na janela
        f_frame = -freq/2+freq/(2*Nframe):freq/Nframe:freq/2-freq/(2*Nframe); % Como o numero de elementos é 48000 é par logo é positivo
        Noverlap = round(Toverlap*freq); % número de amostras sobrepostas na janela
        N = numel(amp);
        h = hamming(N); % janela de hamming
        x = amp .* h;
        freq_relev = [];
        nframes = 0; % para guardar
        tframes = [];
        amp_media = amp_media + amp;
        frame_max =0;
        maximo = 0;
        for ii = 1:Nframe-Noverlap:N-Nframe  %cada ii é uma parte das colunas 
            % aplicar a janela (Hamming) ao sinal do tempo - considear o vetor de dados dentro da janela aplicando a janela de Hamming
            x_frame= x(ii:ii+Nframe-1);
            % obter a magnitude da fft do sinal
            m_X_frame= abs(fftshift(fft(x_frame)));
            potencia_frame = sum(m_X_frame.^2) / Nframe;
            potencia_total = potencia_total + potencia_frame;
            % obter o máximo da magnitude do sinal
            m_X_frame_max= max(m_X_frame);
            % encontrar os índices do máximo da magnitude do sinal (podemos procurar os indices de outras formas - ver exercicio anterior)
            
            ind= find(abs(m_X_frame-m_X_frame_max)<0.001); %posições
            freq_todas= f_frame(ind); %frequencias em todas as posições
            freq_pos = freq_todas(freq_todas>0); %todas as freq maiores que 0
            
%ver momentos onde tem maximos, janela media
            if(m_X_frame_max > maximo)
                maximo = m_X_frame_max;
                frame_max =ii;
                freq_P=[freq_P;mean(freq_pos)];
                titulo = (ii-1)/(Nframe-Noverlap);
            end
            media = [media;ind];
        end
        amp_media = amp_media/50;
        potencia_media = [potencia_media;potencia_total / (N / (Nframe - Noverlap))];  %calculo da potencia media

        % Calcular as frequências dos picos de magnitude
        %frequencies = amp_media * freq / N;
        frequencies = mean(media) * freq / N;
        freq_peak = [freq_peak;mean(frequencies)];
    end
    figure(9)
    plot(i-1,freq_P,'.')
    aaaaa= sprintf('Posição %d dos picos de magnitude: %d',i-1, titulo);
    disp(aaaaa)
    title('Posições dos picos de magnitude');
    hold on
end

%% meta4

%variáveis escolhidas na meta 1
razaoAmp1 =razaoAmp;
ampMinima1= ampMinima;
eTotal1=eTotal;

%variáveis escolhidas na meta 2
SEF;
media_espectral;

%variáveis escolhidas na meta 3

%posiçoes dos picos de magnitude:19
%frequencia dos picos de magnitude
count_zero = 0;
count_um = 0;
count_dois = 0;
count_tres = 0;
count_quatro = 0;
count_cinco = 0;
count_seis = 0;
count_sete= 0;
count_oito = 0;
count_nove = 0;


for i = 1:10
    for j = 1:50
        audio = 20;
        name = strcat(num2str(i-1),'_37_',num2str(j-1),'.wav');
        [amp,freq] = audioread(name);
        ampMax = max(amp);
        ampMin = abs(min(amp));
        amp = amp/ampMax;
        silencio = find(amp>ampMax * 0.15,1,'First');
        amp = amp(silencio:end);
        amp(end:48000) = 0;
        ampMax = max(amp);
        ampMin = abs(min(amp));
        eTotal = sum(abs(amp).^2);
        razaoAmp = ampMax/ampMin;

        if razaoAmp < 1.1  && ampMin >0.9 && eTotal > 1100 && media_espectral(i) <270 && SEF(i) >4500
            audio = 0;
        elseif razaoAmp > 1.1 && ampMin > 0.85 && eTotal < 900 && media_espectral(i) >300 && SEF(i) <3000
            audio = 1;
        elseif razaoAmp < 1  && ampMin > 1 && eTotal > 1100 && media_espectral(i) >270 && SEF(i) <9350
            audio = 2; 
        elseif  razaoAmp > 1.16 && ampMin < 0.87 && eTotal <740 && media_espectral(i) <255 && SEF(i) >8000
            audio = 3;
        elseif razaoAmp > 1.17 && ampMin < 0.88 && eTotal > 930 && media_espectral(i) >430 && SEF(i) >6700 
            audio = 4;
        elseif razaoAmp > 1.3 && ampMin < 0.75 && eTotal < 810 && media_espectral(i) >500 && SEF(i) <3700
            audio = 5;
        elseif razaoAmp < 1.15 && ampMin > 0.9  && eTotal < 600 && media_espectral(i) <290 && SEF(i) >9000
            audio = 6;
        elseif razaoAmp > 1.15  && ampMin < 0.9 && eTotal > 995 && media_espectral(i) >350 && SEF(i) >6000
            audio = 7;
        elseif razaoAmp > 1.18 && ampMin < 0.85 && eTotal < 640 && media_espectral(i) <255 && SEF(i) <4000 
            audio = 8;
        elseif razaoAmp > 1.15 && ampMin > 0.85 && eTotal > 1050 && media_espectral(i) <170 && SEF(i) <3200
            audio = 9;
        end
        
        % Count occurrences of each audio value
        switch audio
            case 0
                count_zero = count_zero + 1;
            case 1
                count_um = count_um + 1;
            case 2
                count_dois = count_dois + 1;
            case 3
                count_tres = count_tres + 1;
            case 4
                count_quatro = count_quatro + 1;
            case 5
                count_cinco = count_cinco + 1;
            case 6
                count_seis = count_seis + 1;
            case 7
                count_sete = count_sete + 1;
            case 8
                count_oito = count_oito + 1;
            case 9
                count_nove = count_nove + 1;
        end
    end
end
count_zero
count_um
count_dois
count_tres
count_quatro
count_cinco
count_seis
count_oito
count_nove
percentagem_total= (count_zero + count_um +count_dois +count_tres +count_quatro +count_cinco +count_seis +count_sete +count_oito +count_nove)/500 *100

% Criar vetor de rótulos para diferenciar os dígitos
digit_labels = repmat((0:9)', 50, 1); % Repetir os números de 0 a 9 50 vezes cada
% % Plot 3D com cores diferentes para cada dígito
figure(10);
scatter3(razaoAmp1(:), ampMinima1(:), eTotal1(:), 50, digit_labels);
xlabel('Razão da Amplitude');
ylabel('Amplitude Miníma');
zlabel('Energia Total');
title('Características 3D: Razão da Amplitude, Amplitude Miníma e Energia Total');
colormap(jet(10)); % Escolher uma colormap com 10 cores diferentes
colorbar; % Adicionar barra de cores