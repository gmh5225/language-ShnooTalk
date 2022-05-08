#include <string>

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/Host.h"

#include "BranchContext.hpp"
#include "FormatStringsContext.hpp"
#include "GenerateModule.hpp"
#include "Print.hpp"
#include "SetupLLVM.hpp"

#include "LLVMTranslator.hpp"

using namespace llvm;

std::string getTargetTriple(translator::Platform platform)
{
    std::map<translator::Platform, std::string> platformTripleMap = {
        { translator::DEFAULT, sys::getDefaultTargetTriple() },
        { translator::LINUX_x86_64, "x86_64-linux-gnu" },
        { translator::LINUX_ARM64, "arm64-linux-gnu" },
        { translator::MACOS_x86_64, "x86_64-apple-darwin" },
        { translator::MACOS_ARM64, "arm64-apple-darwin" },
        { translator::WASM32, "wasm32" },
        { translator::WASM64, "wasm64" }
    };

    return platformTripleMap.at(platform);
}

void optimizeModule(ModuleContext& ctx)
{
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;

    PassBuilder PB;

    FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });

    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(PassBuilder::OptimizationLevel::O3);

    MPM.run(*ctx.LLVMModule, MAM);
}

void generateLLVMModule(ModuleContext& ctx, bool release, Console& console)
{
    BranchContext branchContext;
    FormatStringsContext formatStringsContext;

    generateModule(ctx, branchContext, formatStringsContext);

    initializeTargetRegistry();

    if (release)
        optimizeModule(ctx);
}

void translator::generateObject(icode::ModuleDescription& moduleDescription,
                                icode::StringModulesMap& modulesMap,
                                Platform platform,
                                bool release,
                                Console& console)
{
    ModuleContext moduleContext(moduleDescription, modulesMap, console);
    generateLLVMModule(moduleContext, release, console);
    const std::string targetTriple = getTargetTriple(platform);
    setupPassManagerAndCreateObject(moduleContext, targetTriple);
}

std::string getLLVMModuleString(const Module& LLVMModule)
{
    std::string moduleString;
    raw_string_ostream OS(moduleString);
    OS << LLVMModule;
    OS.flush();

    return moduleString;
}

std::string translator::generateLLVMModuleString(icode::ModuleDescription& moduleDescription,
                                                 icode::StringModulesMap& modulesMap,
                                                 bool release,
                                                 Console& console)
{
    ModuleContext moduleContext(moduleDescription, modulesMap, console);
    generateLLVMModule(moduleContext, release, console);
    return getLLVMModuleString(*moduleContext.LLVMModule);
}
