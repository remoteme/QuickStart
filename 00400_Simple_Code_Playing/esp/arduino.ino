#define WIFI_NAME "$$wifiName$$"
#define WIFI_PASSWORD "$$wifiPassword$$"
#define DEVICE_ID $$esp$$
#define DEVICE_NAME "$$esp_name$$"
#define TOKEN "$$token$$"


#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>

$$IF ESP_type == ESP_32$$
#include <WiFi.h>
$$ELSE$$
#include <ESP8266WiFi.h>
$$/IF$$

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

int32_t counter=0;
void onSomeIntegerChanged(int32_t newValue){
  counter=newValue;
  remoteMe.getVariables()->setBoolean("justBooleanVariable", counter%2==1);
}


void setup() {

	WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(100);
	}

	remoteMe.getVariables()->observeInteger("someIntegerVariable" ,onSomeIntegerChanged);
	remoteMe.setConnector(new RemoteMeSocketConnector());
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}


void loop() {
	 if (!remoteMe.loop()){//no connection established
        return ;
      }
	static long lastTimeSend=0;
  if (lastTimeSend+1000<millis()){
    lastTimeSend=millis();
    remoteMe.getVariables()->setSmallInteger3("just3IntegerVariable", millis()%32000,counter*-10,counter);

  }
}