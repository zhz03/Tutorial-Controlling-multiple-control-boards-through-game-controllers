
#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress1[] = {0x40, 0xF5, 0x20, 0x28, 0xD3, 0x84};



// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int x_hor;
  int y_hor;
  int x_ver;
  int y_ver;
  int pwm;
  int cam_dir;
} struct_message;

// Create a struct_message called test to store variables to be sent
struct_message myData;
int state;
unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

 
void loop() {
  if (Serial.available() > 0)
  {

  myData.x_hor = Serial.read() - '0';
  delay(10);
  myData.x_hor = (myData.x_hor * 10) + Serial.read() - '0';
  delay(10);
//  myData.x_hor = (myData.x_hor * 10) + Serial.read() - '0';
//  delay(10);

  myData.y_hor = Serial.read() - '0';
  delay(10);
  myData.y_hor = (myData.y_hor * 10) + Serial.read() - '0';
  delay(10);
//  myData.y_hor = (myData.y_hor * 10) + Serial.read() - '0';
//  delay(10);

  myData.x_ver = Serial.read() - '0';
  delay(10);
  myData.x_ver = (myData.x_ver * 10) + Serial.read() - '0';
  delay(10);
//  myData.x_ver = (myData.x_ver * 10) + Serial.read() - '0';
//  delay(10);

  myData.y_ver = Serial.read() - '0';
  delay(10);
  myData.y_ver = (myData.y_ver * 10) + Serial.read() - '0';
  delay(10);
//  myData.y_ver = (myData.y_ver * 10) + Serial.read() - '0';
//  delay(10);


  myData.cam_dir = Serial.read() - '0';
  delay(10);
  myData.pwm = Serial.read() - '0';
  delay(10);
  
  esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
  delay(500);
  }
}
