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

//! \class EmitterIface
//! Emitter interface class
//
/// Called by DataSet emit records.
//
#ifndef __JSON_EMITTER_HH__
#define __JSON_EMITTER_HH__
#include "emitter_iface.hh"
#include <string>
#include <memory>
#include <fstream>
#include "factory.hh"

namespace csv {
    /// A JSON Emitter class.
    //
    /// This class emits records as an array of JSON objects, as described
    /// in the csv::EmitterIface interface class.
    ///
    class EmitterJSON: public EmitterIface {
    public:
        /// Default constructor.
        EmitterJSON(void) = default;

        /// Default destructor.
        ~EmitterJSON(void) = default;

        /// Write out the JSON header.
        //
        /// This call will write out a single \c "[" to \a output
        /// in order to start a new vector of JSON objects.n
        ///
        /// @param output The output file stream to emit data to
        /// @param config Additional configuration data. Not used.
        /// @param specification  Record specification.
        ///
        /// @return true - Header data was successfully emitted.
        /// @return false - Header data could not be emitted.
        ///
        bool begin(std::ostream& output,
                   const std::string& config,
                   const csv::Specification& specification) override;

        /// Emit a single JSON record to an output stream.
        //
        /// Will write a single JSON record in the following format:
        /// \code
        /// {
        ///     "<field-name-1>": <value>,
        ///     "<field-name-2>": <value>,
        ///       ...
        ///     "<field-name-N>": <value>
        /// }
        /// \endcode
        ///
        /// The field names are retrieved from the provided specification.
        ///
        /// The field name/value pairs are written in the same order that they appear in the
        /// vector returned by record.fields() .
        ///
        /// String values will be quoted.
        ///
        /// Leading commas will be added before all records except the first one.
        ///
        /// @param output The output file stream to emit the record to to.
        /// @param specification  Record specification to retrieve name and type from.
        /// @param record The record to emit.
        //
        /// @return true - Record was successfully emitted.
        /// @return false - Record data could not be emitted.
        ///
        bool emit_record(std::ostream& output,
                         const csv::Specification& specification,
                         const class Record& record) override;

        /// Emit the JSON footer.
        //
        /// This call emits a single \c "]" to \a output to close out
        /// the JSON object array opened up by begin().
        ///
        /// @return true - Footer was successfully emitted.
        /// @return false - Footer could not be emitted.
        ///
        bool end(std::ostream& output,
                 const csv::Specification& specification) override;
    private:
    };
};

#endif
