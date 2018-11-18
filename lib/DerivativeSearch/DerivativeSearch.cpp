#include "DerivativeSearch.h"
#include "Arduino.h"

DerivativeSearch::DerivativeSearch()
{
    currentState = State::Forward;
    previousMeasureSample = 0;
    previousAverageDistance = 0;
    turnFactor = 1;
    boundary = 10;
}

Result DerivativeSearch::Run()
{
    obstacleDetected = checkForObstacles();

    Result result;

    if(currentState == State::Forward)
    {
        result.motorSpeeds[0] = 5;
        result.motorSpeeds[1] = 5;

        if(obstacleDetected)
        {
            currentState = State::SearchPath;
            previousAverageDistance = 0;
            turnFactor = sensorReadings[0] < sensorReadings[2] ? 1 : -1;
        }
    }
    else if(currentState == State::SearchPath)
    {
        result.motorSpeeds[0] = 5 * turnFactor;
        result.motorSpeeds[1] = -5 * turnFactor;

        if(millis() - previousMeasureSample > 20)
        {
            float averageDistance = (sensorReadings[0] + sensorReadings[1] + sensorReadings[2]) / 3;
            float derivative = averageDistance - previousAverageDistance;

            if(derivative < 1.5 && derivative > 0 && !obstacleDetected)
                currentState = State::Forward;

            previousAverageDistance = averageDistance;
            previousMeasureSample = millis();
        }
    }
    setLEDs(result);
    return result;
}

void DerivativeSearch::setLEDs(Result &r)
{
    if(currentState == State::Forward)
    {
        r.diodesStates[0] = 0;
        r.diodesStates[1] = 0;
        r.diodesStates[2] = 1;
    }
    else
    {
        if(turnFactor > 0)
        {
            r.diodesStates[0] = 0;
            r.diodesStates[1] = 1;
            r.diodesStates[2] = 0;
        }
        else
        {
            r.diodesStates[0] = 1;
            r.diodesStates[1] = 0;
            r.diodesStates[2] = 0;
        }
    }
}
