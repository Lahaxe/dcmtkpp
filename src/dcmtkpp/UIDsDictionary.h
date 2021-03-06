/*************************************************************************
 * dcmtkpp - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _7b126e41_bd4d_443f_8873_d2fec52b1019
#define _7b126e41_bd4d_443f_8873_d2fec52b1019

#include <map>
#include <string>

namespace dcmtkpp
{

/**
 * @brief Entry in a dictionary of DICOM UIDs.
 */
struct UIDsDictionaryEntry
{
    std::string name;
    std::string keyword;
    std::string type;

    UIDsDictionaryEntry(
        std::string const & name, std::string const & keyword,
        std::string const & type);
};

typedef std::map<std::string, UIDsDictionaryEntry> UIDsDictionary;

}

#endif // _7b126e41_bd4d_443f_8873_d2fec52b1019
