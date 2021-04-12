// 4 motors split 

#include <ESP8266WiFi.h>
#include <Servo.h>
#include <espnow.h>


// Servo Objects
Servo servo_dir;
Servo servo_cam;


typedef struct struct_message {
  int x_hor;
  int y_hor;
  int x_ver;
  int y_ver;
  int pwm;
  int cam_dir;

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
int Speed = 0;

int x_1 = 0;
int y_1 = 0;

int mid_servo_ang = 90;
int diff = 70;
int dur = mid_servo_ang - diff;
int min_servo_ang = mid_servo_ang - diff;
int max_servo_ang = mid_servo_ang + diff;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x H: ");
  Serial.println(myData.x_hor);
  Serial.print("y H: ");
  Serial.println(myData.y_hor);
  Serial.print("x V: ");
  Serial.println(myData.x_ver);
  Serial.print("y V: ");
  Serial.println(myData.y_ver);
  Serial.print("PWM: ");
  Serial.println(myData.pwm);
  Serial.println();
  Serial.print("Cam Dir : ");
  Serial.println(myData.cam_dir);

 if (myData.x_hor == 3 && myData.y_hor == 2)
 {
  Serial.println("back");
  backward();
 }
 else if(myData.x_hor == 3 && myData.y_hor == 1)
 {
  Serial.println("stop horizontal");
  stopall();
 }
 else if(myData.x_hor == 3 && myData.y_hor == 0)
 {
  Serial.println("forward");
  forward();
 }
 else if(myData.x_hor == 0 && myData.y_hor == 1)
 {
  Serial.println("left");
  turn_left();
 }
 else if(myData.x_hor == 6 && myData.y_hor == 1)
 {
  Serial.println("right");
  turn_right();
 } else if(myData.x_hor == 5 && myData.y_hor == 1)
 {
  Serial.println("right 5");
  turn_ang(min_servo_ang + dur);
  }else if(myData.x_hor == 4 && myData.y_hor == 1)
 {
  Serial.println("right 4");
  turn_ang(min_servo_ang + dur * 2);
  } else if(myData.x_hor == 2 && myData.y_hor == 1)
 {
  Serial.println("right 2");
  turn_ang(max_servo_ang - dur * 2);
  } else if(myData.x_hor == 1 && myData.y_hor == 1)
 {
  Serial.println("right 1");
  turn_ang(max_servo_ang - dur);
  } 



 if(myData.x_ver == 1)
 {
  Serial.println("stop_vert");
  stop_ver();
 }
 else if(myData.x_ver == 0)
 {
  Serial.println("up");
  up();
 }
 else if(myData.x_ver == 2)
 {
  Serial.println("down");
  down();
 }

 if(Speed != myData.pwm)
 {
  Serial.println("PWM Changed");
  Serial.println(myData.pwm);
  Speed = myData.pwm;
  if(Speed == 3)
  {
    analogWrite(d5,512);
    analogWrite(d2,512);
  }
  else if (Speed == 4)
  {
    analogWrite(d5,1024);
    analogWrite(d2,1024);  
  }
 }


  if(myData.cam_dir == 4)
  {
    Serial.println("Center Camera");
    servo_cam.write(85);
    Serial.println(servo_cam.read());
  }

  else if(myData.cam_dir == 2)
  {
     Serial.println("Right Camera");
     int old_val = servo_cam.read();
     int new_val = old_val + 20;
     servo_cam.write(new_val);
     Serial.println(servo_cam.read());
  }

  else if(myData.cam_dir == 3)
  {
    Serial.println("Left Camera");
    int old_val = servo_cam.read();
    int new_val = old_val - 20;
    servo_cam.write(new_val);
    Serial.println(servo_cam.read());
  }
 
}

void Horizontal_move(boolean a1,boolean a2){
  Serial.println("test hori");
  digitalWrite(d3,a1);
  digitalWrite(d4,a2);
}

void stopall(){
  servo_dir.write(mid_servo_ang);
  Horizontal_move(LOW,LOW);
}

void forward(){
  servo_dir.write(mid_servo_ang);
  Horizontal_move(HIGH,LOW);
}

void backward(){
  servo_dir.write(mid_servo_ang);
  Horizontal_move(LOW,HIGH);
}

void turn_right(){
  servo_dir.write(min_servo_ang);
  Horizontal_move(HIGH,LOW);
}

void turn_left(){
  servo_dir.write(max_servo_ang);
  Horizontal_move(HIGH,LOW);
}

int turn_ang(int ang){
  servo_dir.write(ang);
  Horizontal_move(HIGH,LOW);
}

void vertical_move(boolean a1,boolean a2){
  Serial.println("test vert");
  digitalWrite(d0,a1);
  digitalWrite(d1,a2);
}

void up(){
  vertical_move(HIGH,LOW);
}
void down(){
  vertical_move(LOW,HIGH);
}

void stop_ver(){
  vertical_move(LOW,LOW);
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200); 

  pinMode(d0,OUTPUT);  //AIN1
  pinMode(d1,OUTPUT);  //AIN2
  pinMode(d2,OUTPUT);  //PWM
  pinMode(d3,OUTPUT);  //AIN2
  pinMode(d4,OUTPUT);  //AIN1
  pinMode(d5,OUTPUT);  //PWM


  // Servo
  servo_dir.attach(d7);
  servo_dir.write(90);
  delay(2000);

  servo_cam.attach(d6);
  servo_cam.write(90);
  delay(2000);

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
