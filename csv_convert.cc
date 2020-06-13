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
#include <fstream>
#include <sstream>
#include <getopt.h>



void usage(char* progname)
{
    std::cout << "Usage: " << progname << " -c <csv-file> -o <output-file> -f field_name:field_type [-f ...] [-t <type> ]" << std::endl;
    std::cout << "  -c <csv-file>               CSV file to ingest." << std::endl;
    std::cout << "  -T <type>                   CSV Reader type. Default 'csv'" << std::endl;
    std::cout << "  -t <type>                   Output file type. Default 'json'" << std::endl;
    std::cout << "  -e <escape-char>            Escape character to use. Default [none]." << std::endl;
    std::cout << "  -s <separator-char>         Separator character to use. Default ','." << std::endl;
    std::cout << "  -f <field_name:field_type>  CSV field specification." << std::endl << std::endl;
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
        {"csv-file", required_argument, NULL, 'c'},
        {"reader-type", required_argument, NULL, 'T'},
        {"type", required_argument, NULL, 't'},
        {"output", required_argument, NULL, 'o'},
        {"field", required_argument, NULL, 'f'},
        {"separator", required_argument, NULL, 's'},
        {"escape_char", required_argument, NULL, 'e'},
        {NULL, 0, NULL, 0}
    };

    std::string csv_file("");
    std::string output_type("json");
    std::string output_file("");
    std::string ingestion_type("csv");
    char separator_char(',');
    char escape_char(0);
    std::vector<std::string> field_spec_str;
    int ch(0);

    while ((ch = getopt_long(argc, argv, "c:t:o:T:f:s:", long_options, NULL)) != -1) {
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

        case 'f':
            field_spec_str.push_back(optarg);
            break;

        case 'T':
            ingestion_type = optarg;
            break;

        case 's':
            separator_char = *optarg;
            break;

        case 'e':
            escape_char = *optarg;
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

    if (field_spec_str.size() == 0) {
        std::cout << "Missing: -f <field-name:field-type>" << std::endl << std::endl;
        usage(argv[0]);
        exit(255);
    }

    // Divide the field spec strings up into a tuple vector
    // that is to be fed into the specification
    //
    std::vector<std::tuple<std::string, std::string> > field_spec_tuple;
    for (const auto& field_spec: field_spec_str) {
        std::istringstream f(field_spec);
        std::string name;
        std::string type;

        if (!getline(f, name, ':') || !getline(f, type, ':')) {
            std::cout << "Unknown -f format: " << field_spec << std::endl;
            usage(argv[0]);
            exit(255);
        }

        field_spec_tuple.push_back({ name, type });
    }


    // Create a spec
    csv::Specification spec(field_spec_tuple,
                            separator_char,
                            escape_char);


    // Create an ingester
    auto ingester(csv::Factory<csv::IngestionIface>::produce(ingestion_type));

    // Did we fail to create ingester?
    if (!ingester) {
        std::cout << "Could not create a reader of type " << ingestion_type << std::endl;
        usage(argv[0]);
        exit(255);
    }

    // Create an ingester
    auto emitter(csv::Factory<csv::EmitterIface>::produce(output_type));

    // Did we fail to create emitter
    if (!emitter) {
        std::cout << "Could not create a writer of type " << output_type << std::endl;
        usage(argv[0]);
        exit(255);
    }

    // Open the input file
    std::ifstream input(csv_file);

    if (!input.is_open()) {
        std::cout << "Could not open " << csv_file << " for reading." << std::endl;
        exit(255);
    }

    // Open the input file
    std::ofstream output(output_file);

    if (!output.is_open()) {
        std::cout << "Could not open " << output_file << " for writing." << std::endl;
        exit(255);
    }

    std::size_t record_index = 0;
    std::shared_ptr<csv::Record> record;

    //
    // Parse all records from input string stream, using the
    // created ingester, and emit them back out through
    // the emitter.
    //
    emitter->begin(output, "", spec);
    while(record = ingester->ingest_record(input, spec, record_index)) {
        emitter->emit_record(output, spec, *record);
        ++record_index;
    }
    emitter->end(output, spec);

    input.close();
    output.close();
    exit(0);
}

