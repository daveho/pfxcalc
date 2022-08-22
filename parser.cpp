#include <string>
#include <memory>
#include "cpputil.h"
#include "treeprint.h"
#include "token.h"
#include "exceptions.h"
#include "parser.h"

////////////////////////////////////////////////////////////////////////
// Parser implementation
////////////////////////////////////////////////////////////////////////

// This is the grammar (U is the start symbol):
//
// U -> E ;
// U -> E ; U
// E -> int_literal
// E -> identifier
// E -> + E E
// E -> - E E
// E -> * E E
// E -> / E E
// E -> = identifier E

Parser::Parser(Lexer *lexer_to_adopt)
  : m_lexer(lexer_to_adopt)
  , m_next(nullptr) {
}

Parser::~Parser() {
  delete m_lexer;
}

Node *Parser::parse() {
  // U is the start symbol
  return parse_U();
}

Node *Parser::parse_U() {
  std::unique_ptr<Node> u(new Node(NODE_U));

  // U -> ^ E ;
  // U -> ^ E ; U
  u->append_kid(parse_E());
  u->append_kid(expect(TOK_SEMICOLON));

  // U -> E ; ^
  // U -> E ; ^ U
  if (m_lexer->peek() != nullptr) {
    // there is more input, so the sequence of expressions continues
    u->append_kid(parse_U());
  }

  return u.release();
}

Node *Parser::parse_E() {
  // read the next terminal symbol
  Node *next_terminal = m_lexer->next();

  std::unique_ptr<Node> e(new Node(NODE_E));

  int tag = next_terminal->get_tag();
  if (tag == TOK_INTEGER_LITERAL || tag == TOK_IDENTIFIER) {
    // E -> <int_literal> ^
    // E -> <identifier> ^
    e->append_kid(next_terminal);
  } else if (tag == TOK_ASSIGN) {
    // E -> = ^ <identifier> E
    e->append_kid(next_terminal);
    e->append_kid(expect(TOK_IDENTIFIER));
    e->append_kid(parse_E());
  } else if (tag == TOK_PLUS || tag == TOK_MINUS || tag == TOK_TIMES || tag == TOK_DIVIDE) {
    // E -> + ^ E E
    // E -> - ^ E E
    // E -> * ^ E E
    // E -> / ^ E E

    e->append_kid(next_terminal);

    e->append_kid(parse_E()); // parse first operand
    e->append_kid(parse_E()); // parse second operand
  } else {
    SyntaxError::raise(next_terminal->get_loc(), "Illegal expression (at '%s')", next_terminal->get_str().c_str());
  }

  return e.release();
}

Node *Parser::expect(enum TokenKind tok_kind) {
  std::unique_ptr<Node> next_terminal(m_lexer->next());
  if (next_terminal == nullptr) {
    error_at_current_pos("Unexpected end of input");
  }
  if (next_terminal->get_tag() != tok_kind) {
    SyntaxError::raise(next_terminal->get_loc(), "Unexpected token '%s'", next_terminal->get_str().c_str());
  }
  return next_terminal.release();
}

void Parser::error_at_current_pos(const std::string &msg) {
  SyntaxError::raise(m_lexer->get_current_loc(), "%s", msg.c_str());
}

////////////////////////////////////////////////////////////////////////
// ParserTreePrint implementation
////////////////////////////////////////////////////////////////////////

ParserTreePrint::ParserTreePrint() {
}

ParserTreePrint::~ParserTreePrint() {
}

std::string ParserTreePrint::node_tag_to_string(int tag) const {
  switch (tag) {
  // terminal symbols:
  case TOK_IDENTIFIER:
    return "IDENTIFIER";
  case TOK_INTEGER_LITERAL:
    return "INTEGER_LITERAL";
  case TOK_PLUS:
    return "PLUS";
  case TOK_MINUS:
    return "MINUS";
  case TOK_TIMES:
    return "TIMES";
  case TOK_DIVIDE:
    return "DIVIDE";
  case TOK_ASSIGN:
    return "ASSIGN";
  case TOK_SEMICOLON:
    return "SEMICOLON";

  // nonterminal symbols:
  case NODE_U:
    return "U";
  case NODE_E:
    return "E";

  default:
    RuntimeError::raise("Unknown node tag: %d", tag);
  }
}
