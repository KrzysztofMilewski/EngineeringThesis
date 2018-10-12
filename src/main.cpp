#include <Arduino.h>
#include <SharpSensor.h>
#include <PololuMotor.h>
#include <PIDController.h>

Motor LeftMotor, RightMotor;
Sensor LeftSensor, MidSensor, RightSensor;

PIDController LeftPid, RightPid;
long PreviousSample = 0;
byte SampleTime = 20;

enum State { Forward, Stop, TurnRight };
State CurrentState = Forward;
bool ObstacleDetected = false;

const byte YellowLED = 11;
const byte RedLED = 12;
const byte GreenLED = 13;

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
    float leftDistance = LeftSensor.GetDistance();
    float midDistance = MidSensor.GetDistance();
    float rightDistance = RightSensor.GetDistance();

    //temporary
    Serial.print(leftDistance);
    Serial.print(" ;");
    Serial.print(midDistance);
    Serial.print(" ;");
    Serial.println(rightDistance);

    if(ObstacleDetected)
        return (leftDistance < 15.5 or midDistance < 15.5 or rightDistance < 15.5);
    else
        return (leftDistance < 14.5 or midDistance < 14.5 or rightDistance < 14.5);
}

void SignalizeState()
{
    if(CurrentState == Forward)
    {
        digitalWrite(GreenLED, HIGH);
        digitalWrite(YellowLED, LOW);
        digitalWrite(RedLED, LOW);
    }
    else if(CurrentState == Stop)
    {
        digitalWrite(GreenLED, LOW);
        digitalWrite(YellowLED, LOW);
        digitalWrite(RedLED, HIGH);
    }
    else if(CurrentState == TurnRight)
    {
        digitalWrite(GreenLED, LOW);
        digitalWrite(YellowLED, HIGH);
        digitalWrite(RedLED, LOW);
    }
}

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

void loop()
{
    PIDs();
    SignalizeState();
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
        LeftPid.SetSpeed(5);
        RightPid.SetSpeed(-5);

        if(!ObstacleDetected)
            CurrentState = Stop;
    }
}
