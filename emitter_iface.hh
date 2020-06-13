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
#include "specification.hh"
#include "record.hh"
#include <ostream>
namespace csv {
    class EmitterIface {

    public:
        EmitterIface(void) = default;

        virtual ~EmitterIface(void) {}

        /// Called before records are emitted.
        /// Allows implementation to write headers, etc.
        //
        // @param specification - CSV specification.
        // @param record_count - the number of times Emit() will be called.
        //
        virtual bool begin(std::ostream& output,
                           const std::string& config,
                           const csv::Specification& specification) { return false; };

        /// Called once for every record.
        //
        // @param output - The output stream to emit the record to
        // @param specifcation - The specification of the record to emit
        // @param record - The record to emit.
        //
        virtual bool emit_record(std::ostream& output,
                                 const csv::Specification& specification,
                                 const class Record& record) { return false; }

        /// Called after last record is emmitted.
        //
        // Allows for emitter to close files and clean up
        virtual bool end(std::ostream& output,
                         const csv::Specification& specification) { return false; };
    private:
    };
};
#endif
