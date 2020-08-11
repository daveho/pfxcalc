#include <cctype>
#include <string>
#include "util.h"
#include "lexer.h"

////////////////////////////////////////////////////////////////////////
// Lexer implementation
////////////////////////////////////////////////////////////////////////

struct Lexer {
private:
	FILE *m_in;
	struct Token *m_next;
	int m_line, m_col;
	bool m_eof;

public:
	Lexer(FILE *in);
	~Lexer();

	struct Token *next();
	struct Token *peek();

	int get_line() const { return m_line; }
	int get_col() const { return m_col; }

private:
	int read();
	void unread(int c);
	void fill();
	struct Token *read_token();
	struct Token *read_continued_token(enum TokenKind kind, const std::string &lexeme_start, int line, int col, int (*pred)(int));
};

Lexer::Lexer(FILE *in) : m_in(in), m_next(nullptr), m_line(1), m_col(1), m_eof(false) {
}

Lexer::~Lexer() {
}

struct Token *Lexer::next() {
	fill();
	Token *tok = m_next;
	m_next = nullptr;
	return tok;
}

struct Token *Lexer::peek() {
	fill();
	return m_next;
}

int Lexer::read() {
	if (m_eof) {
		return -1;
	}
	int c = fgetc(m_in);
	if (c < 0) {
		m_eof = true;
	} else if (c == '\n') {
		m_col = 1;
		m_line++;
	} else {
		m_col++;
	}
	return c;
}

void Lexer::unread(int c) {
	ungetc(c, m_in);
}

void Lexer::fill() {
	if (!m_eof && !m_next) {
		m_next = read_token();
	}
}

struct Token *Lexer::read_token() {
	int c, line = -1, col = -1;

	// skip whitespace characters until a non-whitespace character is read
	for (;;) {
		line = m_line;
		col = m_col;
		c = read();
		if (c < 0 || !isspace(c)) {
			break;
		}
	}

	if (c < 0) {
		// reached end of file
		return nullptr;
	}

	std::string lexeme;
	lexeme.push_back(char(c));

	if (isalpha(c)) {
		return read_continued_token(TOK_IDENTIFIER, lexeme, line, col, isalpha);
	} else if (isdigit(c)) {
		return read_continued_token(TOK_INTEGER_LITERAL, lexeme, line, col, isdigit);
	} else {
		switch (c) {
		case '+':
			return token_create(TOK_PLUS, xstrdup(lexeme.c_str()), line, col);
		case '-':
			return token_create(TOK_MINUS, xstrdup(lexeme.c_str()), line, col);
		case '*':
			return token_create(TOK_TIMES, xstrdup(lexeme.c_str()), line, col);
		case '/':
			return token_create(TOK_DIVIDE, xstrdup(lexeme.c_str()), line, col);
		default:
			err_fatal("Line %d: unrecognized character '%c'\n", line, c);
			return nullptr;
		}
	} 
}

// Read the continuation of a (possibly) multi-character token, such as
// an identifier or integer literal.  pred is a pointer to a predicate
// function to determine which characters are valid continuations.
struct Token *Lexer::read_continued_token(enum TokenKind kind, const std::string &lexeme_start, int line, int col, int (*pred)(int)) {
	std::string lexeme(lexeme_start);
	for (;;) {
		int c = read();
		if (c >= 0 && pred(c)) {
			// token has finished
			lexeme.push_back(char(c));
		} else {
			if (c >= 0) {
				unread(c);
			}
			return token_create(kind, xstrdup(lexeme.c_str()), line, col);
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Lexer API functions
////////////////////////////////////////////////////////////////////////

struct Lexer *lexer_create(FILE *in) {
	return new Lexer(in);
}

void lexer_destroy(struct Lexer *lexer) {
	delete lexer;
}

struct Token *lexer_next(struct Lexer *lexer) {
	return lexer->next();
}

struct Token *lexer_peek(struct Lexer *lexer) {
	return lexer->peek();
}

int lexer_get_line(struct Lexer *lexer) {
	return lexer->get_line();
}

int lexer_get_col(struct Lexer *lexer) {
	return lexer->get_col();
}
