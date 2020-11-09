// 4 motors split 

#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
    int x_hor;
  int y_hor;
  int x_ver;
  int y_ver;
  int pwm;

} struct_message;

struct_message myData;

int d0   = 16;
int d1   = 5;
int d2   = 4;
int d3   = 0;
int d4   = 2;
int d5   = 14;
int d6   = 12;
int d7   = 13;
int d8   = 15;
int d9 = 3;
int d10 = 1;
int SpeedH = 0;
int SpeedV = 0;

int x_1 = 0;
int y_1 = 0;


void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
//  Serial.print("Bytes received: ");
//  Serial.println(len);
//  Serial.print("x H: ");
//  Serial.println(myData.x_hor);
//  Serial.print("y H: ");
//  Serial.println(myData.y_hor);
//  Serial.print("x V: ");
//  Serial.println(myData.x_ver);
//  Serial.print("y V: ");
//  Serial.println(myData.y_ver);
//  Serial.print("PWM: ");
//  Serial.println(myData.pwm);
//  Serial.println();

 if (myData.x_hor == 1 && myData.y_hor == 2)
 {
  Serial.println("back");
  backward();
 }
 else if(myData.x_hor == 1 && myData.y_hor == 1)
 {
  Serial.println("stop horizontal");
  stopall();
 }
 else if(myData.x_hor == 1 && myData.y_hor == 0)
 {
  Serial.println("forward");
  forward();
 }
 else if(myData.x_hor == 0 && myData.y_hor == 1)
 {
  Serial.println("left");
  turn_left();
 }
 else if(myData.x_hor == 2 && myData.y_hor == 1)
 {
  Serial.println("right");
  turn_right();
 }


if (myData.x_ver == 1 && myData.y_ver == 2)
 {
  Serial.println("down");
  down();
 }
 else if(myData.x_ver == 1 && myData.y_ver == 1)
 {
  Serial.println("stop vertical");
  stop_ver();
 }
 else if(myData.x_ver == 1 && myData.y_ver == 0)
 {
  Serial.println("up");
  up();
 }
 else if(myData.x_ver == 0 && myData.y_ver == 1)
 {
  Serial.println("head up");
  head_up();
 }
 else if(myData.x_ver == 2 && myData.y_ver == 1)
 {
  Serial.println("tail up");
  tail_up();
 }

 if(SpeedH != myData.pwm)
 {
  Serial.println("Horizontal PWM Changed");
  Serial.println(myData.pwm);
  SpeedH = myData.pwm;
  if(SpeedH == 3)
  {
    analogWrite(d3,127);
  }
  else if (SpeedH == 4)
  {
    analogWrite(d3,255);  
  }
 }

 if(SpeedV != myData.pwm)
 {
  Serial.println("Vertical PWM Changed");
  Serial.println(myData.pwm);
  SpeedV = myData.pwm;
  if(SpeedV == 3)
  {
    analogWrite(d3,127);
  }
  else if (SpeedV == 4)
  {
    analogWrite(d3,255);  
  }  
 }
}

void Horizontal_move(boolean a1,boolean a2,boolean b1,boolean b2){
  digitalWrite(d5,a1);
  digitalWrite(d6,a2);
  digitalWrite(d7,b1);
  digitalWrite(d8,b2);
}

void stopall(){
  Horizontal_move(LOW,LOW,LOW,LOW);
}

void forward(){
  Horizontal_move(LOW,HIGH,LOW,HIGH);
}

void backward(){
  Horizontal_move(HIGH,LOW,HIGH,LOW);
}

void turn_right(){
  Horizontal_move(LOW,HIGH,LOW,LOW);
}

void turn_left(){
  Horizontal_move(LOW,LOW,LOW,HIGH);
}

void vertical_move(boolean a1,boolean a2,boolean b1, boolean b2){
  digitalWrite(d9,a1);
  digitalWrite(d10,a2);
   digitalWrite(d0,b1);
  digitalWrite(d4,b2);
}

void up(){
  vertical_move(HIGH,LOW,HIGH,LOW);
}
void down(){
  vertical_move(LOW,HIGH,LOW,HIGH);
}

void head_up(){
  vertical_move(HIGH,LOW,LOW,LOW);
}

void tail_up(){
  vertical_move(LOW,LOW,HIGH,LOW);
}
void stop_ver(){
  vertical_move(LOW,LOW,LOW,LOW);
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200); 

  pinMode(d5,OUTPUT);  //AIN2
  pinMode(d6,OUTPUT); // AIN1
  pinMode(d7,OUTPUT); //BIN2
  pinMode(d8,OUTPUT); // BIN1

  pinMode(d9,FUNCTION_3);
  pinMode(d10,FUNCTION_3);
  pinMode(d9,OUTPUT); // AIN2
  pinMode(d10,OUTPUT); //AIN1

  pinMode(d3,OUTPUT); // pwm

  pinMode(d0,OUTPUT); //BIN2
  pinMode(d4,OUTPUT); //BIN1

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() { 
}
