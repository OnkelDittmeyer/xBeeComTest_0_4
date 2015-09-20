//drone motor library
#include <DroneMotor.h>

// SoftwareSerial is used to communicate with the XBee
#include <SoftwareSerial.h>



SoftwareSerial XBee(2, 3); // Arduino RX, TX (XBee Dout, Din)
DroneMotor motorOne(13,12); //creates first motor
boolean motorOneSwitch = false;


void setup()
{
  XBee.begin(9600);
  Serial.begin(9600);
  Serial.println("Start programm");
  //printMenu(); // Print a helpful menu:

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

// printMenu
// A big ol' string of Serial prints that print a usage menu over
// to the other XBee.
void printMenu()
{
  // Everything is "F()"'d -- which stores the strings in flash.
  // That'll free up SRAM for more importanat stuff.
  XBee.println();
  XBee.println(F("Arduino XBee Remote Control!"));
  XBee.println(F("============================"));
  XBee.println(F("Usage: "));
  XBee.println(F("w#nnn - analog WRITE pin # to nnn"));
  XBee.println(F("  e.g. w6088 - write pin 6 to 88"));
  XBee.println(F("d#v   - digital WRITE pin # to v"));
  XBee.println(F("  e.g. ddh - Write pin 13 High"));
  XBee.println(F("r#    - digital READ digital pin #"));
  XBee.println(F("  e.g. r3 - Digital read pin 3"));
  XBee.println(F("a#    - analog READ analog pin #"));
  XBee.println(F("  e.g. a0 - Read analog pin 0"));
  XBee.println();
  XBee.println(F("- Use hex values for pins 10-13"));
  XBee.println(F("- Upper or lowercase works"));
  XBee.println(F("- Use 0, l, or L to write LOW"));
  XBee.println(F("- Use 1, h, or H to write HIGH"));
  XBee.println(F("============================"));
  XBee.println();
}

