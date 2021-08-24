#include "TypeDescription.hpp"

namespace icode
{
    TypeDescription::TypeDescription()
    {
        properties = 0;
    }

    void TypeDescription::setProperty(TypeProperties property)
    {
        properties |= (1 << property);
    }

    void TypeDescription::clearProperty(TypeProperties property)
    {
        properties &= ~(1 << property);
    }

    bool TypeDescription::checkProperty(TypeProperties property) const
    {
        return properties & (1 << property);
    }

    void TypeDescription::becomeMutable()
    {
        setProperty(IS_MUT);
    }

    void TypeDescription::becomePointer()
    {
        setProperty(IS_PTR);
    }

    void TypeDescription::becomeString()
    {
        setProperty(IS_STRING_LTRL);
    }

    bool TypeDescription::isMutable() const
    {
        return checkProperty(IS_MUT);
    }

    bool TypeDescription::isPassedByReference() const
    {
        return checkProperty(IS_PARAM) && (isMutable() || (isStructOrArray() && !isPointer()));
    }

    bool TypeDescription::isPointer() const
    {
        return checkProperty(IS_PTR);
    }

    bool TypeDescription::isStruct() const
    {
        return dtype == STRUCT;
    }

    bool TypeDescription::isArray() const
    {
        return dimensions.size() > 0 && !isPointer();
    }

    bool TypeDescription::isStructOrArray() const
    {
        return isStruct() || isArray();
    }

    bool TypeDescription::isMultiDimArray() const
    {
        return dimensions.size() > 1;
    }

    bool TypeDescription::isIntegerType() const
    {
        return isInteger(dtype);
    }

    bool TypeDescription::isStringLtrl() const
    {
        return checkProperty(IS_STRING_LTRL);
    }
}