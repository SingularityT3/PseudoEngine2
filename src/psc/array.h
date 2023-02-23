#pragma once
#include <vector>
#include <memory>
#include <string>
#include <concepts>
#include "psc/types/types.h"
#include "psc/variable.h"

namespace PSC {
    struct ArrayDimension {
        int n;
        int_t lowerBound, upperBound;

        ArrayDimension(int n, int_t lowerBound, int_t upperBound);

        ArrayDimension(const ArrayDimension &other) = default;

        int_t getSize() const;

        bool isValidIndex(int_t idx) const;
    };

    bool operator==(const ArrayDimension &ad1, const ArrayDimension &ad2);

    class Array : public DataHolder {
    private:
        std::vector<std::unique_ptr<Variable>> data;

        static const std::vector<ArrayDimension> copyDimensions(const std::vector<ArrayDimension> &source);

    public:
        const DataType type;
        const std::vector<ArrayDimension> dimensions;

        Array(const std::string &name, DataType type, const std::vector<ArrayDimension> &dimensions);

        Array(const Array &other);

        void copyData(const Array &other);

        constexpr bool isArray() const override {return true;}

        // Allocates memory for elements
        void init(Context &ctx);

        Variable &getElement(const std::vector<int_t> &index);
    };
};
