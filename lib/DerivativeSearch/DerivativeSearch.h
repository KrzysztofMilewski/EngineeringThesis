#ifndef DerivativeSearch_h
#define DerivativeSearch_h

class DerivativeSearch
{
private:
    enum class State {Forward, SearchPath};

    State currentState;
    long previousMeasureSample;
    float previousAverageDistance;
    int turnFactor;
    bool obstacleDetected;

    bool checkForObstacles(const float *sensorReadings);
public:
    DerivativeSearch();
    void Run(const float *sensorReadings, int *motorSpeeds, bool *ledStates);
};

#endif
