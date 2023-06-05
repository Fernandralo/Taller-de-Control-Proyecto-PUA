#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001"; // Dirección del receptor

struct Data_Package {
  byte sta;
  byte sto;
};

Data_Package data;

const int startButtonPin = 3; // Pulsador de start
const int stopButtonPin = 2;  // Pulsador de stop

const int LedStart = 4; // Pulsador de start
const int LedStop = 5;  // Pulsador de stop

void setup() {
  Serial.begin(9600);
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(stopButtonPin, INPUT_PULLUP);
  pinMode(LedStart, OUTPUT);
  pinMode(LedStop, OUTPUT);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  data.sta = 0;
  data.sto = 0;
}

void loop() {
  data.sta = digitalRead(startButtonPin);
  data.sto = digitalRead(stopButtonPin);
  

  if (data.sta == LOW) {
    //enviarDatos(1); // Envía señal de encendido al receptor
    radio.write(&data, sizeof(Data_Package));
    digitalWrite(LedStart, HIGH);
    digitalWrite(LedStop, LOW);
  } 
  if (data.sto == LOW) {
    //enviarDatos(0); // Envía señal de apagado al receptor
    radio.write(&data, sizeof(Data_Package));
    digitalWrite(LedStop, HIGH);
    digitalWrite(LedStart, LOW);
  }
  
  
  delay(10); // Evita envío continuo mientras los pulsadores están presionados
}

//void enviarDatos(int estado) 
  
 
