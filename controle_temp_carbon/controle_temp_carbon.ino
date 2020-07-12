#include <DallasTemperature.h>
#include <OneWire.h>
//#include <math.h>

#define ONE_WIRE_BUS 2
#define T_CARB 21     //Temperatura de carbonatação
#define MARG 1      //Margem de temperatura

int pinRele[2]= {7,8}; //{ISOPOR_1,ISOPOR_2}

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
  //Serial.begin(9600);   //inicializa comunicação serial
  
  sensors.begin();      //inicializa sensores
  int i;
  for(i=0;i<3;i++){
    sensors.setResolution(Thermometers[i], 12); //configura para resolução de 12 bits
  }
  /*for(i=0;i<2;i++){
    pinMode(pinRele[i],OUTPUT);
  }*/
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);  
}

float tempC[2]; //temperaturas -> {isopor_1,isopor_2}
int i;
void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  //Serial.print("\n\r");
  for(i=0;i<2;i++){
    tempC[i] = sensors.getTempC(Thermometers[i]);
    /*Serial.print("\nISOPOR ");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(tempC[i]);
    Serial.print("ºC ");*/
    if(tempC[i]>T_CARB+MARG){
      digitalWrite(7+i,HIGH); //DESLIGAR
    }
    else{
      if(tempC[i]<T_CARB-MARG){
        digitalWrite(7+i,LOW); //LIGAR
       }
    }
  }
  delay(1000);
}
