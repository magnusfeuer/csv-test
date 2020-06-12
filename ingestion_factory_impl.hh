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



#ifndef __INGESTION_FACTORY_IMPL_HH__
#define __INGESTION_FACTORY_IMPL_HH__

#include "ingestion_iface.hh"
#include "factory.hh"
#include <unordered_map>

//
// Force the instantiation of a IngestionIface factory that can
// insstantiate any number of IngestionIface implementations.
//
template <> std::unordered_map<std::string, std::function<std::shared_ptr<csv::IngestionIface>(void) > >
csv::Factory<csv::IngestionIface>::producers_ {};

#endif
