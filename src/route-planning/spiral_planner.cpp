#include "spiral_planner.h"

SpiralPlanner::SpiralPlanner(double radius)
    :_radius(radius)
{

}
Coordinates SpiralPlanner::GetNewCoordinates()
{
    auto UpdatedCoords = Coordinates{
        (_radius / (double)3.14) * _u * cos(-_u),
        (_radius / (double)3.14) * _u * sin(-_u)
    };

    _u += 3.14/4;

    return UpdatedCoords;
}