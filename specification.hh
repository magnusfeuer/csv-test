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
        struct Field {
            std::string name_;
            FieldType type_;
        };

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
        Specification(const std::vector<std::tuple<std::string, std::string> >& fields,
                      const char separator_char,
                      const char escape_char);

        Specification(void) = default;
        const char separator_char(void) const { return separator_char_; }

        const char escape_char(void) const { return escape_char_; }

        const uint32_t field_count(void) const { return field_count_; }

        const std::vector<Field>& fields(void) const { return fields_; }

    private:
        // Map between string data type and their enum equivalent.
        static std::map<std::string, FieldType> enum_string_map_;

        /// CSV field names and types, in order of appearance
        std::vector<Field> fields_;

        char separator_char_;
        char escape_char_;
        uint32_t field_count_;
   };
};
#endif
