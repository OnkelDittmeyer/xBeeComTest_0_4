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

//sensorTriggerPin
int sensorTriggerPin = 6;
int sensorValues[sensorNum - 1];


int sampleSize = 25;

void setup()
{
  pinMode(sensorTriggerPin, OUTPUT);
  pinMode(13,OUTPUT);

  createMotors();

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  XBee.begin(9600);
  Serial.begin(9600);
  XBee.println("Start programm");
  checkMotors();
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
  // sensor data returned is an array average of 50 for each sensor
  // data pull needs about  500ms since the sensors need to trigger after each other to avoid interferences

  if (XBee.available())
  {
    char c = XBee.read();
    switch (c)
    {

      case 'A':
        motors[0].onOff = true;
        XBee.println("got A");
        Serial.println("got A");
        digitalWrite(13, HIGH);
        
        break;

      case 'a':
        motors[0].onOff = false;
        XBee.println("got a");
        Serial.println("got a");
        digitalWrite(13, LOW);
        break;


      case '0':
        //send Sensor Data
        XBee.println(sensorAverage());
        
        
      default:
        //all off
        motors[0].onOff = false;
    }
  }




  createShiftRegNumber();
  writeMotors(regNumbers[0], regNumbers[1]);

  //send data to serial
  //check all sensors for data
}






//shiftReg functions
void createShiftRegNumber() {
  for (int i = 0; i < motorNum; i++) {
    if (motors[i].onOff) {
      switch (motors[i].state) {
        case 0:
          //turns left
          if (i < 5) {
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
          if (i < 5) {
            regNumbers[0] += motors[i].right;
          } else {
            regNumbers[1] += motors[i].right;
          };
          break;
        default:
          break;
      }
    }
  }
}

void createMotors() {
  int n = 1;

  for (int i = 1; i <= motorNum; i++) {
    DroneMotor newMotor = DroneMotor();
    newMotor.setRegNum(round(pow(2, n)), round(pow(2, (n + 1))));
    motors[i - 1] = newMotor;
    if (n < 7) {
      n = n + 2;
    } else {
      n = 1;
    }

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
