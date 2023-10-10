/*
 * File:	tokens.h
 *
 * Description:	This file contains the token definitions for use by the
 *		lexical analyzer and parser for Simple C.  Single character
 *		tokens use their ASCII values, so we can refer to them
 *		either as character literals or as symbolic names.
 */

# ifndef TOKENS_H
# define TOKENS_H

enum {
    // single character operators
    ASSIGN = '=',
    LTN = '<',
    GTN = '>',
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    REM = '%',
    ADDR = '&',
    DEREF = '*',
    NOT = '!',
    NEG = '-',

    // keywords
    AUTO = 256, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, DOUBLE,
    ELSE, ENUM, EXTERN, FLOAT, FOR, GOTO, IF, INT, LONG, REGISTER,
    RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF,
    UNION, UNSIGNED, VOID, VOLATILE, WHILE,

    // two character operators, 
    OR, AND, EQL, NEQ, LEQ, GEQ, STRING, ID, NUM, CHARACTER, ERROR, INCREMENT, DECREMENT, POINTER,
        
    Done = 0
};

# endif /* TOKENS_H */
