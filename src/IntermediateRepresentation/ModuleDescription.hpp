#ifndef IR_MODULE_DESCRIPTION
#define IR_MODULE_DESCRIPTION

#include <map>
#include <string>
#include <vector>

#include "Define.hpp"
#include "FunctionDescription.hpp"
#include "StructDescription.hpp"
#include "TargetDescription.hpp"

namespace icode
{
    struct ModuleDescription
    {
        std::string name;
        std::vector<std::string> uses;
        std::map<std::string, StructDescription> structures;
        std::map<std::string, FunctionDescription> functions;
        std::map<std::string, int> enumerations;
        std::map<std::string, DefineDescription> defines;
        std::map<std::string, DataType> dataTypeNames;
        std::map<std::string, TypeDescription> globals;
        std::map<std::string, std::string> stringsData;

        void initializeTargetInfo(const TargetDescription& target);
        DataType dataTypeFromString(const std::string& dtype_name);

        bool useExists(const std::string&);
        bool getStruct(const std::string&, StructDescription&);
        bool getFunction(const std::string&, FunctionDescription&);
        bool getEnum(const std::string&, int&);
        bool getDefineDescription(const std::string&, DefineDescription&);
        bool getGlobal(const std::string&, TypeDescription&);
        bool symbolExists(const std::string&);
    };

    typedef std::map<std::string, ModuleDescription> StringModulesMap;
}

#endif