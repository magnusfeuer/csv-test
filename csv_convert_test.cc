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
    // Dummy class to ingest directly from a string
    class IngestionString:
        public IngestionIface {
    public:
        bool ingest(const std::string& source,
                    std::istream& input,
                    class csv::Dataset& target) override
        {
            std::string line("");
            std::vector<std::string> tokens;
            uint32_t record_index(0);

            //
            // Traverse all lines in the multi-line string provided in source
            //
            while(std::getline(input, line, '\n')) {
                uint32_t token_count(0);
                tokens.resize(0);
                record_index++;

                // Tokenize the line.
                // Use the separator and escape char from the specification that
                // is tied to the dataset.
                token_count = csv::tokenize_line(line,
                                                 target.specification().separator_char(),
                                                 target.specification().escape_char(),
                                                 tokens);

                // Check for empty lines, which we can ignore.
                if (!token_count)
                    continue;

                // Did we get the correct number of tokens?
                if (token_count != target.specification().field_count()) {
                    std::cout << source << ": line: " << record_index+1 <<
                        ": Incorrect number of fields: "<< token_count<<
                        ". Expected: " << target.specification().field_count() << std::endl;
                    return false;
                }

                // Create a new record and add it to the dataset
                target.append_record(std::make_unique<const csv::Record>(target.specification(),
                                                                         record_index,
                                                                         tokens));
            }
            return true;
        }
    };

    class EmitterString: public EmitterIface {
    public:
        EmitterString(void) {}

        bool begin(std::ostream& output,
                   const std::string& config,
                   const csv::Specification& specification,
                   const std::size_t record_count) override
        {
            return true;
        }

        bool emit_record(std::ostream& output,
                         const csv::Specification& specification,
                         const class Record& record,
                         const std::size_t record_index) override
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

bool ingestion_string_registration_ =
    csv::Factory<csv::IngestionIface>::register_producer("csv_string_ingester",
                                                         [](void) -> std::shared_ptr<csv::IngestionIface> {
                                                             return std::make_shared<csv::IngestionString>();
                                                         });


int main(int argc, char* argv[])
{
    auto csv_ingester(csv::Factory<csv::IngestionIface>::produce("csv_string_ingester"));
    auto csv_emitter(csv::Factory<csv::EmitterIface>::produce("csv_string_emitter"));

    // Setup a specification for the CSV String
    csv::Specification spec({
            { "First Field", "string" },
            { "Second Field", "string" },
            { "Third Field", "int" },
            { "Fourth Field", "double" }
        }, ',', '\\');

    // Setup a dataset and tie it to the specification
    csv::Dataset dataset(spec);

    // Sample data.
    static std::string in_csv_data {
        "A1,B1,    1,    1.1\n"
        "A2,B2,2    ,2.2    \n"
        "A3,B3,    3,    3.3\n"
        "A4,B4,    4    ,    4.4   \n"
    };
    std::istringstream in_str_stream(in_csv_data);

    //
    // Feed sample data to ingester and build up the data set.
    //
    dataset.ingest(*csv_ingester, in_str_stream, "");

    //
    // Emit back to a CSV string
    //
    std::ostringstream out_str_stream;
    dataset.emit(*csv_emitter, out_str_stream, "");

    std::cout << "Result:" <<std::endl << out_str_stream.str() << std::endl;
    std::cout << "Done" << std::endl;
}

