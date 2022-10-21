#include "polygon.h"

Polygon::Polygon()
{

}

bool operator ==(const Polygon& p1, const Polygon& p2)
{
    // this is very primitive, but fast. Probably it can be done more specific.
    if(p1.Points != p2.Points)
        return false;
   return true;
}
