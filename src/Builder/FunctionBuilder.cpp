#include "FunctionBuilder.hpp"
#include "TypeDescriptionUtil.hpp"

using namespace icode;

FunctionBuilder::FunctionBuilder(StringModulesMap& modulesMap,
                                 Console& console,
                                 OperandBuilder& opBuilder,
                                 EntryBuilder& entryBuilder)

  : modulesMap(modulesMap)
  , console(console)
  , opBuilder(opBuilder)
  , entryBuilder(entryBuilder)
{
}

Operand FunctionBuilder::getPointerOperand(const Unit& unit)
{
    ModuleDescription* workingModule = &modulesMap.at(unit.second.moduleName);

    if (!unit.first.isPointer())
        return entryBuilder.createPointer(unit.first, unit.second.dtypeName, workingModule);

    return unit.first;
}

Unit FunctionBuilder::getStructField(const Token& fieldName, const Unit& unit)
{
    StructDescription structDescription = modulesMap[unit.second.moduleName].structures[unit.second.dtypeName];

    TypeDescription fieldType;

    if (!structDescription.getField(fieldName.toString(), fieldType))
        console.compileErrorOnToken("Undefined STRUCT field", fieldName);

    if (unit.second.isMutable())
        fieldType.becomeMutable();

    Operand pointerOperand = getPointerOperand(unit);

    Operand fieldOperand =
      entryBuilder.addressAddOperator(pointerOperand, opBuilder.createLiteralAddressOperand(fieldType.offset));

    fieldOperand.dtype = fieldType.dtype;

    return Unit(fieldOperand, fieldType);
}

Unit FunctionBuilder::getIndexedElement(const Unit& unit, const std::vector<Unit>& indices)
{
    unsigned int dimensionCount = 0;
    unsigned int elementWidth = unit.second.size / unit.second.dimensions[0];

    Operand elementOperand = getPointerOperand(unit);

    TypeDescription elementType = unit.second;

    for (const Unit indexUnit : indices)
    {
        Operand subscriptOperand =
          entryBuilder.addressMultiplyOperator(indexUnit.first, opBuilder.createLiteralAddressOperand(elementWidth));

        if (dimensionCount + 1 != elementType.dimensions.size())
            elementWidth /= elementType.dimensions[dimensionCount + 1];

        elementOperand = entryBuilder.addressAddOperator(elementOperand, subscriptOperand);

        dimensionCount++;
    }

    unsigned int remainingDimensionCount = elementType.dimensions.size() - dimensionCount;

    elementType.dimensions.erase(elementType.dimensions.begin(),
                                 elementType.dimensions.end() - remainingDimensionCount);

    return Unit(elementOperand, elementType);
}

Unit FunctionBuilder::binaryOperator(Instruction instruction, const Unit& LHS, const Unit& RHS)
{
    DataType dtype = LHS.second.dtype;

    Operand result =
      entryBuilder.binaryOperator(instruction, opBuilder.createTempOperand(dtype), LHS.first, RHS.first);

    return Unit(result, LHS.second);
}

Unit FunctionBuilder::castOperator(const Unit& unitToCast, DataType destinationDataType)
{
    Operand result = entryBuilder.castOperator(destinationDataType, unitToCast.first);

    return Unit(result, typeDescriptionFromDataType(destinationDataType));
}

Unit FunctionBuilder::unaryOperator(Instruction instruction, const Unit& unaryOperatorTerm)
{
    const DataType dtype = unaryOperatorTerm.second.dtype;

    Operand result =
      entryBuilder.unaryOperator(instruction, opBuilder.createTempOperand(dtype), unaryOperatorTerm.first);

    return Unit(result, unaryOperatorTerm.second);
}

Operand FunctionBuilder::createLabel(const Token& tok, bool isTrueLabel, std::string prefix)
{
    std::string label_name = tok.getLineColString();

    return opBuilder.createLabelOperand("_" + prefix + "_" + (isTrueLabel ? "true" : "false") + label_name);
}

void FunctionBuilder::insertLabel(const Operand& label)
{
    entryBuilder.label(label);
}

void FunctionBuilder::createIfTrueGoto(const Operand& label)
{
    entryBuilder.createBranch(IF_TRUE_GOTO, label);
}

void FunctionBuilder::createIfFalseGoto(const Operand& label)
{
    entryBuilder.createBranch(IF_FALSE_GOTO, label);
}

void FunctionBuilder::createGoto(const Operand& label)
{
    entryBuilder.createBranch(GOTO, label);
}

void FunctionBuilder::compareOperator(Instruction instruction, const Unit& LHS, const Unit& RHS)
{
    entryBuilder.compareOperator(instruction, LHS.first, RHS.first);
}

void FunctionBuilder::passParameter(const Token& calleeNameToken,
                                    FunctionDescription callee,
                                    const Unit& formalParam,
                                    const Unit& actualParam)
{
    if (formalParam.second.isMutable() || formalParam.second.isStruct() || formalParam.second.isArray())
        entryBuilder.pass(PASS_ADDR, actualParam.first, calleeNameToken.toString(), callee);
    else
        entryBuilder.pass(PASS, actualParam.first, calleeNameToken.toString(), callee);
}

Unit FunctionBuilder::callFunction(const Token& calleeNameToken, FunctionDescription callee)
{
    Operand result = entryBuilder.call(calleeNameToken.toString(), callee);
    return Unit(result, callee.functionReturnType);
}

bool FunctionBuilder::terminateFunction()
{
    return entryBuilder.terminateFunction();
}

void FunctionBuilder::createInput(const Unit& unit)
{
    if (unit.second.isArray())
        entryBuilder.inputOperator(INPUT_STR, unit.first, unit.second.dimensions[0]);
    else
        entryBuilder.inputOperator(INPUT, unit.first);
}

void FunctionBuilder::createPrintStringLtrl(const Unit& stringLiteral)
{
    entryBuilder.printOperator(PRINT_STR, stringLiteral.first);
}

void FunctionBuilder::createPrint(const Unit& unit)
{
    if (unit.second.isArray())
        entryBuilder.printOperator(icode::PRINT_STR, unit.first);
    else
        entryBuilder.printOperator(icode::PRINT, unit.first);
}

void FunctionBuilder::createPrintNewln()
{
    entryBuilder.noArgumentEntry(NEWLN);
}

void FunctionBuilder::createPrintSpace()
{
    entryBuilder.noArgumentEntry(SPACE);
}