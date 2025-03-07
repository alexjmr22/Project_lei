const int buttonPin = 9;                // Pino onde o botão está conectado
const int blinkDelay = 250;              // Tempo de cada pisca
const unsigned long delayLED = 500;      // Tempo que cada LED fica ligado
const unsigned long debounceDelay = 50;  // Delay para debouncing
bool gameStarted = false;                // Estado do jogo
unsigned long pressStartTime = 0;        // Tempo de início da pressão
unsigned long ledStartTime = 0;          // Armazena o tempo de início do LED aceso
int activeLED = -1;                      // LED atualmente aceso
unsigned long lastButtonPressTime = 0;   // Para controle de debounce do botão

bool led2 = false;
bool led3 = false;
bool led4 = false;
bool led5 = false;
bool led6 = false;
bool led7 = false;
bool led8 = false;

// Verificadores: se acertou fica true
bool acert2 = false;
bool acert3 = false;
bool acert4 = false;
bool acert5 = false;
bool acert6 = false;
bool acert7 = false;
bool acert8 = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  for (int i = 2; i < 9; i++) {  // Apagar todos os LEDs inicialmente
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  randomSeed(analogRead(0));  // Gera números aleatórios
}

void loop() {
  if (primir2sec()) {
    if (!gameStarted) {
      startGame();  // Inicia o jogo se ainda não começou
    } else {
      resetGame();  // Reinicia o jogo se já está em andamento
      return;
    }
  }
  if (gameStarted) {
    ronda();
  }
}

// Iniciar o jogo
void startGame() {
  blinkAllLEDs(3);  // Piscar todos os LEDs 3 vezes
  gameStarted = true;
  resetLEDs();
  selectRandomLED();
}

// Reiniciar o jogo
void resetGame() {
  resetLEDs();
  blinkAllLEDs(2);  // Piscar todos os LEDs 2 vezes
  gameStarted = false;
}

// Verifica se botão foi pressionado por 2 segundos
bool primir2sec() {
  if (millis() - lastButtonPressTime < debounceDelay) {
    return false;
  }
  if (digitalRead(buttonPin) == LOW) {
    if (pressStartTime == 0) {
      pressStartTime = millis();
    } else if (millis() - pressStartTime >= 2000) {
      lastButtonPressTime = millis();
      pressStartTime = 0;
      return true;
    }
  } else {
    pressStartTime = 0;
  }

  return false;
}

// Função para piscar todos os LEDs uma quantidade de vezes especificada
void blinkAllLEDs(int times) {
  for (int i = 0; i < times; i++) {
    toggleAllLEDs(HIGH);
    delay(blinkDelay);
    toggleAllLEDs(LOW);
    delay(blinkDelay);
  }
}

// Função para alternar o estado de todos os LEDs
void toggleAllLEDs(int state) {
  for (int i = 2; i < 9; i++) {
    digitalWrite(i, state);
  }
}

void resetLEDs() {
  // Desliga todos os LEDs
  for (int i = 2; i < 9; i++) {
    digitalWrite(i, LOW);
  }

  led2 =false;
  led3 =false;
  led4 =false;
  led5=false;
  led6=false;
  led7=false;
  led8 = false;


  acert2 =false;
  acert3=false;
  acert4 =false;
  acert5 =false;
  acert6=false;
  acert7 =false;
  acert8 = false;
  activeLED = -1;
}

void selectRandomLED() {
  activeLED = random(2, 9);  // Seleciona um LED aleatório entre 2 e 8 pinos onde fiz as ligacoes
  switch (activeLED) {
    case 2: led2 = true; break;
    case 3: led3 = true; break;
    case 4: led4 = true; break;
    case 5: led5 = true; break;
    case 6: led6 = true; break;
    case 7: led7 = true; break;
    case 8: led8 = true; break;
  }

  digitalWrite(activeLED, HIGH);
  ledStartTime = millis();
}

void ronda() {
  if (millis() - ledStartTime >= delayLED) {
    apagarAtivo();

    if (checkVictory()) {//verifica se ganhou caso n ganhe seleciona outro led
      blinkAllLEDs(3);
      resetGame();
    } else {
      selectRandomLED();
    }
  } else if (digitalRead(buttonPin) == LOW) {
    if (millis() - lastButtonPressTime >= debounceDelay) {//verifica se foi um click rapido
      meteAtivo();
      lastButtonPressTime = millis();
    }
  }
}
//apaga o LED ativo se não tiver sido acertado

void apagarAtivo() {
  if (!acertou(activeLED)) {
    digitalWrite(activeLED, LOW);
  }
  switch (activeLED) {
    case 2: led2 = false; break;
    case 3: led3 = false; break;
    case 4: led4 = false; break;
    case 5: led5 = false; break;
    case 6: led6 = false; break;
    case 7: led7 = false; break;
    case 8: led8 = false; break;
  }
}

void meteAtivo() {//mete os ledes ativos (index chamadou na outra func)
  switch (activeLED) {
    case 2: acert2 = true; break;
    case 3: acert3 = true; break;
    case 4: acert4 = true; break;
    case 5: acert5 = true; break;
    case 6: acert6 = true; break;
    case 7: acert7 = true; break;
    case 8: acert8 = true; break;
  }
}

bool acertou(int led) {// verifica se acertou
  switch (led) {
    case 2: return acert2;
    case 3: return acert3;
    case 4: return acert4;
    case 5: return acert5;
    case 6: return acert6;
    case 7: return acert7;
    case 8: return acert8;
  }
  return false;
}

bool checkVictory() {

  return (acert2 && acert3 && acert4 && acert5 && acert6 && acert7 && acert8);
}
