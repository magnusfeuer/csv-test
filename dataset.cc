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

#include "dataset.hh"
#include <iostream>
#include <stdlib.h>

void csv::Dataset::append_record(std::unique_ptr<const csv::Record> record)
{
    records_.push_back(std::move(record));
    ++record_count_;
}

bool csv::Dataset::emit(csv::EmitterIface& emitter,
                        std::ostream& output,
                        const std::string& emitter_config)
{
    std::size_t index = 0;

    if (!emitter.begin(output, emitter_config, specification(), record_count())) {
        std::cout << "Dataset::emit(): Could not initialize emitter." << std::endl;
        return false;
    }

    for(const auto& rec: records_) {
        if (!emitter.emit_record(output, specification(), *rec, index)) {
            std::cout << "Dataset::emit(): Could emit record " << index << "/" << record_count() << std::endl;
            emitter.end(output, specification());
            return false;
        }
        index++;
    }

    emitter.end(output, specification());
    return true;
}


bool csv::Dataset::ingest(csv::IngestionIface& ingester,
                          std::istream& input,
                          const std::string& ingester_config)
{
    // Wipe old content
    records_.clear();
    record_count_ = 0;
    return ingester.ingest(ingester_config, input, *this);
}

