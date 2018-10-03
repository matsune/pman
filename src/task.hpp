#pragma once
#include <string>
#include <condition_variable>

struct Task {
  static int currentId;

  enum Order {
    START,
    STOP
  };

  int id;
  std::condition_variable &cv;
  Order op;
  std::string name;

  Task(std::condition_variable &cv, Order op, std::string name)
    : id(currentId++), cv(cv), op(op), name(name) {}
};
