#pragma once
#include "psc/procedure.h"

namespace PSC {
    struct BuiltinFnLength : public Function {
        BuiltinFnLength();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnRight : public Function {
        BuiltinFnRight();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnMid : public Function {
        BuiltinFnMid();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnLCase : public Function {
        BuiltinFnLCase();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnUCase : public Function {
        BuiltinFnUCase();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnRand : public Function {
        BuiltinFnRand();

        void run(PSC::Context &ctx) override;
    };
}
