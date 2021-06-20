#include "Console.hpp"

void Console::compileErrorOnToken(const std::string& message, const Token& tok)
{
    pp::errorOnToken(fileName, message, *file, tok);
    throw CompileError();
}

void Console::typeError(const Token& tok, icode::TypeDescription& expected, icode::TypeDescription& found)
{
    pp::typeError(fileName, *file, tok, expected, found);
    throw CompileError();
}

void Console::internalBugErrorOnToken(const Token& tok)
{
    pp::errorOnToken(fileName, "Internal compiler error, REPORT THIS BUG", *file, tok);
    throw InternalBugError();
}

void Console::internalBugError()
{
    pp::errorOnModuleName(fileName, "Internal compiler error, REPORT THIS BUG");
    throw InternalBugError();
}

void Console::internalBugErrorMessage(const std::string& message)
{
    pp::println(message);
    throw InternalBugError();
}

void Console::parseError(token::TokenType& expected, Token& found)
{
    pp::parserError(fileName, expected, found, *file);
    throw CompileError();
}

void Console::parserErrorMultiple(const token::TokenType* expected, int ntoks, const Token& found)
{
    pp::parserErrorMultiple(fileName, expected, ntoks, found, *file);
    throw CompileError();
}

void Console::lexerError(const std::string& errorMessage, const std::string& line, int lineno, int col)
{
    pp::errorOnLine(fileName, errorMessage, line, lineno, col);
    throw CompileError();
}

std::ifstream* Console::getStream()
{
    return file;
}

Console::Console(const std::string& fileName, std::ifstream* file)
  : fileName(fileName)
{
    this->file = file;
}