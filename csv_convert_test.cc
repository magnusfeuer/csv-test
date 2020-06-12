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
        bool ingest(const std::string& source,  class csv::Dataset& target)
        {
            std::stringstream ss(source);
            std::string line("");
            std::vector<std::string> tokens;
            uint32_t line_no(0);

            //
            // Traverse all lines in the multi-line string provided in source
            //
            while(std::getline(ss, line, '\n')) {
                uint32_t token_count(0);
                tokens.resize(0);
                line_no++;

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
                    std::cout << source << ": line: " << line_no <<
                        ": Incorrect number of fields: "<< token_count<<
                        ". Expected: " << target.specification().field_count() << std::endl;
                    return false;
                }

                // Create a new record and add it to the dataset
                target.append_record(std::make_unique<const csv::Record>(target.specification(), tokens));
            }
            return true;
        }
    };
};

bool ingestion_string_registration_ =
    csv::Factory<csv::IngestionIface>::register_producer("csv_string",
                                                         [](void) -> std::shared_ptr<csv::IngestionIface> {
                                                             return std::make_shared<csv::IngestionString>();
                                                         });


int main(int argc, char* argv[])
{
    auto csv_ingester(csv::Factory<csv::IngestionIface>::produce("csv_string"));
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
    static std::string csv_data {
        "A1, B1, 1, 1.1\n"
        "A2, B2, 2, 2.2\n"
        "A3, B3, 3, 3.3\n"
        "A4, B4, 4, 4.4\n"
    };

    // Feed sample data to ingester and build up the data set.
    //
    csv_ingester->ingest(csv_data, dataset);

    std::cout << "Done" << std::endl;
}

