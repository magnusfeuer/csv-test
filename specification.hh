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
    /// An enum with all supported data types for a single field.
    //
    /// This is used by Specification::Field::type_ to specify the
    /// data type of a single field.
    ///
    enum class FieldType {
        INT64, DOUBLE, STRING
    };

    /// A specification for record formats.
    //
    /// Instances of this class hosts information about the
    /// records to be transformed from one format to another.
    ///
    /// Currently only a single instance of this class is created per
    /// program run since only a single type of records are read (from
    /// a CSV file).
    ///
    /// A reference to the specification is provided to the ingester and emitter,
    /// allowing them to parse and emit data with the correct format.
    ///
    class Specification {
    public:
        /// A single field specification in a record.
        //
        /// Contains the name and type of a single field in a record.
        ///
        struct Field {
            /// The name of the field
            std::string name_;

            /// The type of the field
            FieldType type_;
        };

        /// Constructor.
        //
        /// The constrcuctor accepts separator and escape characters that
        /// can be used by the ingester to correctly parse input output.
        ///
        /// The vector of \c < \c string, \c string \c > tuples specifies the field name and types
        /// for each processed record.
        ///
        /// Each tuple represents one field (in order of appearance),
        /// where the first string element is the name of the field,
        /// and the second is the type in string format.
        ///
        /// Supported data types for the second element are (case sensitive):
        /// \c "float", \c "double", \c "int32", \c "int64", \c "string"
        ///
        /// The constructor will transform the data type strings to their FieldType enum
        /// equivalent.
        ///
        /// @param fields The vector of tuples to use as field specifications.
        /// @param separator_char The character to use as a separator when ingesting data.
        /// @param escape_char The escape character to use when ingesting data.
        ///
        /// @return n/a
        Specification(const std::vector<std::tuple<std::string, std::string> >& fields,
                      const char separator_char,
                      const char escape_char);

        /// Return the separator character provided to constructor.
        const char separator_char(void) const { return separator_char_; }

        /// Return the escape character provided to constructor.
        const char escape_char(void) const { return escape_char_; }

        /// Return the number of elements that will be returned by a fields() call.
        const uint32_t field_count(void) const { return field_count_; }

        /// Return the field specification.
        //
        /// The vector of returned Field objects contains the name and
        /// data type of each field in a record. The order of the vector elements
        /// matches the order of fields in a processed record.
        ///
        /// @return - A vector of field specifications.
        const std::vector<Field>& fields(void) const { return fields_; }

    private:
        /// Default constructor - Private to force initialized construction.
        Specification(void) = default;

    private:

        /// Map between string data type and their enum equivalent.
        static std::map<std::string, FieldType> enum_string_map_;

        /// CSV field names and types, in order of appearance
        std::vector<Field> fields_;

        /// Escape character.
        char separator_char_;

        /// Escape character.
        char escape_char_;

        /// Number of fields.
        uint32_t field_count_;
   };
};
#endif
