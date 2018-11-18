#ifndef DerivativeSearch_h
#define DerivativeSearch_h
#include <ObstacleAvoidanceAlgorithm.h>

class DerivativeSearch : public ObstacleAvoidanceAlgorithm
{
private:
    enum class State {Forward, SearchPath};

    State currentState;
    long previousMeasureSample;
    float previousAverageDistance;
    int turnFactor;

    void setLEDs(Result &r) override;

public:
    DerivativeSearch();
    Result Run() override;
};

#endif
