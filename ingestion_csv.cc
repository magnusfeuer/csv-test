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
#include "csv_common.hh"
#include "specification.hh"
#include "record.hh"
#include "ingestion_factory_impl.hh"

// Create a factory producer
// See emitter_json.hh for details
//
bool ingestion_csv_registration_ =
    csv::Factory<csv::IngestionIface>::register_producer("csv",
                                                         [](void) -> std::shared_ptr<csv::IngestionIface> {
                                                             return std::make_shared<csv::IngestionCSV>();
                                                         });


std::shared_ptr<csv::Record> csv::IngestionCSV::ingest_record(std::istream& input,
                                                              const csv::Specification& specification,
                                                              const std::size_t record_index)
{
    std::string line {""};
    std::vector<std::string> fields;
    uint32_t field_count(0);

    // Read the next line.
    if (!std::getline(input, line))
        return NULL;

    // Tokenize the line.
    // Use the separator and escape char from the specification that
    // is tied to the dataset.
    //
    field_count = csv::tokenize_line(line,
                                     specification.separator_char(),
                                     specification.escape_char(),
                                     fields);

    // Did we get the correct number of tokens?
    //
    if (field_count != specification.field_count()) {
        std::cout << "IngestionCSV::ingest_record(): line: " << record_index+1 <<
            ": Incorrect number of fields: "<< field_count <<
            ". Expected: " << specification.field_count() << std::endl;
        exit(255);
    }

    // Create a record and return it.
    //
    return std::make_shared<csv::Record>(specification, record_index, fields);
}
