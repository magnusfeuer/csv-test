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

//! \class IngestionIface
//! Ingestion interface class
//
///
//
#ifndef __INGESTION_CSV__
#define __INGESTION_CSV__

#include "ingestion_iface.hh"

namespace csv {
    class IngestionCSV:
        public IngestionIface {
    public:
        IngestionCSV(void);
        bool ingest(const std::string& config,
                    std::istream& input,
                    class csv::Dataset& target) override;
    };
};
#endif
