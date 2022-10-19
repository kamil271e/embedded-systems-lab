void setup() {
  pinMode(5, OUTPUT);
  pinMode(A3, INPUT);
  digitalWrite(5, LOW);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A3));
  if (analogRead(A3) >= 600){
    digitalWrite(5, HIGH);
  } else{
    digitalWrite(5,LOW);
  }
}
