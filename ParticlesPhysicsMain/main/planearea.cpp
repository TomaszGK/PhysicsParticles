#include "planearea.h"

PlaneArea::PlaneArea( int width, int heigth, int planeBorderWidth )
: width {width-2*planeBorderWidth}, height {heigth-2*planeBorderWidth}, planeBorderWidth {planeBorderWidth}
{
    Ensures(getPlaneFieldConstraint()>0);
    planeDivider = std::make_unique<PlaneDivider>(width/2,4,height,0);
}

PlaneArea::PlaneArea( vect2D origin, double radius, int planeBorderWidth )
: origin {origin}, radius {radius}, planeBorderWidth {planeBorderWidth}, planeDivider {nullptr}
{

}
