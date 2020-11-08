#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcastAddress2[] = {0xFF,0xFF ,0xFF,0xFF ,0xFF ,0xFF };
uint8_t broadcastAddress3[] = {0xFF,0xFF ,0xFF ,0xFF ,0xFF ,0xFF };


typedef struct struct_message {
  int x_hor;
  int y_hor;
  int x_ver;
  int y_ver;
  int pwm;
} struct_message;

struct_message myData;
int state;


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
   
  // register peer
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // register second peer  
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  /// register third peer
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {


    if (Serial.available() > 0)
  {
  myData.x_hor = Serial.read() - '0';
  delay(10);
  myData.y_hor = Serial.read() - '0';
  delay(10);
  myData.x_ver = Serial.read() - '0';
  delay(10);
  myData.y_ver = Serial.read() - '0';
  delay(10);
  state = Serial.read() - '0';
  delay(10);
  myData.pwm = Serial.read() - '0';
  delay(10);
  
    if (state == 1)
    {  
      esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    }
    else if (state == 2)
    {
      esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
    }

    else if (state == 3)
    {
      esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
    }
    delay(500);
  }
}
