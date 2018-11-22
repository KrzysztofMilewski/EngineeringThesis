#include "ObstacleAvoidanceAlgorithm.h"

ObstacleAvoidanceAlgorithm::ObstacleAvoidanceAlgorithm()
{
    hysteresis = 0.5;
    obstacleDetected = false;
}

void ObstacleAvoidanceAlgorithm::GetDistances(float left, float middle, float right)
{
    sensorReadings[0] = left;
    sensorReadings[1] = middle;
    sensorReadings[2] = right;
}

void ObstacleAvoidanceAlgorithm::SetBoundary(float newBoundary)
{
    if(newBoundary > 5 && newBoundary < 30)
        boundary = newBoundary;
}

bool ObstacleAvoidanceAlgorithm::checkForObstacles()
{
    if(obstacleDetected)
        return (sensorReadings[0] < boundary + hysteresis or
            sensorReadings[1] < boundary + hysteresis or
            sensorReadings[2] < boundary + hysteresis);
    else
        return (sensorReadings[0] < boundary - hysteresis or
            sensorReadings[1] < boundary - hysteresis or
            sensorReadings[2] < boundary - hysteresis);
}
