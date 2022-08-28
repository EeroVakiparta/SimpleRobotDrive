#include <esp_now.h>
#include <WiFi.h>

#define MAX_MOTOR_SPEED 200
#define SIGNAL_TIMEOUT 1000  //ms
unsigned long lastRecvTime = 0;

// Motor R
int RIN1 = 16; 
int RIN2 = 17; 

// Motor L
int LIN3 = 18;
int LIN4 = 19;

// Setting PWM properties
const int freq = 1000;
const int right1MotorPWMSpeedChannel = 1;
const int right2MotorPWMSpeedChannel = 2;
const int left3MotorPWMSpeedChannel = 3;
const int left4MotorPWMSpeedChannel = 4;
const int resolution = 8;

typedef struct data_packet {
    byte x;
    byte y;
} data_packet;

data_packet payload;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&payload, incomingData, sizeof(payload));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X: ");
  Serial.println(payload.x);
  Serial.print("Y: ");
  Serial.println(payload.y);
  Serial.println();

  throttleAndSteeringMovements();
  lastRecvTime = millis(); 
}

void throttleAndSteeringMovements()
{
  int throttle = map( int(payload.y), 254, 0, -255, 255);
  int steering = map( int(payload.x), 0, 254, -255, 255);  
  int motorDirection = 1;
  Serial.println("throttle ");
  Serial.println(throttle);
  Serial.println("steering ");
  Serial.println(steering);
  if (throttle < 0)       //Move car backward
  {
    motorDirection = -1;    
  }

  int rightMotorSpeed, leftMotorSpeed;
  rightMotorSpeed =  abs(throttle) - steering;
  leftMotorSpeed =  abs(throttle) + steering;
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);

  rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed){
  Serial.println("rightMotorSpeed ");
  Serial.println(rightMotorSpeed);
  Serial.println("leftMotorSpeed ");
  Serial.println(leftMotorSpeed);
  if (rightMotorSpeed < 0){
    ledcWrite(right2MotorPWMSpeedChannel, abs(rightMotorSpeed));   
  }
  else if (rightMotorSpeed > 0){
    ledcWrite(right1MotorPWMSpeedChannel, abs(rightMotorSpeed));     
  }
  else{
    ledcWrite(right2MotorPWMSpeedChannel, 1);
    ledcWrite(right1MotorPWMSpeedChannel, 1);      
  }
  
  if (leftMotorSpeed < 0){
    ledcWrite(left4MotorPWMSpeedChannel, abs(leftMotorSpeed));    
  }
  else if (leftMotorSpeed > 0){
    ledcWrite(left3MotorPWMSpeedChannel, abs(leftMotorSpeed));      
  }
  else{
    ledcWrite(left4MotorPWMSpeedChannel, 1); 
    ledcWrite(left3MotorPWMSpeedChannel, 1);      
  }   
  
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  //Set up PWM for motor speed
  ledcSetup(right1MotorPWMSpeedChannel, freq, resolution);
  ledcSetup(right2MotorPWMSpeedChannel, freq, resolution);  
  ledcSetup(left3MotorPWMSpeedChannel, freq, resolution);
  ledcSetup(left4MotorPWMSpeedChannel, freq, resolution);

  ledcAttachPin(RIN1, right1MotorPWMSpeedChannel);
  ledcAttachPin(RIN2, right2MotorPWMSpeedChannel); 
  ledcAttachPin(LIN3, left3MotorPWMSpeedChannel);
  ledcAttachPin(LIN4, left4MotorPWMSpeedChannel); 
  
  rotateMotor(0, 0);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  //Check Signal lost.
  unsigned long now = millis();
  if ( now - lastRecvTime > SIGNAL_TIMEOUT ) 
  {
    Serial.println("Connection lost...");
    rotateMotor(0, 0);
  }
}
