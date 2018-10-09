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

  void Begin(byte pin); //Konfiguracja - parametr to położenie czujnika
  int GetVoltage();     //Zwraca napięcia z wyjścia czujnika (charakterystyka eksponencjalna)
  float GetDistance();  //Do zrobienia ;)
  
};

#endif
