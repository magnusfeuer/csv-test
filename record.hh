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

//! \class Record
//! A Single record.
//
/// This class hosts a single CSV record, as provided by a ReaderIface
/// implementation.
//  Once constructed, an instance is immutable.
//
#ifndef __RECORD_HH__
#define __RECORD_HH__
#include "specification.hh"
#include <variant>
#include <memory>
#include <list>
#include "emitter_iface.hh"
#include "ingestion_iface.hh"
#include <istream>
#include <ostream>

namespace csv {
    class Record {
    public:
        Record(const Specification& specification,
               std::size_t index,
               const std::vector<std::string>& tokens);

        /// Retrieve a single field. Throw an exception on type mismatch.
        template<typename T>
        const T& field(int field_index) const { std::get<T>(fields_[field_index]); }

        const std::vector<std::variant<int64_t, double, std::string> >& fields(void) const { return fields_; };

        const std::size_t index() const { return index_; }

    private:
        std::vector<std::variant<int64_t, double, std::string> > fields_;
        std::size_t index_;
    };
};
#endif
