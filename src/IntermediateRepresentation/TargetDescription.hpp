#ifndef IR_TARGET_DESCRIPTION
#define IR_TARGET_DESCRIPTION

#include <map>

#include "DataType.hpp"
#include "Define.hpp"

namespace icode
{
    struct TargetEnums
    {
        std::map<std::string, DefineDescription> defines;
    };
}

#endif
