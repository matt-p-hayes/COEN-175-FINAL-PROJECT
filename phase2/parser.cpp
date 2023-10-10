# include <iostream>
# include "tokens.h"
# include "lexer.h"

using namespace std;

int lookahead;

void expression();
void declaration();
void statement();
void specifier();
void pointers();
void declarations();
void statements();

void match(int t) {
	if(lookahead == t) {	
		lookahead = yylex();	
	} else {
		cout << "ERROR" << endl;
	}
}

void parameter() {
	specifier();
	pointers();
	match(ID);
}

void parameter_list() {
	parameter();
	while(lookahead == ',') {
		match(',');
		parameter();
	}
}

void parameters() {
	if(lookahead == VOID) {
		match(VOID);
		if(lookahead != ')') {
			parameter_list();
		}
	} else {
		parameter_list();
	}
}

void global_declarator() {
	pointers();
	match(ID);

	if(lookahead == '(') {
		match('(');
		match(')');
	} else if(lookahead == '[') {
		match('[');
		match(NUM);
		match(']');
	}
}

void remaining_global_declarations() {
	while(lookahead == ',') {
		match(',');
		global_declarator();
	}
	match(';');
}

void functionOrGlobal() {
		specifier();
		pointers();
		match(ID);

		if(lookahead == '(') {
			match('(');
			if(lookahead != ')') {
				parameters();
				match(')');
				match('{');
				declarations();
				statements();
				match('}');
			} else {
				match(')');
				remaining_global_declarations();
			}
		} else if(lookahead == '[') {
			match('[');
			match(NUM);
			match(']');
			remaining_global_declarations();
		} else {
			remaining_global_declarations();
		}	
}


void statements() {
	if(lookahead != '}') {
		statement();
		statements();
	}
}

void declarations() {
	if(lookahead == INT || lookahead == CHAR || lookahead == LONG || lookahead == VOID) {
		declaration();
		declarations();
	}
}

void specifier() {
	if(lookahead == INT) {
		match(INT);
	} else if(lookahead == CHAR) {
		match(CHAR);
	} else if(lookahead == LONG) {
		match(LONG);
	} else if(lookahead == VOID) {
		match(VOID);
	}
}

void pointers() {
	while(lookahead == '*') {
		match('*');
	}
}

void declarator() {
	pointers();
	match(ID);
	if(lookahead == '[') {
		match('[');
		match(NUM);
		match(']');
	}
}

void declarator_list() {
	declarator();
	while(lookahead == ',') {
		match(',');
		declarator();
	}
}

void declaration() {
	specifier();
	declarator_list();
	match(';');
}

void assignment() {
	expression();
	if(lookahead == ASSIGN) {
		match(ASSIGN);
		expression();
	}
}

void statement() {
	if(lookahead == '{') {
		match('{');
		declarations();
		statements();
		match('}');
	} else if(lookahead == RETURN) {
		match(RETURN);
		expression();
		match(';');
	} else if(lookahead == WHILE) {
		match(WHILE);
		match('(');
		expression();
		match(')');
		statement();
	} else if(lookahead == FOR) {
		match(FOR);
		match('(');
		assignment();
		match(';');
		expression();
		match(';');
		assignment();
		match(')');
		statement();
	} else if(lookahead == IF) {
		match(IF);
		match('(');
		expression();
		match(')');
		statement();
		if(lookahead == ELSE) {
			match(ELSE);
			statement();
		}
	} else {
		assignment();
		match(';');
	}
	
}

void or_precedence();
void expression();

void expression_list() {
	expression();

	while(lookahead == ',') {
		match(',');
		expression();
	} 
}

void identifier_precedence() {
	while(1) {
		if(lookahead == '(') {
			match('(');
			expression();
			match(')');
		} else if(lookahead == ID) {
			match(ID);
			if(lookahead == '(') {
				match('(');
				if(lookahead == ')') {
					match(')');
				} else {
					expression_list();
					match(')');
				}
			}
		} else if(lookahead == NUM) {
			match(NUM);
		} else if(lookahead == STRING) {
			match(STRING);
		} else if(lookahead == CHARACTER) {
			match(CHARACTER);
		} else {
			break;
		}
	}
}

void bracket_precedence() {
	identifier_precedence();

	while(1) {
		if(lookahead == '[') {
			match('[');
			expression();
			match(']');
			cout << "index" << endl;
		} else {
			break;
		}
	}
}

void unary_precedence() {
	if(lookahead == ADDR) {
		match(ADDR);
		unary_precedence();
		cout << "addr" << endl;
	} else if(lookahead == DEREF) {
		match(DEREF);
		unary_precedence();
		cout << "deref" << endl;
	} else if(lookahead == NEG) {
		match(NEG);
		unary_precedence();
		cout << "neg" << endl;
	} else if(lookahead == SIZEOF) {
		match(SIZEOF);
		if(lookahead == '(') {
			match('(');
			expression();
			match(')');
		}
		unary_precedence();
		cout << "sizeof" << endl;
	} else if(lookahead == NOT) {
		match(NOT);
		unary_precedence();
		cout << "not" << endl;
	} 
	bracket_precedence();;
}

void multiplication_precedence() {
	unary_precedence();

	while(1) {
		if(lookahead == MUL) {
			match(MUL);
			unary_precedence();
			cout << "mul" << endl;
		} else if(lookahead == DIV) {
			match(DIV);
			unary_precedence();
			cout << "div" << endl;
		} else if(lookahead == '%') {
			match(lookahead);
			unary_precedence();
			cout << "rem" << endl;	
		} else {
			break;
		}
		
	}
}

void addition_precedence() {
	multiplication_precedence();

	while(1) {
		if(lookahead == ADD) {
			match(ADD);
			multiplication_precedence();
			cout << "add" << endl;
		} else if(lookahead == SUB) {
			match(SUB);
			multiplication_precedence();
			cout << "sub" << endl;
		} else {
			break;
		}
	}
}

void less_than_precedence() {
	addition_precedence();

	while(1) {
		if(lookahead == LTN) {
			match(LTN);
			addition_precedence();
			cout << "ltn" << endl;
		} else if(lookahead == GTN) {
			match(GTN);
			addition_precedence();
			cout << "gtn" << endl;
		} else if(lookahead == LEQ) {
			match(LEQ);
			addition_precedence();
			cout << "leq" << endl;
		} else if(lookahead == GEQ) {
			match(GEQ);
			addition_precedence();
			cout << "geq" << endl;
		} else {
			break;
		}
	}
}

void equals_precedence() {
	less_than_precedence();

	while(1) {
		if(lookahead == EQL) {
			match(EQL);
			less_than_precedence();
			cout << "eql" << endl;
		} else if(lookahead == NEQ) {
			match(NEQ);
			less_than_precedence();
			cout << "neq" << endl;
		} else {
			break;
		}
	}
}

void and_precedence() {
	equals_precedence();

	while(1) {
		if(lookahead == AND) {
			match(AND);
			equals_precedence();
			cout << "and" << endl;
		} else {
			break;
		}
	}
}

void or_precedence() {
	and_precedence();

	while(1) {
		if(lookahead == OR) {
			match(OR);
			and_precedence();
			cout << "or" << endl;
		} else {
			break;
		}
	}
}

void expression() {
	or_precedence();
}

int main()
{
	lookahead = yylex();
	while(lookahead != 0) {
		functionOrGlobal();
	}
	
}

