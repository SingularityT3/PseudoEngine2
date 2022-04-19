#include "psc/array.h"

using namespace PSC;

ArrayDimension::ArrayDimension(int n, int_t lowerBound, int_t upperBound)
    : n(n), lowerBound(lowerBound), upperBound(upperBound)
{}

int_t ArrayDimension::getSize() const {
    return (upperBound - lowerBound) + 1;
}

bool ArrayDimension::isValidIndex(int_t idx) const {
    return idx >= lowerBound && idx <= upperBound;
}


Array::Array(const std::string &name, DataType type, const std::vector<ArrayDimension> &dimensions)
    : name(name), type(type), dimensions(dimensions)
{}

void Array::init() {
    unsigned long size = 1;
    for (auto &dim : dimensions) {
        size *= dim.getSize();
    }

    data.reserve(size);

    switch (type) {
        case PSC::DataType::INTEGER:
            _init<PSC::Integer>();
            break;
        case PSC::DataType::REAL:
            _init<PSC::Real>();
            break;
        case PSC::DataType::BOOLEAN:
            _init<PSC::Boolean>();
            break;
        case PSC::DataType::CHAR:
            _init<PSC::Char>();
            break;
        case PSC::DataType::STRING:
            _init<PSC::String>();
            break;
        default:
            std::abort();
    }
}

Value &Array::getElement(const std::vector<int_t> &index) {
    if (index.size() != dimensions.size()) std::abort();

    size_t realIndex = 0;
    size_t prevSize = 1;

    for (size_t i = 0; i < index.size(); i++) {
        realIndex += (index[i] - dimensions[i].lowerBound) * prevSize;
        prevSize *= dimensions[i].getSize();
    }

    return *(data[realIndex]);
}
