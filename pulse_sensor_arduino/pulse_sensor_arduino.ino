#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

const int PulseWire = 0; 
const int LED13 = 13;
int Signal;
int Threshold = 550;
const int PULSE_FADE = 8;
const int OUTPUT_TYPE = PROCESSING_VISUALIZER;
const int buzzer = 11;

PulseSensorPlayground pulseSensor;

void myTone(byte pin, uint16_t frequency, uint16_t duration)
{ // input parameters: Arduino pin number, frequency in Hz, duration in milliseconds
  unsigned long startTime=millis();
  unsigned long halfPeriod= 1000000L/frequency/2;
  pinMode(pin,OUTPUT);
  while (millis()-startTime< duration)
  {
    digitalWrite(pin,HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin,LOW);
    delayMicroseconds(halfPeriod);
  }
  pinMode(pin,INPUT);  
}

void setup() {
  Serial.begin(9600);

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  pulseSensor.fadeOnPulse(PULSE_FADE);  
  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() 
{   
  int myBPM = pulseSensor.getBeatsPerMinute(); 
  
  if (pulseSensor.sawStartOfBeat()) {          // Constantly test to see if "a beat happened".
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                        // Print phrase "BPM: " 
    Serial.println(myBPM);   
    myTone(11, 440, 400);  
    digitalWrite(PulseWire, HIGH);
    digitalWrite(PulseWire, LOW);             // Print the value inside of myBPM. 
  }
  
  delay(300);
}