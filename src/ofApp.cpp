#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableSmoothing();
  subscriber.connect("tcp://localhost:5555");
  request.connect("tcp://localhost:5556");
  //executionTrace.setup("Execution trace", false);
  //executionTrace.addListener(this, &ofApp::exectionTraceToggled);
  messageParserThread = ofPtr<MessageParserThread>(new MessageParserThread(&subscriber));

  messageParserThread->startThread();
  cam.setDistance(300);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

  ofBackground(248, 248, 245);
  //executionTrace.draw();
  //ofEnableDepthTest();
  cam.begin();
  ofDrawAxis(300);
  //messageParserThread->lock();
  drawBoxes();
  //messageParserThread->unlock();
  //ofDrawBox(x, y, z, <#float width#>, <#float height#>, <#float depth#>)
  cam.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'r') {
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

//void ofApp::exectionTraceToggled(bool &value) {
  //if (value) {
    //request.send("start_cpu_profile");
  //} else {
    //request.send("stop_cpu_profile");
  //}
  //string response;
  //request.receive(response);
  //cout << "Received : " << response << "\n";
//}

void ofApp::drawBoxes() {
  map<unsigned long long, ofPtr<MethodInfoVector> >::iterator it;
  map<unsigned long long, ofPtr<MethodInfoVector> > threadExecutionTraceMap = messageParserThread->threadExecutionTraceMap;
  int thread_count = 0;
  for(it = threadExecutionTraceMap.begin(); it != threadExecutionTraceMap.end(); ++it, ++thread_count) {
    unsigned long long thread_id = it->first;
    ofPtr<MethodInfoVector> vptr = it->second;
    drawBoxesForThread(thread_id, thread_count * 150, vptr);
  }
}

void ofApp::drawBoxesForThread(unsigned long long thread_id, float ypos, ofPtr<MethodInfoVector> vptr) {

  //cout << "ExecutionTrace : " << messageParserThread->m_vExecutionTrace.size() << std::endl;
  float x = 0,y = ypos,z = 0;
  ofSetColor(0, 0, 0);
  ofDrawBitmapString("ThreadID:" + ofToString(thread_id), x - 100, y, z);
  float width = 10, height = 10, depth = 10;
  double previous_start_time = 0;
  for (int i = 0; i < vptr->size() ; ++i) {
    ofPtr<MethodInfo> ptr = vptr->at(i);
    width = (ptr->end_time - ptr->start_time) ;
    ofPushStyle();
    y = ypos + ptr->stack_level * height;
    if(i == 0)
      x = 0;
    else
      x += (ptr->start_time - previous_start_time);// + (width/2);
    previous_start_time = ptr->start_time;
    //ofDrawBox(x, y, z, width, height, depth);
    ofSetColor(187, 232, 9);
    ofFill();
    ofRect(x, y, z, width, height);
    ofSetColor(48, 239, 249);
    ofNoFill();
    ofRect(x, y, z, width, height);
    ofSetColor(0,0,0);
    if(width > 30) {
      glDepthFunc(GL_ALWAYS); // draw on top of everything
      ofDrawBitmapString(ptr->method_name, x + 2, y, z);
      glDepthFunc(GL_LESS);
    }
    ofPopStyle();
  }
}
