#include "GetAndSetLLVM.hpp"

#include "CompareOperator.hpp"

using namespace llvm;

Value* equal(const ModuleContext& ctx, Value* LHS, Value* RHS, icode::DataType dtype)
{
    /* Converts mikuro EQ to llvm ir */

    if (icode::isInteger(dtype))
        return ctx.builder->CreateICmpEQ(LHS, RHS);

    if (icode::isFloat(dtype))
        return ctx.builder->CreateFCmpUEQ(LHS, RHS);

    ctx.console.internalBugError();
}

Value* notEqual(const ModuleContext& ctx, Value* LHS, Value* RHS, icode::DataType dtype)
{
    /* Converts mikuro NEQ to llvm ir */

    if (icode::isInteger(dtype))
        return ctx.builder->CreateICmpNE(LHS, RHS);

    if (icode::isFloat(dtype))
        return ctx.builder->CreateFCmpUNE(LHS, RHS);

    ctx.console.internalBugError();
}

Value* lessThan(const ModuleContext& ctx, Value* LHS, Value* RHS, icode::DataType dtype)
{
    /* Converts mikuro LT to llvm ir */

    if (icode::isSignedInteger(dtype))
        return ctx.builder->CreateICmpSLT(LHS, RHS);

    if (icode::isUnsignedInteger(dtype))
        return ctx.builder->CreateICmpULT(LHS, RHS);

    if (icode::isFloat(dtype))
        return ctx.builder->CreateFCmpULT(LHS, RHS);

    ctx.console.internalBugError();
}

Value* lessThanOrEqualTo(const ModuleContext& ctx, Value* LHS, Value* RHS, icode::DataType dtype)
{
    /* Converts mikuro LTE to llvm ir */

    if (icode::isSignedInteger(dtype))
        return ctx.builder->CreateICmpSLE(LHS, RHS);

    if (icode::isUnsignedInteger(dtype))
        return ctx.builder->CreateICmpULE(LHS, RHS);

    if (icode::isFloat(dtype))
        return ctx.builder->CreateFCmpULE(LHS, RHS);

    ctx.console.internalBugError();
}

Value* greaterThan(const ModuleContext& ctx, Value* LHS, Value* RHS, icode::DataType dtype)
{
    /* Converts mikuro GT to llvm ir */

    if (icode::isSignedInteger(dtype))
        return ctx.builder->CreateICmpSGT(LHS, RHS);

    if (icode::isUnsignedInteger(dtype))
        return ctx.builder->CreateICmpUGT(LHS, RHS);

    if (icode::isFloat(dtype))
        return ctx.builder->CreateFCmpUGT(LHS, RHS);

    ctx.console.internalBugError();
}

Value* greaterThanOrEqualTo(const ModuleContext& ctx, Value* LHS, Value* RHS, icode::DataType dtype)
{
    /* Converts mikuro GTE to llvm ir */

    if (icode::isSignedInteger(dtype))
        return ctx.builder->CreateICmpSGE(LHS, RHS);

    if (icode::isUnsignedInteger(dtype))
        return ctx.builder->CreateICmpUGE(LHS, RHS);

    if (icode::isFloat(dtype))
        return ctx.builder->CreateFCmpUGE(LHS, RHS);

    ctx.console.internalBugError();
}

Value* getCompareOperatorValue(const ModuleContext& ctx, const icode::Entry& e)
{
    Value* LHS = getLLVMValue(ctx, e.op1);
    Value* RHS = getLLVMValue(ctx, e.op2);
    icode::DataType dtype = e.op1.dtype;

    switch (e.opcode)
    {
        case icode::EQ:
            return equal(ctx, LHS, RHS, dtype);
        case icode::NEQ:
            return notEqual(ctx, LHS, RHS, dtype);
        case icode::LT:
            return lessThan(ctx, LHS, RHS, dtype);
        case icode::LTE:
            return lessThanOrEqualTo(ctx, LHS, RHS, dtype);
        case icode::GT:
            return greaterThan(ctx, LHS, RHS, dtype);
        case icode::GTE:
            return greaterThanOrEqualTo(ctx, LHS, RHS, dtype);
        default:
            ctx.console.internalBugError();
    }
}

void compareOperator(const ModuleContext& ctx, BranchContext& branchContext, const icode::Entry& e)
{
    branchContext.branchFlags.push(getCompareOperatorValue(ctx, e));
}