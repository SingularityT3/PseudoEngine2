#include "procedure.h"

using namespace PSC;

Parameter::Parameter(const std::string &name, PSC::DataType type)
    : name(name), type(type)
{}

Procedure::Procedure(const std::string &name)
    : name(name)
{}
