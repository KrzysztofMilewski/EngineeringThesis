#ifndef RuleSet_h
#define RuleSet_h
#include <ObstacleAvoidanceAlgorithm.h>

class RuleSet : public ObstacleAvoidanceAlgorithm
{
private:
    enum class State { Forward, Stop, TurnRight};
    State currentState;

    void setLEDs(Result &r) override;

public:
    RuleSet();
    Result Run() override;
};

#endif
