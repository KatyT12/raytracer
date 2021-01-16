#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


struct rgbType{
    double r;
    double g;
    double b;
};

inline void savebmp(const char* filename, int w, int h, int dpi, rgbType *data)
{
    FILE* f;
    int k = w*h;
    int s = 4*k;
    int filesize =  54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = m * dpi;
    unsigned char  bpmfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bpminfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bpmfileheader[2] = (unsigned char) (filesize);
    bpmfileheader[3] = (unsigned char) (filesize >> 8);
    bpmfileheader[4] = (unsigned char) (filesize >> 16);
    bpmfileheader[5] = (unsigned char) (filesize >> 24);
     
    bpminfoheader[4] = (unsigned char) (w);
    bpminfoheader[5] = (unsigned char) (w >> 8);
    bpminfoheader[6] = (unsigned char) (w >> 16);
    bpminfoheader[7] = (unsigned char) (w >> 24);

    bpminfoheader[8] = (unsigned char) (h);
    bpminfoheader[9] = (unsigned char) (h >> 8);
    bpminfoheader[10] = (unsigned char) (h >> 16);
    bpminfoheader[11] = (unsigned char) (h >> 24);

    bpminfoheader[21] = (unsigned char) (s);
    bpminfoheader[22] = (unsigned char) (s >> 8);
    bpminfoheader[23] = (unsigned char) (s >> 16);
    bpminfoheader[24] = (unsigned char) (s >> 24);

    bpminfoheader[25] = (unsigned char) (ppm);
    bpminfoheader[26] = (unsigned char) (ppm >> 8);
    bpminfoheader[27] = (unsigned char) (ppm >> 16);
    bpminfoheader[28] = (unsigned char) (ppm >> 24);

    bpminfoheader[29] = (unsigned char) (ppm);
    bpminfoheader[30] = (unsigned char) (ppm >> 8);
    bpminfoheader[31] = (unsigned char) (ppm >> 16);
    bpminfoheader[32] = (unsigned char) (ppm >> 24);

    f = fopen(filename,"wb");
    fwrite(bpmfileheader,1,14,f);
    fwrite(bpminfoheader,1,40,f);

    for(int i = 0; i < k; i++)
    {
        rgbType rgb = data[i];

        double red = rgb.r * 255;
        double green = rgb.g * 255;
        double blue = rgb.b * 255;


        unsigned char color[3] = {(int)floor(blue), (int)floor(green), (int)floor(red)};
        fwrite(color,1,3,f);

    }

    fclose(f);

}