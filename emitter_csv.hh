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
#ifndef __CSV_EMITTER_HH__
#define __CSV_EMITTER_HH__
#include "emitter_iface.hh"
#include <string>
#include <memory>
#include <fstream>
#include "factory.hh"

namespace csv {
    /// A CSV Emitter class.
    //
    /// This class emits records CSV lines, as described
    /// in the csv::EmitterIface interface class.
    ///
    /// This class is used by the ./csv_convert_test code
    /// to ensure that we emit the same data that we ingest.
    ///
    class EmitterCSV: public EmitterIface {
    public:
        /// Default constructor.
        EmitterCSV(void) = default;

        /// Default destructor.
        ~EmitterCSV(void) = default;


        /// No-op
        //
        /// This call does nothing since no CSV headers are needed.
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

        /// Emit a single CSV record to an output stream.
        //
        /// Will write a single CSV record in the following format:
        /// \code
        ///    <value-1>,<value-2>,...<value-N>
        /// \endcode
        ///
        /// The field values are written in the same order that they appear in the
        /// vector returned by record.fields() .
        ///
        /// The field names are not written out.
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
        /// This call does nothing since no CSV footers are needed.
        //
        /// @param output Not used
        /// @param specification  Not used
        //
        /// @return true
        ///
        bool end(std::ostream& output,
                 const csv::Specification& specification) override;
    private:
        std::string outfile_;
        std::ofstream outstream_;
    };


};

#endif
