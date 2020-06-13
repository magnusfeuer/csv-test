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

//! \class IngestionIface
//! Ingestion interface class
//
///
//
#ifndef __INGESTION_CSV__
#define __INGESTION_CSV__

#include "ingestion_iface.hh"

namespace csv {
    /// Class to ingest CSV data
    //
    /// Intances of this class can read and parse CSV data from an input stream.
    ///
    class IngestionCSV:
        public IngestionIface {
    public:
        /// Default constructor.
        IngestionCSV(void) = default;

        /// Default destructor.
        ~IngestionCSV(void) = default;

        /// Read and parse a single CSV line.
        //
        /// A single line is retrieved from \a input and parsed according
        /// to the data in \a specification.
        ///
        /// The parsed fields of the CSV line will have their data types
        /// determined by the corresponding element in the csv::Field vector
        /// returned by specification.fields().
        ///
        /// The CSV line is assumed to have the same number of fields as
        /// the number of elements returned by specification.fields().
        ///
        /// The line is assumed to have fields separated by the
        /// character returned by specification.separator_char().
        ///
        /// Field data can be escaped by the character returned by
        /// specification.escape_char(). If an escape character is
        /// encoutered, the next character will be added to the field
        /// data even if it is a separator character.
        ///
        /// White-spaces are removed before and after numerical values
        /// (csv::FieldType::INT64 and csv::FieldType::DOUBLE) prior
        /// to parsing the value. Strings retain their white spaces.
        ///
        /// String quotes are not supported.
        ///
        /// @param input The input stream to read and parse a CSV line from
        /// @param specification The specification to use when parsing the CSV data.
        /// @param record_index The  index of the current record (starting at 0). Not used.
        ///
        /// @return A shared pointer to a newly created csv::Record with the parsed CSV data.
        /// @return NULL input has reached an end.
        ///
        /// \bug There is no way for this function to report a failed CSV line parsing.
        std::shared_ptr<csv::Record> ingest_record(std::istream& input,
                                                   const csv::Specification& specification,
                                                   const std::size_t record_index) override;
    };
};
#endif
