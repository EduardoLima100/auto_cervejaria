int RELE_1 = 8;

void setup() {
  
  // put your setup code here, to run once:
  
  pinMode(RELE_1,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RELE_1, HIGH);
  delay(1000);
  digitalWrite(RELE_1, LOW);
  delay(1000);
}
