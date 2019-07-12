int sinal = 3;
int LED1 = 4;
int LED2 = 5;
int val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sinal, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  val = digitalRead(sinal);
  Serial.print(val);

  if (val==0) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  } else {
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
  }

  delay(1000);
}
