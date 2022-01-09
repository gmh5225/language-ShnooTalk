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

    void TypeDescription::becomeArrayPointer()
    {
        setProperty(IS_PTR);
        dimensions.clear();
        dimensions.push_back(1);
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

    bool TypeDescription::isMutableAndPointer() const
    {
        return isPointer() && isMutable();
    }

    bool TypeDescription::isMutableOrPointer() const
    {
        return isPointer() || isMutable();
    }

    bool TypeDescription::isStruct() const
    {
        return dtype == STRUCT;
    }

    bool TypeDescription::isArray() const
    {
        return dimensions.size() > 0;
    }

    bool TypeDescription::isStructOrArray() const
    {
        return isStruct() || isArray();
    }

    bool TypeDescription::isStructOrArrayAndNotPointer() const
    {
        return isStructOrArray() && !isPointer();
    }

    bool TypeDescription::isMultiDimArray() const
    {
        return dimensions.size() > 1;
    }

    bool TypeDescription::isIntegerType() const
    {
        return isInteger(dtype);
    }

    bool TypeDescription::isFloatType() const
    {
        return isFloat(dtype);
    }

    bool TypeDescription::isStringLtrl() const
    {
        return checkProperty(IS_STRING_LTRL);
    }
}