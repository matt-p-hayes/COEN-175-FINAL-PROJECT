# ifndef CHECKER_H
# define CHECKER_H
#include "type.h"

void openScope();
void closeScope();
void defineFunction(const std::string &name, const Type &type);
void declareFunction(const std::string &name, const Type &type);
void declareVariable(const std::string &name, const Type &type);
void checkIdentifier(const std::string &name);

# endif