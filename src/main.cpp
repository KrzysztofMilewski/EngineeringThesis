#include <Arduino.h>
#include <PololuMotor.h>
#include <SharpSensor.h>
#include <PIDController.h>
#include <RuleSet.h>

Motor LeftMotor, RightMotor;
Sensor LeftSensor, MidSensor, RightSensor;
PIDController LeftPid, RightPid;
RuleSet RuleSet;

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
        LeftMotor.SetRPM(LeftPid.Control(LeftMotor.GetEncoderTicks()));
        RightMotor.SetRPM(RightPid.Control(RightMotor.GetEncoderTicks()));
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

void loop()
{
    PIDs();
    GetDistances();

    RuleSet.Run(Distances, MotorSpeeds, LEDStates);

    SetMotorSpeeds();
    FlashLEDs();
}
