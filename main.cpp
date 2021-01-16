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
#include "RGB.h"
#include "Renderer.h"



int main()
{
    std::cout << "Raytracing\n";
    

    int dpi = 72;
    int width = 640;
    int height = 480;
    double aspectRatio = (double)width / (double) height;

    World world;

    world.setAmbientLight(0.2);


    Vector CameraPosition(3,1.5,-4);

    Camera scene_cam;
    scene_cam.lookAt(CameraPosition,world.getO(),world.getY());
    world.cam = &scene_cam;

    Color whiteLight(1.0,1.0,1.0,1.0);
    Color cool_green(0.5,1.0,0.5,0.3);
    Color gray(0.5,0.5,0.5,0.0);
    Color maroon(0.5,0.25,0.25,0);

    Vector lightPosition(-7,10,-10);

    //Vector lightPosition(-4,7,10);
    Light scene_light(lightPosition,whiteLight);
    world.worldLights.push_back(dynamic_cast<Source*>(&scene_light));
    //Scene objects
    Sphere scene_Sphere(world.getO(),1,cool_green);//Sphere is at origin 
    Plane scene_plane(world.getY(),-1,maroon);//Directly beneath sphere

    world.worldObjects.push_back(dynamic_cast<Object*>(&scene_Sphere));
    world.worldObjects.push_back(dynamic_cast<Object*>(&scene_plane));
    
    Renderer renderer(world,width,height);
 
    savebmp("test.bmp",width,height,dpi,renderer.render());    
     
    
    
    return 0;
}