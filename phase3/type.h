#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include <ostream>

typedef std::vector<class Type> Parameters;

class Type {
    int _specifier;
    unsigned  _indirection;
    enum declarator{ ARRAY, ERROR, FUNCTION, SCALAR } _declarator;
    unsigned long _length;
    Parameters *_parameters; //if length is zero there are no parameters, if it's a null pointer then we don't know how many

    public: 
        Type(int specifier, unsigned indirection = 0); //creates a scalar
        Type(int specifier, unsigned indirection, unsigned long length); //creates an array
        Type(int specifier, unsigned indirection, Parameters *parameters);  //creates a function
        Type(); //creates an error

        bool isArray() const {
            return _declarator == ARRAY;
        }
        bool isFunction() const {
            return _declarator == FUNCTION;
        }
        bool isScalar() const {
            return _declarator == SCALAR;
        }


        int specifier() const {
            return _specifier;
        }
        unsigned indirection() const {
            return _indirection;
        }
        unsigned long length() const {
            return _length;
        }
        declarator declarator() const {
            return _declarator;
        }
        Parameters* parameters() const {
            return _parameters;
        }


        bool operator==(const Type &rhs) const;
        bool operator!=(const Type &rhs) const;
};

std::ostream &operator<<(std::ostream &ostr, const Type &type);

#endif