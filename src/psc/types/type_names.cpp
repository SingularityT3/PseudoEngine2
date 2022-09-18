#include "pch.h"

#include "psc/types/types.h"

std::ostream &PSC::operator<<(std::ostream &os, PSC::DataType &type) {
    switch (type) {
        case PSC::DataType::NONE:
            os << "None";
            break;
        case PSC::DataType::INTEGER:
            os << "Integer";
            break;
        case PSC::DataType::REAL:
            os << "Real";
            break;
        case PSC::DataType::BOOLEAN:
            os << "Boolean";
            break;
        case PSC::DataType::CHAR:
            os << "Char";
            break;
        case PSC::DataType::STRING:
            os << "String";
            break;
    }
    return os;
}
