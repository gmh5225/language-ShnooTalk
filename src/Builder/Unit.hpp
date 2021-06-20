#ifndef BUILDER_UNIT
#define BUILDER_UNIT

#include <utility>

#include "../IntermediateRepresentation/Operand.hpp"
#include "../IntermediateRepresentation/TypeDescription.hpp"

typedef std::pair<icode::Operand, icode::TypeDescription> Unit;

#endif