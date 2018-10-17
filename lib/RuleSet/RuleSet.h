#ifndef RuleSet_h
#define RuleSet_h

class RuleSet
{
private:
    enum class State { Forward, Stop, TurnRight};

    State currentState;
    bool obstacleDetected;

    bool checkForObstacles(const float *sensorReadings);
    void setLEDStates(bool *ledStates);

public:
    RuleSet();
    void Run(const float *sensorReadings, int *motorSpeeds, bool *ledStates);
};

#endif
