
#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int x_hor;
  int y_hor;
  int x_ver;
  int y_ver;
  int pwm;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X Horizontal: ");
  Serial.println(myData.x_hor);
  Serial.print("Y Horizontal: ");
  Serial.println(myData.y_hor);
  Serial.print("X Vertical: ");
  Serial.println(myData.x_ver);
  Serial.print("Y Vertical: ");
  Serial.println(myData.y_ver);
  Serial.print("PWM : ");
  Serial.println(myData.pwm);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
