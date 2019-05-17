#define WIFI_NAME "$$wifiName$$"
#define WIFI_PASSWORD "$$wifiPassword$$"
#define DEVICE_ID $$esp$$
#define DEVICE_NAME "$$esp_name$$"
#define TOKEN "$$token$$"


#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
//#include <RemoteMeDirectWebSocketConnector.h>// uncomment if u want to enable direct connections

$$IF ESP_type == ESP_32$$
#include <WiFi.h>
$$ELSE$$
#include <ESP8266WiFi.h>
$$/IF$$

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data){
   Serial.printf("got data size %d from device %d \n",dataSize,senderDeviceId);

    uint16_t pos = 0;
    String str=RemoteMeMessagesUtils::getString(data, pos);
    double doubleValue=RemoteMeMessagesUtils::getDouble(data, pos);

    Serial.printf("on User message string:'%s' and doublevalue '%f' \n",str.c_str(),doubleValue);


    dataSize = dataSize+10;// its length of "Received: ";
    pos = 0;
    data = (uint8_t*)malloc(dataSize );

    RemoteMeMessagesUtils::putString(data, pos, "Received: "+str);
    RemoteMeMessagesUtils::putDouble(data, pos, doubleValue*-2);

    remoteMe.sendUserMessage(senderDeviceId , data, dataSize );//lets send back modifued
}


void setup() {

    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
    Serial.begin(9600);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }

    remoteMe.setUserMessageListener(onUserMessage);
    remoteMe.setConnector(new RemoteMeSocketConnector());
    //remoteMe.setDirectConnector(new RemoteMeDirectWebSocketConnector()); //uncomment if u want to enable direct connections
    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}

void loop() {
  remoteMe.loop();
}