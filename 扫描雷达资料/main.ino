#include <Servo.h>
#include <NewPing.h>
 
Servo my_servo;
const int echo = PA_14;
const int trig = PA_13;
int distance;
NewPing sonar(trig, echo, 200); // NewPing setup of pins and maximum distance.

void setup() {
  pinMode(PB_1, OUTPUT);//LED
  pinMode(echo,INPUT);//设置echo为输入脚
  pinMode(trig,OUTPUT);//设置trig为输出脚
  my_servo.attach(PA_5);
  Serial.begin(115200);
}
void loop() {
  digitalWrite(PB_1, !digitalRead(PB_1));
  static int angle=0,direction=1;
  my_servo.write(angle);
  if(direction == 1){
    angle++;
  }else{
    angle--;
  }
  if(angle > 180){
    direction = -1;
    angle = 180;
  }
  if(angle < 0){
    direction = 1;
    angle = 0;
  }

  distance = sonar.ping_cm();
  if(distance < 0){
    distance = 0;
  }
  if(distance > 255){
    distance = 255;
  }

  const char data[4] = {0x5B,angle,distance,0x5D};
  Serial.write(data,4);
  delay(100);
}
