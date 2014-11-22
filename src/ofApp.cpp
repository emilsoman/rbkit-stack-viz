#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
  subscriber.connect("tcp://localhost:5555");
  request.connect("tcp://localhost:5556");

  world = ofPtr<World>(new World);
  messageParserThread = ofPtr<MessageParserThread>(new MessageParserThread(&subscriber, world));
  messageParserThread->startThread(true);
  ofEnableDepthTest();
  light.setPosition(0,700,700);
  light.enable();
  cam.setDistance(300);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

  ofBackground(248, 248, 245);
  cam.begin();
  ofDrawAxis(300);
  MapThreadMethodNodes mapThreadMethodNodes = messageParserThread->mapThreadMethodNodes;
  int threadCount = 0;
  MapThreadMethodNodes::iterator it = mapThreadMethodNodes.begin();
  while(it != mapThreadMethodNodes.end()) {
    float z = threadCount * 60;
    ofSetColor(0,0,0);
    ofDrawBitmapString("Thread:" + ofToString(threadCount + 1), -20, 0, z);
    ofPtr<MethodNodeVector> nodes = it->second;
    for (int i = 0; i < nodes->size(); ++i) {
      nodes->at(i)->drawAtZ(z);
      nodes->at(i)->mouseOverCheck(cam, mouseX, mouseY);
    }

    threadCount += 1;
    it++;
  }
  cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'q') {
    cam.setPosition(0,0,300);
    ofVec3f pos(0,0,0);
    cam.setTarget(pos);
  }
  if(key == OF_KEY_UP) {
    cam.boom(10);
  }
  if(key == OF_KEY_DOWN) {
    cam.boom(-10);
  }
  if(key == OF_KEY_LEFT) {
    cam.truck(-10);
  }
  if(key == OF_KEY_RIGHT) {
    cam.truck(10);
  }
  if(key == 'a') {
    cam.dolly(-10);
  }
  if(key == 'z') {
    cam.dolly(10);
  }
  if(key == 's') {
    world->scale += 1;
  }
  if(key == 'x') {
    world->scale -= 1;
  }
  if(key == 'r') {
    messageParserThread->mapThreadMethodNodes.clear();
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::exit() {
  messageParserThread->stopThread();
}
