#pragma once

#include "ofMain.h"
#include "method_info.h"
#include "ofxZmq.h"
#include <msgpack.hpp>
#include "method_node.h"
//#include "ofxBox2d.h"

//typedef vector<ofPtr<MethodInfo> > MethodInfoVector;
typedef vector<ofPtr<MethodNode> > MethodNodeVector;
typedef map<unsigned long long, ofPtr<MethodNodeVector> > MapThreadMethodNodes;
//typedef vector<ofPtr<ofxBox2dCircle> > CircleVector;

class MessageParserThread : public ofThread {
  public:
    ofxZmqSubscriber *m_subscriber;
    //map<unsigned long long, ofPtr<ofxBox2dCircle> > mapThreadRootNode;
    MapThreadMethodNodes mapThreadMethodNodes;
    //map<unsigned long long, ofPtr<CircleVector> > mapThreadCircles;

    //vector<ofPtr<ofxBox2dCircle> >nodes;
    //vector <ofPtr<ofxBox2dJoint> > joints;
    //vector <ofPtr<MethodNode> > nodes;
    ofPtr<World> world;
    vector<ofColor> colors;
    int colorIndex;

    MessageParserThread(ofxZmqSubscriber *subscriber, ofPtr<World> _world) : ofThread() {
      m_subscriber = subscriber;
      world = _world;
      colorIndex = 0;
      colors.push_back(ofColor::green);
      colors.push_back(ofColor::red);
      colors.push_back(ofColor::blue);
      colors.push_back(ofColor::rosyBrown);
      colors.push_back(ofColor::fireBrick);
      colors.push_back(ofColor::darkCyan);
      colors.push_back(ofColor::darkGoldenRod);
      colors.push_back(ofColor::lightSalmon);
    }

    ofColor nextColor();
    void threadedFunction();
};
