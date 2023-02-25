cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(PseudoEngine2 VERSION 0.5)

add_executable(PseudoEngine2)

target_include_directories(PseudoEngine2 PRIVATE src)

if(MSVC)
    string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    target_compile_options(PseudoEngine2 PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_precompile_headers(PseudoEngine2 PRIVATE src/pch.h)

target_sources(PseudoEngine2 PRIVATE 
    src/lexer/symbolLexer.cpp
    src/lexer/lexer.cpp
    src/lexer/tokens.cpp

    src/nodes/eval/arithmetic.cpp
    src/nodes/eval/comparison.cpp
    src/nodes/eval/stringcat.cpp
    src/nodes/eval/logic.cpp
    src/nodes/loop/while.cpp
    src/nodes/loop/for.cpp
    src/nodes/loop/repeatUntil.cpp
    src/nodes/loop/control.cpp
    src/nodes/functions/procedure.cpp
    src/nodes/functions/function.cpp
    src/nodes/selection/ifStatement.cpp
    src/nodes/selection/case.cpp
    src/nodes/variable/variable.cpp
    src/nodes/variable/array.cpp
    src/nodes/variable/enum.cpp
    src/nodes/variable/pointer.cpp
    src/nodes/variable/composite.cpp
    src/nodes/variable/resolver.cpp
    src/nodes/cast.cpp
    src/nodes/io/file.cpp
    src/nodes/io/io.cpp
    src/nodes/nodeResult.cpp
    src/nodes/base.cpp

    src/parser/evalExprParser.cpp
    src/parser/variableExprParser.cpp
    src/parser/arrayOperationParser.cpp
    src/parser/userType.cpp
    src/parser/procedureParser.cpp
    src/parser/functionParser.cpp
    src/parser/selectionParser.cpp
    src/parser/loopParser.cpp
    src/parser/ioParser.cpp
    src/parser/parser.cpp

    src/psc/types/numeric.cpp
    src/psc/types/boolean.cpp
    src/psc/types/character.cpp
    src/psc/types/date.cpp
    src/psc/types/userType.cpp
    src/psc/types/types.cpp
    src/psc/types/type_definitions.cpp
    src/psc/types/type_names.cpp
    src/psc/scope/block.cpp
    src/psc/scope/context.cpp
    src/psc/variable.cpp
    src/psc/array.cpp
    src/psc/procedure.cpp
    src/psc/file.cpp
    src/psc/builtinFunctions/string.cpp
    src/psc/builtinFunctions/char.cpp
    src/psc/builtinFunctions/numeric.cpp
    src/psc/builtinFunctions/math.cpp
    src/psc/error.cpp
    
    src/launch/repl.cpp
    src/launch/runFile.cpp

    src/main.cpp
)

install(TARGETS PseudoEngine2)

enable_testing()

function(test test_file)
    add_test(NAME ${test_file} COMMAND PseudoEngine2 ${CMAKE_CURRENT_LIST_DIR}/tests/${test_file})
endfunction()

test(selection.pseudo)
test(loop.pseudo)
test(date.pseudo)
test(arrays.pseudo)
test(procedure.pseudo)
test(function.pseudo)
test(builtin_functions.pseudo)
test(str.pseudo)
test(enum.pseudo)
test(pointer.pseudo)
test(types.pseudo)
test(files.pseudo)
