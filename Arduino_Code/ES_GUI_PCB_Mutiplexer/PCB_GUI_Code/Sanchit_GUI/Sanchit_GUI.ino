//Libraries to be imported are 
#include <Wire.h> //This is the library used for I2C transmission

//for interfacing sense_arr sparkFun MAX3010x Pulse and Proximity sensor Library that is arduino library for MAX30102 pulse oximeter sensor
#include "heartRate.h"
#include "MAX30105.h"
#include <WebSocketsServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#define SDA 6
#define SCL 7

TwoWire Wire_2 = TwoWire(0);
WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "BHUMIK";//add here wifi name
const char* password = "jssu6891";//add here wifi password

#define I2CMux 0x70//Macro for address of I2C multiplexer 0x70 
//const int num = 3;
#define num1 3 //macro for number of sensors 
// const char* sensorNames[num] = {"Vata", "Pitta", "Kapha"};
//const byte SIZE = 4;
#define SIZE 4 // macro for Size
byte rates[num1][SIZE];
byte rSpot[num1] = {0,  0};//array for rate spot index
long lBeat[num1] = {0, 0}; // array for lastbeat
float bpm[num1]; 
const int signalThreshold = 20; // Adjust threshold as needed

MAX30105 sense_arr[num1]; 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

                // send message to client
                webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            // Handle incoming text message (if needed)
            break;
    }
}

template <int order> // order is 1 or 2
class LowPass
{
  private:
    float a[order];
    float b[order+1];
    float omega0;
    float dt;
    bool adapt;
    float tn1 = 0;
    float x[order+1]; // Raw values
    float y[order+1]; // Filtered values

  public:  
    LowPass(float f0, float fs, bool adaptive){
      // f0: cutoff frequency (Hz)
      // fs: sample frequency (Hz)
      // adaptive: boolean flag, if set to 1, the code will automatically set
      // the sample frequency based on the time history.
      
      omega0 = 6.28318530718*f0;
      dt = 1.0/fs;
      adapt = adaptive;
      tn1 = -dt;
      for(int k = 0; k < order+1; k++){
        x[k] = 0;
        y[k] = 0;        
      }
      setCoef();
    }

    void setCoef(){
      if(adapt){
        float t = micros()/1.0e6;
        dt = t - tn1;
        tn1 = t;
      }
      
      float alpha = omega0*dt;
      if(order==1){
        a[0] = -(alpha - 2.0)/(alpha+2.0);
        b[0] = alpha/(alpha+2.0);
        b[1] = alpha/(alpha+2.0);        
      }
      if(order==2){
        float alphaSq = alpha*alpha;
        float beta[] = {1, sqrt(2), 1};
        float D = alphaSq*beta[0] + 2*alpha*beta[1] + 4*beta[2];
        b[0] = alphaSq/D;
        b[1] = 2*b[0];
        b[2] = b[0];
        a[0] = -(2*alphaSq*beta[0] - 8*beta[2])/D;
        a[1] = -(beta[0]*alphaSq - 2*beta[1]*alpha + 4*beta[2])/D;      
      }
    }

    float filt(float xn){
      // Provide me with the current raw value: x
      // I will give you the current filtered value: y
      if(adapt){
        setCoef(); // Update coefficients if necessary      
      }
      y[0] = 0;
      x[0] = xn;
      // Compute the filtered values
      for(int k = 0; k < order; k++){
        y[0] += a[k]*y[k+1] + b[k]*x[k];
      }
      y[0] += b[order]*x[order];

      // Save the historical values
      for(int k = order; k > 0; k--){
        y[k] = y[k-1];
        x[k] = x[k-1];
      }
  
      // Return the filtered value    
      return y[0];
    }
};

// Filter instance
//lp(cutoff,sampling,adaptive)
LowPass<2> lp1(3,1000,true);
LowPass<2> lp2(3,1000,true);
LowPass<2> lp3(3,1000,true);

void setup()
{
  // for (int i = 0;i<20;i++){
  //   Serial.println("  k  ");
  // }
  Serial.begin(115200); // baud rate
  Serial.println("Initializing...");
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
        Serial.println("connecting to wifi...");
    }
    Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Wire_2.begin(SDA, SCL, I2C_SPEED_FAST); 

  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

/*there are 3 sensors so using a for loop to select 3 sensors on mux lines and initialize them */
  for (int i = 0; i < num1; i++) {
    Wire_2.beginTransmission(I2CMux);
    Wire_2.write(1 << i);
    Wire_2.endTransmission();

    if (sense_arr[i].begin(Wire_2, I2C_SPEED_FAST)) {
      //Serial.print(sensorNames[i]);
      sense_arr[i].setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
      sense_arr[i].setPulseAmplitudeRed(0x10); // to define the ampltude of red light for pulse sensing
      sense_arr[i].setPulseAmplitudeGreen(0); //
    }
    else{
      Serial.print("Problems ");
      Serial.println(i);
      //while(1);
    }
   
  }

}

void loop()
{
    webSocket.loop();
    // this activity has to be repeated for each sensor 
    Wire_2.beginTransmission(I2CMux);// transmission begin 
    Wire_2.write(1 << 0);
    Wire_2.endTransmission();
    float data1 = lp1.filt(sense_arr[0].getIR());
    Serial.print(data1);
    Serial.print(',');
    Wire_2.beginTransmission(I2CMux);// transmission begin 
    Wire_2.write(1 << 1);
    Wire_2.endTransmission();
    float data2 = lp2.filt(sense_arr[1].getIR());
    Serial.print(lp2.filt(data2));
    Serial.print(',');
    Wire_2.beginTransmission(I2CMux);// transmission begin 
    Wire_2.write(1 << 2);
    Wire_2.endTransmission();
    float data3 = lp3.filt(sense_arr[2].getIR());
    Serial.print(data3);
    Serial.print(',');
    Serial.println(); // Print a newline
    String sensorDataString = String(data1) + ", " + String(data2) + ", " + String(data3); 
    webSocket.broadcastTXT(sensorDataString);
  //delay(100);
}