#ifndef INTERP_H
#define INTERP_H

#include <map>
#include "node.h"

class Interpreter {
private:
  Node *m_tree;
  std::map<std::string, long> m_vars;

public:
  Interpreter(Node *tree);
  ~Interpreter();

  long exec();

private:
  long eval(Node *expr);
};

#endif // INTERP_H
