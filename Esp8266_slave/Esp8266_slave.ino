
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <espnow.h>

// Servo Objects
Servo servo_dir;
Servo servo_cam;

int d0 = 16;
int d1 = 5;
int d2 = 4;
int d3 = 0;
int d4 = 2;
int d5 = 14;
int d6 = 12;
int d7 = 13;
int d8 = 15;
int d9 = 3;
int d10 = 1;


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int x_hor;
  int y_hor;
  int x_ver;
  int y_ver;
  int pwm;
  int cam_dir;
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
  Serial.print("Cam Dir : ");
  Serial.println(myData.cam_dir);
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

  // Servo

  servo_dir.attach(d6);
  servo_write(90);
  delay(2000);

  servo_cam.attach(d7);
  servo_cam.write(90);
  delay(2000);

  pinMode(d0,OUTPUT); // AIN1
  pinMode(d1,OUTPUT); // AIN2
  pinMode(d2,OUTPUT); // PWM
  pinMode(d3,OUTPUT); // AIN2
  pinMode(d4,OUTPUT); // AIN1
  pinMode(d5,OUTPUT); // PWM


  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
