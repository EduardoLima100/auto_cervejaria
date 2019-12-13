#include <DallasTemperature.h>

/* ========================================================================================================
 
   Curso de Arduino e AVR 149
   
   WR Kits Channel


   Sensor de Temperatura DS18B20 Dallas

    
   Autor: Eng. Wagner Rambo  Data: Dezembro de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits

   HARDWARE Termômetro olhando-o de frente:

   Terminal da direita   -> 5V do Arduino
   Terminal da esquerda  -> GND do Arduino
   Terminal central      -> digital 10 do Arduino (ligar um resistor de 4,7k entre esta entrada e os 5V)
   
   
======================================================================================================== */


// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <OneWire.h>
#include <DallasTemperature.h>


// ========================================================================================================
// --- Constantes Auxiliares ---
#define ONE_WIRE_BUS 10


// ========================================================================================================
// --- Declaração de Objetos ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer = { 0x28, 0x37, 0xE8, 0xB2, 0x13, 0x19, 0x01, 0xAA };
DeviceAddress outsideThermometer = { 0x28, 0xB0, 0xFE, 0xB1, 0x13, 0x19, 0x01, 0xAD };


// ========================================================================================================
// --- Protótipo das Funções ---
void printTemperature(DeviceAddress deviceAddress);


// ========================================================================================================
// --- Configurações Iniciais ---
void setup()
{
 
  Serial.begin(9600);   //inicializa comunicação serial
  int resolution;
  resolution = 12;
  sensors.begin();      //inicializa sensores
 
  sensors.setResolution(insideThermometer, resolution); //configura para resolução de 10 bits
  sensors.setResolution(outsideThermometer, resolution);
 
} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop(void)
{ 
  
  Serial.println("Sensor DS18B20 inside");
  sensors.requestTemperatures();
  printTemperature(insideThermometer);
  Serial.println("Sensor DS18B20 outside");
  printTemperature(outsideThermometer);
  
  delay(1000);
  
} //end loop


// ========================================================================================================
// --- Desenvolvimento das Funções ---
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) 
  {
    Serial.print("Erro de leitura");
  } 
  else 
  {
    Serial.print(tempC);
    Serial.print(" graus C ");
    //Serial.print("  |  ");
    //Serial.print(DallasTemperature::toFahrenheit(tempC));
    //Serial.print(" graus F ");
  }

  Serial.print("\n\r");
  
} //end printTemperature
