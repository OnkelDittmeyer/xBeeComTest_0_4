//drone motor library
#include <DroneMotor.h>

// SoftwareSerial is used to communicate with the XBee
#include <SoftwareSerial.h>


SoftwareSerial XBee(2, 3); // Arduino RX, TX (XBee Dout, Din)
DroneMotor motorOne(13,12); //creates first motor, attached to pin 13 & 12
boolean motorOneSwitch = false;


void setup()
{
  XBee.begin(9600);
  Serial.begin(9600);
  Serial.println("Start programm");
}

void loop()
{

  if (XBee.available())
  {
    char c = XBee.read();
    switch (c)
    {

      case '0':
        XBee.print("Activate Flipper One");
        motorOneSwitch = true;
        motorOne.motorTest();        
        break;

      case '1':
        XBee.print("Turn motor1 right");
        break;
        
     default:
       motorOneSwitch = false;
    }
  }
  //motorOne.flapping(true);

  
}



// ASCIItoHL
// Helper function to turn an ASCII value into either HIGH or LOW
int ASCIItoHL(char c)
{
  // If received 0, byte value 0, L, or l: return LOW
  // If received 1, byte value 1, H, or h: return HIGH
  if ((c == '0') || (c == 0) || (c == 'L') || (c == 'l'))
    return LOW;
  else if ((c == '1') || (c == 1) || (c == 'H') || (c == 'h'))
    return HIGH;
  else
    return -1;
}

// ASCIItoInt
// Helper function to turn an ASCII hex value into a 0-15 byte val
int ASCIItoInt(char c)
{
  if ((c >= '0') && (c <= '9'))
    return c - 0x30; // Minus 0x30
  else if ((c >= 'A') && (c <= 'F'))
    return c - 0x37; // Minus 0x41 plus 0x0A
  else if ((c >= 'a') && (c <= 'f'))
    return c - 0x57; // Minus 0x61 plus 0x0A
  else
    return -1;
}



