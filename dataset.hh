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

//! \class Record
//! A Single record.
//
/// This class hosts a single CSV record, as provided by a ReaderIface
/// implementation.
//  Once constructed, an instance is immutable.
//
#ifndef __DATASET_HH__
#define __DATASET_HH__
#include "specification.hh"
#include <variant>
#include <memory>
#include <list>
#include "emitter_iface.hh"
#include "ingestion_iface.hh"
#include <istream>
#include <ostream>

namespace csv {


    class Dataset {
    public:
        Dataset(const Specification& specification):
            specification_(specification),
            record_count_(0) {}

        const Specification& specification(void) { return specification_; }

        void append_record(std::unique_ptr<const csv::Record> record);

        std::size_t record_count(void) const { return record_count_; }

        bool emit(csv::EmitterIface& emitter,
                  std::ostream& output,
                  const std::string& emitter_config);

        bool ingest(csv::IngestionIface& ingester,
                    std::istream& input,
                    const std::string& ingester_config);

    private:
        const Specification specification_;
        std::size_t record_count_;
        std::list<std::unique_ptr<const csv::Record> > records_;
    };
};
#endif
