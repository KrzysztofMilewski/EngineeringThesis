#include "PIDController.h"

PIDController::PIDController()
{
    previousTicks = 0;
    speedSetPoint = 0;
    integral = 0;
    Kp = 0.3;
    Ki = 0.25;
    changedSign = false;
}

void PIDController::SetSpeed(int speed)
{
    if(speed * speedSetPoint <= 0)
        resetPid();
    speedSetPoint = speed;
}

float PIDController::Control(long currentTicks)
{
    currentSpeed = currentTicks - previousTicks;
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

    integral += speedError * Ki;
    float controlSignal = Kp * speedError + integral;
    return controlSignal;
}

int PIDController::GetCurrentSpeed()
{
    return currentSpeed;
}

//Private members

void PIDController::resetPid()
{
    integral = 0;
    changedSign = true;
}
