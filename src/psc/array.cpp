#include "psc/array.h"

using namespace PSC;

ArrayDimension::ArrayDimension(int n, int_t lowerBound, int_t upperBound)
    : n(n), lowerBound(lowerBound), upperBound(upperBound)
{}

int_t ArrayDimension::getSize() {
    return (upperBound - lowerBound) + 1;
}

bool ArrayDimension::isValidIndex(int_t idx) {
    return idx >= lowerBound && idx <= upperBound;
}


Array::Array(const std::string &name, DataType type)
    : name(name), type(type)
{}

void Array::init() {
    unsigned long size = 1;
    for (auto &dim : dimensions) {
        size *= dim.getSize();
    }

    data.reserve(size);

    switch (type) {
        case PSC::DT_INTEGER:
            _init<PSC::Integer>();
            break;
        case PSC::DT_REAL:
            _init<PSC::Real>();
            break;
        case PSC::DT_BOOLEAN:
            _init<PSC::Boolean>();
            break;
        case PSC::DT_CHAR:
            _init<PSC::Char>();
            break;
        case PSC::DT_STRING:
            _init<PSC::String>();
            break;
        default:
            std::abort();
    }
}

Value &Array::getElement(const std::vector<int_t> &index) {
    if (index.size() != dimensions.size()) std::abort();

    unsigned long realIndex = 0;
    unsigned long prevSize = 1;

    for (unsigned int i = 0; i < index.size(); i++) {
        realIndex += (index[i] - dimensions[i].lowerBound) * prevSize;
        prevSize *= dimensions[i].getSize();
    }

    return *(data[realIndex]);
}
