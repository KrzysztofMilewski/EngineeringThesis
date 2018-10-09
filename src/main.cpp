#include <Arduino.h>
#include <SharpSensor.h>
#include <PololuMotor.h>

struct PidVariables
{
private:
    long previousTicks;
    int speedSetPoint;
    float integral;
    float Kp;
    float Ki;

    void resetPid()
    {
        integral = 0;
        previousTicks = 0;
    }

public:
    PidVariables()
    {
        previousTicks = 0;
        speedSetPoint = 0;
        integral = 0;
        Kp = 0.3;
        Ki = 0.25;
    }

    void SetSpeed(int speed)
    {
        if(speed * speedSetPoint <= 0)
            resetPid();
        speedSetPoint = speed;
    }

    float Control(long currentTicks)
    {
        int speedError;
        int currentSpeed = currentTicks - previousTicks;

        if(speedSetPoint > 0)
            speedError = speedSetPoint - currentSpeed;
        else
            speedError = speedSetPoint + currentSpeed;

        integral += speedError* Ki;
        float controlSignal = Kp * speedError + integral;
        previousTicks = currentTicks;
        return controlSignal;
    }
};

Motor LeftMotor, RightMotor, f;
Sensor LeftSensor, MidSensor, RightSensor;

PidVariables LeftPid, RightPid;
long PreviousSample = 0;

void PIDs()
{
    if(millis() - PreviousSample > 20)
    {
        LeftMotor.SetRPM(LeftPid.Control(LeftMotor.GetEncoderTicks()));
        RightMotor.SetRPM(RightPid.Control(RightMotor.GetEncoderTicks()));
        PreviousSample = millis();
    }
}

// temporary
int speed = 10;

void forward()
{
    speed = - speed;
    LeftPid.SetSpeed(speed);
    RightPid.SetSpeed(speed);
}
long current = 0;
//

void setup()
{
    LeftMotor.Begin(White, Left);
    RightMotor.Begin(White, Right);

    LeftSensor.Begin(LeftSensorPin);
    MidSensor.Begin(MidSensorPin);
    RightSensor.Begin(RightSensorPin);

    Serial.begin(9600);
}


void loop()
{
    if(millis() - current > 5000)
    {
        forward();
        current = millis();
    }

    PIDs();
}
