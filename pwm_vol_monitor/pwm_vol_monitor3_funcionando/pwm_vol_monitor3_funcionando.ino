
float flowRate;             // Variável para armazenar o valor em L/min
int   pulseCount;           // Variável para a quantidade de pulsos

float liters =      0;      // Variável para Quantidade de agua
float mililiters =  0;      // Variavel para Conversão

int sensorPin = 2;              // Atribuição do pino de entrada do sinal do sensor

void pulse();

// Variaveis para controle
unsigned long int TempoCompensado = 0;
unsigned long int Tempo_1         = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);            // Pino que receberá os pulsos do sensor
  attachInterrupt(0, pulse, RISING);    // Configura o pino 2(Interrupção 0)
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  pulseCount =  0;
  //sei();             //Habilita interrupção
  delay (1000 - TempoCompensado);       // Aguarda 1 segundo - tempo de looping
  //cli();             //Desabilita interrupção

  // Variavel de sincronia 
  Tempo_1 = millis();
  
  flowRate = (pulseCount*10 / 7.5)/9 ;    // Converte o número de pulsos para L/min MUDADO DE 5.5 PARA 7.5, adicionado uma correção de 10/9 por empirismo
  mililiters = flowRate / 60;     // Calculo de mililitros
  liters = liters + mililiters;   // Calculo de quantos litros passou pelo sensor

  if(flowRate!=0){
    Serial.print("\n\nVolume: ");
    Serial.print(liters);
    Serial.print("L");
    Serial.print("\nVazão: ");
    Serial.print(flowRate);
    Serial.print("L/min\n");
  }
  TempoCompensado = millis() - Tempo_1;
  Serial.println(TempoCompensado);
}

void pulse ()
{
  pulseCount++; //Incrementa a variável de pulsos
}
