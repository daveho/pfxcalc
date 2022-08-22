#include <string>
#include <map>
#include <cassert>
#include "cpputil.h"
#include "token.h"
#include "exceptions.h"
#include "interp.h"

////////////////////////////////////////////////////////////////////////
// Interpreter implementation
////////////////////////////////////////////////////////////////////////

Interpreter::Interpreter(Node *tree)
  : m_tree(tree) {
}

Interpreter::~Interpreter() {
}

long Interpreter::exec() {
  long result = -1;
  Node *unit = m_tree;

  while (unit) {
    // first child is (E)xpression
    Node *expr = unit->get_kid(0);

    // evaluate the expression!
    result = eval(expr);

    // if there are more expressions, the next (U)nit
    // is the third child
    if (unit->get_num_kids() == 3) {
      unit = unit->get_kid(2);
    } else {
      // no more expressions
      unit = nullptr;
    }
  }

  return result;
}

long Interpreter::eval(Node *expr) {
  // the number of children and the first child's tag will determine
  // how to evaluate the expression
  Node *first = expr->get_kid(0);
  int num_kids = expr->get_num_kids();
  int tag = first->get_tag();

  if (num_kids == 1) {
    // leaf expression (either an integer literal or identifier)
    std::string lexeme = first->get_str();
    if (tag == TOK_INTEGER_LITERAL) {
      // convert lexeme to an integer value
      return strtol(lexeme.c_str(), nullptr, 10);
    } else {
      // look up value of variable
      assert(tag == TOK_IDENTIFIER);
      std::map<std::string, long>::const_iterator i = m_vars.find(lexeme);
      if (i == m_vars.end()) {
        SemanticError::raise(expr->get_loc(), "Undefined variable '%s'", lexeme.c_str());
      }
      return i->second;
    }
  }

  // left and right operands follow
  Node *left = expr->get_kid(1);
  Node *right = expr->get_kid(2);

  // Do the evaluation
  switch (tag) {
  case TOK_PLUS:
    return eval(left) + eval(right);
  case TOK_MINUS:
    return eval(left) - eval(right);
  case TOK_TIMES:
    return eval(left) * eval(right);
  case TOK_DIVIDE:
    return eval(left) / eval(right);

  case TOK_ASSIGN:
    // in this case, the left operand is an identifier naming
    // the variable
    {
      // get the variable name
      std::string varname = left->get_str();
      // evaluate the expression producing the value to be assigned
      long rvalue = eval(right);
      // store the value
      m_vars[varname] = rvalue;
      // result of the evaluation is the value assigned
      return rvalue;
    }

  default:
    RuntimeError::raise("Unknown operator: %d", tag);
  }
}

////////////////////////////////////////////////////////////////////////
// Interpreter API functions
////////////////////////////////////////////////////////////////////////

Interpreter *interp_create(struct Node *tree) {
  return new Interpreter(tree);
}

void interp_destroy(Interpreter *interp) {
  delete interp;
}

long interp_exec(Interpreter *interp) {
  return interp->exec();
}
