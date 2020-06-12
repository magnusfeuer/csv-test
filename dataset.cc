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
                        const std::string& emitter_config)
{
    std::size_t index = 0;

    if (!emitter.begin(emitter_config, specification(), record_count())) {
        std::cout << "Dataset::emit(): Could not initialize emitter." << std::endl;
        return false;
    }

    for(const auto& rec: records_)
        if (!emitter.emit_record(*rec, index)) {
            std::cout << "Dataset::emit(): Could emit record " << index << "/" << record_count() << std::endl;
            emitter.end();
            return false;
        }

    emitter.end();

    std::cout << "Dataset::emit(): Emitted " << index << "/" <<
        record_count() << " records" << std::endl;

    return true;
}

csv::Record::Record(const Specification& csv_spec, const std::vector<std::string>& tokens)
{
    auto dt_iter(csv_spec.data_types().begin());

    // We will assume that csv_spec.data_types().size() == tokens.size()
    // We will assume that the token length is non-zero.
    for(const auto& t: tokens) {
        switch(*dt_iter) {

        case csv::FieldType::INT64: {
            char* endptr = 0;
            int64_t val =  strtoll(t.c_str(), &endptr, 0);
            if (*endptr) {
                std::cout << "Token " << t << " is not an integer." << std::endl;
                exit(255);
            }
            values_.push_back(val);
            break;
        }


        case csv::FieldType::DOUBLE: {
            char* endptr = 0;
            double val =  strtoll(t.c_str(), &endptr, 0);
            if (*endptr) {
                std::cout << "Token " << t << " is not an integer." << std::endl;
                exit(255);
            }

            values_.push_back(val);
            break;
        }

        case csv::FieldType::STRING: {
            values_.push_back(t);
            break;
        }

        default:
            std::cout << "Unknown data type: " << int(*dt_iter)  << std::endl;
            exit(255);
        }
        dt_iter++;
    }
}

