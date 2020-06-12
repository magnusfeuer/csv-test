// (C) 2020 - Magnus Feuer
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include "record.hh"
#include <iostream>
#include <stdlib.h>
//
// Helper function. Not visible to the outside.
//
static std::string strip_whitespaces(const std::string& str)
{
    // Strip out leading and trailing white spaces
    std::size_t first_char = str.find_first_not_of (" \t", 0);
    std::size_t last_char = str.find_last_not_of(" \t", std::string::npos);

    // Calculate number of bytes to copy. Bear in mind that first_char and
    // last_char may be std::string::npos, which needs conversion

    first_char = (first_char == std::string::npos)?0:first_char;
    last_char = (last_char == std::string::npos)?str.length():(last_char + 1);
    std::size_t copy_len =  last_char - first_char;
    std::string res { str.substr(first_char, copy_len) };

//    std::cout << "Before [" << str << "] After [" << res << "]" << std::endl;
    return res;
}

csv::Record::Record(const Specification& specification,
                    const std::size_t index,
                    const std::vector<std::string>& tokens):
    index_(index)
{
    auto field_iter(specification.fields().begin());

    // We will assume that specification.data_types().size() == tokens.size()
    // We will assume that the token length is non-zero.
    for(const auto& t: tokens) {
        switch(field_iter->type_) {
        case csv::FieldType::INT64: {
            std::string data { strip_whitespaces(t) };
            char* endptr = 0;
            int64_t val =  strtoll(data.c_str(), &endptr, 0);
            if (*endptr) {
                std::cout << "Token for field " << field_iter->name_ << ": " << data << " is not an integer." << std::endl;
                exit(255);
            }
            fields_.push_back(val);
            break;
        }


        case csv::FieldType::DOUBLE: {
            char* endptr = 0;
            std::string data { strip_whitespaces(t) };

            double val =  strtod(data.c_str(), &endptr);
            if (*endptr) {
                std::cout << "Token for field " << field_iter->name_ << ": " << data << " is not a double." << std::endl;
                exit(255);
            }

            fields_.push_back(val);
            break;
        }

        case csv::FieldType::STRING: 
            fields_.push_back(t);
            break;


        default:
            std::cout << "Unknown data type: " << int(field_iter->type_)  << std::endl;
            exit(255);
        }
        field_iter++;
    }
}

