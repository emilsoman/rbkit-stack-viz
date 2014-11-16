#pragma once

class MethodInfo {
  public:
    MethodInfo(string method_name_from_trace, double cpu_time, unsigned long long thread_id_from_trace) {
      method_name = method_name_from_trace;
      start_time = cpu_time;
      end_time = 0;
      stack_level = 0;
      thread_id = thread_id_from_trace;
    }

    string method_name;
    double start_time;
    double end_time;
    int stack_level;
    unsigned long long thread_id;
};
