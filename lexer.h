#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Lexer;

struct Lexer *lexer_create(FILE *in);
void lexer_destroy(struct Lexer *lexer);

struct Token *lexer_next(struct Lexer *lexer);
struct Token *lexer_peek(struct Lexer *lexer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LEXER_H
