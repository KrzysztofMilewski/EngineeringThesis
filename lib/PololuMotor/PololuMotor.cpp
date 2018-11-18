#include "Arduino.h"
#include "PololuMotor.h"

Motor * Motor::instanceLeft;
Motor * Motor::instanceRight;

void Motor::Begin(byte colour, byte side)
{
    SetUpEncoder(side);

    if(colour == White and side == Left)
    {
      controlPin1 = 5;
      controlPin2 = 6;
    }
    else if((colour == White and side == Right) or (colour == Red and side == Right))
    {
      controlPin1 = 9;
      controlPin2 = 10;
    }
    else if(colour == Red and side == Left)
    {
      controlPin1 = 6;
      controlPin2 = 5;
    }
    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, LOW);
}

void Motor::SetPWM(int duty)
{
  if(duty>0)
  {
    digitalWrite(controlPin1, LOW);
    analogWrite(controlPin2, duty);
  }
  else
  {
    digitalWrite(controlPin2, LOW);
    analogWrite(controlPin1, -duty);
  }
}

long Motor::GetEncoderTicks()
{
  return encoderTicks;
}


//Private Members
void Motor::SetUpEncoder(byte side)
{
    if(side == Left)
    {
      attachInterrupt(digitalPinToInterrupt(Left),ISRLeft,CHANGE);
      instanceLeft = this;
    }
    else if(side == Right)
    {
      attachInterrupt(digitalPinToInterrupt(Right),ISRRight,CHANGE);
      instanceRight = this;
    }
}

void Motor::IncrementTickNumber()
{
  encoderTicks++;
}

void Motor::ISRLeft()
{
  instanceLeft->IncrementTickNumber();
}

void Motor::ISRRight()
{
  instanceRight->IncrementTickNumber();
}
