#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "node.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Lexer;

struct Lexer *lexer_create(FILE *in);
void lexer_destroy(struct Lexer *lexer);

struct Node *lexer_next(struct Lexer *lexer);
struct Node *lexer_peek(struct Lexer *lexer);

int lexer_get_line(struct Lexer *lexer);
int lexer_get_col(struct Lexer *lexer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LEXER_H
