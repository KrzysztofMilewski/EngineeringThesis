#ifndef SharpSensor_h
#define SharpSensor_h
#include "Arduino.h"

const byte LeftSensorPin = A2;
const byte MidSensorPin = A1;
const byte RightSensorPin = A0;

class Sensor
{
private:

  byte pinAttached;

public:

  void Begin(byte pin); 
  int GetVoltage();
  float GetDistance();

};

#endif
