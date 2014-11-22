#include "message_parser_thread.h"

ofColor MessageParserThread::nextColor() {
  if(++colorIndex == 8)
    colorIndex = 0;

  return colors[colorIndex];
}

void MessageParserThread::threadedFunction() {
  MapThreadMethodNodes mapThreadStack;
  while(isThreadRunning()) {
    while (m_subscriber->hasWaitingMessage()) {
      ofBuffer data;
      m_subscriber->getNextMessage(data);
      msgpack::unpacked msg;
      msgpack::unpack(&msg, data.getBinaryBuffer(), data.size());
      msgpack::object obj = msg.get();
      auto map = obj.as< std::map<unsigned int, msgpack::object> >();
      auto event_type = map[0].as<unsigned int>();
      // If event collection
      if(event_type == 7) {
        auto map = obj.as<std::map<unsigned int, msgpack::object> >();
        auto event_array = map[2].as<std::vector<msgpack::object> >();
        for(auto &obj : event_array) {
          map = obj.as<std::map<unsigned int, msgpack::object> >();
          event_type = map[0].as<unsigned int>();
          double cpu_time = map[11].as<double>();
          map = map[2].as<std::map<unsigned int, msgpack::object> >();
          string method_name = map[10].as<string>();
          unsigned long long thread_id = map[12].as<unsigned long long>();

          if(event_type == 8) {
            if(mapThreadStack.empty() && !world->referenceTime) {
              world->referenceTime = cpu_time;
            }
            if(mapThreadStack.find(thread_id) == mapThreadStack.end()) {
              mapThreadStack[thread_id] = ofPtr<MethodNodeVector>(new MethodNodeVector);
            }
            ofPtr<MethodNode> node = ofPtr<MethodNode>(new MethodNode());
            node->setup(world, cpu_time, cpu_time);
            node->color = nextColor();
            mapThreadStack[thread_id]->push_back(node);
          } else if( event_type == 9) {
            if(mapThreadStack.empty() && !world->referenceTime) {
              world->referenceTime = cpu_time;
            }
            if(mapThreadStack.find(thread_id) == mapThreadStack.end()) {
              continue;
            }
            ofPtr<MethodNodeVector> stackVptr = mapThreadStack[thread_id];
            if(stackVptr->empty()) {
              cout << method_name << " didn't get a call event! This should never happen" << endl;
              continue;
            }
            if(mapThreadMethodNodes.find(thread_id) == mapThreadMethodNodes.end())
              mapThreadMethodNodes[thread_id] = ofPtr<MethodNodeVector>(new MethodNodeVector);

            ofPtr<MethodNodeVector> vptr = mapThreadMethodNodes[thread_id];

            ofPtr<MethodNode> node = stackVptr->back();
            stackVptr->pop_back();
            node->end_time = cpu_time;
            node->method_name = method_name;
            node->stack_level = stackVptr->size();
            vptr->push_back(node);
          }
        }
      }
    }
  }
}
