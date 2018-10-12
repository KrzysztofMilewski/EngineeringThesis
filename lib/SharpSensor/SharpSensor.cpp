#include "Arduino.h"
#include "SharpSensor.h"

void Sensor::Begin(byte pin)
{
    pinAttached = pin;
    pinMode(pinAttached, INPUT);
}

//temporary
int Sensor::GetVoltage()
{
    return analogRead(pinAttached);
}

float Sensor::GetDistance()
{//Wartości testowe
    int inputVoltage = analogRead(pinAttached);
    float distance = 42.26 * exp(-0.01477 * inputVoltage) + 17.92 * exp(-0.002935*inputVoltage);
    return distance;
}
