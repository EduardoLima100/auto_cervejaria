#include <DallasTemperature.h>
#include <OneWire.h>
//#include <math.h>

#define ONE_WIRE_BUS 2
#define T_MOST 76     //Temperatura de mostura
#define T_LAV 76       //Temperatura de lavagem
#define MARG 0.5      //Margem de temperatura

int pinRele[2]= {7,8}; //{MOSTURA,LAVAGEM}

// --- Declaração de Objetos ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometers[3] = {{0x28, 0xB0, 0xFE, 0xB1, 0x13, 0x19, 0x01, 0xAD},
                                 {0x28, 0xDE, 0x6D, 0xBB, 0x13, 0x19, 0x01, 0x9F},
                                 {0x28, 0x37, 0xE8, 0xB2, 0x13, 0x19, 0x01, 0xAA}};

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
  Serial.begin(9600);   //inicializa comunicação serial
  
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

float tempC[3];
int i;
void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  
  for(i=0;i<3;i++){
    tempC[i] = sensors.getTempC(Thermometers[i]);
    if(i==0){
      Serial.print("\nLAVAGEM ");
    }
    else{
      Serial.print("MOSTURA");
      Serial.print(i);
      Serial.print(" ");
    }
    Serial.print(tempC[i]);
    Serial.print("ºC ");
    Serial.print("\n\r");
  }
  
  if(tempC[0]>T_LAV+MARG){
    digitalWrite(8,HIGH); //DESLIGAR
  }
  else{
    if(tempC[0]<T_LAV-MARG){
    digitalWrite(8,LOW); //LIGAR
    }
  }
  
  if(tempC[1]>T_MOST+MARG || tempC[2]>T_MOST+MARG*2){
    digitalWrite(7,HIGH); //DESLIGAR
    //Serial.print("DES MOST");
  }
  if(tempC[1]<T_MOST-MARG && tempC[2]<T_MOST+MARG*2){
    digitalWrite(7,LOW); //LIGAR
    //Serial.print("LIG MOST");
  }
  
  delay(1000);
  
}
