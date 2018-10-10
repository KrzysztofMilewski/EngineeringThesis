#ifndef PIDController_h
#define PIDController_h

class PIDController
{
private:
    long previousTicks;
    int speedSetPoint;
    int currentSpeed;
    float integral;
    float Kp;
    float Ki;
    bool changedSign;

    void resetPid();

public:
    PIDController();
    void SetSpeed(int speed);
    float Control(long currentTicks);
    int GetCurrentSpeed();
};

#endif
