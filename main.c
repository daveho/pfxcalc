#include <stdio.h>
#include "lexer.h"

int main(void) {
	struct Lexer *lexer = lexer_create(stdin);

	int done = 0;
	while (!done) {
		struct Token *tok = lexer_next(lexer);
		if (!tok) {
			done = 1;
		} else {
			printf("%d:%s\n", tok->kind, tok->lexeme);
		}
	}

	return 0;
}
