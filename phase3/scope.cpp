#include "scope.h"
# include <iostream>

using namespace std;

Scope::Scope(Scope *nextScope, Symbols *symbols)
    : _nextScope(nextScope), _symbols(symbols)
    { }

void Scope::insert(Symbol *symbol) {
    _symbols->push_back(symbol);
}

void Scope::remove(const string &name) {
    for(unsigned i = 0; i < _symbols->size(); i++) {
        if(_symbols->at(i)->name() == name) {
            _symbols->erase(_symbols->begin() + i);
            return;
        }
    }
}

Symbol *Scope::find(const string &name) const {
    for(unsigned i = 0; i < _symbols->size(); i++) {
        if(_symbols->at(i)->name() == name) {
            return _symbols->at(i);
        }
    }
    return nullptr;
}

Symbol *Scope::lookup(const string &name) const {
    //check in current scope
    for(unsigned i = 0; i < _symbols->size(); i++) {
        if(_symbols->at(i)->name() == name) {
            return _symbols->at(i);
        }
    }

    //check enclosing scopes
    Scope *currentScope = _nextScope;
    while(currentScope != nullptr) {
        for(unsigned i = 0; i < currentScope->symbols()->size(); i++) {
            if(currentScope->symbols()->at(i)->name() == name) {
                return currentScope->symbols()->at(i);
            }
        }
        currentScope = currentScope->nextScope();
    }
    return nullptr;
}