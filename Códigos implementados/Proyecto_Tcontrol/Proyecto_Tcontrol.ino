#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

//se define los pines del receptor
#define CE_PIN 2
#define CSN_PIN 0
//se define los pines del acelerometro
#define SDA_PIN 4 //Iniciar la comunicación I2C
#define SCL_PIN 5


RF24 radio(CE_PIN, CSN_PIN); // CE_PIN (TXD1), CSN_PIN (GPIO0)
const byte address[6] = "00001"; // Dirección del receptor

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
unsigned long tiempoInicio = 0;
struct Data_Package {
  byte sta;
  byte sto;
};
Data_Package data; //Create a variable with the above structure

int ValueStart = 0;
int ValueStop = 0;
short int ax , ay, az;
void setup() {
  Wire.begin();
  
  Serial.begin(9600);
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  
  SPI.begin();
  radio.begin();
  radio.openReadingPipe(0, address); // Dirección del receptor
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void Medir_aceleracion(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //Serial.print("Aceleracion en x: ");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  //Serial.print("Aceleracion en y: ");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  //Serial.print("Aceleracion en z: ");
  Serial.print(a.acceleration.z);
  Serial.print(",");

  delay(1000);
  
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
    lastReceiveTime = millis();
  }  
  ValueStart = data.sta;
  ValueStop = data.sto; 
  Serial.println(ValueStart); 
  if (ValueStart == LOW){
    Medir_aceleracion();
      }
  
  if (ValueStop == LOW){

    }
  }
