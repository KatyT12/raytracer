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

#include "Vector.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"



struct rgbType{
    double r;
    double g;
    double b;
};

void savebmp(const char* filename, int w, int h, int dpi, rgbType *data)
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

//Returns index winning object
int winningObjectIndex(std::vector<double> intersections)
{
    //Return the index of the winning intersection
    int iMinVal;

    //prevent unecessary calculations
    
    if(intersections.size() == 0)
    {   
        return  -1; //No intersections
    }
    
    else if(intersections.size() == 1)
    {
        if(intersections[0] >  0) //If Greater than zero it is in the right range
        {
            return 0;            
        }
        else{
            return -1;
        }

    }

    //More than one intersection
    else{
        double max = 0;
        //Find max value
        for(int i =0; i < intersections.size(); i++)
        {
             if(max < intersections[i]){
                 max = intersections[i];
             }
        }
        
        if(max > 0) 
        {
            for (int i = 0; i < intersections.size(); i++)
            {
                if(intersections[i] > 0 && intersections[i] <= max) // Want the smallest value (above 0) for the winning intersection
                {   
                    max = intersections[i];
                    iMinVal = i;
                }
            }
            return iMinVal;
        }
        else {
        return -1;}

    }

}


int thisone;


int main()
{
    std::cout << "Raytracing\n";
    

    int dpi = 72;
    int width = 640;
    int height = 480;
    double aspectRatio = (double)width / (double) height;


    int n  = width * height;
    rgbType* pixels = new rgbType[n];

    Vector O {0,0,0};
    Vector X(1,0,0);
    Vector Y(0,1,0); 
    Vector Z(0,0,1); 
     
    Vector CameraPosition(0,0,4);

    Camera scene_cam;
    scene_cam.lookAt(CameraPosition,O,Y);

    Color whiteLight(1.0,1.0,1.0,1.0);
    Color cool_green(0.5,1.0,0.5,1.0);
    Color gray(0.5,0.5,0.5,0.0);
    Color maroon(0.5,0.25,0.25,0);

    Vector lightPosition(-7,10,10);
    Light scene_light(lightPosition,whiteLight);

    //Scene objects
    Sphere scene_Sphere(O,1,cool_green);//Sphere is at origin 
    Plane scene_plane(Y,-1,maroon);//Directly beneath sphere

    std::vector<Object*> sceneObjects;
    sceneObjects.push_back(dynamic_cast<Object*>(&scene_Sphere));
    sceneObjects.push_back(dynamic_cast<Object*>(&scene_plane));


    double xamnt, yamnt;


    for(int x =0; x < width; x++)
    {
        for(int y = 0; y < height;y++)
        {

            thisone = y*width + x;


            /* I feel like xamnt and yamnt are like where on the view plane or whatever*/

            //Start  with no anti-aliasing
            if(width > height)
            {
                //The image is wider than it is tall
                xamnt = ((x + 0.5)/width) * aspectRatio - (((width - height)/ (double) height)/2);
                yamnt = ((height - y) + 0.5)/height;
            }
            else if (height >  width)
            {
                //The image is taller than it is wide
                xamnt = (x + 0.5)/width;
                yamnt = (((height - y) + 0.5)/height)/aspectRatio - (((height - width)/ (double) width)/2);
            }
            else
            {
                
                //The image is square
                xamnt = (x + 0.5)/width;
                yamnt = ((height-y) + 0.5)/height;
            }
         
            Vector cameraRayOrigin = scene_cam.getCameraPosition();
            //I am pretty sure here we are saying  the direction of the camera ray going through the viewplane. That is why we minus 0.5 to each because the camera is in the middle not the top right
            Vector cameraRayDirection = scene_cam.getCameraDirection().vectorAdd(scene_cam.getCameraRight().scalarMult(xamnt - 0.5).vectorAdd(scene_cam.getCameraDown().scalarMult(yamnt - 0.5))).getNormalized();

            Ray cameraRay(cameraRayOrigin,cameraRayDirection);

            //Find intersections between camera ray and objects
            std::vector<double> intersections;
            for(int i = 0; i < sceneObjects.size(); i++)
            {
                intersections.push_back(sceneObjects[i]->findIntersection(cameraRay));
            }

            int indexOfWinningObjects = winningObjectIndex(intersections);


            if(indexOfWinningObjects == -1) //No intersection
            {
                pixels[thisone].r = 0;
                pixels[thisone].g = 0;
                pixels[thisone].b = 0;
            }
            else{
                Color color = sceneObjects[indexOfWinningObjects]->getColor();
                pixels[thisone].r = color.getColorRed();
                pixels[thisone].g = color.getColorGreen();
                pixels[thisone].b = color.getColorBlue();

            }
        }
    }
    
    savebmp("test.bmp",width,height,dpi,pixels);    
    
    return 0;
}