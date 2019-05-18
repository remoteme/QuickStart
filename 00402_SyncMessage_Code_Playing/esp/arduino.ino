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

void onUserSyncMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data, uint16_t &returnDataSize, uint8_t *&returnData) {
    uint16_t pos = 0;
    double double1Value=RemoteMeMessagesUtils::getDouble(data, pos);
    double double2Value=RemoteMeMessagesUtils::getDouble(data, pos);
    Serial.printf("on User sync message d1:'%f' d2: '%f' \n",double1Value,double2Value);

    returnDataSize = 8;
    returnData = (uint8_t*)malloc(returnDataSize);
    pos=0;
    RemoteMeMessagesUtils::putDouble(returnData, pos, double1Value+double2Value);


}




void setup() {

    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
    Serial.begin(9600);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }

    remoteMe.setUserSyncMessageListener(onUserSyncMessage);
    remoteMe.setConnector(new RemoteMeSocketConnector());
    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}

void loop() {
  remoteMe.loop();
}