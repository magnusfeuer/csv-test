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

/// \class CSVSpecification
/// Contains a type specification for a given CSV file.
//
/// Host a CSV file specification with field name and type.
/// Used when decoding CSV files and encoding the given output format.
/// Once constructed, this object is immutable.
///
#ifndef __CSV_SPEC_HH__
#define __CSV_SPEC_HH__
#include <tuple>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace csv {
    enum class FieldType {
        INT64, DOUBLE, STRING
    };

    class Specification {
    public:

        /// Constructor with a vector of tuples.
        //
        /// Each tuple represents one field (in order of appearance),
        /// where the first string element is the name of the field,
        /// and the second is the type in string format.
        ///
        /// Supported data types are (case insensitive):
        /// "float", "double", "int32", "int64", "string"
        ///
        /// NOTE: UTF-8 is not supported.
        Specification(const std::vector<std::tuple<std::string, std::string> >& spec,
                      const uint8_t separator_char,
                      const uint8_t escape_char);

        const uint8_t separator_char(void) const { return separator_char_; }

        const uint8_t escape_char(void) const { return escape_char_; }

        const uint32_t field_count(void) const { return field_count_; }

        const std::vector<std::string>& field_names(void) const { return field_names_; }

        const std::vector<FieldType>& data_types(void) const { return data_types_; }

    private:
        // Map between string data type and their enum equivalent.
        static std::map<std::string, FieldType> enum_string_map_;

        /// CSV field names, in order of appearance
        std::vector<std::string> field_names_;

        /// CSV field data types, in order of appearance.
        std::vector<FieldType> data_types_;

        const uint8_t separator_char_;
        const uint8_t escape_char_;
        const uint32_t field_count_;
   };
};
#endif
