#define WIFI_NAME "$$wifiName$$"
#define WIFI_PASSWORD "$$wifiPassword$$"
#define DEVICE_ID $$esp$$
#define DEVICE_NAME "$$esp_name$$"
#define TOKEN "$$token$$"

#define btn D3

#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>

$$IF ESP_type == ESP_32$$
#include <WiFi.h>
$$ELSE$$
#include <ESP8266WiFi.h>
$$/IF$$

#include <SparkFunBME280.h>
#include<Wire.h>
#include<OLED.h>
#include <RBD_Button.h> 

OLED display(D2,D1);
BME280 mySensor;
RBD::Button button(D3);

String date, date_2, r_weather, r_weather_2;
int16_t r_temp, r_temp_2, r_press, r_press_2;

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//*************** CODE FOR COMFORTABLE VARIABLE SET *********************

inline void setHumm(double d) {remoteMe.getVariables()->setInteger("humm", d); }
inline void setPress(double d) {remoteMe.getVariables()->setInteger("press", d); }
inline void setTemp(double d) {remoteMe.getVariables()->setInteger("temp", d); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************




void onWeather_NowChange(int16_t i1, int16_t i2,String s1,String s2) 
{
    Serial.printf("onWeather_NowChange: i1: %d,i2: %d,s1: %s,s2: %s\n",i1,i2,s1.c_str(),s2.c_str());
    r_temp = i1;
    r_press = i2;
    date = s1;
    r_weather = s2;
    r_weather.toLowerCase();
    r_weather.replace('_',' ');
    r_weather.replace("night"," ");
    r_weather.trim();
}

void onWeather_TomorrowChange(int16_t i1, int16_t i2,String s1,String s2) 
{
    Serial.printf("onWeather_TomorrowChange: i1: %d,i2: %d,s1: %s,s2: %s\n",i1,i2,s1.c_str(),s2.c_str());
    r_temp_2 = i1;
    r_press_2 = i2;
    date_2 = s1;
    r_weather_2 = s2;
    r_weather_2.toLowerCase();
    r_weather_2.replace('_',' ');
    r_weather_2.replace("night"," ");
    r_weather_2.trim();
}

void sort(double a[], int size) 
{
    for(int i=0; i<(size-1); i++) 
    {
        for(int o=0; o<(size-(i+1)); o++) 
        {
              if(a[o] > a[o+1]) 
              {
                  double t = a[o];
                  a[o] = a[o+1];
                  a[o+1] = t;
              }
        }
    }
}


void setup() 
{
    Serial.begin(921600);
    display.begin();
    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
    }

    remoteMe.getVariables()->observeSmallInteger2Text2("Weather_Now" ,onWeather_NowChange);
    remoteMe.getVariables()->observeSmallInteger2Text2("Weather_Tomorrow" ,onWeather_TomorrowChange);
    remoteMe.setConnector(new RemoteMeSocketConnector());
    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

    mySensor.settings.commInterface = I2C_MODE;
    mySensor.settings.I2CAddress = 0x76;
    mySensor.settings.runMode = 3; //Normal mode
    mySensor.settings.tStandby = 0;
    mySensor.settings.filter = 4;
    mySensor.settings.tempOverSample = 5;
    mySensor.settings.pressOverSample = 5;
    mySensor.settings.humidOverSample = 5;
    mySensor.begin();
}

void sendData()
{
  double temp[10];
  double pressure[10];
  double humm[10];
  for(int i=0;i<9;i++)
  {
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

void displayRoom()
{
  char buff[80];
  
  display.print("Room Weather:", 1, 1);
  sprintf(buff, "TEMP: %d *C", (int)mySensor.readTempC());
  display.print(buff, 3, 1);
  sprintf(buff, "PRESS: %d hPa", (int)mySensor.readFloatPressure()/100);
  display.print(buff, 4, 1);
  sprintf(buff, "HUM: %d %", (int)mySensor.readFloatHumidity());
  display.print(buff, 5, 1);
}

void displayRemote()
{
  char buff[80];
  display.print("Weather Now:", 1, 1);
  
  if (r_temp==-1000)
  {
   display.print("loading ...", 3, 1);
  }
  else
  { 
    sprintf(buff,"TEMP: %d *C", (int)r_temp);
    display.print(buff, 3, 1);
    sprintf(buff,"PRESS: %d hPa", (int)r_press);
    display.print(buff, 4, 1);
    sprintf(buff,"%s ", r_weather.c_str());
    display.print(buff, 5, 1);
    sprintf(buff,"%s ", date.c_str());
    display.print(buff, 7, 1);
  }
}

void displayRemote2()
{
  char buff[80];
  display.print("Tomorrow:", 1, 1);
  
  if (r_temp==-1000)
  {
   display.print("loading ...", 3, 1);
  }
  else
  { 
    sprintf(buff,"TEMP: %d *C", (int)r_temp_2);
    display.print(buff, 3, 1);
    sprintf(buff,"PRESS: %d hPa", (int)r_press_2);
    display.print(buff, 4, 1);
    sprintf(buff,"%s ", r_weather_2.c_str());
    display.print(buff, 5, 1);
    sprintf(buff,"%s ", date_2.c_str());
    display.print(buff, 7, 1);
  }
}

void loop() 
{
  static int flag = 0; 
  if (button.onPressed()) 
  {
    display.clear();
  
    flag=(flag+1)%3;
  }

  if (flag==0)
  {
    displayRoom();
  }
  else if (flag==1)
  {
     displayRemote();
  }
  else if (flag==2)
  {
     displayRemote2();
  }
  
   if (!remoteMe.loop()){//no connection established
      return false;
    }
  static long time=0;
    if (time+10000<millis())
    {
      Serial.print("Temp:");
      Serial.println(mySensor.readTempC());
      Serial.print("Pressure:");
      Serial.println(mySensor.readFloatPressure()/100);
      Serial.print("Humidity:");
      Serial.println(mySensor.readFloatHumidity());
      time = millis();
      sendData();
    }
}
