//drone motor library
#include <DroneMotor.h>

#include <math.h>

// SoftwareSerial is used to communicate with the XBee
#include <SoftwareSerial.h>

#define sensorNum 6
#define motorNum 6


//receivingSerial
SoftwareSerial XBee(2, 3); // Arduino RX, TX (XBee Dout, Din)
//sendingSerial


//shiftReg setup
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//motor setup
DroneMotor motors[motorNum];
//numbers send to shiftReg 1&2
long regNumbers[2];
int motorRegRef[12] = {1, 2, 8, 4, 16, 32, 128, 64, 1, 2, 8, 4};

//sensorTriggerPin
int sensorTriggerPin = 6;
int sensorValues[sensorNum];

long time;
long lastTrigger;

int sampleSize = 25;

void setup()
{
  pinMode(sensorTriggerPin, OUTPUT);
  pinMode(13, OUTPUT);

  createMotors();

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  XBee.begin(9600);
  Serial.begin(9600);
  XBee.println("Start programm");
  //checkMotors();

  blinkAll_2Bytes(3, 500);

}

void loop()
{
  //receiveData.println("Online");

  //update all motor animation states
  for (int i = 0; i < motorNum; i++) {
    motors[i].update();
  }


  //structure
  //   letters: A,B,C,D,E,F switch specific motor to on
  //  motors remain active as long as set to false again
  //   letters: a,b,c,d,e,f switch specific motor to off
  //  '0' pulls sensor data as string csv
  //  '1' turns all motors off
  // sensor data returned is an array[50] average for each sensor
  // data pull needs about  600ms since the sensors need to trigger after each other to avoid interferences

  if (XBee.available())
  {
    char c = XBee.read();
    switch (c)
    {

      case 'A':
        motors[0].onOff = true;
        //XBee.println("got A");
        //XBee.println(motors[0].onOff);
        break;
      case 'a':
        motors[0].onOff = false;
        //XBee.println("got a");
        //XBee.println(motors[0].onOff);
        break;
      case 'B':
        motors[1].onOff = true;
        break;
      case 'b':
        motors[1].onOff = false;
        break;
      case 'C':
        motors[2].onOff = true;
        break;
      case 'c':
        motors[2].onOff = false;
        break;
      case 'D':
        motors[3].onOff = true;
        break;
      case 'd':
        motors[3].onOff = false;
        break;
      case 'E':
        motors[4].onOff = true;
        break;
      case 'e':
        motors[4].onOff = false;
        break;  
      case 'F':
        motors[5].onOff = true;
        break;
      case 'f':
        motors[5].onOff = false;
        break;        

      case '0':
        //send Sensor Data
        //   sensors can only trigger every 600ms since they have to finish their trigger cycle
        //  to avoid interferences
        //XBee.println("Sensor Data");
        time = millis();
        if(time - lastTrigger > 600){
                  startSensor();
                  readSensor();
                  //delay(600);
                  lastTrigger = millis();
                  XBee.println(sensorIntoString());  
        }else{
                  XBee.println("sensors not ready yet");  

        }

        break;
      
      case '1':
        //allOff
        allOff();
        break;

      default:
        //all off
        allOff();
    }
  }




  createShiftRegNumber();
  writeMotors(regNumbers[1], regNumbers[0]);

  //send data to serial
  //check all sensors for data
}






//shiftReg functions
void createShiftRegNumber() {
  regNumbers[0] = 0;
  regNumbers[1] = 0;

  for (int i = 0; i < motorNum; i++) {
    //Serial.print("OnOff: ");
    //Serial.println(motors[i].onOff);

    if (motors[i].onOff) {
      Serial.print("State: ");
      Serial.println(motors[i].state);
      Serial.print("Left: ");
      Serial.println(motors[i].left);
      Serial.print("Right: ");
      Serial.println(motors[i].right);
      switch (motors[i].state) {
        case 0:
          //turns left
          if (i <= 3) {
            regNumbers[0] += motors[i].left;
          } else {
            regNumbers[1] += motors[i].left;
          };
          break;
        case 1:
          //is off
          break;
        case 2:
          //turns right
          if (i <=3) {
            regNumbers[0] += motors[i].right;
          } else {
            regNumbers[1] += motors[i].right;
          };
          break;
        default:
          break;
      }
      Serial.print("Send Reg numbers: ");
      Serial.print(regNumbers[0]);
      Serial.print(" & ");
      Serial.println(regNumbers[1]);
      Serial.println();
    }
  }

}

void createMotors() {
  int n = 0;

  for (int i = 0; i < motorNum; i++) {
    DroneMotor newMotor = DroneMotor();
    newMotor.setRegNum(motorRegRef[n], motorRegRef[n + 1]);
    motors[i] = newMotor;
    n += 2;
  }

}

void checkMotors() {

  for (int i = 0; i < motorNum; i++) {
    Serial.print("Motor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(motors[i].onOff);
    Serial.print(" , " );
    Serial.print(motors[i].left);
    Serial.print(" , " );
    Serial.println(motors[i].right);
  }
}

void allOff(){
  for (int i = 0; i < motorNum; i++) {
      motors[i].onOff = false;
  }

}
