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
#include <getopt.h>


void usage(char* progname)
{
    std::cout << "Usage: " << progname << "-c <csv-file> -t <type> -o <output-file> field_name1:field_type1 [...]" << std::endl;
    std::cout << "  -c <csv-file>            CSV file to ingest." << std::endl;
    std::cout << "  -T <type>                CSV Reader type. Default 'csv'" << std::endl;
    std::cout << "  -t <type>                Output file type." << std::endl;
    std::cout << "  -o <output-file>         Output file name." << std::endl;
    std::cout << "  field_nameN:field_typeN  CSV field specification." << std::endl << std::endl;
    std::cout << "field_name is the name of the given field." << std::endl;
    std::cout << "field_type is data type. Supported values are int, double, and string." << std::endl << std::endl;

    std::list<std::string> lst;

    std::cout << "Available output types: "<< std::endl;
    csv::Factory<csv::EmitterIface>::producers(lst);
    for(auto& name: lst)
        std::cout  << name << " ";

    std::cout << std::endl << std::endl;

    std::cout << "Available reader types: "<< std::endl;
    csv::Factory<csv::IngestionIface>::producers(lst);

    for(auto& name: lst)
        std::cout  << name << " ";

    std::cout << std::endl;

}

int main(int argc, char* argv[])
{

    static struct option long_options[] =  {
        {"csv-file", required_argument, NULL, 's'},
        {"reader-type", required_argument, NULL, 's'},
        {"type", required_argument, NULL, 's'},
        {"output", required_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    std::string csv_file("");
    std::string output_type("");
    std::string output_file("");
    std::string ingestion_type("csv");
    int ch(0);

    while ((ch = getopt_long(argc, argv, "c:t:o:T:", long_options, NULL)) != -1) {
        switch (ch)
        {
            // short option 't'
        case 'c':
            csv_file = optarg;
            break;

        case 'o':
            output_file = optarg;
            break;

        case 't':
            output_type = optarg;
            break;

        case 'T':
            ingestion_type = optarg;
            break;

        default:
            usage(argv[0]);
            exit(255);
        }
    }


    if (csv_file.empty()) {
        std::cout << "Missing: -c <csv-file>" << std::endl << std::endl;
        usage(argv[0]);
        exit(255);
    }

    if (output_file.empty()) {
        std::cout << "Missing: -o <output-file>" << std::endl << std::endl;
        usage(argv[0]);
        exit(255);
    }

    if (output_type.empty()) {
        std::cout << "Missing: -t <output-type>" << std::endl << std::endl;
        usage(argv[0]);
        exit(255);
    }
}

