# ifndef SYMBOL_H 
# define SYMBOL_H 
# include "type.h"
# include <string>

class Symbol {
    typedef std::string string;
    string _name;
    Type _type;
    public:
        Symbol(const string &name, const Type &type);
        const string &name() const { return _name; }
        const Type &type() const { return _type; }
};

#endif