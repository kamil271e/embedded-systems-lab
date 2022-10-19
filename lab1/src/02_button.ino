int btn = HIGH;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  digitalWrite(5, LOW);
}

void loop() {
  btn = digitalRead(3);
  if (btn == LOW){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5,LOW);
  }
}
