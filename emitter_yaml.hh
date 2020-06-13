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
#ifndef __YAML_EMITTER_HH__
#define __YAML_EMITTER_HH__
#include "emitter_iface.hh"
#include <string>
#include <memory>
#include <fstream>
#include "factory.hh"

namespace csv {
    /// A YAML Emitter class.
    //
    /// This class emits records as an array of YAML objects, as described
    /// in the csv::EmitterIface interface class.
    ///
    class EmitterYAML: public EmitterIface {
    public:
        /// Default constructor.
        EmitterYAML(void) = default;

        /// Default destructor.
        ~EmitterYAML(void) = default;

        /// No-op
        //
        /// This call does nothing since no YAML headers are needed.
        //
        /// @param output Not used
        /// @param config Not used
        /// @param specification  Not used
        //
        /// @return true
        ///
        bool begin(std::ostream& output,
                   const std::string& config,
                   const csv::Specification& specification) override;

        /// Emit a single YAML record to an output stream.
        //
        /// Will write a single YAML record in the following format:
        /// \code
        /// {
        ///     - <field-name-1>: <value>,
        ///       <field-name-2>: <value>,
        ///       ...
        ///       <field-name-N>: <value>
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

        /// No-op
        //
        /// This call does nothing since no YAML footers are needed.
        //
        /// @param output Not used
        /// @param specification  Not used
        //
        /// @return true
        ///
        bool end(std::ostream& output,
                 const csv::Specification& specification) override;
    private:
    };


};

#endif
