#include "Strings.hpp"

const std::string tokenTypeToString[] = {
    "NONE",
    "SPACE",
    "FUNCTION",
    "EXTERN_FUNCTION",
    "EXTERN_C",
    "IDENTIFIER",
    "LPAREN",
    "RPAREN",
    "STRUCT",
    "ENUM",
    "DEF",
    "BEGIN",
    "END",
    "COLON",
    "DOUBLE_COLON",
    "COMMA",
    "DOT",
    "VAR",
    "CONST",
    "STR_LITERAL",
    "CHAR_LITERAL",
    "INT_LITERAL",
    "HEX_LITERAL",
    "BIN_LITERAL",
    "FLOAT_LITERAL",
    "IF",
    "ELSEIF",
    "ELSE",
    "WHILE",
    "DO",
    "FOR",
    "LOOP",
    "BREAK",
    "CONTINUE",
    "RETURN",
    "VOID",
    "EQUAL",
    "WALRUS",
    "PLUS_EQUAL",
    "MINUS_EQUAL",
    "DIVIDE_EQUAL",
    "MULTIPLY_EQUAL",
    "OR_EQUAL",
    "AND_EQUAL",
    "XOR_EQUAL",
    "NOT",
    "CONDN_NOT",
    "CAST",
    "PTR_CAST",
    "ARRAY_PTR_CAST",
    "OPEN_SQUARE",
    "CLOSE_SQUARE",
    "OPEN_BRACE",
    "CLOSE_BRACE",
    "EMPTY_SUBSCRIPT",
    "RIGHT_ARROW",
    "LEFT_ARROW",
    "PLUS",
    "MINUS",
    "BITWISE_OR",
    "BITWISE_XOR",
    "RIGHT_SHIFT",
    "LEFT_SHIFT",
    "CONDN_OR",
    "GREATER_THAN",
    "LESS_THAN",
    "GREATER_THAN_EQUAL",
    "LESS_THAN_EQUAL",
    "CONDN_EQUAL",
    "CONDN_NOT_EQUAL",
    "MULTIPLY",
    "DIVIDE",
    "MOD",
    "BITWISE_AND",
    "CONDN_AND",
    "USE",
    "GENERIC",
    "AS",
    "FROM",
    "MUTABLE",
    "SEMICOLON",
    "END_OF_LINE",
    "END_OF_FILE",
    "INVALID",
    "SIZEOF",
    "MAKE",
    "ADDR",
    "TYPEOF",
    "PRINT",
    "PRINTLN",
    "INPUT",
    "GENERATED",
};

const std::string nodeTypeToString[] = {
    "PROGRAM",
    "USE",
    "GENERIC",
    "GENERIC_TYPE_PARAM",
    "FROM",
    "BLOCK",
    "MODULE",
    "FUNCTION",
    "EXTERN_FUNCTION",
    "FUNCTION_EXTERN_C",
    "STRUCT",
    "ENUM",
    "DEF",
    "VAR",
    "CONST",
    "WALRUS_VAR",
    "WALRUS_CONST",
    "DESTRUCTURE_LIST",
    "DESTRUCTURE_VAR",
    "DESTRUCTURE_CONST",
    "IF",
    "ELSEIF",
    "ELSE",
    "WHILE",
    "DO_WHILE",
    "FOR",
    "LOOP",
    "CAST",
    "PTR_CAST",
    "PTR_ARRAY_CAST",
    "BREAK",
    "CONTINUE",
    "ASSIGNMENT",
    "RETURN",
    "FUNCCALL",
    "GENERIC_FUNCCALL",
    "METHODCALL",
    "PARAM",
    "MUT_PARAM",
    "EXPRESSION",
    "STR_LITERAL",
    "MULTILINE_STR_LITERAL",
    "LITERAL",
    "IDENTIFIER",
    "ASSIGN_OPERATOR",
    "OPERATOR",
    "UNARY_OPR",
    "SUBSCRIPT",
    "POINTER_STAR",
    "EMPTY_SUBSCRIPT",
    "STRUCT_FIELD",
    "INITLIST",
    "TERM",
    "SIZEOF",
    "MAKE",
    "ADDR",
    "TYPEOF",
    "PRINT",
    "PRINTLN",
    "SPACE",
    "INPUT",
};

const std::string typePropertiesString[] = { "isMutable", "isPointer",       "isParameter",     "isGlobal",
                                             "isLocal",   "isStringLiteral", "isIncompleteType" };

const std::string operandTypeToString[] = {
    "temp",      "tempPtr", "tempPtrPtr",  "bytes",         "stringData",        "var",
    "globalVar", "ptr",     "returnValue", "returnPointer", "calleeReturnValue", "calleeReturnPointer",
    "literal",   "label",   "module",      "none"
};

const std::string dimensionTypesToString[] = { "fixedDim", "stringLiteralDim" };

const std::string instructionToString[] = { "pass",
                                            "passPointer",
                                            "passPointerPointer",
                                            "call",
                                            "return",
                                            "addressAdd",
                                            "addressMultiply",
                                            "equal",
                                            "pointerAssign",
                                            "read",
                                            "write",
                                            "createPointer",
                                            "allocPointer",
                                            "allocArrayPointer",
                                            "cast",
                                            "addr",
                                            "pointerCast",
                                            "unaryMinus",
                                            "not",
                                            "multiply",
                                            "divide",
                                            "remainder",
                                            "add",
                                            "subtract",
                                            "rightShift",
                                            "leftShift",
                                            "lessThan",
                                            "lessThanOrEqualTo",
                                            "greaterThan",
                                            "greaterThanOrEqualTo",
                                            "isEqual",
                                            "isNotEqual",
                                            "bitwiseAnd",
                                            "bitwiseXor",
                                            "bitwiseOr",
                                            "goto",
                                            "gotoIfTrue",
                                            "gotoIfFalse",
                                            "createLabel",
                                            "print",
                                            "printString",
                                            "space",
                                            "newLine",
                                            "input",
                                            "inputString",
                                            "memoryCopy" };