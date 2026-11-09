#define BOTAO_A D1

void setup() {
  Serial.begin(9600);
  pinMode(BOTAO_A, INPUT_PULLUP);
}

void loop() {
  Serial.println(digitalRead(BOTAO_A));
  delay(200);
}