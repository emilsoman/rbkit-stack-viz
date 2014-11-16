#pragma once

#include "ofMain.h"
#include "method_info.h"
#include "ofxZmq.h"
#include <msgpack.hpp>

typedef vector<ofPtr<MethodInfo> > MethodInfoVector;

class MessageParserThread : public ofThread {
  public:
    ofxZmqSubscriber *m_subscriber;
    map<unsigned long long, ofPtr<MethodInfoVector> > threadExecutionTraceMap;
    map<unsigned long long, ofPtr<MethodInfoVector> > threadCallStackMap;

    MessageParserThread(ofxZmqSubscriber *subscriber) : ofThread() {
      m_subscriber = subscriber;
    }

    void threadedFunction() {
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
                if(threadCallStackMap.find(thread_id) == threadCallStackMap.end()) {
                  ofPtr<MethodInfoVector> ptr(new MethodInfoVector);
                  threadCallStackMap[thread_id] = ptr;
                }
                ofPtr<MethodInfo> ptr(new MethodInfo(method_name, cpu_time, thread_id));
                threadCallStackMap[thread_id]->push_back(ptr);
              } else if( event_type == 9) {
                if(threadCallStackMap.find(thread_id) == threadCallStackMap.end())
                  continue;
                ofPtr<MethodInfoVector> vptr = threadCallStackMap[thread_id];
                if(threadExecutionTraceMap.find(thread_id) == threadExecutionTraceMap.end()) {
                  ofPtr<MethodInfoVector> ptr(new MethodInfoVector);
                  threadExecutionTraceMap[thread_id] = ptr;
                }
                ofPtr<MethodInfo> ptr = vptr->back();
                vptr->pop_back();
                ptr->end_time = cpu_time;
                ptr->stack_level = vptr->size();
                //lock();
                threadExecutionTraceMap[thread_id]->push_back(ptr);
                //unlock();
              }
            }
          }

        }
      }
    }
};
