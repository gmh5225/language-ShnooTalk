#include "../Builder/TypeCheck.hpp"
#include "Expression.hpp"
#include "UnitFromIdentifier.hpp"

#include "Assignment.hpp"

icode::Instruction assignmentTokenToBinaryOperator(const generator::GeneratorContext& ctx, const Token tok)
{
    switch (tok.getType())
    {
        case token::PLUS_EQUAL:
            return icode::ADD;
        case token::MINUS_EQUAL:
            return icode::SUB;
        case token::DIVIDE_EQUAL:
            return icode::DIV;
        case token::MULTIPLY_EQUAL:
            return icode::MUL;
        case token::OR_EQUAL:
            return icode::BWO;
        case token::AND_EQUAL:
            return icode::BWA;
        case token::XOR_EQUAL:
            return icode::BWX;
        default:
            ctx.console.internalBugErrorOnToken(tok);
    }
}

void assignmentFromTree(generator::GeneratorContext& ctx, const Node& root, const Unit& LHS, const Unit& RHS)
{
    Token assignOperator = root.getNthChildTokenFromLast(2);

    if (LHS.isLiteral())
        ctx.console.compileErrorOnToken("Cannot assign to LITERAL", root.children[0].tok);

    if (!isSameType(LHS, RHS))
        ctx.console.typeError(root.getNthChildTokenFromLast(1), LHS, RHS);

    if (!LHS.isMutable() && root.type == node::ASSIGNMENT)
        ctx.console.compileErrorOnToken("Cannot modify IMMUTABLE variable or parameter", root.children[0].tok);

    if ((LHS.isStruct() || LHS.isArray()) && !assignOperator.isEqualOrLeftArrow())
        ctx.console.compileErrorOnToken("Only EQUAL operator allowed on STRUCT and ARRAY", assignOperator);

    if (assignOperator.isBitwiseOperation() && !LHS.isIntegerType())
        ctx.console.compileErrorOnToken("Bitwise operation not allowed on FLOAT", assignOperator);

    if (assignOperator.getType() == token::EQUAL)
    {
        if (LHS.isPointer() && root.type != node::ASSIGNMENT)
            ctx.console.compileErrorOnToken("Non pointer initialization for POINTER", assignOperator);

        if (LHS.isArray() && RHS.isPointer())
            ctx.console.compileErrorOnToken("Cannot assign POINTER to ARRAY", root.getNthChildTokenFromLast(1));

        ctx.ir.functionBuilder.unitCopy(LHS, RHS);
    }
    else if (assignOperator.getType() == token::LEFT_ARROW)
    {
        if (!LHS.isPointer())
            ctx.console.compileErrorOnToken("Pointer assignment on a NON POINTER", assignOperator);

        if (!RHS.isValidForPointerAssignment())
            ctx.console.compileErrorOnToken("Invalid expression for POINTER ASSIGNMENT",
                                            root.getNthChildTokenFromLast(1));

        ctx.ir.functionBuilder.unitPointerAssign(LHS, RHS);
    }
    else
    {
        icode::Instruction instruction = assignmentTokenToBinaryOperator(ctx, assignOperator);
        ctx.ir.functionBuilder.unitCopy(LHS, ctx.ir.functionBuilder.binaryOperator(instruction, LHS, RHS));
    }
}

void assignment(generator::GeneratorContext& ctx, const Node& root)
{

    Unit LHS = expression(ctx, root.children[0]);

    Unit RHS = expression(ctx, root.children[2]);

    assignmentFromTree(ctx, root, LHS, RHS);
}