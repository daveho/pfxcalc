#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "node.h"
#include "treeprint.h"

// Enumeration to define the nonterminal symbols:
// these should have different integer values than
// the members of the TokenKind enumeration (i.e., so they
// can be distinguished from terminal symbols)
enum Nonterminal {
  NODE_U = 1000,   // "Unit", sequence of expressions
  NODE_E,          // "Expression"
};

class Parser {
private:
  Lexer *m_lexer;
  Node *m_next;

public:
  Parser(Lexer *lexer_to_adopt);
  ~Parser();

  Node *parse();

private:
  // Parse functions for nonterminal grammar symbols
  Node *parse_U();
  Node *parse_E();

  // Consume a specific token, wrapping it in a Node
  Node *expect(enum TokenKind tok_kind);

  // Report an error at current lexer position
  void error_at_current_pos(const std::string &msg);
};

class ParserTreePrint : public TreePrint {
public:
  ParserTreePrint();
  virtual ~ParserTreePrint();

  virtual std::string node_tag_to_string(int tag) const;
};

#endif // PARSER_H
