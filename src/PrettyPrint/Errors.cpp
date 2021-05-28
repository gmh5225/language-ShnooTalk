#include <fstream>
#include <iostream>

#include "Strings.hpp"

#include "Errors.hpp"

namespace mikpp
{
    void println(const std::string& msg)
    {
        std::cout << msg << std::endl;
    }

    void errorOnLine(const std::string& moduleName,
                     const std::string& errorMessage,
                     const std::string& line,
                     int lineno,
                     int col)
    {
        /* Accepts line as string and column, prints line and '^' symbol at col
            along with error message */
        std::cout << "MODULE " + moduleName << std::endl;
        std::cout << "ERROR in ";
        std::cout << "Line " << lineno;
        std::cout << " Col " << col + 1 << std::endl;
        std::cout << std::endl;
        std::cout << line << std::endl;
        std::cout << std::string(col, ' ') << "^" << std::endl;
        std::cout << errorMessage << std::endl;
    }

    void errorOnModuleName(const std::string& moduleName, const std::string& errorMessage)
    {
        std::cout << "MODULE " + moduleName << std::endl;
        std::cout << errorMessage << std::endl;
    }

    void errorOnToken(const std::string& moduleName,
                      const std::string& errorMessage,
                      std::ifstream& file,
                      const token::Token& tok)
    {
        /* Prints the exact line from file using tok
            and error message */

        /* Goto beginning */
        file.clear();
        file.seekg(0, std::ios::beg);

        /* Get line */
        std::string line;
        for (size_t i = 0; i < tok.getLine(); i++)
            getline(file, line);

        errorOnLine(moduleName, errorMessage, line, tok.getLine(), tok.getColumn());
    }

    void parserError(const std::string& moduleName, token::tokenType expected, token::Token& found, std::ifstream& file)
    {
        /* Used by parser when it finds some other token type than expected */

        std::string errorMessage = "Did not expect " + tokenTypeToString[found.getType()];
        errorMessage += ",\nexpected " + tokenTypeToString[expected];
        errorOnToken(moduleName, errorMessage, file, found);
    }

    void parserErrorMultiple(const std::string& moduleName,
                             const token::tokenType* expected,
                             int ntoks,
                             const token::Token& found,
                             std::ifstream& file)
    {
        /* Used by parser when if finds a token type that does not match
           multiple token types expected */

        std::string errorMessage = "Did not expect " + tokenTypeToString[found.getType()];
        errorMessage += ",\nexpected ";

        for (size_t i = 0; i < ntoks - 1; i++)
            errorMessage += ((tokenTypeToString[expected[i]]) + " or ");

        errorMessage += tokenTypeToString[expected[ntoks - 1]];

        errorOnToken(moduleName, errorMessage, file, found);
    }

    std::string variableDescriptionToTypeString(icode::VariableDescription& var)
    {
        std::string var_str = var.dtypeName;

        for (unsigned int dim : var.dimensions)
            var_str += "[" + std::to_string(dim) + "]";

        return var_str;
    }

    void typeError(const std::string& moduleName,
                   std::ifstream& file,
                   const token::Token& tok,
                   icode::VariableDescription& expected,
                   icode::VariableDescription& found)
    {
        /* Used by ir generator for type errors */

        std::string found_str = variableDescriptionToTypeString(found);
        std::string expected_str = variableDescriptionToTypeString(expected);

        std::string expect_msg = "Type error, did not expect " + found_str;
        expect_msg += ",\nexpected " + expected_str;

        errorOnToken(moduleName, expect_msg, file, tok);
    }

    void internalCompilerErrorToken(const std::string& moduleName, std::ifstream& file, const token::Token& tok)
    {
        errorOnToken(moduleName, "Internal compiler error, REPORT THIS BUG", file, tok);
    }

    void internalCompileError(const std::string& moduleName)
    {
        errorOnModuleName(moduleName, "Internal compiler error, REPORT THIS BUG");
    }
}