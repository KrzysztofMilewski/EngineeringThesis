#include "RuleSet.h"
#include "Arduino.h"

RuleSet::RuleSet()
{
    currentState = State::Forward;
    boundary = 15;
}

Result RuleSet::Run()
{
    obstacleDetected = checkForObstacles();

    Result result;

    if(currentState == State::Forward)
    {
        result.motorSpeeds[0] = 8;
        result.motorSpeeds[1] = 8;

        if(obstacleDetected)
            currentState = State::Stop;
    }
    else if(currentState == State::Stop)
    {
        result.motorSpeeds[0] = 0;
        result.motorSpeeds[1] = 0;

        currentState = obstacleDetected ? State::TurnRight : State::Forward;
    }
    else if(currentState == State::TurnRight)
    {
        result.motorSpeeds[0] = 5;
        result.motorSpeeds[1] = -5;

        if(!obstacleDetected)
            currentState = State::Stop;
    }

    setLEDs(result);
    return result;
}

//private members

void RuleSet::setLEDs(Result &r)
{
    if(currentState == State::Forward)
    {
        r.diodesStates[0] = 0;
        r.diodesStates[1] = 0;
        r.diodesStates[2] = 1;
    }
    else if(currentState == State::Stop)
    {
        r.diodesStates[0] = 0;
        r.diodesStates[1] = 1;
        r.diodesStates[2] = 0;
    }
    else if(currentState == State::TurnRight)
    {
        r.diodesStates[0] = 1;
        r.diodesStates[1] = 0;
        r.diodesStates[2] = 0;
    }
}
