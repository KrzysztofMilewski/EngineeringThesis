#ifndef PololuMotor_h
#define PololuMotor_h
#include "Arduino.h"

const byte Left = 2;
const byte Right = 3;
const byte White = 0;
const byte Red = 1;

class Motor
{
private:
  byte controlPin1;
  byte controlPin2;
  volatile long encoderTicks;
  static Motor *instanceLeft;
  static Motor *instanceRight;

  void SetUpEncoder(byte side);
  void IncrementTickNumber();
  static void ISRLeft();
  static void ISRRight();
  
public:

  void Begin(byte colour, byte side); //Konfiguracja silnika - parametry to kolor felg robota oraz strona po której jest dany silnik
  void SetRPM(int rpm);               //Ustawienie zadanej prędkości
  long GetEncoderTicks();             //Zwraca wartość zmiennej w której jest zapisana ilość impulsów z enkodera
};

#endif
