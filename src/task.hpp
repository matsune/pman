#pragma once
#include <string>
#include <condition_variable>

struct Task {
  static int currentId;

  enum Order {
    START,
    STOP,
    RESTART
  };

  int requestId;
  std::condition_variable &cv;
  Order op;
  std::string name;

  Task(int id, std::condition_variable &cv, Order op, std::string name)
    : requestId(id), cv(cv), op(op), name(name) {}
};
