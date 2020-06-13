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


    /// An interface class to read records from an input stream.
    //
    /// This class can be subclassed to CSV and other
    /// input formats that are to be transformed to an output format
    /// through an csv::EmitterIface subclass.
    ///
    /// Subclasses are managed through the csv::Factory<IngestionIFace> template
    /// instantiation, and should never be instantiated directly.
    ///
    class IngestionIface {
    public:
        /// Read and parse a single record.
        //
        /// The implementation of this method shall:
        ///   - Read a single record from \a input
        ///   - Parse the read record according to \a specification
        ///   - Return a newly created csv:Record object with the parsed data.
        //
        /// @return Shared pointer to a newly created csv::Record if record was parsed.
        /// @return NULL if \a input has reached its end.
        //
        virtual std::shared_ptr<csv::Record> ingest_record(std::istream& input,
                                                           const csv::Specification& specification,
                                                           const std::size_t record_index) = 0;

    };
};
#endif
