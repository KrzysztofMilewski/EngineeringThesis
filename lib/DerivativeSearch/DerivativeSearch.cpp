#include "DerivativeSearch.h"
#include "Arduino.h"

DerivativeSearch::DerivativeSearch()
{
    obstacleDetected = false;
    currentState = State::Forward;
    previousMeasureSample = 0;
    previousAverageDistance = 0;
    turnFactor = 1;
}

void DerivativeSearch::Run(const float *sensorReadings, int *motorSpeeds, bool *ledStates)
{
    obstacleDetected = checkForObstacles(sensorReadings);

    if(currentState == State::Forward)
    {
        motorSpeeds[0] = 5;
        motorSpeeds[1] = 5;

        if(obstacleDetected)
        {
            currentState = State::SearchPath;
            previousAverageDistance = 0;
            turnFactor = sensorReadings[0] < sensorReadings[2] ? 1 : -1;
        }
    }
    else if(currentState == State::SearchPath)
    {
        motorSpeeds[0] = 5 * turnFactor;
        motorSpeeds[1] = -5 * turnFactor;

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
}

bool DerivativeSearch::checkForObstacles(const float *sensorReadings)
{
    if(obstacleDetected)
        return (sensorReadings[0] < 10.2 or sensorReadings[1] < 10.2 or sensorReadings[2] < 10.2);
    else
        return (sensorReadings[0] < 9.8 or sensorReadings[1] < 9.8 or sensorReadings[2] < 9.8);
}
