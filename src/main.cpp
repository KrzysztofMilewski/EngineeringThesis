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
    bool changedSign;

    void resetPid()
    {
        integral = 0;
        previousTicks = 0;
        changedSign = true;
    }

public:
    PidVariables()
    {
        previousTicks = 0;
        speedSetPoint = 0;
        integral = 0;
        Kp = 0.3;
        Ki = 0.25;
        changedSign = false;
    }

    void SetSpeed(int speed)
    {
        if(speed * speedSetPoint <= 0)
            resetPid();
        speedSetPoint = speed;
    }

    float Control(long currentTicks)
    {
        int currentSpeed = currentTicks - previousTicks;
        previousTicks = currentTicks;

        if(changedSign)
        {
            if(currentSpeed == 0)
                changedSign = false;
            return 0;
        }

        int speedError;
        if(speedSetPoint > 0)
            speedError = speedSetPoint - currentSpeed;
        else
            speedError = speedSetPoint + currentSpeed;

        integral += speedError* Ki;
        float controlSignal = Kp * speedError + integral;
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
    PIDs();
}
