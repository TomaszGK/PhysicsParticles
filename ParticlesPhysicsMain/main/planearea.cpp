#include "planearea.h"

PlaneArea::PlaneArea( int _width, int _heigth, int _planeBorderWidth )
: width {_width-2*_planeBorderWidth}, height {_heigth-2*_planeBorderWidth}, planeBorderWidth {_planeBorderWidth}
{
    planeDivider = std::make_unique<PlaneDivider>(width/2,4,height,0);
}

PlaneArea::PlaneArea( vect2D _origin, double _radius, int _planeBorderWidth )
: origin {_origin}, radius {_radius}, planeBorderWidth {_planeBorderWidth}, planeDivider {nullptr}
{

}
