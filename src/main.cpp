#include <Arduino.h>
#include <SharpSensor.h>
#include <PololuMotor.h>
#include <PIDController.h>

Motor LeftMotor, RightMotor;
Sensor LeftSensor, MidSensor, RightSensor;

PIDController LeftPid, RightPid;
long PreviousSample = 0;
byte SampleTime = 20;

void PIDs()
{
    if(millis() - PreviousSample > SampleTime)
    {
        LeftMotor.SetRPM(LeftPid.Control(LeftMotor.GetEncoderTicks()));
        RightMotor.SetRPM(RightPid.Control(RightMotor.GetEncoderTicks()));
        PreviousSample = millis();
    }
}

bool CheckForObstacles()
{
    return (LeftSensor.GetDistance() < 15 or MidSensor.GetDistance() < 15 or RightSensor.GetDistance() < 15);
}

enum State { Forward, Stop, TurnRight };
State CurrentState = Forward;

void setup()
{
    LeftMotor.Begin(White, Left);
    RightMotor.Begin(White, Right);

    LeftSensor.Begin(LeftSensorPin);
    MidSensor.Begin(MidSensorPin);
    RightSensor.Begin(RightSensorPin);

    Serial.begin(9600);
}

long timeTurning = 0;
void loop()
{
    PIDs();
    if(CurrentState == Forward)
    {
        LeftPid.SetSpeed(8);
        RightPid.SetSpeed(8);

        if(CheckForObstacles())
            CurrentState = Stop;
    }
    else if(CurrentState == Stop)
    {
        LeftPid.SetSpeed(0);
        RightPid.SetSpeed(0);

        if(LeftPid.GetCurrentSpeed() == 0 and RightPid.GetCurrentSpeed() == 0)
            CurrentState = TurnRight;
    }
    else if(CurrentState == TurnRight)
    {
        LeftPid.SetSpeed(8);
        RightPid.SetSpeed(-8);

        if(!CheckForObstacles())
            CurrentState = Forward;
    }
}
