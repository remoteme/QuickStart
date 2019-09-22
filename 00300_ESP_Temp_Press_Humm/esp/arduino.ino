#define WIFI_NAME "$$wifiName$$"
#define WIFI_PASSWORD "$$wifiPassword$$"
#define DEVICE_ID $$esp$$
#define DEVICE_NAME "$$esp_name$$"
#define TOKEN "$$token$$"

//#include <Wire.h> // with some boards this includes are mandatory otherwise code will not compile
//#include <SPI.h> // with some boards this includes are mandatory otherwise code will not compile

#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>

$$IF ESP_type == ESP_32$$
#include <WiFi.h>
$$ELSE$$
#include <ESP8266WiFi.h>
$$/IF$$

#include <SparkFunBME280.h>

BME280 mySensor;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//*************** CODE FOR COMFORTABLE VARIABLE SET *********************

inline void setHumm(double d) {remoteMe.getVariables()->setInteger("$$humm$$", d); }
inline void setPress(double d) {remoteMe.getVariables()->setInteger("$$press$$", d); }
inline void setTemp(double d) {remoteMe.getVariables()->setInteger("$$temp$$", d); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************



void sort(double a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    double t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

void setup() {

    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    remoteMe.setConnector(new RemoteMeSocketConnector());

    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);


    mySensor.settings.commInterface = I2C_MODE;
    mySensor.settings.I2CAddress = 0x76;
  
  
    //***Operation settings*****************************//
  
    //runMode can be:
    //  0, Sleep mode
    //  1 or 2, Forced mode
    //  3, Normal mode
    mySensor.settings.runMode = 3; //Normal mode
  
                     //tStandby can be:
                     //  0, 0.5ms
                     //  1, 62.5ms
                     //  2, 125ms
                     //  3, 250ms
                     //  4, 500ms
                     //  5, 1000ms
                     //  6, 10ms
                     //  7, 20ms
    mySensor.settings.tStandby = 0;
  
   
    mySensor.settings.filter = 4;
  
   
    mySensor.settings.tempOverSample = 5;
    mySensor.settings.pressOverSample = 5;
    mySensor.settings.humidOverSample = 5;
   
   
   mySensor.begin();
}


void sendData(){
  double temp[10];
  double pressure[10];
  double humm[10];
  for(int i=0;i<9;i++){
    temp[i]= mySensor.readTempC();
    pressure[i]= mySensor.readFloatPressure();
    humm[i]= mySensor.readFloatHumidity();
    delay(100);
  }

  sort(temp,10);
  sort(humm,10);
  sort(pressure,10);


  setHumm(round(humm[5]));
  setPress(round(pressure[5]/100));
  setTemp(round(temp[5]));

}

void loop() {
     if (!remoteMe.loop()){//no connection established
        return false;
      }
    static long time=0;
    if (time+10000<millis()){
      time=millis();

      sendData();
    }
}