#include "method_node.h"

void MethodNode::setup(ofPtr<World> _world, double _start_time, double _end_time) {
  world = _world;
  start_time = _start_time;
  end_time  = _end_time;
}

void MethodNode::customDraw() {
  int scale = world->scale;
  x = (start_time - world->referenceTime) * scale;
  y = stack_level * 20;
  width = (end_time - start_time) * scale;
  height = 20;
  depth = 20;
  float centerX = x + (width/2);
  float centerY = y + (height/2);
  float centerZ = z + (depth/2);
  ofSetColor(color);
  ofFill();
  ofDrawBox(centerX, centerY, centerZ, width, height, depth);
  drawn = true;
}

void MethodNode::mouseOverCheck(const ofEasyCam &cam, int mouseX, int mouseY) {
  if(!drawn) {
    return;
  }
  vector<ofPoint> points;
  ofPoint vertex1(x, y, z);
  points.push_back((ofPoint)cam.worldToScreen(vertex1));
  ofPoint vertex2(x + width, y, z);
  points.push_back((ofPoint)cam.worldToScreen(vertex2));
  ofPoint vertex3(x, y + height, z);
  points.push_back((ofPoint)cam.worldToScreen(vertex3));
  ofPoint vertex4(x, y, z + depth);
  points.push_back((ofPoint)cam.worldToScreen(vertex4));
  ofPoint vertex5(x + width, y, z + depth);
  points.push_back((ofPoint)cam.worldToScreen(vertex5));
  ofPoint vertex6(x + width, y + height, z);
  points.push_back((ofPoint)cam.worldToScreen(vertex6));
  ofPoint vertex7(x, y + height, z + depth);
  points.push_back((ofPoint)cam.worldToScreen(vertex7));
  ofPoint vertex8(x + width, y + height, z + depth);
  points.push_back((ofPoint)cam.worldToScreen(vertex8));
  ofPolyline polyline(points);
  ofVec2f mouse(mouseX, mouseY);

  polyline.close();
  ofRectangle rect = polyline.getBoundingBox();
  if(rect.inside(mouseX, mouseY)) {
    ofVec3f point(mouseX, mouseY - 100, 0);
    ofDrawBitmapString(method_name, cam.screenToWorld(point));
  }
}

void MethodNode::drawAtZ(float _z) {
  z = _z;
  draw();
}
