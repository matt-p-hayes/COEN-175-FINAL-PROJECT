# include <iostream>
# include "checker.h"
# include "type.h"
# include "scope.h"
# include "lexer.h"

using namespace std;

Symbols *symbols = new Symbols;
Scope *current = new Scope(nullptr, symbols);

const string E1 = "redefinition of '%s'";
const string E2 = "conflicting types for '%s'";
const string E3 = "redeclaration of '%s'";
const string E4 = "'%s' undeclared";
const string E5 = "'%s' has type void";

void openScope() {
    Symbols *newScopeSymbols = new Symbols;
    current = new Scope(current, newScopeSymbols);

    //cout << "open scope" << endl;
}

void closeScope() {
    Scope *temp = current;
    current = current->nextScope();
    delete temp;

    //cout << "close scope" << endl;
}


void declareFunction(const string &name, const Type &type) {
    Symbol *sym = current->find(name);
    if(sym == nullptr) {
        current->insert(new Symbol(name, type));
    } else if(sym->type() != type) {
        report(E2, name);
    } else if(current->nextScope() != nullptr) {
        report(E3, name);
    }

    //cout << name << ": " << type << endl;
}

void defineFunction(const string &name, const Type &type) {
    Symbol *sym = current->find(name);
    if(sym == nullptr) {
        current->insert(new Symbol(name, type));
    } else if(sym->type().parameters() != nullptr && type.parameters() != nullptr) {
        report(E1, name);
        current->remove(name);
        current->insert(new Symbol(name, type));
    } else if(sym->type() != type) {
        report(E2, name);
    }

    //cout << name << ": " << type << endl;
}

void declareVariable(const string &name, const Type &type) {
    Symbol *sym = current->find(name);
    if(sym == nullptr) {
        if(type.specifier() == 285 && type.indirection() == 0) {
            report(E5, name);
        }
        current->insert(new Symbol(name, type));
    } else if(sym->type() != type) {
        report(E2, name);
    } else if(current->nextScope() != nullptr) {
        report(E3, name);
    } else if(type.specifier() == 285 && sym->type().indirection() == 0) {
        report(E5, name);
    }

    //cout << name << ": " << type << endl;
}

void checkIdentifier(const string &name) {
    Symbol *sym = current->lookup(name);
    if(sym == nullptr) {
        report(E4, name);
    }
}








// void defineFunction(const std::string &name, const Type &type) {
//     cout << name << ": " << type << endl;
// }

// void declareFunction(const std::string &name, const Type &type) {
//     cout << name << ": " << type << endl;
// }

// void declareVariable(const std::string &name, const Type &type) {
    
// }

// void checkIdentifier(const std::string &name) {

// }