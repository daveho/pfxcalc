#include <string>
#include "util.h"
#include "cpputil.h"
#include "treeprint.h"
#include "token.h"
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
	// Parse functions for nonterminal grammar symbols
	struct Node *parse_U();
	struct Node *parse_E();

	// Consume a specific token, wrapping it in a Node
	struct Node *expect(enum TokenKind tok_kind);

	// Report an error at current lexer position
	void error_at_current_pos(const std::string &msg);

	// Report an error at the source position indicated by given Node
	void error_at(struct Node *node, const std::string &msg);

	// Report an error at indicated source position
	void error_at(const SourceInfo &source_pos, const std::string &msg);
};

Parser::Parser(Lexer *lexer_to_adopt) : m_lexer(lexer_to_adopt), m_next(nullptr) {
}

Parser::~Parser() {
}

struct Node *Parser::parse() {
	// U is the start symbol
	return parse_U();
}

struct Node *Parser::parse_U() {
	struct Node *u = node_build0(NODE_U);

	// U -> ^ E ;
	// U -> ^ E ; U
	struct Node *e = parse_E();
	node_add_kid(u, e);
	node_add_kid(u, expect(TOK_SEMICOLON));

	// U -> E ;
	// U -> E ; ^ U

	// If there is more input, then the sequence of expressions continues
	if (lexer_peek(m_lexer)) {
		node_add_kid(u, parse_U());
	}

	return u;
}

struct Node *Parser::parse_E() {
	// read the next terminal symbol
	struct Node *next_terminal = lexer_next(m_lexer);
	if (!next_terminal) {
		error_at_current_pos("Parser error (missing expression)");
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
		std::string errmsg = ::format("Illegal expression (at '%s')", node_get_str(next_terminal));
		error_at(next_terminal, errmsg);
	}

	return e;
}

struct Node *Parser::expect(enum TokenKind tok_kind) {
	struct Node *next_terminal = lexer_next(m_lexer);
	if (node_get_tag(next_terminal) != tok_kind) {
		error_at(next_terminal, "Unexpected token");
	}
	return next_terminal;
}

// This function translates token and parse node tags into strings
// for parse tree printing
const char *pfxcalc_stringify_node_tag(int tag) {
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
	case TOK_SEMICOLON:
		return "SEMICOLON";

	// nonterminal symbols:
	case NODE_U:
		return "U";
	case NODE_E:
		return "E";

	default:
		err_fatal("Unknown node tag: %d\n", tag);
		return "<<UNKNOWN>>";
	}
}

void Parser::error_at_current_pos(const std::string &msg) {
	struct SourceInfo current_pos = lexer_get_current_pos(m_lexer);
	error_at(current_pos, msg);
}

void Parser::error_at(struct Node *node, const std::string &msg) {
	struct SourceInfo node_pos = node_get_source_info(node);
	error_at(node_pos, msg);
}

void Parser::error_at(const SourceInfo &source_pos, const std::string &msg) {
	err_fatal("%s:%d:%d: Error: %s\n", source_pos.filename, source_pos.line, source_pos.col, msg.c_str());
}

////////////////////////////////////////////////////////////////////////
// Parser API functions
////////////////////////////////////////////////////////////////////////

struct Parser *parser_create(struct Lexer *lexer_to_adopt) {
	return new Parser(lexer_to_adopt);
}

void parser_destroy(struct Parser *parser) {
	delete parser;
}

struct Node *parser_parse(struct Parser *parser) {
	return parser->parse();
}

void parser_print_parse_tree(struct Node *root) {
	treeprint(root, pfxcalc_stringify_node_tag);
}
