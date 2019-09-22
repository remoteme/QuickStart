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

#define PIN D5
#define numberOfMoves 2
#define timeoutSeconds 20
#define repeatingSendTimeoutSeconds 120


RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

void setup() {
  pinMode(PIN, INPUT);
  Serial.begin(19200);

  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  remoteMe.setConnector(new RemoteMeSocketConnector());
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}

void loop() {
  remoteMe.loop();
  static long firstReadTime = 0;
  static long sendTime = 0;
  static int state = 0;
  // 0 no move and waiting
  // 1 move occurs we are witing to move sensor to go 0
  // 2 - there was move and now its gone waiting for another move
  // 3 - second move appears
  // etc



  int oldState = state;
  if ((state != 0) && (firstReadTime + timeoutSeconds * 1000 <  millis())) {  // no second move appears
    state = 0;
  }

  if (digitalRead(PIN)) {
    if (state == 0) {  // first move occures
      firstReadTime = millis();
    }
    if (state % 2 == 0) {
      state++;
    }
  } else {
    if (state % 2 == 1) {
      state++;
    }
  }

  if (oldState != state) {
    Serial.printf(" %d ->  %d  \n", oldState, state);
  }

  if (state >= numberOfMoves*2 - 1) {
    state = 0;
    if ((sendTime == 0) ||  (sendTime + repeatingSendTimeoutSeconds * 1000 < millis())) {//so we dont send to frequently
      String body = "Move appears";
      remoteMe.sendPushNotificationMessage(2, "Move appears", body, "badge.png", "icon192.png", "");
      Serial.println("send push");
    } else {
      Serial.println("push not send because of repeating timeout time");
    }

    sendTime = millis();
  }
}
