
float volts, fonte;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  volts = analogRead(0)*(5/1023.0);
  fonte = analogRead(1);
  
  Serial.println(volts);
  Serial.println(fonte);
  Serial.println(analogRead(0));
  delay(1000);
  
}
