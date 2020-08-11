#include <stdlib.h>
#include "util.h"
#include "token.h"

struct Token *token_create(enum TokenKind kind, char *lexeme_to_adopt) {
	struct Token *tok = xmalloc(sizeof(struct Token));
	tok->kind = kind;
	tok->lexeme = lexeme_to_adopt;
	return tok;
}

void token_destroy(struct Token *tok) {
	free(tok->lexeme);
	free(tok);
}
