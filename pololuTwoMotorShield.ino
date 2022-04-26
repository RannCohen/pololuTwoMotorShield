// M1 = solenoid    || Left
// M2 = Pully Motor || Right

#include "DualVNH5019MotorShield.h"
#define MAX_SPEED  400
#define SPEED  400

DualVNH5019MotorShield md;

String inputString = "";
bool stringComplete = false;


void setup()
{
  Serial.begin(115200);
  md.init();
  inputString.reserve(200);
}


void loop()
{
  if (stringComplete)
  {
    if (inputString == "o" || inputString == "O")
    {
      Serial.println("Open Basket");
      md.setSpeeds(-MAX_SPEED, -MAX_SPEED);
      stopIfFault();
    }
    else if (inputString == "c" || inputString == "C")
    {
      Serial.println("Close Basket");
      md.setSpeeds(MAX_SPEED, MAX_SPEED);
      stopIfFault();
    }
    else if (inputString == "d" || inputString == "D")
    {
      Serial.println("Going Down");
      pullyDown();
      Serial.println("Done");
    }
    else if (inputString == "u" || inputString == "U")
    {
      Serial.println("Going Up");
      for (int i = 0; i <= SPEED; i++)
      {
        md.setM2Speed(i);
        stopIfFault();
        delay(2);
      }
      Serial.println("Done");
    }
    else if (inputString == "s" || inputString == "S")
    {
      openSolenoid();
    }
    else
    {
      Serial.println("STOP");
      md.setBrakes(MAX_SPEED, MAX_SPEED);
      closeSolenoid();
    }
  }
  inputString = "";
  stringComplete = false;
  delay(10);
}



void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      inputString.trim();
      stringComplete = true;
    }
  }
}

void openSolenoid()
{
  md.setM1Speed(MAX_SPEED);
  stopIfFault();
  Serial.println("solenoid open");
}

void closeSolenoid()
{
  md.setM1Brake(MAX_SPEED);
  stopIfFault();
  Serial.println("solenoid closed");
}

void pullyDown()
{
  openSolenoid();
  for (int i = 0; i <= 200; i++)
  {
    md.setM2Speed(i);
    stopIfFault();
    if ( i == 200)
    {
      md.setM2Brake(MAX_SPEED);
      break;
    }
    delay(2);
  }
  md.setM2Speed(-SPEED);
}

void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while (1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while (1);
  }
}
