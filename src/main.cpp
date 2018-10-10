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

    //temporary
    Serial.begin(9600);
}

bool ObstacleDetected = false;

void loop()
{
    PIDs();
    ObstacleDetected = CheckForObstacles();

    if(CurrentState == Forward)
    {
        LeftPid.SetSpeed(8);
        RightPid.SetSpeed(8);

        if(ObstacleDetected)
            CurrentState = Stop;
    }
    else if(CurrentState == Stop)
    {
        LeftPid.SetSpeed(0);
        RightPid.SetSpeed(0);

        if(LeftPid.GetCurrentSpeed() == 0 and RightPid.GetCurrentSpeed() == 0)
            CurrentState = ObstacleDetected ? TurnRight : Forward;
    }
    else if(CurrentState == TurnRight)
    {
        LeftPid.SetSpeed(8);
        RightPid.SetSpeed(-8);

        if(!ObstacleDetected)
            CurrentState = Stop;
    }
}
