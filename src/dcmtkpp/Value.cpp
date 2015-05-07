/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "dcmtkpp/Value.h"

#include <cstdint>
#include <string>
#include <vector>

#include <boost/container/vector.hpp>

#include "dcmtkpp/DataSet.h"
#include "dcmtkpp/Exception.h"

namespace dcmtkpp
{

Value
::Value()
: _type(Type::Empty)
{
    // Nothing else.
}

Value
::Value(Integers const & integers)
: _type(Type::Integers), _integers(integers)
{
    // Nothing else.
}

Value
::Value(Reals const & reals)
: _type(Type::Reals), _reals(reals)
{
    // Nothing else.
}

Value
::Value(Strings const & strings)
: _type(Type::Strings), _strings(strings)
{
    // Nothing else.
}

Value
::Value(DataSets const & datasets)
: _type(Type::DataSets), _data_sets(datasets)
{
    // Nothing else.
}

Value::Type
Value
::get_type() const
{
    return this->_type;
}

#define DECLARE_CONST_ACCESSOR(type, name) \
Value::type const & \
Value \
::as_##name() const \
{ \
    if(this->get_type() != Type::type) \
    { \
        throw Exception("Type mismatch"); \
    } \
    return this->_##name; \
}

#define DECLARE_NON_CONST_ACCESSOR(type, name) \
Value::type & \
Value \
::as_##name() \
{ \
    if(this->get_type() != Type::type) \
    { \
        throw Exception("Type mismatch"); \
    } \
    return this->_##name; \
}

DECLARE_CONST_ACCESSOR(Integers, integers)
DECLARE_NON_CONST_ACCESSOR(Integers, integers)

DECLARE_CONST_ACCESSOR(Reals, reals)
DECLARE_NON_CONST_ACCESSOR(Reals, reals)

DECLARE_CONST_ACCESSOR(Strings, strings)
DECLARE_NON_CONST_ACCESSOR(Strings, strings)

DECLARE_CONST_ACCESSOR(DataSets, data_sets)
DECLARE_NON_CONST_ACCESSOR(DataSets, data_sets)

}