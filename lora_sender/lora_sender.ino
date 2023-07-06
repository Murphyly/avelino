#include <LoRa.h>
#include <SSD1306.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 oled(128, 64);

// Pinos do display (comunicação i2c)
const int DISPLAY_ADDRESS_PIN = 0x3c;
const int DISPLAY_SDA_PIN = 4;
const int DISPLAY_SCL_PIN = 15;
const int DISPLAY_RST_PIN = 16;

// Pinos do lora (comunicação spi)
const int LORA_SCK_PIN = 5;
const int LORA_MISO_PIN = 19;
const int LORA_MOSI_PIN = 27;
const int LORA_SS_PIN = 18;
const int LORA_RST_PIN = 15;
const int LORA_DI00_PIN = 26;

// Frequência de comunicação
const int BAND = 433E6;

// Contador de pacotes enviados via lora
int counter = 0;

// Altura da fonte (correspondente a fonte ArialMT_Plain_16)
const int fontHeight = 16; 

// Objeto do display
SSD1306 display(DISPLAY_ADDRESS_PIN, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

//Variáveis Pulse sensor
int PulseSensorPurplePin = 36;     
int Signal;                
int Threshold = 2050;   
int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
#define UpperThreshold 560
#define LowerThreshold 530

// Função que inicializa o display
bool displayBegin()
{
  // Reiniciamos o display
  pinMode(DISPLAY_RST_PIN, OUTPUT);
  digitalWrite(DISPLAY_RST_PIN, LOW);
  delay(1);
  digitalWrite(DISPLAY_RST_PIN, HIGH);
  delay(1);

  return display.init(); 
}

// Função que faz algumas configuções no display
void displayConfig()
{
  // Invertemos o display verticalmente
  display.flipScreenVertically();
  // Setamos a fonte
  display.setFont(ArialMT_Plain_16);
  // Alinhamos a fonta à esquerda
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}


// Função que inicializa o radio lora
bool loraBegin()
{
  // Iniciamos a comunicação SPI
  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
  // Setamos os pinos do lora
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI00_PIN);
  // Iniciamos o lora
  return LoRa.begin(BAND);
}

void setup() 
{   
  // Iniciamos a serial com velocidade de 9600
  Serial.begin(9600);

  pinMode(12,OUTPUT);  
  
  // Exibimos "Starting..." na serial (debug)
  Serial.println("Starting...");

  // Iniciamos o display
  if(!displayBegin())
  {
    // Se não deu certo, exibimos falha de display na serial
    Serial.println("Display failed!");
    // E deixamos em loop infinito
    while(1);
  }

  // Configuramos o posicionamento da tela, fonte e o alinhamento do texto
  displayConfig();
  
  // Iniciamos o lora
  if(!loraBegin()) 
  {
    // Se não deu certo, exibimos falha de lora na serial
    Serial.println("LoRa failed!");
    // E deixamos em loop infinito
    while (1);
  }  
}

void loop() 
{   
  Signal = analogRead(PulseSensorPurplePin);

  Serial.println(Signal);                 
  // Variável usada para indicar em qual linha o cursor deverá estar quando uma mensagem no display for exibida
  int line;

  // Limpamos o display
  display.clear();
  
  // Iniciamos na primeira linha (zero)
  line = 0;

  if(Signal > Threshold){   
    digitalWrite(12, HIGH);
  } else {
    digitalWrite(12,LOW);                
  }

  // Escrevemos a mensagem "Sending packet: " na primeira linha
  display.drawString(0, line, "BPM: " + String(Signal));

  // Exibimos as alterações no display
  display.display();

  // Enviamos um pacote com a mensagem "hello" concatenado com o número "counter"
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  // Incrementamos o contador
  counter++;

  // Aguardamos 1 segundo
  delay(1000);
}