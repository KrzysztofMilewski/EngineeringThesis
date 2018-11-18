#ifndef ObstacleAvoidanceAlgorithm_h
#define ObstacleAvoidanceAlgorithm_h

struct Result
{
    int motorSpeeds[2];
    bool diodesStates[3];
};

class ObstacleAvoidanceAlgorithm
{
protected:
    bool obstacleDetected;
    float hysteresis;
    float boundary;
    float sensorReadings[3];

    bool checkForObstacles();
    virtual void setLEDs(Result &r) = 0;

public:
    ObstacleAvoidanceAlgorithm();
    void GetDistances(float left, float middle, float right);
    void SetBoundary(float newBoundary);
    virtual Result Run() = 0;
};

#endif
