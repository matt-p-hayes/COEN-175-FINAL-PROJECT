# ifndef SCOPR_H
# define SCOPE_H
# include "type.h"
# include "symbol.h"
# include <string>

typedef std::vector<class Symbol*> Symbols;

class Scope {
    typedef std::string string;
    Scope *_nextScope;
    Symbols *_symbols;
    public:
        Scope(Scope *nextScope, Symbols *symbols);
        Scope *nextScope() { return _nextScope; }
        Symbols *symbols() { return _symbols; }
        void insert(Symbol *symbol);
        void remove(const string &name);
        Symbol *find(const string &name) const;
        Symbol *lookup(const string &name) const;
};

#endif