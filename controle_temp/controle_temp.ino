#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal.h>

#define ONE_WIRE_BUS 2
#define T_MOST  76   //Temperatura de mostura  //inicial = 67, pós colocação dos grãos = 65, fim da mostura = 76                      
#define T_LAV   76    //Temperatura de lavagem
#define MARG    0.5   //Margem de temperatura
#define AM      1     //Amostras de temperatura para filtro
#define CT      1     //Uso do serial
int pinRele[2] = {11, 13}; //[MOSTURA,LAVAGEM]

// --- Declaração de Objetos ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometers[3] = {{0x28, 0xB0, 0xFE, 0xB1, 0x13, 0x19, 0x01, 0xAD},  // L
                                 {0x28, 0xDE, 0x6D, 0xBB, 0x13, 0x19, 0x01, 0x9F},  // M1
                                 //{0x28, 0x37, 0xE8, 0xB2, 0x13, 0x19, 0x01, 0xAA} // M2
                                 {0x28, 0xDE, 0x6D, 0xBB, 0x13, 0x19, 0x01, 0x9F}   // M1
};

/* ENDEREÇOS DOS TERMÔMETROS
  Encontrado sensor 'DS18B20' com endereco:
  0x28, 0xB0, 0xFE, 0xB1, 0x13, 0x19, 0x01, 0xAD
  Encontrado sensor 'DS18B20' com endereco:
  0x28, 0xDE, 0x6D, 0xBB, 0x13, 0x19, 0x01, 0x9F
  Encontrado sensor 'DS18B20' com endereco:
  0x28, 0x37, 0xE8, 0xB2, 0x13, 0x19, 0x01, 0xAA
*/

// Define as conexões e cria o objeto para acesso ao LCD
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int backLight = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void showTemp(LiquidCrystal lcd, float tempC[3],int err[3]){
  if(err[0]+err[1]+err[2]){lcd.clear();}
  lcd.setCursor(0,0);
  lcd.print("TEMP(");
  lcd.write(223); // Simbolo grau
  lcd.print("C) L ");
  if(err[0] == 0){
    lcd.print(tempC[0],1);
  }
  else{
    lcd.print("ERR");
  }
  
  lcd.setCursor(0,1);
  lcd.print("M1 ");
  if(err[1] == 0){
    lcd.print(tempC[1],1);
  }
  else{
    lcd.print("ERR");
  }

  lcd.print(" M2 ");
  if(err[2] == 0){
    lcd.print(tempC[2],1);
  }
  else{
    lcd.print("ERR");
  }
}

void setup() {
  if(CT){Serial.begin(9600);}
  sensors.begin(); //inicializa sensores
  int i;
  for (i = 0; i<3; i++) {
    sensors.setResolution(Thermometers[i], 12); // configura para resolução de 12 bits
  }

  for(i=0; i<2; i++){
    pinMode(pinRele[i], OUTPUT); // Programa os pinos dos relés como saída
  }

  pinMode(backLight, OUTPUT); //Progama o pino de backlight como saída
  lcd.begin(16, 2); // Inicia o display
  digitalWrite(backLight, HIGH); // Acende o backlight
}

float tempC[3], tempAnt[3], tempList[3][AM]; //temperaturas -> {lavagem,mostura 1 (interna), mostura 2 (superior)}
int i, j, err[3];
float s;

int loop_counter;       //holds the count for every loop pass
long loop_timer_now;    //holds the current millis
long previous_millis = millis();   //holds the previous millis
float loop_time;              //holds difference (loop_timer_now - previous_millis) = total execution time
float loop_test_times = AM;  //Run loop AM times then calculate time

void loop() {
  // solicita as temperaturas para os sensores
  sensors.requestTemperatures();
  for (i = 0; i<3; i++){
    err[i] = 0;
    tempC[i] = sensors.getTempC(Thermometers[i]);
    if(tempC[i]<0){// || ((tempC[i]-10>tempList[i][AM-1] || tempC[i]+10<tempList[i][AM-1]) && tempList[i][0]!= 0)){
      err[i]=1;
      tempC[i] = tempAnt[i];
    }
  }

  //Atualiza os valores da lista de temperaturas e calcula a média móvel dos últimos AM valores
  for(i=0;i<3;i++){
    s = tempC[i];
    for(j=0;j<AM-1;j++){
      tempList[i][j] = tempList[i][j+1];
      s = s + tempList[i][j];
    }
    tempList[i][AM-1] = tempC[i];
    tempC[i] = s/AM;
  }
      
  //Atualiza o LCD e os estados dos relés caso a temperatura mudar
  if(tempAnt != tempC || err[0]+err[1]+err[2]){
    for(i=0;i<3;i++){
      tempAnt[i] = tempC[i];
    }
    showTemp(lcd, tempC,err);
    
    if (tempC[0] > T_LAV + MARG) {
      digitalWrite(pinRele[1], HIGH); //DESLIGAR
    }
    else {
      if (tempC[0] < T_LAV - MARG && tempC[0] != -127) {
        digitalWrite(pinRele[1], LOW); //LIGAR
      }
    }
    if (tempC[1] > T_MOST + MARG || tempC[2] >= T_MOST + MARG * 2) {
      digitalWrite(pinRele[0], HIGH); //DESLIGAR
      //Serial.print("DES MOST");
    }
    if (tempC[1] < T_MOST - MARG && tempC[2] < T_MOST + MARG * 2 && tempC[1] != -127 && tempC[2] != -127) {
      digitalWrite(pinRele[0], LOW); //LIGAR
      //Serial.print("LIG MOST");
    }
  }
  if(CT){
    loop_counter++;
    if (loop_counter >= loop_test_times){ 
      loop_timer_now = millis();
      loop_counter = 0;
      loop_time = float((loop_timer_now - previous_millis) / loop_test_times);
      previous_millis = loop_timer_now;
      Serial.println(loop_time);
    }
  }
}
