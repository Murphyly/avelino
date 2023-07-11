//Compila apenas se MASTER não estiver definido no arquivo principal
#ifndef MASTER

//Variáveis Pulse sensor
int PulseWire = 36;
float sensorValue = 0;                            
int count = 9;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;   

void setup(){
    Serial.begin(115200);
    //Chama a configuração inicial do display
    //Iniciamos o led do circuito
    pinMode(12,OUTPUT);  
    //Inicia display
    setupDisplay();
    //Chama a configuração inicial do LoRa
    setupLoRa();
    display.clear();
    display.drawString(0, 0, "Slave esperando...");
    display.display();
}

void loop(){
  starttime = millis();
  while (millis()<starttime+10000)                   // Reading pulse sensor for 10 seconds
  {
    sensorValue = analogRead(PulseWire);
    if (sensorValue > 550 && counted == false)  // Threshold value is 550 (~ 2.7V)
    {
      count++;
      Serial.print ("count = ");
      Serial.println (count);
      digitalWrite (12,HIGH);
      delay (50);
      digitalWrite (12, LOW);
      counted = true;
    }
    else if (sensorValue < 550)
    {
      counted = false;
      digitalWrite (13, LOW);
    }

  }
  heartrate = count*6;                               // Multiply the count by 6 to get beats per minute
  Serial.println ();
  Serial.print ("BPM = ");
  Serial.println (heartrate);                        // Display BPM in the Serial Monitor
  Serial.println ();
  count = 0;
  // Signal = analogRead(PulseWire);  
  // if(Signal > Threshold){   
  //   digitalWrite(12, HIGH);
  //   display.drawString(0, 0, "BPM: " + String(Signal)); 
  //   BPM = Signal;
  // } else {
  //   digitalWrite(12,LOW);   
  //   display.drawString(0, 0, "Not detected !");
  // }    
  //Tenta ler o pacote
  int packetSize = LoRa.parsePacket();

  //Verifica se o pacote possui a quantidade de caracteres que esperamos
  if (packetSize == GETDATA.length()){
    String received = "";

    //Armazena os dados do pacote em uma string
    while(LoRa.available()){
      received += (char) LoRa.read();
    }

    if(received.equals(GETDATA)){
      //Simula a leitura dos dados
      String data = readData();
      Serial.println("Criando pacote para envio");
      //Cria o pacote para envio
      LoRa.beginPacket();
      LoRa.print(SETDATA + data);
      //Finaliza e envia o pacote
      LoRa.endPacket();
      //Mostra no display
      display.clear();
      display.drawString(0, 0, "Enviou: " + String(data));
      display.display();
    }
  }
}

//Função onde se faz a leitura dos dados que queira enviar
//Poderia ser o valor lido por algum sensor por exemplo
//Aqui vamos enviar apenas um contador para testes
//mas você pode alterar a função para fazer a leitura de algum sensor
String readData(){
  return String(heartrate);
}

#endif