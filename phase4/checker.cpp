/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		If a symbol is redeclared, the redeclaration is discarded
 *		and the original declaration is retained.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"


using namespace std;

static Scope *outermost, *toplevel;
static const Type error;

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";
static string void_object = "'%s' has type void";

static string E1 = "invalid return type";
static string E2 = "invalid type for test expression";
static string E3 = "lvalue required in expression";
static string E4 = "invalid operands to binary %s";
static string E5 = "invalid operand to unary %s";
static string E6 = "called object is not a function";
static string E7 = "invalid arguments to called function";

/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;
    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  This
 *		definition always replaces any previous definition or
 *		declaration.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol != nullptr) {
	if (symbol->type().isFunction() && symbol->type().parameters()) {
	    report(redefined, name);
	    delete symbol->type().parameters();

	} else if (type != symbol->type())
	    report(conflicting, name);

	outermost->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    outermost->insert(symbol);
    return symbol;
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		redeclaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol == nullptr) {
	symbol = new Symbol(name, type);
	outermost->insert(symbol);

    } else if (type != symbol->type()) {
	report(conflicting, name);
	delete type.parameters();

    } else
	delete type.parameters();

    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		redeclaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = toplevel->find(name);

    if (symbol == nullptr) {
	if (type.specifier() == VOID && type.indirection() == 0)
	    report(void_object, name);

	symbol = new Symbol(name, type);
	toplevel->insert(symbol);

    } else if (outermost != toplevel)
	report(redeclared, name);

    else if (type != symbol->type())
	report(conflicting, name);

    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	toplevel->insert(symbol);
    }

    return symbol;
}

Type checkIfStatement(const Type &left) {
    if(left == error) {
        return error;
    }
    if(!left.isPredicate()) {
        report(E2);
        return error;
    } else {
        return left;
    }
}

Type checkForLoop(const Type &left) {
    if(left == error) {
        return error;
    }
    if(!left.isPredicate()) {
        report(E2);
        return error;
    } else {
        return left;
    }
}

Type checkWhileLoop(const Type &left) {
    if(left == error) {
        return error;
    }
    if(!left.isPredicate()) {
        report(E2);
        return error;
    } else {
        return left;
    }
}

Type checkReturn(const Type &left, const Type &returnType) {
    if(left == error) {
        return error;
    }
    if(!left.isCompatibleWith(returnType)) {
        report(E1);
    }
    return left;
}

Type checkLogicalOr(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(left.isPredicate() && right.isPredicate()) {
        return Type(INT);
    }
    report(E4, "||");
    return error;
}

Type checkLogicalAnd(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(left.isPredicate() && right.isPredicate()) {
        return Type(INT);
    }
    report (E4, "&&");
    return error;
}

Type checkNot(const Type &left) {
    if(left == error) {
        return error;
    }
    if(!left.isPredicate()) {
        report(E5, "!");
        return error;
    }
    return Type(INT);
}

Type checkMultiplication(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(!left.isNumeric() || !right.isNumeric()) {
        report(E4, "*");
        return error;
    }
    if(left.specifier() == LONG || right.specifier() == LONG) {
        return Type(LONG);
    } else {
        return Type(INT);
    }
}
Type checkDivision(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(!left.isNumeric() || !right.isNumeric()) {
        report(E4, "/");
        return error;
    }
    if(left.specifier() == LONG || right.specifier() == LONG) {
        return Type(LONG);
    } else {
        return Type(INT);
    }
}
Type checkModulus(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(!left.isNumeric() || !right.isNumeric()) {
        report(E4, "%");
        return error;
    }
    if(left.specifier() == LONG || right.specifier() == LONG) {
        return Type(LONG);
    } else {
        return Type(INT);
    }
}

Type checkNegate(const Type &left) {
    if(left == error) {
        return error;
    }
    if(!left.isNumeric()) {
        report(E5, "-");
        return error;
    } else {
        return Type(left.specifier());
    }
}

Type checkLessThan(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();
    if(left.isNumeric() && right.isNumeric()) {
        return Type(INT);
    } else if((leftPromoted.specifier() == rightPromoted.specifier()) && (leftPromoted.indirection() == rightPromoted.indirection())) {
        return Type(INT);
    } else {
        report(E4, "<");
        return error;
    }
}
Type checkGreaterThan(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();
    if(left.isNumeric() && right.isNumeric()) {
        return Type(INT);
    } else if((leftPromoted.specifier() == rightPromoted.specifier()) && (leftPromoted.indirection() == rightPromoted.indirection())) {
        return Type(INT);
    } else {
        report(E4, ">");
        return error;
    }
}
Type checkLessThanOrEqual(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();
    if(left.isNumeric() && right.isNumeric()) {
        return Type(INT);
    } else if((leftPromoted.specifier() == rightPromoted.specifier()) && (leftPromoted.indirection() == rightPromoted.indirection())) {
        return Type(INT);
    } else {
        report(E4, "<=");
        return error;
    }
}
Type checkGreaterThanOrEqual(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();
    if(left.isNumeric() && right.isNumeric()) {
        return Type(INT);
    } else if((leftPromoted.specifier() == rightPromoted.specifier()) && (leftPromoted.indirection() == rightPromoted.indirection())) {
        return Type(INT);
    } else {
        report(E4, ">=");
        return error;
    }
}

Type checkEqual(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(!left.isCompatibleWith(right)) {
        report(E4, "==");
        return error;
    }
    return Type(INT);
}
Type checkNotEqual(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    if(!left.isCompatibleWith(right)) {
        report(E4, "!=");
        return error;
    }
    return Type(INT);
}

Type checkAddition(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();
    if(leftPromoted.isNumeric() && rightPromoted.isNumeric()) {
        if((leftPromoted.specifier() == LONG) || (rightPromoted.specifier() == LONG)) {
            return Type(LONG);
        } else {
            return Type(INT);
        }
    }
    if(leftPromoted.isPointer() && rightPromoted.isNumeric()) {
        if(leftPromoted.specifier() == VOID) {
            report(E4, "+");
            return error;
        } else {
            return Type(leftPromoted.specifier(), leftPromoted.indirection());
        }
    }
    if(leftPromoted.isNumeric() && rightPromoted.isPointer()) {
        if(rightPromoted.specifier() == VOID) {
            report(E4, "+");
            return error;
        } else {
            return Type(rightPromoted.specifier(), rightPromoted.indirection());
        }
    }
    report(E4, "+");
    return error;
}
Type checkSubtraction(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();
    if(leftPromoted.isNumeric() && rightPromoted.isNumeric()) {
        if((leftPromoted.specifier() == LONG) || (rightPromoted.specifier() == LONG)) {
            return Type(LONG);
        } else {
            return Type(INT);
        }
    }
    if(leftPromoted.isPointer() && rightPromoted.isNumeric()) {
        if(leftPromoted.specifier() == VOID) {
            report(E4, "-");
            return error;
        } else {
            return Type(leftPromoted.specifier(), leftPromoted.indirection());
        }
    }
    if(leftPromoted.isPointer() && rightPromoted.isPointer()) {
        if((leftPromoted.specifier() == VOID) || (rightPromoted.specifier() == VOID)) {
            report(E4, "-");
            return error;
        } 
        if(leftPromoted.specifier() != rightPromoted.specifier()) {
            report(E4, "-");
            return error;
        }
        return Type(LONG);
    }
    report(E4, "-");
    return error;
}

Type checkDereference(const Type &left) {
    if(left == error) {
        return error;
    }

    Type leftPromoted = left.promote();
    if(leftPromoted.isPointer()) {
        if(leftPromoted.specifier() != VOID) {
            return Type(leftPromoted.specifier());
        }
    }

    report(E5, "*");
    return error;
}

Type checkAddress(const Type &left, bool lvalue) {
    if(left == error) {
        return error;
    }
    if(!lvalue) {
        report(E3);
        return error;
    }
    return Type(left.specifier(), left.indirection() + 1);
}

Type checkIndex(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type leftPromoted = left.promote();
    Type rightPromoted = right.promote();

    if(!leftPromoted.isPointer() || (leftPromoted.specifier() == VOID && leftPromoted.indirection() == 1) || !rightPromoted.isNumeric()) {
        report(E4, "[]");
        return error;
    }
    return Type(leftPromoted.specifier(), leftPromoted.indirection() - 1);
}

Type checkSizeOf(const Type &left) {
    if(left == error) {
        return error;
    }
    if(left.isPredicate()) {
        return Type(LONG);
    }
    report(E5, "sizeof");
    return error;
}


Type checkFunction(const Type &left, const Parameters *parameters) {
    if(left == error) {
        return error;
    }
    if(!left.isFunction()) {
        report(E6);
        return error;
    }
    if(left.parameters() == nullptr) {
        for(unsigned int x = 0; x < parameters->size(); x ++) {
            Type currentParameter = parameters->at(x);
            if(currentParameter == error) {
                return error;
            }
            if(!(currentParameter.promote().isPredicate())) {
                report(E7);
                return error;
            }
        }
    } else {
        if(parameters->size() != left.parameters()->size()) {
            report(E7);
            return error;
        }
        for(unsigned int y = 0; y < parameters->size(); y++) {
            Type currentParameter = parameters->at(y);
            if(currentParameter == error) {
                return error;
            }
            if(!(currentParameter.promote().isPredicate()) || !(currentParameter.isCompatibleWith(left.parameters()->at(y)))) {
                report(E7);
                return error;
            }
        }
    }
    return Type(left.specifier(), left.indirection());
    

}

void checkAssignment(const Type &left, const Type &right, bool lvalue) {
    if(left == error || right == error) {
    } else {
        if(!lvalue) {
            report(E3);
            return;
        }
        if(!(left.isCompatibleWith(right))) {
            report(E4, "=");
        }
    }
    
}