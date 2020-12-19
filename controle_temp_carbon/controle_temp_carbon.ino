#include <DallasTemperature.h>
#include <OneWire.h>
//#include <math.h>

#define ONE_WIRE_BUS 2
#define T_CARB 21     //Temperatura de carbonatação
#define MARG 1      //Margem de temperatura
#define SERIAL 0    //Uso do monitor serial

int pinRele[2]= {8,6}; //{ISOPOR_0,ISOPOR_1} 0 - de cima(termômetro L) 1 - de baixo (termômetro M1)

// --- Declaração de Objetos ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometers[3] = {{0x28, 0xB0, 0xFE, 0xB1, 0x13, 0x19, 0x01, 0xAD},
                                 {0x28, 0xDE, 0x6D, 0xBB, 0x13, 0x19, 0x01, 0x9F}};

/* ENDEREÇOS DOS TERMÔMETROS
  Encontrado sensor 'DS18B20' com endereco:
  
  0x28, 0xB0, 0xFE, 0xB1, 0x13, 0x19, 0x01, 0xAD
  
  Encontrado sensor 'DS18B20' com endereco:
  
  0x28, 0xDE, 0x6D, 0xBB, 0x13, 0x19, 0x01, 0x9F
  
  Encontrado sensor 'DS18B20' com endereco:
  
  0x28, 0x37, 0xE8, 0xB2, 0x13, 0x19, 0x01, 0xAA 
*/

void setup() {
  // put your setup code here, to run once:
  if(SERIAL){
    Serial.begin(9600);   //inicializa comunicação serial
  }
  sensors.begin();      //inicializa sensores
  int i;
  for(i=0;i<3;i++){
    sensors.setResolution(Thermometers[i], 12); //configura para resolução de 12 bits
  }
  /*for(i=0;i<2;i++){
    pinMode(pinRele[i],OUTPUT);
  }*/
  pinMode(pinRele[0],OUTPUT);
  pinMode(pinRele[1],OUTPUT);  
}

float tempC[2]; //temperaturas -> {isopor_1,isopor_2}
int i;
void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  if(SERIAL){
    Serial.print("\n\r");
  }
  for(i=0;i<2;i++){
    tempC[i] = sensors.getTempC(Thermometers[i]);
    if(SERIAL){
      Serial.print("\nISOPOR ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(tempC[i]);
      Serial.print("ºC ");
    }
    if(tempC[i]>T_CARB+MARG){
      digitalWrite(pinRele[i],HIGH); //DESLIGAR
    }
    else{
      if(tempC[i]<T_CARB-MARG){
        digitalWrite(pinRele[i],LOW); //LIGAR
       }
    }
  }
  delay(1000);
}
