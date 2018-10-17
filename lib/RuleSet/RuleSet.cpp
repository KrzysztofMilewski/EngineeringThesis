#include "RuleSet.h"
#include "Arduino.h"

RuleSet::RuleSet()
{
    currentState = State::Forward;
    obstacleDetected = false;
}

void RuleSet::Run(const float *sensorReadings, int *motorSpeeds, bool *ledStates)
{
    obstacleDetected = checkForObstacles(sensorReadings);
    if(currentState == State::Forward)
    {
        motorSpeeds[0] = 8;
        motorSpeeds[1] = 8;

        if(obstacleDetected)
            currentState = State::Stop;
    }
    else if(currentState == State::Stop)
    {
        motorSpeeds[0] = 0;
        motorSpeeds[1] = 0;

        currentState = obstacleDetected ? State::TurnRight : State::Forward;
    }
    else if(currentState == State::TurnRight)
    {
        motorSpeeds[0] = 5;
        motorSpeeds[1] = -5;

        if(!obstacleDetected)
            currentState = State::Stop;
    }

    setLEDStates(ledStates);
}

//private members

bool RuleSet::checkForObstacles(const float *sensorReadings)
{
    if(obstacleDetected)
        return (sensorReadings[0] < 15.8 or sensorReadings[1] < 15.8 or sensorReadings[2] < 15.8);
    else
        return (sensorReadings[0] < 14.2 or sensorReadings[1] < 14.2 or sensorReadings[2] < 14.2);
}

void RuleSet::setLEDStates(bool *ledStates)
{
    if(currentState == State::Forward)
    {
        ledStates[0] = 0;
        ledStates[1] = 0;
        ledStates[2] = 1;
    }
    else if(currentState == State::Stop)
    {
        ledStates[0] = 0;
        ledStates[1] = 1;
        ledStates[2] = 0;
    }
    else if(currentState == State::TurnRight)
    {
        ledStates[0] = 1;
        ledStates[1] = 0;
        ledStates[2] = 0;
    }
}
