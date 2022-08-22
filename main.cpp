#include <cstdio>
#include <memory>
#include <unistd.h> // for getopt
#include "lexer.h"
#include "parser.h"
#include "interp.h"
#include "exceptions.h"

enum {
  INTERPRET,
  PRINT_TOKENS,
  PRINT_PARSE_TREE,
};

int execute(int argc, char **argv) {
  int mode = INTERPRET, opt;
  while ((opt = getopt(argc, argv, "lp")) != -1) {
    switch (opt) {
    case 'l':
      mode = PRINT_TOKENS;
      break;
    case 'p':
      mode = PRINT_PARSE_TREE;
      break;
    default:
      RuntimeError::raise("Unknown option: %c", opt);
    }
  }

  FILE *in;
  const char *filename;

  if (optind < argc) {
    // read input from file
    filename = argv[optind];
    in = fopen(filename, "r");
    if (!in) {
      RuntimeError::raise("Could not open input file '%s'", filename);
    }
  } else {
    filename = "<stdin>";
    in = stdin;
  }

  std::unique_ptr<Lexer> lexer(new Lexer(in, filename));

  if (mode == PRINT_TOKENS) {
    bool done = false;
    while (!done) {
      Node *tok = lexer->peek();
      if (tok == nullptr) {
        done = true;
      } else {
        int kind = tok->get_tag();
        std::string lexeme = tok->get_str();
        printf("%d:%s\n", kind, lexeme.c_str());
        delete tok;
      }
    }
  } else {
    std::unique_ptr<Parser> parser(new Parser(lexer.release()));
    std::unique_ptr<Node> root(parser->parse());

    if (mode == PRINT_PARSE_TREE) {
      ParserTreePrint ptp;
      ptp.print(root.get());
    } else {
      std::unique_ptr<Interpreter> interp(new Interpreter(root.get()));
      long result = interp->exec();
      printf("Result: %ld\n", result);
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  try {
    return execute(argc, argv);
  } catch (NearlyCException &ex) {
    if (ex.has_location()) {
      const Location &loc = ex.get_loc();
      fprintf(stderr, "%s:%d: Error: %s\n", loc.get_srcfile().c_str(), loc.get_line(), ex.what());
    } else {
      fprintf(stderr, "Error: %s\n", ex.what());
    }
    return 1;
  }
}
