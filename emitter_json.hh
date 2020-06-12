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
    class EmitterJSON: public EmitterIface {
    public:
        EmitterJSON(void);

        /// Called before records are emitted.
        /// Allows implementation to write headers, etc.
        //
        // @param record_count - the number of times Emit() will be called.
        //
        bool begin(std::ostream& output,
                   const std::string& config,
                   const csv::Specification& specification,
                   const std::size_t record_count) override;

        /// Called once for every record.
        //
        // @param record - The record to emit.
        //
        bool emit_record(std::ostream& output,
                         const csv::Specification& specification,
                         const class Record& record) override;

        /// Called after last record is emmitted.
        //
        // Allows for emitter to close files and clean up
        bool end(std::ostream& output,
                 const csv::Specification& specification) override;
    private:
        std::string outfile_;
        std::ofstream outstream_;
    };


};

#endif
