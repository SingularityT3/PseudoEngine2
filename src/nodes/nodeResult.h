#pragma once
#include <memory>
#include <concepts>
#include "psc/types/types.h"

struct NodeResult {
    std::unique_ptr<const PSC::Value> data;
    PSC::DataType type;

    NodeResult(const PSC::Value *data, PSC::DataType type);

    NodeResult(std::unique_ptr<const PSC::Value> &&data, PSC::DataType type);

    void implicitCast(PSC::DataType target);

    template<std::derived_from<PSC::Value> T>
    const T &get() const { return *((const T*) data.get()); }
};
