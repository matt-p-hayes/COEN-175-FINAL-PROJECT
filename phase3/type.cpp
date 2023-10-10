#include "type.h"
# include <iostream>
# include <assert.h>

using namespace std;

Type::Type(int specifier, unsigned indirection) 
    :  _specifier(specifier), _indirection(indirection), _declarator(SCALAR)
    { }

Type::Type(int specifier, unsigned indirection, unsigned long length)
    : _specifier(specifier), _indirection(indirection), _declarator(ARRAY), _length(length)
    { }

Type::Type(int specifier, unsigned indirection, Parameters *parameters)
    : _specifier(specifier), _indirection(indirection), _declarator(FUNCTION), _parameters(parameters)
    { }

Type::Type()
    {
        cout << "ERROR!!!" << endl;
    }

/*
    class A {
        B b;
        C c;
        D d;

        A(....) -> this will call default constructor for B, C, and D without the syntax used above
    }
*/

bool Type::operator!=(const Type &rhs) const {
    return !operator==(rhs);
}

bool Type::operator==(const Type &rhs) const {
    if(_declarator != rhs._declarator) {
        return false;
    }
    if(_declarator == ERROR) {
        return true;
    }
    if(_specifier != rhs._specifier) {
        return false;
    }
    if(_indirection != rhs._indirection) {
        return false;
    }
    if(_declarator == SCALAR) {
        return true;
    }
    if(_declarator == ARRAY) {
        return _length == rhs._length;
    }
    assert(_declarator == FUNCTION); //have to #include c_assert (don't know if that's the right name)
    if(!_parameters || !rhs._parameters) {
        return true;
    }
    return *_parameters == *rhs._parameters; //this makes this a recursive function because parameters stores Type
}

std::ostream &operator<<(std::ostream &ostr, const Type &type) {
    std::string actualSpecifier = "DON'T RECOGNIZE THAT TYPE";
    std::string actualDeclarator = "DECLARATOR DOESN'T EXIST";

    if(type.specifier() == 272) {
        actualSpecifier = "INT";
    } else if(type.specifier() == 273) {
        actualSpecifier = "LONG";
    } else if(type.specifier() == 259) {
        actualSpecifier = "CHAR";
    } else if(type.specifier() == 285) {
        actualSpecifier = "VOID";
    } 

    if(type.declarator() == 0) {
        ostr << actualSpecifier;
        for(unsigned x = 0; x < type.indirection(); x++) {
            ostr << "*";
        }
        ostr << "[" << type.length() << "]";
        actualDeclarator = "ARRAY";
    } else if(type.declarator() == 1) {
        actualDeclarator = "ERROR";
    } else if(type.declarator() == 2) {
        ostr << actualSpecifier << " ";
        for(unsigned x = 0; x < type.indirection(); x++) {
            ostr << "*";
        }
        cout << "()";
        actualDeclarator = "FUNCTION";
    } else if(type.declarator() == 3) {
        ostr << actualSpecifier;
        for(unsigned x = 0; x < type.indirection(); x++) {
            ostr << "*";
        }
        actualDeclarator = "SCALAR";
    }

    // ostr << "(" << actualSpecifier << ", " << type.indirection() << ", " << actualDeclarator << ")" << endl;
    // if(actualDeclarator == "FUNCTION" && type.parameters() != nullptr) {
    //     cout << "PARAMETERS:" << endl;
    //     for(int i = 0; i < type.parameters()->size(); i++) {
    //         ostr << "   ";

    //         if((*type.parameters())[i].specifier() == 272) {
    //             actualSpecifier = "INT";
    //         } else if((*type.parameters())[i].specifier() == 273) {
    //             actualSpecifier = "LONG";
    //         } else if((*type.parameters())[i].specifier() == 259) {
    //             actualSpecifier = "CHAR";
    //         } else if((*type.parameters())[i].specifier() == 285) {
    //             actualSpecifier = "VOID";
    //         } 
    //         if((*type.parameters())[i].declarator() == 0) {
    //             actualDeclarator = "ARRAY";
    //         } else if((*type.parameters())[i].declarator() == 1) {
    //             actualDeclarator = "ERROR";
    //         } else if((*type.parameters())[i].declarator() == 2) {
    //             actualDeclarator = "FUNCTION";
    //         } else if((*type.parameters())[i].declarator() == 3) {
    //             actualDeclarator = "SCALAR";
    //         }

    //         ostr << "(" << actualSpecifier << ", " << (*type.parameters())[i].indirection() << ", " << actualDeclarator << ")" << endl;
    //     }
    // }
    return ostr;
}