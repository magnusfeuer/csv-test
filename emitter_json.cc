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

#include "emitter_json.hh"
#include <iostream>
#include "factory.hh"
#include "factory_impl.hh"
#include "emitter_factory_impl.hh"


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
bool emitter_json_registration_ =
    csv::Factory<csv::EmitterIface>::
    register_producer("json",
                      [](void) -> std::shared_ptr<csv::EmitterIface> {
                          return std::make_shared<csv::EmitterJSON>();
                      });

csv::EmitterJSON::EmitterJSON()
{
    std::cout << "EmitterJSON::EmitterJSON(): Called" << std::endl;
}

bool csv::EmitterJSON::begin(std::ostream& output,
                           const std::string& config,
                           const csv::Specification& specification,
                           const std::size_t record_count)
{
    std::cout << "EmitterJSON::begin(" << config << "): Called" << std::endl;
    //
    // Emit the start of an array.
    //
    output << "[" << std::endl;
    return true;
}

bool csv::EmitterJSON::emit_record(std::ostream& output,
                                 const csv::Specification& specification,
                                 const class Record& record)
{
    std::cout << "EmitterJSON::emit_record(): Called" << std::endl;

    std::string line {""};
    auto field_type_iter{ specification.fields().begin() };

    // Start with a new object.
    output << "{" << std::endl;

    for(const auto& field: record.fields()) {


        // Emit the field name.
        output << "    \"" << field_type_iter->name_ << "\": ";

        // Convert the given data type of the record's field
        // to a string field.
        switch(field_type_iter->type_) {
        case csv::FieldType::INT64:
            output << std::get<int64_t>(field) << std::endl;
            break;

        case csv::FieldType::DOUBLE:
            output << std::get<double>(field) << std::endl;
            break;

        case csv::FieldType::STRING:
            output << std::get<std::string>(field) << std::endl;
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

    return true;
}

bool csv::EmitterJSON::end(std::ostream& output,
                           const csv::Specification& specification)
{
    std::cout << "EmitterJSON::end(): Called" << std::endl;
    return true;
}


