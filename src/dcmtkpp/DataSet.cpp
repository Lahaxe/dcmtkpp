/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "dcmtkpp/DataSet.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "dcmtkpp/Exception.h"
#include "dcmtkpp/Tag.h"
#include "dcmtkpp/VR.h"

namespace dcmtkpp
{

DataSet
::DataSet()
{
    // Nothing to do.
}

void
DataSet
::add(Tag const & tag, Element const & element)
{
    this->_elements[tag] = element;
}


void
DataSet
::add(Tag const & tag, VR vr)
{
    Value value;

    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }

    if(::dcmtkpp::is_int(vr))
    {
        value = Value::Integers();
    }
    else if(::dcmtkpp::is_real(vr))
    {
        value = Value::Reals();
    }
    else if(::dcmtkpp::is_string(vr))
    {
        value = Value::Strings();
    }
    else if(::dcmtkpp::is_binary(vr))
    {
        value = Value::Binary();
    }
    else if(vr == VR::SQ)
    {
        value = Value::DataSets();
    }
    else
    {
        throw Exception("Unknown VR: "+::dcmtkpp::as_string(vr));
    }

    this->add(tag, Element(value, vr));
}

void
DataSet
::add(Tag const & tag, Value::Integers const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(Tag const & tag, Value::Reals const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(Tag const & tag, Value::Strings const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(Tag const & tag, Value::DataSets const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(Tag const & tag, Value::Binary const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(Tag const & tag, std::initializer_list<int> const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(
    Tag const & tag, std::initializer_list<Value::Integer> const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(
    Tag const & tag, std::initializer_list<Value::Real> const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(
    Tag const & tag, std::initializer_list<Value::String> const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::add(
    Tag const & tag, std::initializer_list<DataSet> const & value, VR vr)
{
    if(vr == VR::UNKNOWN)
    {
        vr = as_vr(tag);
    }
    this->add(tag, Element(value, vr));
}

void
DataSet
::remove(Tag const & tag)
{
    if(!this->has(tag))
    {
        throw Exception("No such element");
    }

    this->_elements.erase(tag);
}

bool
DataSet
::empty() const
{
    return this->_elements.empty();
}

std::size_t
DataSet
::size() const
{
    return this->_elements.size();
}

Element const &
DataSet
::operator[](Tag const & tag) const
{
    ElementMap::const_iterator const it = this->_elements.find(tag);
    if(it == this->_elements.end())
    {
        throw Exception("No such element");
    }

    return it->second;
}

Element &
DataSet
::operator[](Tag const & tag)
{
    ElementMap::iterator it = this->_elements.find(tag);
    if(it == this->_elements.end())
    {
        throw Exception("No such element");
    }

    return it->second;
}

bool
DataSet
::has(Tag const & tag) const
{
    return (this->_elements.find(tag) != this->_elements.end());
}

VR
DataSet
::get_vr(Tag const & tag) const
{
    ElementMap::const_iterator const it = this->_elements.find(tag);
    if(it == this->_elements.end())
    {
        throw Exception("No such element");
    }

    return it->second.vr;
}

bool
DataSet
::empty(Tag const & tag) const
{
    ElementMap::const_iterator const it = this->_elements.find(tag);
    if(it == this->_elements.end())
    {
        throw Exception("No such element");
    }

    return it->second.empty();
}

std::size_t
DataSet
::size(Tag const & tag) const
{
    ElementMap::const_iterator const it = this->_elements.find(tag);
    if(it == this->_elements.end())
    {
        throw Exception("No such element");
    }

    return it->second.size();
}

bool
DataSet
::operator==(DataSet const & other) const
{
    return (this->_elements == other._elements);
}

bool
DataSet
::operator!=(DataSet const & other) const
{
    return !(*this == other);
}

}
