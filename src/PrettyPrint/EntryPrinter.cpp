#include <iostream>
#include <string>
#include <vector>

#include "EntryPrinter.hpp"
#include "FlatJSONPrinter.hpp"
#include "Strings.hpp"

std::string operandToName(const icode::Operand& op)
{
    switch (op.operandType)
    {
        case icode::TEMP:
            return icode::dataTypeToString(op.dtype) + " temp" + std::to_string(op.operandId);
        case icode::TEMP_PTR:
            return icode::dataTypeToString(op.dtype) + "* tempPtr" + std::to_string(op.operandId);
        case icode::VAR:
        case icode::GBL_VAR:
            return icode::dataTypeToString(op.dtype) + " " + op.name;
        case icode::PTR:
            return icode::dataTypeToString(op.dtype) + "* " + op.name;
        case icode::RET_VALUE:
            return icode::dataTypeToString(op.dtype) + " returnValue" + std::to_string(op.operandId);
        case icode::CALLEE_RET_VAL:
            return icode::dataTypeToString(op.dtype) + " calleeReturnValue" + std::to_string(op.operandId);
        case icode::STR_DATA:
            return "stringDataAt(" + op.name + ")";
        case icode::ADDR:
            return "bytes " + std::to_string(op.val.address);
        case icode::LITERAL:
        {
            if (icode::isInteger(op.dtype))
                return icode::dataTypeToString(op.dtype) + " " + std::to_string(op.val.integer);
            else
                return icode::dataTypeToString(op.dtype) + " " + std::to_string(op.val.floating);

            break;
        }
        case icode::LABEL:
            return op.name;
        case icode::MODULE:
            return "module " + op.name;
        case icode::NONE:
            return "";
    }
}

std::string prettyPrintEntryEqual(const icode::Entry& entry)
{
    std::string entryString = "    ";

    entryString += operandToName(entry.op1) + " = ";

    entryString += instructionToString[entry.opcode] + " ";

    entryString += operandToName(entry.op2);

    if (entry.op3.operandType != icode::NONE)
    {
        entryString += ", ";
        entryString += operandToName(entry.op3);
    }

    return entryString;
}

std::string prettyPrintEntryCompare(const icode::Entry& entry)
{
    std::string entryString = "    flag = ";

    entryString += instructionToString[entry.opcode] + " ";

    entryString += operandToName(entry.op1);
    entryString += ", ";
    entryString += operandToName(entry.op2);

    return entryString;
}

std::string prettyPrintNoLHS(const icode::Entry& entry)
{
    std::string entryString = "    ";

    entryString += instructionToString[entry.opcode] + " ";

    if (entry.op1.operandType != icode::NONE)
        entryString += operandToName(entry.op1);

    if (entry.op2.operandType != icode::NONE)
    {
        entryString += ", ";
        entryString += operandToName(entry.op2);
    }

    if (entry.op3.operandType != icode::NONE)
    {
        entryString += ", ";
        entryString += operandToName(entry.op3);
    }

    return entryString;
}

std::string prettyPrintEntry(const icode::Entry& entry)
{
    switch (entry.opcode)
    {
        case icode::EQUAL:
        case icode::ADD:
        case icode::SUB:
        case icode::MUL:
        case icode::DIV:
        case icode::MOD:
        case icode::LSH:
        case icode::RSH:
        case icode::BWA:
        case icode::BWO:
        case icode::BWX:
        case icode::NOT:
        case icode::UNARY_MINUS:
        case icode::CAST:
        case icode::CREATE_PTR:
        case icode::ADDR_ADD:
        case icode::ADDR_MUL:
        case icode::READ:
        case icode::WRITE:
        case icode::MEMCPY:
            return prettyPrintEntryEqual(entry);
        case icode::EQ:
        case icode::NEQ:
        case icode::LT:
        case icode::LTE:
        case icode::GT:
        case icode::GTE:
        case icode::CALL:
            return prettyPrintEntryCompare(entry);
        case icode::CREATE_LABEL:
            return operandToName(entry.op1) + ":";
        case icode::IF_TRUE_GOTO:
            return "    if(flag) goto " + operandToName(entry.op1);
        case icode::IF_FALSE_GOTO:
            return "    if(not flag) goto " + operandToName(entry.op1);
        case icode::GOTO:
            return "    goto " + operandToName(entry.op1);
        case icode::PRINT:
        case icode::PRINT_STR:
        case icode::PASS:
        case icode::PASS_ADDR:
        case icode::INPUT:
        case icode::INPUT_STR:
            return prettyPrintNoLHS(entry);
        case icode::NEWLN:
        case icode::SPACE:
        case icode::RET:
        case icode::EXIT:
            return "    " + instructionToString[entry.opcode];
        default:
            return "Error pretty printing entry";
    }
}

void prettyPrintIcodeTable(const std::vector<icode::Entry>& icodeTable, int indentLevel)
{
    std::cout << std::endl << std::string((indentLevel - 1) * 4, ' ') << "\"icode\": [" << std::endl;

    for (const icode::Entry& e : icodeTable)
    {
        std::cout << std::string(indentLevel * 4, ' ');
        std::cout << prettyPrintEntry(e) << std::endl;
    }

    std::cout << std::string((indentLevel - 1) * 4, ' ') << "]";
}

std::vector<std::string> icodeTableToStringArray(const std::vector<icode::Entry>& icodeTable)
{
    std::vector<std::string> stringArray;

    for (icode::Entry e : icodeTable)
        stringArray.push_back(prettyPrintEntry(e));

    return stringArray;
}