/***************************************************************************
         Leitura de um sensor de vazão e indicação em um display
                 do Nókia 5110, controlador PCD8544
****************************************************************************
  -- IDE do Arduino Versão 1.8.3
  -- Autor do codigo base do display: Limor Fried/Ladyada, Adafruit Industries.
         https://github.com/adafruit/Adafruit-GFX-Library
         https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
  -- Modificação e implementação da leitura e indicação: Eduardo Avelar
  -- Blog: easytromlabs.com
  -- email: contato@easytromlabs.com

  -- Agostp, 2017
****************************************************************************
                      Conexões entre Hardwares:
             Graphic LCD Pin --------------- Arduino Pin
                 1-RST       ----------------  3
                 2-CE        ----------------  4
                 3-DC        ----------------  5
                 4-DIN(MOSI) ----------------  6
                 5-CLK       ----------------  7
                 6-VCC       ----------------  3,3V
                 7-LED       ----------------  11
                 8-GND       ----------------  GND

****************************************************************************/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

                                     //CLK, DIN, DC, CE, RST//
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

float flowRate;             // Variável para armazenar o valor em L/min
int   pulseCount;           // Variável para a quantidade de pulsos

float liters =      0;      // Variável para Quantidade de agua
float mililiters =  0;      // Variavel para Conversão
int   page = 1;             // variável que determina as páginas

volatile boolean select = false;

int selectButtonState = 0;      // Variável de estado do botao
int lastSelectButtonState = 0;  // Variável do ultimo estado do botao
int selectButtun = 8;           // Atribuição do pino do botao 
int backled = 11;               // Atribuição do pino do backlight
int sensorPin = 2;              // Atribuição do pino de entrada do sinal do sensor

// Imagem de uma pequena torneira 
static const unsigned char PROGMEM tap_logo[] = { 
  B00000000,  B00000000,  B00000001,  B10001000,  B00000001,  B10010100,
  B00000010,  B01100100,  B00000010,  B00001000,  B00000110,  B01110000,
  B00000011,  B11000000,  B00111100,  B00100000,  B00000000,  B00010000,
  B00000000,  B00001000,  B00000000,  B00001000,  B00111110,  B00001000,
  B00000001,  B10001000,  B00000000,  B01110000,  B00000000,  B00000000,
  B00000000,  B00000000 };

void setup() {

  pinMode(selectButtun, INPUT_PULLUP);  // Pino da entrada digital do botao
  pinMode(backled, OUTPUT);             // Pino de controle do BackLight
  pinMode(sensorPin, INPUT);            // Pino que receberá os pulsos do sensor
  attachInterrupt(0, pulse, RISING);    // Configura o pino 2(Interrupção 0)
  digitalWrite(backled, LOW);           // Ativa o Led do backlight
  display.begin();                      // Inicializa o controlador do display
  display.setContrast(50);              // Configura contraste para o valor de 55
  display.clearDisplay();               // Apaga a tela do display
  display.display();                    // Manda atalizar o display
}

void loop() {

  selectButtonState = digitalRead(selectButtun);  // Faz leitura do estado do botao
  checkIfSelectButtonIsPressed();                 // Confere por Pulling o estado do botão

  pulseCount =  0;
  sei();             //Habilita interrupção
  delay (1000);      //Aguarda 1 segundo
  cli();             //Desabilita interrupção

  flowRate = pulseCount / 5.5;    // Converte para L/min
  mililiters = flowRate / 60;     // Calculo de mililitros
  liters = liters + mililiters;   // Calculo de quantos litros passou pelo sensor

  if (select)
  {
    select = false;
    if (page == 1)
    {
      page = 2;
    }
    else if (page == 2)
    {
      page = 3;
    }
    else if (page == 3)
    {
      page = 1;
    }
  }

  if (page == 1)
  {
    display.setTextSize(1);                 // Configura o tamanho do texto
    display.clearDisplay();                 // Limpa a tela do display
    display.setTextColor(BLACK, WHITE);     // Inverte as cores das letras
    display.setCursor(2, 0);                // Posiciona o cursor
    display.print("IND. DE VAZAO");         // Envia texto ao display
    display.drawFastHLine(0, 9, 84, BLACK); // Desenha uma linha horizontal

    display.setCursor(0, 10);
    display.setTextColor(WHITE, BLACK);
    display.print("  Instantanea ");
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 20);
    display.print(flowRate);
    display.print(" L/min ");
    display.drawFastHLine(0, 29, 84, BLACK);

    display.setCursor(0, 30);
    display.setTextColor(WHITE, BLACK);
    display.print("  Totalizador ");
    display.setCursor(24, 40);
    display.setTextColor(BLACK, WHITE);
    display.print(liters);
    display.print(" L ");
    display.display();
  }

  if (page == 2)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.drawFastHLine(0, 0, 84, BLACK);
    display.setCursor(0, 1);
    display.setTextColor(WHITE, BLACK);
    display.print("  INSTANTANEA ");

    display.setTextColor(BLACK, WHITE);
    display.setCursor(5, 11);
    display.print("Valor");
    display.setCursor(5, 22);
    display.setTextSize(2);
    display.print(flowRate);
    display.setCursor(0, 40);
    display.setTextSize(1);
    display.print(liters);
    display.print(" L");
    display.setCursor(53, 40);
    display.print("L/min");
    display.drawBitmap(69, 9,  tap_logo, 16, 16, 1);
    display.display();
  }

  if (page == 3)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.drawFastHLine(0, 0, 84, BLACK);
    display.setCursor(0, 1);
    display.setTextColor(WHITE, BLACK);
    display.print("  TOTALIZADOR ");
    display.setTextColor(BLACK, WHITE);

    display.setCursor(5, 11);
    display.print("Valor");
    display.setCursor(5, 22);
    display.setTextSize(2);
    display.print(liters);
    display.setCursor(0, 40);
    display.setTextSize(1);
    display.print(flowRate);
    display.print(" L/m");
    display.setCursor(60, 40);
    display.print("Lit.");
    display.drawBitmap(69, 9,  tap_logo, 16, 16, 1);
    display.display();
  }
}

//***************************************************************************//
// Função que monitora por pulling o estado do botão para mudanca de tela se //
// esta pressionado ou nao                                                   //
//***************************************************************************//
void checkIfSelectButtonIsPressed()
{
  if (selectButtonState != lastSelectButtonState)
  {
    if (selectButtonState == 0) {
      select = true;
    }
    delay(20);
  }
  lastSelectButtonState = selectButtonState;
}

//***************************************************************************//
// Função de interrupção que monitora o estado da entrada D2                 //
// Interrupção 0                                                             //
//***************************************************************************//
void pulse ()
{
  pulseCount++; //Incrementa a variável de pulsos
}
