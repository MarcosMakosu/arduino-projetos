#include <Arduino.h>

// ===== Sensor de nível =====
const int PIN_SENSOR = A0;
const int AMOSTRAS   = 10;

// ===== Semáforo =====
const int PIN_VERDE    = D5;
const int PIN_AMARELO  = D1;
const int PIN_VERMELHO = D2;

// ===== Limiares =====
const int LIM_VERDE   = 300;
const int LIM_AMARELO = 500;
const int HISTERESE   = 20;

enum Estado { VERDE, AMARELO, VERMELHO };
Estado estado = VERDE;

// ===== Utilitários =====
void apagarTodos() {
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_AMARELO, LOW);
  digitalWrite(PIN_VERMELHO, LOW);
}

void aplicarEstado(Estado e) {
  digitalWrite(PIN_VERDE,    e == VERDE    ? HIGH : LOW);
  digitalWrite(PIN_AMARELO,  e == AMARELO  ? HIGH : LOW);
  digitalWrite(PIN_VERMELHO, e == VERMELHO ? HIGH : LOW);
}

// Acende cada cor em sequência, com mensagem no serial
void testarSemaforo() {
  Serial.println("=== AUTOTESTE DO SEMAFORO ===");

  apagarTodos();
  delay(300);

  Serial.println("TEST: VERDE (D5)");
  digitalWrite(PIN_VERDE, HIGH);
  delay(700);
  digitalWrite(PIN_VERDE, LOW);
  delay(200);

  Serial.println("TEST: AMARELO (D6)");
  digitalWrite(PIN_AMARELO, HIGH);
  delay(700);
  digitalWrite(PIN_AMARELO, LOW);
  delay(200);

  Serial.println("TEST: VERMELHO (D7)");
  digitalWrite(PIN_VERMELHO, HIGH);
  delay(700);
  digitalWrite(PIN_VERMELHO, LOW);
  delay(200);

  // Pisca todos juntos 3x (confirma que os 3 acendem juntos)
  Serial.println("TEST: TODOS JUNTOS");
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_VERDE, HIGH);
    digitalWrite(PIN_AMARELO, HIGH);
    digitalWrite(PIN_VERMELHO, HIGH);
    delay(150);
    apagarTodos();
    delay(150);
  }

  Serial.println("TEST: FIM");
  Serial.println("Se alguma cor nao acendeu, verifique pino/fio/modulo dessa cor.");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARELO, OUTPUT);
  pinMode(PIN_VERMELHO, OUTPUT);

  // 1) Autoteste antes de qualquer leitura
  testarSemaforo();

  // 2) Entra no estado inicial
  aplicarEstado(VERDE);

  Serial.println("READY");
}

void loop() {
  // Comando 't' via serial para repetir o autoteste
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 't' || c == 'T') {
      testarSemaforo();
      aplicarEstado(estado);  // retorna ao estado atual
      Serial.println("READY");
    }
  }

  // Média das amostras do sensor
  long soma = 0;
  for (int i = 0; i < AMOSTRAS; i++) {
    soma += analogRead(PIN_SENSOR);
    delay(10);
  }
  int raw = soma / AMOSTRAS;

  // Máquina de estados com histerese
  switch (estado) {
    case VERDE:
      if (raw > LIM_VERDE + HISTERESE) estado = AMARELO;
      break;
    case AMARELO:
      if (raw < LIM_VERDE - HISTERESE)   estado = VERDE;
      else if (raw > LIM_AMARELO + HISTERESE) estado = VERMELHO;
      break;
    case VERMELHO:
      if (raw < LIM_AMARELO - HISTERESE) estado = AMARELO;
      break;
  }
  aplicarEstado(estado);

  const char* nome = (estado == VERDE) ? "GREEN"
                   : (estado == AMARELO) ? "YELLOW" : "RED";
  Serial.print("R:");  Serial.print(raw);
  Serial.print(";S:"); Serial.println(nome);

  delay(300);
}