// Pinos dos botões
const int buttonAnd = 4;
const int buttonOr = 3;
const int buttonXor = 2;

// Pinos dos LEDs
const int leds[] = {8, 9, 10, 11};

// Variáveis de tempo e jogo
unsigned long timeLimit = 20000; // Tempo limite (10 segundos)
unsigned long startTime;
bool gameRunning = false;
bool timeRunning = false;

// Valores do jogo
int targetValue, currentValue, playerInput = -1;
bool andAvailable, xorAvailable;

// Controle do debounce
int buttonState[3] = {HIGH, HIGH, HIGH};
int lastButtonState[3] = {HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[3] = {0, 0, 0};
const int debounceDelay = 50; // 50ms para debounce
//restart 
unsigned long orButtonPressTime = 0; // Tempo inicial de pressionamento do botão OR
const unsigned long longPressDuration = 2000; // Duração mínima para reiniciar (2 segundos)

void setup() {
  Serial.begin(9600);

  // Configuração dos botões
  pinMode(buttonAnd, INPUT_PULLUP);
  pinMode(buttonOr, INPUT_PULLUP);
  pinMode(buttonXor, INPUT_PULLUP);

  // Configuração dos LEDs
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], HIGH); // Todos os LEDs começam ligados
  }

  Serial.println("Jogo a comecar");
  resetGame();
}

void loop() {
  if (!gameRunning) return;

  // Verificar se o botão OR foi pressionado por 2 segundos
  checkLongPress();

  // Atualizar LEDs com o tempo restante
  if (timeRunning) {
    unsigned long elapsedTime = millis() - startTime;
    updateLeds(elapsedTime);

    if (elapsedTime > timeLimit) {
      Serial.println("Acabou o tempo. Perdeste!!!");
      resetGame();
      return;
    }
  }

  // Ler entrada via Serial
  if (Serial.available() > 0) {
    playerInput = Serial.parseInt();
    if (playerInput >= 0 && playerInput <= 255) {
      Serial.print("Entrada recebida (decimal): ");
      Serial.println(playerInput);
      Serial.print("Entrada recebida (binario): ");
      Serial.println(playerInput, BIN);

      showAvailableOperations();
    } else {
      Serial.println("Numero invalido. Insira um numero entre 0 e 255.");
      playerInput = -1;
    }
  }

  // Verificar botões pressionados
  if (playerInput != -1) {
    if (debouncedRead(buttonOr, 0)) {
      Serial.println("Botao OR pressionado.");
      applyOperation('|');
    } else if (andAvailable && debouncedRead(buttonAnd, 1)) {
      Serial.println("Botao AND pressionado.");
      applyOperation('&');
    } else if (xorAvailable && debouncedRead(buttonXor, 2)) {
      Serial.println("Botao XOR pressionado.");
      applyOperation('^');
    }
  }

  // Verificar vitória
  if (currentValue == targetValue) {
    Serial.println("Parabens! Valor alvo alcancado");
    resetGame();
  }
}
void resetGame() {
  targetValue = random(256);
  currentValue = random(256);
  andAvailable = targetValue & 0x02;
  xorAvailable = !(targetValue & 0x02);

  Serial.println("\n=== NOVA RODADA ===");
  Serial.print("Valor alvo (binario): ");
  Serial.println(targetValue, BIN);
  Serial.print("Valor inicial (binario): ");
  Serial.println(currentValue, BIN);

  startTime = millis();
  gameRunning = true;
  timeRunning = true;

  // Todos os LEDs começam ligados
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
  }
}

void checkLongPress() {
  int orButtonState = digitalRead(buttonOr);

  if (orButtonState == LOW) { // Botão OR pressionado
    if (orButtonPressTime == 0) {
      orButtonPressTime = millis(); // Registrar o momento em que o botão foi pressionado
    } else if (millis() - orButtonPressTime >= longPressDuration) {
      Serial.println("Botao OR pressionado por 2 segundos. Reiniciando o jogo...");
      resetGame();
      orButtonPressTime = 0; // Resetar o tempo de pressão
    }
  } else {
    orButtonPressTime = 0; // Resetar se o botão foi solto antes do tempo
  }
}


void updateLeds(unsigned long elapsedTime) {
  // Dividimos o tempo limite em 4 segmentos de 2,5 segundos
  int segments = timeLimit / 4;

  for (int i = 0; i < 4; i++) {
    // Desligar cada LED gradualmente
    if (elapsedTime > segments * (i + 1)) {
      digitalWrite(leds[i], LOW); // Desliga o LED após o tempo do segmento
    }
  }
}

void applyOperation(char op) {
  Serial.print("\nOperacao aplicada: ");
  Serial.println(op == '|' ? "OR" : op == '&' ? "AND" : "XOR");

  Serial.print("Valor atual (binario): ");
  Serial.println(currentValue, BIN);

  switch (op) {
    case '|':
      currentValue |= playerInput;
      break;
    case '&':
      currentValue &= playerInput;
      break;
    case '^':
      currentValue ^= playerInput;
      break;
  }

  Serial.print("Valor atual apos operacao (decimal): ");
  Serial.println(currentValue);
  Serial.print("Valor atual apos operacao (binario): ");
  Serial.println(currentValue, BIN);
  Serial.print("Valor alvo (binario): ");
  Serial.println(targetValue, BIN);
}

bool debouncedRead(int buttonPin, int buttonIndex) {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState[buttonIndex]) {
    lastDebounceTime[buttonIndex] = millis();
  }

  if ((millis() - lastDebounceTime[buttonIndex]) > debounceDelay) {
    if (reading != buttonState[buttonIndex]) {
      buttonState[buttonIndex] = reading;

      if (buttonState[buttonIndex] == LOW) {
        return true; // Botão pressionado
      }
    }
  }

  lastButtonState[buttonIndex] = reading;
  return false;
}

void showAvailableOperations() {
  Serial.print("Operacoes disponiveis: ");
  Serial.print("OR (azul) ");
  if (andAvailable) Serial.print("AND (laranja) ");
  if (xorAvailable) Serial.print("XOR (amarelo) ");
  Serial.println();
}
