#include <Arduino.h>
#include <PololuMotor.h>
#include <SharpSensor.h>
#include <PIDController.h>
#include <RuleSet.h>
#include <DerivativeSearch.h>

Motor LeftMotor, RightMotor;
Sensor LeftSensor, MidSensor, RightSensor;
PIDController LeftPid, RightPid;

ObstacleAvoidanceAlgorithm *Algorithm;
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

void InitializeLEDs()
{
    pinMode(YellowLED, OUTPUT);
    pinMode(GreenLED, OUTPUT);
    pinMode(RedLED, OUTPUT);
}

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

void SetMotorSpeeds(int *speeds)
{
    LeftPid.SetSpeed(speeds[0]);
    RightPid.SetSpeed(speeds[1]);
}


void FlashLEDs(bool *states)
{
    digitalWrite(YellowLED, states[0]);
    digitalWrite(RedLED, states[1]);
    digitalWrite(GreenLED, states[2]);
}

void setup()
{
    LeftMotor.Begin(White, Left);
    RightMotor.Begin(White, Right);

    LeftSensor.Begin(LeftSensorPin);
    MidSensor.Begin(MidSensorPin);
    RightSensor.Begin(RightSensorPin);

    InitializeLEDs();
    Algorithm = &DerivativeSearch;
    //temporary
    Serial.begin(9600);
}

void loop()
{
    PIDs();
    Algorithm->GetDistances(LeftSensor.GetDistance(), MidSensor.GetDistance(), RightSensor.GetDistance());
    Result result = Algorithm->Run();

    SetMotorSpeeds(result.motorSpeeds);
    FlashLEDs(result.diodesStates);
}
