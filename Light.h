#pragma once

#include "Vector.h"
#include "Color.h"
#include "Source.h"


class Light : public Source{
    Vector position;
    Color color;

    public:
        Light();
        Light(Vector,Color);

        Vector getLightPosition(){return position;}
        Color getColor(){return color;}
};


Light::Light()
: position(Vector(0,0,0)), color(Color(1,0,0,1))
{
}
Light::Light(Vector p, Color c)
:position(p), color(c)
{
}



