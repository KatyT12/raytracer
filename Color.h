#pragma once
#include "math.h"

class Color{
    double red,green,blue,special;

    public:
        Color();
        Color(double,double,double,double);

        double getColorRed(){return red;}
        double getColorGreen(){return green;}
        double getColorBlue(){return blue;}
        double getColorSpecial(){return special;}

        void setColorRed(double redVal){ red = redVal;}
        void setColorGreen(double greenVal){green = greenVal;}
        void setColorBlue(double blueVal){blue = blueVal;}
        void setColorSpecial(double specialVal){special = specialVal;}

        double getBrightness(){
            return (red + green + blue)/3;
        }

        Color scale(double val)
        {
            return Color (red*val, green*val,blue*val,special);
        }


        Color addColor(Color color)
        {
            return Color(red+color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(),special);
        }
        Color multiplyColor(Color color)
        {
            return Color(red*color.getColorRed(),green + color.getColorGreen(), blue * color.getColorBlue(),special);
        }

        Color colorAverage(Color color)
        {
            return Color((red*color.getColorRed())/2,(green + color.getColorGreen()/2), (blue * color.getColorBlue()/2),special);
        }

};


Color::Color(double r, double g, double b, double a)
:red(r), green(g), blue(b), special(a)
{
} 


Color::Color()
:red(0),green(0),blue(0),special(1)
{
}





