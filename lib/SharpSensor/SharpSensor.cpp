#include "Arduino.h"
#include "SharpSensor.h"

void Sensor::Begin(byte pin)
{
    pinAttached = pin;
    pinMode(pinAttached, INPUT);
}

int Sensor::GetVoltage()
{
    return analogRead(pinAttached);
}

float Sensor::GetDistance()
{
    int inputVoltage = analogRead(pinAttached);
    float distance = 71.83 * exp(-0.016 * inputVoltage) + 16.99 * exp(-0.00283 * inputVoltage);
    return distance;
}
