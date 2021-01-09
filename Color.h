#pragma once
#include "math.h"

class Color{
    double red,green,blue,alpha;

    public:
        Color();
        Color(double,double,double,double);

        double getColorRed(){return red;}
        double getColorGreen(){return green;}
        double getColorBlue(){return blue;}
        double getColorAlpha(){return alpha;}

        void setColorRed(double redVal){ red = redVal;}
        void setColorGreen(double greenVal){green = greenVal;}
        void setColorBlue(double blueVal){blue = blueVal;}
        void setColorAlpha(double alphaVal){alpha = alphaVal;}

};


Color::Color(double r, double g, double b, double a)
:red(r), green(g), blue(b), alpha(a)
{
} 


Color::Color()
:red(0),green(0),blue(0),alpha(1)
{
}





