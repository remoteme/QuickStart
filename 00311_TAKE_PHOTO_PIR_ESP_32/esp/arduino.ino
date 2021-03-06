#define WIFI_NAME "$$wifiName$$"
#define WIFI_PASSWORD "$$wifiPassword$$"
#define DEVICE_ID $$esp$$
#define DEVICE_NAME "$$esp_name$$"
#define TOKEN "$$token$$"

#include "esp_camera.h"
#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
#include <WiFi.h>

#define PIR_PIN 12 // PIR on GPIO12
#define LED_PIN 4 // GPIO4 is the Flash LED
#define repeatingSendTimeoutSeconds 120

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled
//

// Select camera model
#define $$camera_type$$

//other suported modules
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
//#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

void initCamera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;

#if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
#endif

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
    else {
        Serial.println("Camera init sucess");
    }

    sensor_t* s = esp_camera_sensor_get();
    //initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID) {
        s->set_vflip(s, 1); //flip it back
        s->set_brightness(s, 1); //up the blightness just a bit
        s->set_saturation(s, -2); //lower the saturation
    }

#if defined(CAMERA_MODEL_M5STACK_WIDE)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif

    Serial.println("Camera end");
}

void setup()
{

    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    initCamera();

    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    remoteMe.setConnector(new RemoteMeSocketConnector());

    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

    remoteMe.setUserMessageListener(onUserMessage);
}

void takePhoto()
{
    //acquire a frame
    digitalWrite(LED_PIN, HIGH);//flash on
    camera_fb_t* fb = esp_camera_fb_get();
    digitalWrite(LED_PIN, LOW);//flash off
    if (!fb) {
        Serial.println("Camera Capture Failed");
        return;
    }

    //return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    Serial.println("photo took");

    remoteMe.setFileContent($$webpage$$, "photos/photo.jpg", fb->len, fb->buf);
    Serial.printf(" buff size %d \n", fb->len);
    Serial.println("sent");
}

void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data)
{
    takePhoto();
}

void loop()
{
    static long lastSend = 0;
    remoteMe.loop();

    if (digitalRead(PIR_PIN) && ((lastSend == 0) || (lastSend + repeatingSendTimeoutSeconds * 1000 < millis()))) {
        takePhoto();
        lastSend = millis();
        remoteMe.sendPushNotificationMessage($$webpage$$, "Move appears", "Move", "badge.png", "icon192.png", "photos/photo.jpg");
    }
}
