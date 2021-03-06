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

#include "emitter_csv.hh"
#include <iostream>
#include "factory.hh"
#include "factory_impl.hh"


//
// We need to ensure that we don't call csv::Factory<>::register_producer()
// before the static csv::Factory<>::producers_ has been initialized.
// Since static variables are initialized before program execution,
// we can safely do set tmp_ using a register_producer() call
// all global variables are initialized after static variables.
//
// We use a lambda function here as a creator function, mostly
// to show that it can be done. There are other, possibly more
// elegant ways of doing this.
//
bool emitter_csv_registration_ =
    csv::Factory<csv::EmitterIface>::
    register_producer("csv",
                      [](void) -> std::shared_ptr<csv::EmitterIface> {
                          return std::make_shared<csv::EmitterCSV>();
                      });


bool csv::EmitterCSV::begin(std::ostream& output,
                           const std::string& config,
                           const csv::Specification& specification)
{
    return true;
}

bool csv::EmitterCSV::emit_record(std::ostream& output,
                                 const csv::Specification& specification,
                                 const class Record& record)
{
    std::string line {""};
    bool first_field { true };
    auto field_type_iter{ specification.fields().begin() };

    for(const auto& field: record.fields()) {
        // Do we need to add a separator
        if (!first_field) 
            line += specification.separator_char();

        first_field = false;

        // Convert the given data type of the record's field
        // to a string field.
        switch(field_type_iter->type_) {
        case csv::FieldType::INT64:
            line.append(std::to_string(std::get<int64_t>(field)));
            break;

        case csv::FieldType::DOUBLE:
            line.append(std::to_string(std::get<double>(field)));
            break;

        case csv::FieldType::STRING:
            line.append(std::get<std::string>(field));
            break;

        default:
            std::cout << "Unknown data type: " << int(field_type_iter->type_)  << std::endl;
            exit(255);
        }
        ++field_type_iter;
    }

    // Add to output stream
    output << line << std::endl;
    return true;
}

bool csv::EmitterCSV::end(std::ostream& output,
                           const csv::Specification& specification)
{
    return true;
}


