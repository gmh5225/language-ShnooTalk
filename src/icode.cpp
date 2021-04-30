#include "icode.hpp"

namespace icode
{
    std::string data_type_strs[] = { "I8",
                                     "UI8",
                                     "I16",
                                     "UI16",
                                     "I32",
                                     "UI32"
                                     "I64",
                                     "UI64",
                                     "F32",
                                     "F64"
                                     "VM_INT",
                                     "VM_UINT",
                                     "VM_FLOAT",
                                     "INT",
                                     "FLOAT",
                                     "STRUCT",
                                     "VOID" };

    entry::entry()
    {
        op1.optype = NONE;
        op2.optype = NONE;
        op3.optype = NONE;
    }

    bool operand::operator<(const operand& other) const
    {
        if (*this == other)
            return false;

        return temp_id < other.temp_id;
    }

    bool operand::operator==(const operand& other) const
    {
        if (optype != other.optype)
            return false;

        switch (optype)
        {
            case TEMP:
            case TEMP_PTR:
            case RET_PTR:
                return temp_id == other.temp_id;
            case VAR:
            case GBL_VAR:
            case PTR:
            case STR_DATA:
            case LABEL:
            case MODULE:
                return name == other.name;
            case ADDR:
            case LITERAL:
                return val.size == other.val.size;
            case NONE:
                return false;
        }

        return false;
    }

    bool operand::operator!=(const operand& other) const { return !(*this == other); }

    void operand::update_dtype(const var_info& var)
    {
        dtype = var.dtype;
        dtype_name = var.dtype_name;
    }

    /*
        Helper functions for type checking and other data type operations.
    */

    bool is_sint(data_type dtype)
    {
        return (dtype == I8 || dtype == I16 || dtype == I32 || dtype == I64 || dtype == VM_INT || dtype == INT);
    }

    bool is_uint(data_type dtype)
    {
        return (dtype == UI8 || dtype == UI16 || dtype == UI32 || dtype == UI64 || dtype == VM_UINT);
    }

    bool is_int(data_type dtype) { return is_sint(dtype) || is_uint(dtype); }

    bool is_float(data_type dtype) { return (dtype == F32 || dtype == F64 || dtype == FLOAT || dtype == VM_FLOAT); }

    bool dtype_eq(data_type dtype1, data_type dtype2)
    {
        return dtype1 == dtype2 || (dtype1 == INT && is_int(dtype2)) || (is_int(dtype1) && dtype2 == INT) ||
               (dtype1 == FLOAT && is_float(dtype2)) || (is_float(dtype1) && dtype2 == FLOAT);
    }

    bool type_eq(var_info var1, var_info var2)
    {
        if (var1.dtype == STRUCT || var2.dtype == STRUCT)
            return (var1.dtype_name == var2.dtype_name && var1.dimensions == var2.dimensions &&
                    var1.module_name == var2.module_name);

        return (dtype_eq(var1.dtype, var2.dtype) && var1.dimensions == var2.dimensions);
    }

    var_info var_from_dtype(data_type dtype, target_desc& target)
    {
        var_info var;

        var.dtype = dtype;

        if (dtype == INT)
            var.dtype_name = "int";
        else if (dtype == FLOAT)
            var.dtype_name = "float";
        else if (dtype == VOID)
            var.dtype_name = "void";
        else
        {
            for (auto pair : target.dtype_strings_map)
                if (pair.second == dtype)
                    var.dtype_name = pair.first;
        }

        var.dtype_size = dtype_size[dtype];
        var.size = var.dtype_size;
        var.offset = 0;
        var.scope_id = 0;

        return var;
    }

    data_type from_dtype_str(const std::string& dtype_name, target_desc& target)
    {
        if (target.dtype_strings_map.find(dtype_name) != target.dtype_strings_map.end())
            return target.dtype_strings_map[dtype_name];

        return STRUCT;
    }

    /*
        Helper functions to get and set variable properties
    */

    var_info::var_info() { properties = 0; }

    void var_info::set_prop(var_prop prop) { properties |= (1 << prop); }

    void var_info::clear_prop(var_prop prop) { properties &= ~(1 << prop); }

    bool var_info::check(var_prop prop) const { return properties & (1 << prop); }

    /*
        Helper functions to check for existence of symbols
        and fetch them
    */

    template<class K, class V>
    bool get_elem(std::map<K, V>& map, const K& key, V& val)
    {
        auto pair = map.find(key);

        if (pair != map.end())
        {
            val = pair->second;
            return true;
        }

        return false;
    }

    bool struct_desc::field_exists(const std::string& name) { return fields.find(name) != fields.end(); }

    bool func_desc::symbol_exists(const std::string& name) { return symbols.find(name) != symbols.end(); }

    bool func_desc::get_symbol(const std::string& name, var_info& val)
    {
        return get_elem<std::string, var_info>(symbols, name, val);
    }

    bool module_desc::use_exists(const std::string& name)
    {
        return std::find(uses.begin(), uses.end(), name) != uses.end();
    }

    bool module_desc::get_struct(const std::string& name, struct_desc& val)
    {
        return get_elem<std::string, struct_desc>(structures, name, val);
    }

    bool module_desc::get_func(const std::string& name, func_desc& val)
    {
        return get_elem<std::string, func_desc>(functions, name, val);
    }

    bool module_desc::get_enum(const std::string& name, int& val)
    {
        return get_elem<std::string, int>(enumerations, name, val);
    }

    bool module_desc::get_def(const std::string& name, def& val)
    {
        return get_elem<std::string, def>(defines, name, val);
    }

    bool module_desc::get_global(const std::string& name, var_info& val)
    {
        return get_elem<std::string, var_info>(globals, name, val);
    }

    bool module_desc::symbol_exists(const std::string& name, target_desc& target)
    {
        return structures.find(name) != structures.end() || functions.find(name) != functions.end() ||
               use_exists(name) || from_dtype_str(name, target) != STRUCT ||
               enumerations.find(name) != enumerations.end() || globals.find(name) != globals.end() ||
               defines.find(name) != defines.end() || target.defines.find(name) != target.defines.end();
    }

    bool target_desc::get_def(const std::string& name, def& val)
    {
        return get_elem<std::string, def>(defines, name, val);
    }

    /*
        Helper functions for optimizer
    */
    bool is_ltrl(operand_type optype) { return optype == icode::LITERAL || optype == icode::ADDR; }

    bool is_ptr(operand_type optype)
    {
        return optype == icode::PTR || optype == icode::TEMP_PTR || optype == icode::RET_PTR;
    }

    /*
        Helper functions for generating icode operands
    */

    operand temp_opr(data_type dtype, const std::string& dtype_name, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.dtype = dtype;
        temp.dtype_name = dtype_name;
        temp.optype = TEMP;

        return temp;
    }

    operand temp_ptr_opr(data_type dtype, const std::string& dtype_name, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.dtype = dtype;
        temp.dtype_name = dtype_name;
        temp.optype = TEMP_PTR;

        return temp;
    }

    operand str_dat_opr(const std::string& name, unsigned int size, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.name = name;
        temp.dtype = icode::UI8;
        temp.optype = STR_DATA;
        temp.val.size = size;

        return temp;
    }

    operand addr_opr(unsigned int address, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.val.address = address;
        temp.optype = ADDR;

        return temp;
    }

    operand var_opr(data_type dtype,
                    const std::string& dtype_name,
                    const std::string& symbol,
                    unsigned int id,
                    bool global,
                    bool ptr)
    {
        operand temp;
        temp.temp_id = id;
        temp.name = symbol;
        temp.dtype = dtype;
        temp.dtype_name = dtype_name;

        if (global)
            temp.optype = GBL_VAR;
        else if (ptr)
            temp.optype = PTR;
        else
            temp.optype = VAR;

        return temp;
    }

    operand ret_ptr_opr(data_type dtype, const std::string& dtype_name, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.dtype = dtype;
        temp.dtype_name = dtype_name;
        temp.optype = RET_PTR;

        return temp;
    }

    operand ret_val_opr(data_type dtype, const std::string& dtype_name, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.dtype = dtype;
        temp.dtype_name = dtype_name;
        temp.optype = RET_VAL;

        return temp;
    }

    operand literal_opr(data_type dtype, float literal, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.val.floating = literal;
        temp.dtype = dtype;
        temp.optype = LITERAL;

        return temp;
    }

    operand literal_opr(data_type dtype, int literal, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.val.integer = literal;
        temp.dtype = dtype;
        temp.optype = LITERAL;

        return temp;
    }

    operand label_opr(const std::string& label, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.name = label;
        temp.optype = LABEL;

        return temp;
    }

    operand module_opr(const std::string& module, unsigned int id)
    {
        operand temp;
        temp.temp_id = id;
        temp.name = module;
        temp.optype = MODULE;

        return temp;
    }
} // namespace icode
