#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "interp.h"

int main(void) {
  struct Lexer *lexer = lexer_create(stdin, "<stdin>");
#if 0
  int done = 0;
  while (!done) {
    struct Node *tok = lexer_next(lexer);
    if (!tok) {
      done = 1;
    } else {
      int kind = node_get_tag(tok);
      const char *lexeme = node_get_str(tok);
      printf("%d:%s\n", kind, lexeme);
      node_destroy(tok);
    }
  }
#endif
  struct Parser *parser = parser_create(lexer);
  struct Node *root = parser_parse(parser);
  parser_print_parse_tree(root);

  struct Interpreter *interp = interp_create(root);

  long result = interp_exec(interp);
  printf("Result: %ld\n", result);

  return 0;
}
