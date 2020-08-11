#include "util.h"
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

struct Parser {
private:
	struct Lexer *m_lexer;
	struct Node *m_next;

public:
	Parser(Lexer *lexer_to_adopt);
	~Parser();

	struct Node *parse();

private:
	// Consume next token, wrapping it in a Node
	struct Node *next();

	// Peek ahead to see the next token, wrapping it in a Node
	struct Node *peek();

	// Parse functions for nonterminal grammar symbols
	struct Node *parse_U();
	struct Node *parse_E();
};

Parser::Parser(Lexer *lexer_to_adopt) {
}

Parser::~Parser() {
}

struct Node *Parser::parse() {
	return parse_E();
}

struct Node *Parser::next() {
	peek();
	if (!m_next) {
		err_fatal("Line %d: unexpected end of input\n", lexer_get_line(m_lexer));
	}
	struct Node *n = m_next;
	m_next = nullptr;
	return n;
}

struct Node *Parser::peek() {
	if (!m_next) {
		// This is where we actually read a Token from the lexer
		// and create a Node for it
		struct Token *tok = lexer_next(m_lexer);
		if (tok) {
			struct Node *n = node_alloc_str_copy(tok->kind, tok->lexeme);
			node_set_source_info(n, "<input>", tok->line, tok->col);
			m_next = n;
			token_destroy(tok);
		}
	}
	return m_next;
}

struct Node *Parser::parse_U() {
}

struct Node *Parser::parse_E() {
	// read the next terminal symbol (wrapping it in a Node)
	struct Node *next_terminal = next();
	if (!next_terminal) {
		err_fatal("Line %d: Parse error (missing expression)\n", lexer_get_line(m_lexer));
	}

	struct Node *e = node_build0(NODE_E);

	int tag = node_get_tag(next_terminal);
	if (tag == TOK_INTEGER_LITERAL || tag == TOK_IDENTIFIER) {
		// E -> <int_literal> ^
		// E -> <identifier> ^
		node_add_kid(e, next_terminal);
	} else if (tag == TOK_PLUS || tag == TOK_MINUS || tag == TOK_TIMES || tag == TOK_DIVIDE) {
		// E -> + ^ E E
		// E -> - ^ E E
		// E -> * ^ E E
		// E -> / ^ E E

		node_add_kid(e, next_terminal);

		node_add_kid(e, parse_E()); // parse first operand
		node_add_kid(e, parse_E()); // parse second operand
	} else {
		err_fatal("Line %d: Illegal expression (at '%s')\n", next_terminal->line, node_get_str(next_terminal));
	}
}

////////////////////////////////////////////////////////////////////////
// Parser API functions
////////////////////////////////////////////////////////////////////////

struct Parser *parser_create(struct Lexer *lexer_to_adopt) {
}

void parser_destroy(struct Parser *parser) {
}

struct Node *parser_parse(struct Parser *parser) {
}
