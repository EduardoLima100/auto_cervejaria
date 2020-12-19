int X;
int Y;
float TIME = 0;
float FREQUENCY = 0;
float WATER = 0;
float TOTAL = 0;
float LS = 0;
const int input = A0;
void setup()
{
Serial.begin(9600);
pinMode(input,INPUT);
}
void loop(){
  X = pulseIn(input, HIGH);
  Y = pulseIn(input, LOW);
  TIME = X + Y;
  FREQUENCY = 1000000/TIME;
  WATER = FREQUENCY/7.5;
  LS = WATER/60;
  
  if(isinf(FREQUENCY)){}
  else{
    TOTAL = TOTAL + LS;
    Serial.print("\nVolume: ");
    Serial.print(TOTAL);
    Serial.print(" Litros");
    Serial.print("\nVazão:");
    Serial.print(LS*60);
    Serial.print(" L/min");
  }
  delay(1000);
}
