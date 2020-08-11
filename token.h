#ifndef TOKEN_H
#define TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum TokenKind {
	TOK_IDENTIFIER,
	TOK_INTEGER_LITERAL,
	TOK_PLUS,
	TOK_MINUS,
	TOK_TIMES,
	TOK_DIVIDE,

	// special token kind for whitespace: the lexer should not
	// actually return a token of this kind
	TOK_WHITESPACE,
};

struct Token {
	enum TokenKind kind;
	char *lexeme;
	int line, col;
};

struct Token *token_create(enum TokenKind kind, char *lexeme_to_adopt, int line, int col);
void token_destroy(struct Token *tok);

#ifdef __cplusplus
}
#endif

#endif // TOKEN_H
