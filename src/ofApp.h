#pragma once

#include "ofMain.h"
#include "method_info.h"
#include "message_parser_thread.h"
#include "method_node.h"
#include "world.h"

class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    float camDist;
    ofxZmqSubscriber subscriber;
    ofxZmqRequest request;
    ofEasyCam cam; // add mouse controls for camera movement
    ofLight light;
    ofPtr<MessageParserThread> messageParserThread;
    ofPtr<World> world;
};
