#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <SFE_BMP180.h>

unsigned long tiempoInicio;

bool releActivo;


//codigo para BMP180
// Sensor BMP180
SFE_BMP180 bmp180;
//tempo para BMP180
unsigned long ultimoEnvio = 0;
const unsigned long tiempoEnvio = 3000; // 3 segundos

// Modo funcionamiento BMP180
const byte modoPresBMP180 = 3;

// Altitud actual en metros
const double altitudActual = 106;

// servo
Servo servo;  // Crea un objeto de la clase Servo

// Configuración de los pines del gps
const int RX_PIN = 3;  // Conectar el pin RX del GPS al pin D3 del Arduino
const int TX_PIN = 4;  // Conectar el pin TX del GPS al pin D4 del Arduino

//configuración del relay
const int relayPin = 6;  // Pin D6

//configuración del servo
const int servoPin = 5;  // Pin al que está conectado el servo

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);  // Crear un objeto de comunicación serial


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  SoftwareSerial gpsSerial(RX_PIN, TX_PIN);  // Crear un objeto de comunicación serial
  pinMode(relayPin, OUTPUT);  // Configura el pin como salida
  servo.attach(servoPin);  // Asocia el pin al objeto servo
  servo.write(0);
  bmp180.begin();
  
}


void medir_temperatura ()
{
    // Variables para obtener datos del sensor BMP180
    byte status;
    double temp, pres, altitud;

    //Inicio de lectura de temperatura
    status = bmp180.startTemperature();

    // Comprobar resultado de la lectura temperatura
    // Devuelve 0 si hay error
    // milisegundos que hay que esperar en caso contrario
    if (status != 0)
    {
      // Pausa hasta que finalice la lectura
      delay(status);

      // Obtener temperatura
      // La temperatura se almacena en temp en grados Celsius
      status = bmp180.getTemperature(temp);

      // Comprobar resultado de la temperatura
      // Devuelve 0 si hay error
      // Devuelve 1 si todo ha ido bien
      if (status != 0)
      {
        Serial.print(F("Temperatura: "));
        Serial.print(temp, 2);
        Serial.println(F(" ºC"));

        // Inicio lectura de presión
        status = bmp180.startPressure(modoPresBMP180);

        // Comprobar resultado de la lectura presión
        // Devuelve 0 si hay error
        // milisegundos que hay que esperar en caso contrario
        if (status != 0)
        {
          // Pausa hata que finalice la lectura
          delay(status);

          // Obtener presión atomosférica
          // La prsión se almacena en la variable pres en milibar (mbar)
          // Esta función requiere la medida de temperatura
          status = bmp180.getPressure(pres, temp);

          // Comprobar resultado de la presión
          // Devuelve 0 si hay error
          // Devuelve 1 si todo ha ido bien
          if (status != 0)
          {
            Serial.print(F("Presión: "));
            Serial.print(pres, 2);
            Serial.println(F(" mbar"));

            // Obtener altitud

            // Paso 1, establecer la presión a la altitud actual para que nos devuelva la presión compensada a nivel del mar
            double presNivelMar = bmp180.sealevel(pres, altitudActual);

            Serial.print(F("Presión: "));
            Serial.print(pres, 2);
            Serial.println(F(" mbar"));

            // Paso 2, obtener la altitud con las dos presiones
            altitud = bmp180.altitude(pres, presNivelMar);

            Serial.print(F("Altitud: "));
            Serial.print(altitud, 2);
            Serial.println(F(" metros"));
          }
        }
      }
    }
  }


void prender_servo(){
  //despues de 5 segundos se abre el servo
  servo.write(90);  // Mueve el servo al ángulo de 45 grados
}

void loop() {
  releActivo = false;
  if (Serial.available()){
    int valor = Serial.parseInt();
    if (valor == 0){
      prender_servo();
    }
    if (valor == 0 && tiempoInicio <= 2000 ){
      releActivo =true;
      tiempoInicio = millis();
      //prender_servo();
      //digitalWrite(relayPin, HIGH); // Apaga el LED
      digitalWrite(relayPin, HIGH); // Apaga el LED
      delay(1000);
     }

    else if (valor == 0 && tiempoInicio >= 2000 ){
      //if (millis()-tiempoInicio >=2000){
        digitalWrite(relayPin, HIGH); // Apaga el 
        medir_temperatura();
        delay(100);
      }
   else if (valor == 1){
      //if (millis()-tiempoInicio >=2000){
        digitalWrite(relayPin, HIGH); // Apaga el LED
        delay(100);
        servo.write(0);
      } 
    
  
    }
}
