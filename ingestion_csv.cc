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
#include "csv_spec.hh"

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

bool csv::IngestionCSV::ingest(const std::string& source,  Dataset& target)
{
    std::ifstream ifs(source);
    std::string line {""};
    std::vector<std::string> tokens;
    uint32_t line_no(0);

    std::cout << "IngestionCSV::ingest(" << source << "): Called" << std::endl;

    if (!ifs.is_open()) {
        std::cout << "Ingest::ingest(): Could not open " << source << std::endl;
        return false;
    }

    // Read the entire file in one go.
    // FIXME: Make re-entrant by chunking reads.
    //
    while(!ifs.eof()) {
        uint32_t token_count(0);
        std::getline(ifs, line);
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
            ifs.close();
            return false;
        }

        // Create a new record and add it to the dataset
        target.append_record(std::make_unique<const csv::Record>(target.specification(), tokens));
    }
    return true;
}
