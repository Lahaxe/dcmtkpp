/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _aa2965aa_e891_4713_9c90_e8eacd2944ea
#define _aa2965aa_e891_4713_9c90_e8eacd2944ea

#include <functional>
#include <istream>
#include <string>
#include <utility>

#include "dcmtkpp/DataSet.h"
#include "dcmtkpp/Element.h"
#include "dcmtkpp/Tag.h"
#include "dcmtkpp/Value.h"
#include "dcmtkpp/VR.h"

namespace dcmtkpp
{

/// @brief Read DICOM objects from a stream.
class Reader
{
public:
    /// @brief Input stream.
    std::istream & stream;

    /// @brief Transfer syntax used to read the file.
    std::string transfer_syntax;

    /// @brief Endianness (LITTLE_ENDIAN or BIG_ENDIAN).
    int byte_ordering;

    /// @brief Explicit-ness of the Value Representations.
    bool explicit_vr;

    /// @brief Flag to keep or discard group length tags.
    bool keep_group_length;

    /**
     * @brief Build a reader, derive byte ordering and explicit-ness of VR
     * from transfer syntax.
     */
    Reader(
        std::istream & stream, std::string const & transfer_syntax,
        bool keep_group_length=false);

    /// @brief Read a data set.
    DataSet read_data_set(
        std::function<bool(Tag const &)> halt_condition = [](Tag const &) { return false;}) const;

    /// @brief Read a tag.
    Tag read_tag() const;

    /**
     * @brief Read an element (VR and value), try to guess the VR from the tag,
     * partially read data set, and transfer syntax for implicit VR transfer
     * syntaxes.
     */
    Element read_element(
        Tag const & tag=Tag(0xffff,0xffff),
        DataSet const & data_set=DataSet()) const;

    static std::pair<DataSet, DataSet> read_file(
        std::istream & stream,
        bool keep_group_length=false);

private:
    struct Visitor
    {
        typedef void result_type;

        std::istream & stream;
        VR vr;

        std::string transfer_syntax;
        int byte_ordering;
        bool explicit_vr;
        bool keep_group_length;

        Visitor(
            std::istream & stream, VR vr, std::string const & transfer_syntax,
            int byte_ordering, bool explicit_vr, bool keep_group_length);

        result_type operator()(Value::Integers & value) const;
        result_type operator()(Value::Reals & value) const;
        result_type operator()(Value::Strings & value) const;
        result_type operator()(Value::DataSets & value) const;
        result_type operator()(Value::Binary & value) const;

        uint32_t read_length() const;

        Value::Strings split_strings(std::string const & string) const;
        DataSet read_item(std::istream & specific_stream) const;
    };
};

}

#endif // _aa2965aa_e891_4713_9c90_e8eacd2944ea
