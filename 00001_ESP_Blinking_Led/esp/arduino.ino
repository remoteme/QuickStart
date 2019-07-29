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

//*************** CODE FOR COMFORTABLE VARIABLE SET *********************

inline void setLed(boolean b) {remoteMe.getVariables()->setBoolean("$$led_variable$$", b); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************

void onLedChange(boolean b) {
	Serial.printf("onLedChange: b: %d\n",b);
	digitalWrite(D1, b?HIGH:LOW);
}


void setup() {
	Serial.begin(9600);
	pinMode(D1, OUTPUT);
	  
	WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(100);
	}

	remoteMe.getVariables()->observeBoolean("$$led_variable$$" ,onLedChange);
	remoteMe.setConnector(new RemoteMeSocketConnector());
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}


void loop() {
	remoteMe.loop();
}

