#define VERDE D5
#define AMARELO D6
#define VERMELHO D7

void setup() {
  pinMode(VERDE, OUTPUT);
  pinMode(AMARELO, OUTPUT);
  pinMode(VERMELHO, OUTPUT);
}

void loop() {
  digitalWrite(VERDE, HIGH);
  digitalWrite(AMARELO, LOW);
  digitalWrite(VERMELHO, LOW);
  delay(5000);

  digitalWrite(VERDE, LOW);
  digitalWrite(AMARELO, HIGH);
  delay(2000);

  digitalWrite(AMARELO, LOW);
  digitalWrite(VERMELHO, HIGH);
  delay(5000);
}