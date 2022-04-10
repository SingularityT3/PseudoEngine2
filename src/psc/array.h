#pragma once
#include <vector>
#include <memory>
#include <string>
#include <concepts>
#include "psc/variable.h"

namespace PSC {
    struct ArrayDimension {
        int n;
        int_t lowerBound, upperBound;

        ArrayDimension(int n, int_t lowerBound, int_t upperBound);

        int_t getSize();

        bool isValidIndex(int_t idx);
    };

    class Array {
    private:
        std::vector<std::unique_ptr<Value>> data;

        template<std::derived_from<Value> T>
        void _init() {
            size_t size = data.capacity();
            for (size_t i = 0; i < size; i++) {
                data.emplace_back(new T());
            }
        }

    public:
        const std::string name;
        const DataType type;
        std::vector<ArrayDimension> dimensions;

        Array(const std::string &name, DataType type);

        // Allocates memory for elements
        void init();

        Value &getElement(const std::vector<int_t> &index);
    };
};
