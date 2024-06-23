#include <Wire.h>
#include "MAX30105.h"

//#include "heartbeat.h"
#define SDA_1 21
#define SCL_1 22

TwoWire I2Cone = TwoWire(0);
#define I2C_ADDR0 0x57
#define I2C_ADDR1 0x56
#define I2C_ADDR2 0x55

MAX30105 ps0,ps1,ps2;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  I2Cone.begin(SDA_1, SCL_1, I2C_SPEED_FAST);
  // Initialize sensor 0
  // if (!ps0.begin(I2Cone, I2C_SPEED_FAST,I2C_ADDR0)) //Use default I2C port, 400kHz speed
  // {
  //   Serial.println("Sensor 0 not found");
  //   //while (1);
  // }
  // Initialize sensor 1
  if (!ps1.begin(I2Cone, I2C_SPEED_FAST,I2C_ADDR1)) 
  {
    Serial.println("Sensor 1 not found");
    //while (1);
  }
  // Initialize sensor 2
  if (!ps2.begin(I2Cone, I2C_SPEED_FAST,I2C_ADDR2)) 
  {
    Serial.println("Sensor 2 not found");
    //while (1);
  }

  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1A; //Options: 0=Off to 255=50mA
  byte sampleAverage = 32; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 69; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384

  // ps0.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  ps1.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  ps2.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  // ps0.setPulseAmplitudeRed(0x0A); 
  // ps0.setPulseAmplitudeGreen(0);
  
  // ps1.setPulseAmplitudeRed(0x00); 
  // ps1.setPulseAmplitudeGreen(0x3A);
  
  // ps2.setPulseAmplitudeRed(0x0A); 
  // ps2.setPulseAmplitudeGreen(0);

  //Arduino plotter auto-scales annoyingly. To get around this, pre-populate
  //the plotter with 500 of an average reading from the sensor

  //Take an average of IR readings at power up
  // const byte avgAmount = 64;
  // long baseValue = 0;
  // for (byte x = 0 ; x < avgAmount ; x++)
  // {
  //   baseValue += particleSensor.getIR(); //Read the IR value
  // }
  // baseValue /= avgAmount;

  // //Pre-populate the plotter so that the Y scale is close to IR values
  // for (int x = 0 ; x < 500 ; x++)
  //   Serial.println(baseValue);
}

// long irValue0 = 0;
long irValue1 = 0;
long irValue2 = 0;
void loop()
{
  // irValue0 = ps0.getIR();
  irValue1 = ps1.getIR();
  irValue2 = ps2.getIR();
  //Serial.print(" Sensor 0 ");
  // Serial.print(irValue0);
  // Serial.print(",");
  // Serial.print(" Sensor 1 ");
  Serial.print(irValue1);
  Serial.print(",");
  // Serial.print(" Sensor 2 ");
  Serial.print(irValue2);
  Serial.println();
}
