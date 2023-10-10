/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"
# include "Type.h"

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);
Type checkIfStatement(const Type &left);
Type checkForLoop(const Type &left);
Type checkWhileLoop(const Type &left);
Type checkReturn(const Type &left, const Type &returnType);
Type checkLogicalOr(const Type &left, const Type &right);
Type checkLogicalAnd(const Type &left, const Type &right);
Type checkNot(const Type &left);
Type checkMultiplication(const Type &left, const Type &right);
Type checkDivision(const Type &left, const Type &right);
Type checkModulus(const Type &left, const Type &right);
Type checkNegate(const Type &left); 
Type checkLessThan(const Type &left, const Type &right);
Type checkGreaterThan(const Type &left, const Type &right);
Type checkLessThanOrEqual(const Type &left, const Type &right);
Type checkGreaterThanOrEqual(const Type &left, const Type &right);
Type checkEqual(const Type &left, const Type &right);
Type checkNotEqual(const Type &left, const Type &right);
Type checkAddition(const Type &left, const Type &right);
Type checkSubtraction(const Type &left, const Type &right);
Type checkDereference(const Type &left);
Type checkAddress(const Type &left, bool lvalue);
Type checkIndex(const Type &left, const Type &right);
Type checkSizeOf(const Type &left);
Type checkFunction(const Type &left, const Parameters *parameters);
void checkAssignment(const Type &left, const Type &right, bool lvalue);
# endif /* CHECKER_H */
