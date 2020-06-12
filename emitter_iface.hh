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
#ifndef __EMITTER_IFACE_HH__
#define __EMITTER_IFACE_HH__
#include <cstddef>
#include <string>
#include "csv_spec.hh"

namespace csv {
    class EmitterIface {

    public:
        virtual ~EmitterIface(void) {}
        /// Called before records are emitted.
        /// Allows implementation to write headers, etc.
        //
        // @param csv_spec - CSV specification.
        // @param record_count - the number of times Emit() will be called.
        //
        virtual bool begin(const std::string& config,
                           const csv::Specification& csv_spec,
                           std::size_t record_count) { return false; };

        /// Called once for every record.
        //
        // @param record - The record to emit.
        // @param record_index - The index of the current record.
        //
        virtual bool emit_record(const class Record& record, std::size_t record_index) { return false; }

        /// Called after last record is emmitted.
        //
        // Allows for emitter to close files and clean up
        virtual bool end(void) { return false; };
    };
};
#endif
