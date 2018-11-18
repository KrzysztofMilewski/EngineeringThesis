#include <Arduino.h>
#include <PololuMotor.h>
#include <SharpSensor.h>
#include <PIDController.h>
#include <RuleSet.h>
#include <DerivativeSearch.h>

Motor LeftMotor, RightMotor;
Sensor LeftSensor, MidSensor, RightSensor;
PIDController LeftPid, RightPid;
RuleSet RuleSet;
DerivativeSearch DerivativeSearch;

long PreviousSample = 0;
byte SampleTime = 20;

float Distances[3];
int MotorSpeeds[2];
bool LEDStates[3];

const byte YellowLED = 11;
const byte RedLED = 12;
const byte GreenLED = 13;

void PIDs()
{
    if(millis() - PreviousSample > SampleTime)
    {
        LeftMotor.SetPWM(LeftPid.Control(LeftMotor.GetEncoderTicks()));
        RightMotor.SetPWM(RightPid.Control(RightMotor.GetEncoderTicks()));
        PreviousSample = millis();
    }
}

void GetDistances()
{
    Distances[0] = LeftSensor.GetDistance();
    Distances[1] = MidSensor.GetDistance();
    Distances[2] = RightSensor.GetDistance();
}

void SetMotorSpeeds()
{
    LeftPid.SetSpeed(MotorSpeeds[0]);
    RightPid.SetSpeed(MotorSpeeds[1]);
}

void InitializeLEDs()
{
    pinMode(YellowLED, OUTPUT);
    pinMode(GreenLED, OUTPUT);
    pinMode(RedLED, OUTPUT);
}

void FlashLEDs()
{
    digitalWrite(YellowLED, LEDStates[0]);
    digitalWrite(RedLED, LEDStates[1]);
    digitalWrite(GreenLED, LEDStates[2]);
}

void setup()
{
    LeftMotor.Begin(White, Left);
    RightMotor.Begin(White, Right);

    LeftSensor.Begin(LeftSensorPin);
    MidSensor.Begin(MidSensorPin);
    RightSensor.Begin(RightSensorPin);

    InitializeLEDs();

    //temporary
    Serial.begin(9600);
}

bool ObstacleDetected;

bool CheckForObstacles()
{
    if(ObstacleDetected)
        return (Distances[0] < 8.2 or Distances[1] < 8.2 or Distances[2] < 8.2);
    else
        return (Distances[0] < 7.8 or Distances[1] < 7.8 or Distances[2] < 7.8);
}

enum State {Forward, Search};
State state = Forward;
unsigned long lastMeasure = 0;
float LastAverage = 0;
int wsp = 1;

void loop()
{
    PIDs();
    GetDistances();

    DerivativeSearch.Run(Distances, MotorSpeeds, LEDStates);

    // ObstacleDetected = CheckForObstacles();
    //
    // if(state == Forward)
    // {
    //     RightPid.SetSpeed(5);
    //     LeftPid.SetSpeed(5);
    //
    //     digitalWrite(GreenLED, 1);
    //     digitalWrite(RedLED, 0);
    //
    //     if(ObstacleDetected)
    //     {
    //         state = Search;
    //         LastAverage = 0;
    //         if(Distances[0] < Distances[2])
    //             wsp = -1;
    //         else
    //             wsp = 1;
    //     }
    // }
    // else if(state == Search)
    // {
    //     digitalWrite(GreenLED, 0);
    //     digitalWrite(RedLED, 1);
    //
    //     RightPid.SetSpeed(5 * wsp);
    //     LeftPid.SetSpeed(-5 * wsp);
    //
    //     if(millis() - lastMeasure > 20)
    //     {
    //         float averageDistance = (Distances[0] + Distances[1] + Distances[2]) / 3.00;
    //         float derivative = averageDistance - LastAverage;
    //
    //         if(derivative < 1.5 && derivative > 0 && !ObstacleDetected)
    //             state = Forward;
    //
    //         LastAverage = averageDistance;
    //         lastMeasure = millis();
    //     }
    // }

    SetMotorSpeeds();
    FlashLEDs();
}
