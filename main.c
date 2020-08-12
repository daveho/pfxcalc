#include <stdio.h>
#include "lexer.h"

int main(void) {
	struct Lexer *lexer = lexer_create(stdin);

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

	return 0;
}
