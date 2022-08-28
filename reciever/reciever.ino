#include <esp_now.h>
#include <WiFi.h>

#define MAX_MOTOR_SPEED 200
#define SIGNAL_TIMEOUT 1000  //ms
unsigned long lastRecvTime = 0;

// Motor R
int motorR_Pin1 = 16; 
int motorR_Pin2 = 17; 
int enableR_Pin = 22; 

// Motor L
int motorL_Pin1 = 18; 
int motorL_Pin2 = 19; 
int enableL_Pin = 23; 

// Setting PWM properties
const int freq = 1000;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;
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
  if (rightMotorSpeed < 0)
  {
    digitalWrite(motorR_Pin1,LOW);
    digitalWrite(motorR_Pin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(motorR_Pin1,HIGH);
    digitalWrite(motorR_Pin2,LOW);      
  }
  else
  {
    digitalWrite(motorR_Pin1,LOW);
    digitalWrite(motorR_Pin2,LOW);      
  }
  
  if (leftMotorSpeed < 0)
  {
    digitalWrite(motorL_Pin1,LOW);
    digitalWrite(motorL_Pin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(motorL_Pin1,HIGH);
    digitalWrite(motorL_Pin2,LOW);      
  }
  else
  {
    digitalWrite(motorL_Pin1,LOW);
    digitalWrite(motorL_Pin2,LOW);      
  } 

  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));    
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(enableR_Pin,OUTPUT);
  pinMode(motorR_Pin1,OUTPUT);
  pinMode(motorR_Pin2,OUTPUT);
  
  pinMode(enableL_Pin,OUTPUT);
  pinMode(motorL_Pin1,OUTPUT);
  pinMode(motorL_Pin2,OUTPUT);

  //Set up PWM for motor speed
  ledcSetup(rightMotorPWMSpeedChannel, freq, resolution);
  ledcSetup(leftMotorPWMSpeedChannel, freq, resolution);  
  ledcAttachPin(enableR_Pin, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableL_Pin, leftMotorPWMSpeedChannel); 
  
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
