import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

/**
 * A classe Jogador representa um jogador no jogo.
 * Cada jogador tem um nome, uma pontuação, data e hora de criação,
 * perguntas acertadas e perguntas erradas.
 */
class Jogador implements Serializable {

    /**
     * O nome do jogador.
     */
    protected String nome;

    /**
     * A pontuação do jogador.
     */
    protected int pontos;

    /**
     * A data e hora de criação do jogador.
     */
    protected Date dataHora;

    /**
     * Lista de perguntas acertadas pelo jogador.
     */
    protected List<Pergunta> perguntasAcertadas;

    /**
     * Lista de perguntas erradas pelo jogador.
     */
    protected List<Pergunta> perguntasErradas;

    /**
     * Construtor da classe Jogador.
     *
     * @param nome O nome do jogador.
     */
    public Jogador(String nome) {
        this.nome = nome;
        this.pontos = 0;
        this.dataHora = obterDataHoraAtual();
        this.perguntasAcertadas = new ArrayList<>();
        this.perguntasErradas = new ArrayList<>();
    }

    /**
     * Obtém a pontuação atual do jogador.
     *
     * @return A pontuação do jogador.
     */
    public int obterPontos() {
        return pontos;
    }

    /**
     * Obtém a data e hora atuais.
     *
     * @return A data e hora atuais.
     */
    private Date obterDataHoraAtual() {
        return new Date();
    }

    /**
     * Obtém a data e hora formatada como string.
     *
     * @return A data e hora formatada.
     */
    public String obterDataHoraFormatada() {
        SimpleDateFormat formato = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
        return formato.format(dataHora);
    }

    /**
     * Obtém o nome do jogador.
     *
     * @return O nome do jogador.
     */
    public String obterNome() {
        return nome;
    }

    /**
     * Atualiza a pontuação e adiciona uma pergunta acertada à lista.
     *
     * @param pergunta A pergunta acertada.
     */
    public void acertouPergunta(Pergunta pergunta) {
        pontos += pergunta.pontos;
        perguntasAcertadas.add(pergunta);
    }

    /**
     * Adiciona uma pergunta errada à lista.
     *
     * @param pergunta A pergunta errada.
     */
    public void errouPergunta(Pergunta pergunta) {
        perguntasErradas.add(pergunta);
    }

    /**
     * Guarda o estado atual do jogo do jogador em um arquivo.
     */
    public void guardarEstadoJogo() {
        SimpleDateFormat formatoDataHora = new SimpleDateFormat("yyyyMMddHHmm");
        String dataHoraFormatada = formatoDataHora.format(dataHora);

        String nomeArquivo = "pootrivia_jogo_" + dataHoraFormatada + "_" + obterIniciaisNome() + ".dat";

        try (ObjectOutputStream outputStream = new ObjectOutputStream(new FileOutputStream(nomeArquivo))) {
            // Salvando o objeto Jogador no arquivo
            outputStream.writeObject(this);
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Arquivo salvo em: " + new File(nomeArquivo).getAbsolutePath());
    }

    private String obterIniciaisNome() {
        // Obtém as iniciais do nome (máximo de 3 caracteres)
        StringBuilder iniciais = new StringBuilder();
        String[] partesNome = nome.split(" ");
        for (String parte : partesNome) {
            if (parte.length() > 0) {
                iniciais.append(parte.charAt(0));
            }
        }
        return iniciais.toString().toUpperCase();
    }

    /**
     * Carrega o estado do jogo de um jogador a partir de um arquivo.
     *
     * @param nomeJogador O nome do jogador.
     * @return O objeto Jogador carregado ou null se falhar.
     */
    public static Jogador carregarEstadoJogo(String nomeJogador) {
        ObjectInputStream inputStream = null;
        try {
            inputStream = new ObjectInputStream(new FileInputStream(nomeJogador));
            // Carregando o objeto Jogador do arquivo
            Jogador jogador = (Jogador) inputStream.readObject();
            return jogador;
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            return null;
        } finally {
            if (inputStream != null) {
                try {
                    inputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * Imprime as informações do jogador, incluindo nome, pontuação, data e hora de criação,
     * perguntas acertadas e perguntas erradas.
     */
    public void imprimirInformacoes() {
        System.out.println("Informações do Jogador:");
        System.out.println("Nome: " + nome);
        System.out.println("Pontuação: " + pontos);
        System.out.println("Data e Hora de Criação: " + obterDataHoraFormatada());
        System.out.println("Perguntas Acertadas:");
        imprimirListaPerguntas(perguntasAcertadas);
        System.out.println("Perguntas Erradas:");
        imprimirListaPerguntas(perguntasErradas);
    }

    private void imprimirListaPerguntas(List<Pergunta> perguntas) {
        for (Pergunta pergunta : perguntas) {
            System.out.println("- " + pergunta.pergunta);
        }
    }

    /**
     * Carrega todos os estados de jogo existentes e retorna uma lista de jogadores.
     *
     * @return A lista de jogadores.
     */
    public static List<Jogador> carregarTodosEstadosJogo() {
        List<Jogador> jogadores = new ArrayList<>();
    
        File diretorio = new File(".");
        File[] arquivos = diretorio.listFiles((dir, nome) -> nome.startsWith("pootrivia_jogo_") && nome.endsWith(".dat"));
    
        if (arquivos != null) {
            for (File arquivo : arquivos) {
                Jogador jogador = carregarEstadoJogo(arquivo.getName());
                if (jogador != null) {
                    jogadores.add(jogador);
                }
            }
        }
    
        return jogadores;
    }

    /**
     * Calcula o top 3 jogadores com base na pontuação.
     *
     * @param jogadores A lista de jogadores.
     * @return O top 3 jogadores.
     */
    public static List<Jogador> calcularTop3(List<Jogador> jogadores) {
        Collections.sort(jogadores, Comparator.comparing(Jogador::obterPontos).reversed());

        return jogadores.subList(0, Math.min(3, jogadores.size()));
    }
}
