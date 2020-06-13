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

int main(int argc, char* argv[])
{
    // Produce a CSV file ingester
    auto csv_ingester(csv::Factory<csv::IngestionIface>::produce("csv"));

    // Produce a CSV file emitter (different factory than above)
    auto csv_emitter(csv::Factory<csv::EmitterIface>::produce("csv"));

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
    csv::convert(spec,
                 *csv_ingester, in_str_stream1,
                 *csv_emitter, out_str_stream1);

    //
    // Do a second pass, based on the output CSV string above, to ensure
    // that we have normalized data with removed whitespaces and same
    // double precision on both input and output.
    //
    std::istringstream in_str_stream2(out_str_stream1.str());
    std::ostringstream out_str_stream2;
    csv::convert(spec,
                 *csv_ingester, in_str_stream2,
                 *csv_emitter, out_str_stream2);

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

