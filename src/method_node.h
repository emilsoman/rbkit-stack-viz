#pragma once

#include "ofMain.h"
#include "world.h"

class MethodNode : public ofNode {
  public:
    MethodNode(){
      drawn = false;
    };
    virtual ~MethodNode(){};
    double start_time;
    double end_time;
    string method_name;
    int stack_level;
    ofPtr<World> world;
    ofColor color;
    float x;
    float y;
    float z;
    float width;
    float height;
    float depth;
    bool drawn;


    void setup(ofPtr<World> world, double start_time, double end_time);
    void customDraw();
    void mouseOverCheck(const ofEasyCam &cam, int mouseX, int mouseY);
    void drawAtZ(float z);
};
