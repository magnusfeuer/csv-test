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

#ifndef __CSV_COMMON_HH__
#define __CSV_COMMON_HH__
#include <iostream>
#include <fstream>

namespace csv {
    /// Extract fields from a single line.
    //
    /// This function will divide \a line into a vector of strings placed in \a result.
    ///
    /// The line is assumed to have fields separated by the
    /// character specified by \a separator.
    ///
    /// Field data can be escaped by the character specified by \a
    /// escape. If an escape character is encoutered, the next
    /// character will be added to the field data even if it is a
    /// separator character.
    ///
    /// @param line The line to separate into fields.
    /// @param separator The separator character to use.
    /// @param escape The escape character to use. 
    /// @param result The string vector to add fields to. \
    ///               Fields will be added after any existing elements in the vector.
    ///
    /// @return The number of fields added to \a result.
    ///
    extern uint32_t tokenize_line(const std::string& line,
                                  uint8_t separator,
                                  uint8_t escape,
                                  std::vector<std::string>& result);

    class IngestionIface;
    class EmitterIface;
    class Specification;

    /// Convert all records from an input stream to a new format.
    //
    /// This function provides top-level functionality to
    /// convert records from one format to another.
    ///
    /// It will use \a ingester read and parse data from \a input, record by record, and
    /// then emit them to \a output using \a emitter.
    ///
    /// The format and data types of the records read is provided by \a specification.
    ///
    /// The ingester is created by a call to csv::Factory<csv::IngesterIface>::produce() .
    ///
    /// The emitter is created by a call to csv::Factory<csv::EmitterIface>::produce() .
    ///
    /// The call flow to the emitter is like follows.
    ///
    /// \code
    /// emitter.begin();
    /// emitter.emit_record();
    /// emitter.emit_record();
    /// emitter.emit_record();
    /// ...
    /// emitter.end();
    /// \endcode
    ///
    /// @param specification The specification of the records read from \a input.
    /// @param ingester The ingestion instance to use to read data from \a input.
    /// @param input The input data stream to read records from.
    /// @param emitter The emitter instance to use to write data to \a output
    /// @param output The output data stream to write converted records to.
    ///
    /// @return The number of records converted.
    ///
    extern uint32_t convert(const csv::Specification& specification,
                            csv::IngestionIface& ingester,
                            std::istream& input,
                            csv::EmitterIface& emitter,
                            std::ostream& output);
};
#endif
