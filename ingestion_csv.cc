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

#include "ingestion_csv.hh"
#include <iostream>
#include <fstream>
#include "factory.hh"
#include "factory_impl.hh"
#include "ingestion_factory_impl.hh"
#include "csv_common.hh"
#include "dataset.hh"
#include "specification.hh"

// Create a factory producer
// See emitter_json.hh for details
bool ingestion_csv_registration_ =
    csv::Factory<csv::IngestionIface>::register_producer("csv",
                                                         [](void) -> std::shared_ptr<csv::IngestionIface> {
                                                             return std::make_shared<csv::IngestionCSV>();
                                                         });

csv::IngestionCSV::IngestionCSV()
{
    std::cout << "IngestionCSV::IngestionCSV(): Called" << std::endl;
}

bool csv::IngestionCSV::ingest(const std::string& config,
                               std::istream& input,
                               Dataset& target)
{
    std::string line {""};
    std::vector<std::string> fields;
    uint32_t record_index(0);

    std::cout << "IngestionCSV::ingest(" << config << "): Called" << std::endl;


    // Read the entire file in one go.
    // FIXME: Make re-entrant by chunking reads.
    //
    while(!input.eof()) {
        uint32_t field_count(0);
        std::getline(input, line);
        fields.resize(0);
        record_index++;

        // Tokenize the line.
        // Use the separator and escape char from the specification that
        // is tied to the dataset.
        field_count = csv::tokenize_line(line,
                                         target.specification().separator_char(),
                                         target.specification().escape_char(),
                                         fields);

        // Check for empty lines, which we can ignore.
        if (!field_count)
            continue;

        // Did we get the correct number of tokens?
        if (field_count != target.specification().field_count()) {
            std::cout << config << ": line: " << record_index+1 <<
                ": Incorrect number of fields: "<< field_count <<
                ". Expected: " << target.specification().field_count() << std::endl;
            return false;
        }

        //
        // Create a new record and add it to the dataset
        //
        target.append_record(std::make_unique<const csv::Record>(target.specification(),
                                                                 record_index,
                                                                 fields));
    }
    return true;
}
