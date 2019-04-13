#define WIFI_NAME "$$wifiName$$"
#define WIFI_PASSWORD "$$wifiPassword$$"
#define DEVICE_ID $$esp$$
#define DEVICE_NAME "$$esp_name$$"
#define TOKEN "$$token$$"


#include <RemoteMe.h>
#include <RBD_Button.h> 
#include <RemoteMeSocketConnector.h>

$$IF ESP_type == ESP_32$$
#include <WiFi.h>
$$ELSE$$
#include <ESP8266WiFi.h>
$$/IF$$

RBD::Button button(D1);

boolean currentLedState;

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//*************** CODE FOR CONFORTABLE VARIABLE SET *********************

inline void setLed(boolean b) {remoteMe.getVariables()->setBoolean("$$led_variable$$", b); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************

void onLedChange(boolean b) {
	currentLedState=b;
	digitalWrite(D2, b?HIGH:LOW);
}


void setup() {
	pinMode(D2, OUTPUT);
	  
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
	if (button.onPressed()) {
		currentLedState=!currentLedState;
		String body="You've change button state to ";
		body+=currentLedState?"ON":"OFF";
		setLed(currentLedState);
		remoteMe.sendPushNotificationMessage(204,"Change by ESP",body,"badge.png","icon192.png","");
	}
}


