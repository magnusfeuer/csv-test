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

#include "csv_spec.hh"
#include <iostream>

csv::Specification::Specification(const std::vector<std::tuple<std::string, std::string> >& spec,
                                  const uint8_t separator_char,
                                  const uint8_t escape_char):
    separator_char_(separator_char),
    escape_char_(escape_char),
    field_count_(0)
{
    for(auto t: spec) {
        // Did we have a correct type?
        if (enum_string_map.find(std::get<1>(t)) == enum_string_map.end()) {
            std::cout << "Incorrect field type: " << std::get<1>(t) << std::endl;
            exit(255);
        }

        // Add field name
        field_names.push_back(std::get<0>(t));

        // Add field type enum
        field_types.push_back(enum_string_map[std::get<0>(t)]);
    }
}
