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
#ifndef __INGESTION_IFACE__
#define __INGESTION_IFACE__
#include <istream>
#include <memory>
namespace csv {

    class Specification;
    class Record;

    class IngestionIface {
    public:
        /// Read, parse, and emit a record.
        //
        /// The implementation of this method shall:
        /// 1. Read a single record from the 'input' stream.
        /// 2. Parse the record according to 'specification'.
        /// 3. Return  a shared pointer to a csv::Record object with the parsed data.
        //
        // @return pointer if record was transformed.
        // @return NULL if end of file.
        // @return NULL if record could not be parsed.
        //
        virtual std::shared_ptr<csv::Record> ingest_record(std::istream& input,
                                                           const csv::Specification& specification,
                                                           const std::size_t record_index) = 0;

    };
};
#endif
