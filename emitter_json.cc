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

bool csv::EmitterJSON::begin(const std::string& config,
                             const Specification& csv_spec,
                             std::size_t record_count)
{
    std::cout << "EmitterJSON::begin(" << config << "): Called" << std::endl;
    return true;
}

bool csv::EmitterJSON::emit_record(const class Record& record, std::size_t record_index)
{
    std::cout << "EmitterJSON::emit_record(): Called" << std::endl;
    return true;
}

bool csv::EmitterJSON::end(void)
{
    std::cout << "EmitterJSON::end(): Called" << std::endl;
    return true;
}


