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
#include "World.h"
#include "Source.h"

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


Color getColorAt(Vector intersectionPos,Vector intersectionDir,std::vector<Object*> worldObjects,int indexOfWinningObjects,std::vector<Source*>worldLights,double accuracy,double ambientLight)
{
    Color final_color = worldObjects[indexOfWinningObjects]->getColor().scale(ambientLight); //If in shadow then it will just have the ambient
    
       
    
    Vector intersectionNormal = worldObjects[indexOfWinningObjects]->getNormalAt(intersectionPos);

    Color winningObjCol = worldObjects[indexOfWinningObjects]->getColor();

        

    for(int l_index = 0; l_index < worldLights.size();l_index++)
    {
        
        Vector lightdir = worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative()).getNormalized(); //Vector from intersection to light

        float dotProduct = intersectionNormal.getDotProductWith(lightdir);
        
        

        if(dotProduct > 0) //90 degrees or less different from the normal
        { 

            bool inShadow = false; //Not in shadow by default
            float distanceToLight = worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative()).getMagnitude();

            Ray shadowRay(intersectionPos,(worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative())).getNormalized()); 

            std::vector<double> shadowIntersections;
            /*If there is an intersection between the first intersection and the light source the the first intersection is in shadow*/
            for(int obj_index = 0; obj_index < worldObjects.size() && inShadow == false; obj_index++)
            {
                if(obj_index != indexOfWinningObjects)
                {
                    shadowIntersections.push_back(worldObjects[obj_index]->findIntersection(shadowRay)); 
                }
            }
            for(int s_i_index = 0; s_i_index < shadowIntersections.size();s_i_index++)
            {
                if(shadowIntersections[s_i_index] > accuracy)
                {
                    if(shadowIntersections[s_i_index] <= distanceToLight)
                    {
                        inShadow = true;
                    }
                }
            }
   
            if(inShadow == false)
            {
             
                final_color = final_color.addColor(winningObjCol.multiplyColor(worldLights[l_index]->getColor().scale(dotProduct))); //diffuse color
                final_color.Clamp();
                //final_color.Clamp();             
              
                //if(worldObjects[indexOfWinningObjects]->getColor().getColorSpecial() > 0 && worldObjects[indexOfWinningObjects]->getColor().getColorSpecial() < 1)
                //{
                //    double dot1 = intersectionNormal.getDotProductWith(intersectionDir.getNegative());
                //    Vector scalar = intersectionNormal.scalarMult(dot1);
                //    Vector
                //}
            }            
        }
    
    }
    return final_color;

}


int thisone;


int main()
{
    std::cout << "Raytracing\n";
    

    int dpi = 72;
    int width = 640;
    int height = 480;
    double aspectRatio = (double)width / (double) height;

    double ambientLight = 0.2;
    double accuracy = 0.000001;

    int n  = width * height;
    rgbType* pixels = new rgbType[n];

    World world;
     
    Vector CameraPosition(0,0,4);

    Camera scene_cam;
    scene_cam.lookAt(CameraPosition,world.getO(),world.getY());

    Color whiteLight(1.0,1.0,1.0,1.0);
    Color cool_green(0.5,1.0,0.5,1.0);
    Color gray(0.5,0.5,0.5,0.0);
    Color maroon(0.5,0.25,0.25,0);

    Vector lightPosition(-7,10,10);
    Light scene_light(lightPosition,whiteLight);
    world.worldLights.push_back(dynamic_cast<Source*>(&scene_light));
    //Scene objects
    Sphere scene_Sphere(world.getO(),1,cool_green);//Sphere is at origin 
    Plane scene_plane(world.getY(),-1,maroon);//Directly beneath sphere

    world.worldObjects.push_back(dynamic_cast<Object*>(&scene_Sphere));
    world.worldObjects.push_back(dynamic_cast<Object*>(&scene_plane));



    double xamnt, yamnt;

    for(int x =0; x < width; x++)
    {
        for(int y = 0; y < height;y++)
        {

            thisone = y*width + x;

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
            /*Direction of camera rays throught the view plane*/
            Vector cameraRayDirection = scene_cam.getCameraDirection().vectorAdd(scene_cam.getCameraRight().scalarMult(xamnt - 0.5).vectorAdd(scene_cam.getCameraDown().scalarMult(yamnt - 0.5))).getNormalized();



            Ray cameraRay(cameraRayOrigin,cameraRayDirection);

            std::vector<double> intersections = world.findIntersections(cameraRay);

            int indexOfWinningObjects = winningObjectIndex(intersections);


            if(indexOfWinningObjects == -1) //No intersection
            {
                Color bg_col = world.getBackgroundColor();
                pixels[thisone].r = bg_col.getColorRed();
                pixels[thisone].g = bg_col.getColorGreen();
                pixels[thisone].b = bg_col.getColorBlue();
            }
            else{
                double this_intersection = intersections[indexOfWinningObjects];
                if(this_intersection > accuracy){ //If intersecion is greater than accuracy
                    Vector intersectionPos  = cameraRayOrigin.vectorAdd(cameraRayDirection.scalarMult(this_intersection)); 
                    Vector intersectionDir = cameraRayDirection;

                    Color intersectionCol = getColorAt(intersectionPos,intersectionDir,world.worldObjects,indexOfWinningObjects,world.worldLights,accuracy,ambientLight);


                    pixels[thisone].r = intersectionCol.getColorRed();
                    pixels[thisone].g = intersectionCol.getColorGreen();
                    pixels[thisone].b = intersectionCol.getColorBlue();
                } 
                
                
                
                

            }
        }
    }
         savebmp("test.bmp",width,height,dpi,pixels);    
     
    
    
    return 0;
}