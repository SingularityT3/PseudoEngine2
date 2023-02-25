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

    struct BuiltinFnEOF : public Function {
    private:
        static Token errToken;
    
    public:
        BuiltinFnEOF();

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


    struct BuiltinFnDAY : public Function {
        BuiltinFnDAY();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnMONTH : public Function {
        BuiltinFnMONTH();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnYEAR : public Function {
        BuiltinFnYEAR();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnDAYINDEX : public Function {
        BuiltinFnDAYINDEX();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnSETDATE : public Function {
    private:
        static Token errToken;

    public:
        BuiltinFnSETDATE();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnTODAY : public Function {
        BuiltinFnTODAY();

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


    struct BuiltinFnPow : public Function {
        BuiltinFnPow();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnExp : public Function {
        BuiltinFnExp();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnSin : public Function {
        BuiltinFnSin();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnCos : public Function {
        BuiltinFnCos();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnTan : public Function {
        BuiltinFnTan();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnASin : public Function {
        BuiltinFnASin();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnACos : public Function {
        BuiltinFnACos();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnATan : public Function {
        BuiltinFnATan();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnATan2 : public Function {
        BuiltinFnATan2();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnSqrt : public Function {
        BuiltinFnSqrt();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnLog : public Function {
        BuiltinFnLog();

        void run(PSC::Context &ctx) override;
    };

    struct BuiltinFnLn : public Function {
        BuiltinFnLn();

        void run(PSC::Context &ctx) override;
    };
}
