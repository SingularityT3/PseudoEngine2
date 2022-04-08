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

    struct BuiltinFnLeft : public Function {
        BuiltinFnLeft();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnToUpper : public Function {
        BuiltinFnToUpper();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnToLower : public Function {
        BuiltinFnToLower();

        void run(PSC::Context &ctx) override;
    };


    struct BuiltinFnNumToStr : public Function {
        BuiltinFnNumToStr();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnStrToNum : public Function {
        BuiltinFnStrToNum();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnIsNum : public Function {
        BuiltinFnIsNum();

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

    struct BuiltinFnASC : public Function {
        BuiltinFnASC();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnCHR : public Function {
        BuiltinFnCHR();

        void run(PSC::Context &ctx) override;
    };


    struct BuiltinFnRand : public Function {
        BuiltinFnRand();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnInt : public Function {
        BuiltinFnInt();

        void run(PSC::Context &ctx) override;
    };
}
