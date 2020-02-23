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
$$IF rental == 1$$
void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data, uint16_t sessionId, uint16_t identifier, uint16_t credit, uint16_t time){
$$ELSE$$
void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data){
$$/IF$$
    uint16_t pos = 0;
    int16_t  integerValue=RemoteMeMessagesUtils::getInt16(data, pos);
    //here u can do wahtever You want with int value
    String str="hello its ESP I just received ";
    str+=integerValue;
    str+=" from webPage";

    remoteMe.getVariables()->setText("label", str);
$$IF logs == 1$$
    Serial.printf("on User message integer: %d  \n",str.c_str(),integerValue);
$$/IF$$

$$IF rental == 1$$
    if (sessionId!=0) {
        remoteMe.sendDecreaseGuestKeyCreditAndTimeMessage(sessionId, 3, 4);//lets decrease credit and time
    }
$$/IF$$

}

inline void setButton1Active(boolean button1Active){
    remoteMe.getVariables()->setBoolean("button1Active", button1Active);
}


inline void setButton2Active(boolean button2Active){
    remoteMe.getVariables()->setBoolean("button2Active", button2Active);
}




inline void setButton1Text(String button1Text){
    remoteMe.getVariables()->setText("button1Text", button1Text);
}

inline void setButton2Text(String button2Text){
     remoteMe.getVariables()->setText("button2Text", button2Text);
}


void setup() {

    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
    Serial.begin(115200);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }

    remoteMe.setUserMessageListener(onUserMessage);
    remoteMe.setConnector(new RemoteMeSocketConnector());
    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);


    setButton1Active(true);
    setButton2Active(true);
    setButton1Text("button 1 some initial text");
    setButton2Text("button 2 some initial text");
}

void loop() {
    remoteMe.loop();
    static unsigned long time=millis();
    if (millis()+2000<time){//better then sleep since its not stopping loop code after if will be run once for 2 seconds
        time=millis();
        setButton1Active(millis()%2);//some stupid random
        setButton2Active(millis()%2);//some stupid random

        if (millis()%3==0){
             setButton1Text("button 1 OTHER Text");
        }else{
            setButton1Text("button 1 some initial text");
        }

        if (millis()%5==0){
             setButton1Text("button 2 DIFFRENT Text");
        }else{
            setButton1Text("button 2 some initial text");
        }
    }
}