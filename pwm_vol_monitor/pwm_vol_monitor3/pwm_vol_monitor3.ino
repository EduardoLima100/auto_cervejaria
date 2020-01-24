
float flowRate;             // Variável para armazenar o valor em L/min
int   pulseCount;           // Variável para a quantidade de pulsos

float liters =      0;      // Variável para Quantidade de agua
float mililiters =  0;      // Variavel para Conversão

int sensorPin = 2;              // Atribuição do pino de entrada do sinal do sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);            // Pino que receberá os pulsos do sensor
  attachInterrupt(0, pulse, RISING);    // Configura o pino 2(Interrupção 0)

}

void loop() {
  // put your main code here, to run repeatedly:
  pulseCount =  0;
  sei();             //Habilita interrupção
  delay (1000);      //Aguarda 1 segundo
  cli();             //Desabilita interrupção

  flowRate = pulseCount / 7.5;    // Converte para L/min MUDADO DE 5.5 PARA 7.5
  mililiters = flowRate / 60;     // Calculo de mililitros
  liters = liters + mililiters;   // Calculo de quantos litros passou pelo sensor

  if(flowRate!=0){
    Serial.println(liters);
  }
}

void pulse ()
{
  pulseCount++; //Incrementa a variável de pulsos
}
