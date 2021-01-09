#pragma once
#include "math.h"

class Vector{
    double x,y,z;

    public:
        Vector();
        Vector(double,double,double);

        double getVectorX(){return x;}
        double getVectorY(){return y;}
        double getVectorZ(){return z;}

        double getMagnitude(){
            return sqrt((x*x) + (y*y) + (z*z));
        }

        Vector getNormalized()
        {
            double magnitude = sqrt((x*x) + (y*y) + (z*z));
            return Vector(x/magnitude,y/magnitude,z/magnitude);
        }

        Vector getNegative()
        {
            return Vector(-x,-y,-z);
        }

        double getDotProductWith(Vector v)
        {
            return x*v.getVectorX() + y*v.getVectorY() + z*v.getVectorZ();
        }

        Vector getCrossProductWith(Vector v)
        {
            return Vector(y*v.getVectorZ() - z*v.getVectorY(), z*v.getVectorX() - x*v.getVectorZ(),x*v.getVectorY() - y*v.getVectorX());
        }

        Vector vectorAdd(Vector v)
        {
            return Vector(x+v.getVectorX(), y + v.getVectorY(), z + v.getVectorZ());
        }

        Vector scalarMult(double s)
        {
            return Vector(x * s, y * s, z * s);
        }
};


Vector::Vector(double i, double j, double k)
{
    x=i;
    y=j;
    z=k;
}


Vector::Vector()
{
    x=0;
    y=0;
    z=0;
}






