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

#include "emitter_iface.hh"
#include "ingestion_iface.hh"
#include "factory.hh"
#include "factory_impl.hh"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include "csv_common.hh"


namespace csv {
    class EmitterString: public EmitterIface {
    public:
        EmitterString(void) {}

        bool begin(std::ostream& output,
                   const std::string& config,
                   const csv::Specification& specification) override
        {
            return true;
        }

        bool emit_record(std::ostream& output,
                         const csv::Specification& specification,
                         const class Record& record) override
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
        bool end(std::ostream& output,
                 const csv::Specification& specification) override { return true; }

    private:
    };
}

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
bool emitter_string_registration_ =
    csv::Factory<csv::EmitterIface>::
    register_producer("csv_string_emitter",
                      [](void) -> std::shared_ptr<csv::EmitterIface> {
                          return std::make_shared<csv::EmitterString>();
                      });


int main(int argc, char* argv[])
{
    auto csv_ingester(csv::Factory<csv::IngestionIface>::produce("csv"));
    auto csv_emitter(csv::Factory<csv::EmitterIface>::produce("csv_string_emitter"));

    // Setup a specification for the CSV String
    csv::Specification spec({
            { "First Field", "string" },
            { "Second Field", "string" },
            { "Third Field", "int" },
            { "Fourth Field", "double" }
        }, ',', '\\');


    //
    // Sample CSV data with whitespace
    //
    static std::string in_csv_data {
        "A1,B1,    1,    1.1\n"
        "A2,B2,2    ,2.2    \n"
        "A3,B3,    3,    3.3\n"
        "A4,B4,    4    ,    4.4   \n"
    };
    std::istringstream in_str_stream1(in_csv_data);
    std::ostringstream out_str_stream1;

    std::size_t record_index = 0;
    std::shared_ptr<csv::Record> record;

    //
    // Parse all records from input string stream and emit them back out
    // as another CSV string
    //
    while(record = csv_ingester->ingest_record(in_str_stream1, spec, record_index)) {
        csv_emitter->emit_record(out_str_stream1, spec, *record);
        ++record_index;
    }

    //
    // Do a second pass, based on the output CSV string above, to ensure
    // that we have normalized data with removed whitespaces and same
    // double precision on both input and output.
    //
    std::istringstream in_str_stream2(out_str_stream1.str());
    std::ostringstream out_str_stream2;
    while(record = csv_ingester->ingest_record(in_str_stream2, spec, record_index)) {
        csv_emitter->emit_record(out_str_stream2, spec, *record);
        ++record_index;
    }

    // First and second output stream should be identical
    if (out_str_stream1.str() != out_str_stream2.str()) {
        std::cout << "FAILED" << std::endl << std::endl;

        std::cout << "Original input:" << std::endl << in_csv_data << std::endl << std::endl;
        std::cout << "First pass:" <<std::endl << out_str_stream1.str() << std::endl<< std::endl;
        std::cout << "Second pass:" <<std::endl << out_str_stream2.str() << std::endl<< std::endl;
        exit(255);
    }
    std::cout << "pass." << std::endl;
    exit(0);
}

