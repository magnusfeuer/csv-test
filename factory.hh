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

//! \class EmitterIface
//! Emitter interface class
//
/// Called by DataSet emit records.
//

#ifndef __FACTORY_HH__
#define __FACTORY_HH__
#include <memory>
#include <list>
#include <string>
#include <unordered_map>
#include <iostream>
#include <functional>

namespace csv {

    template <typename T>
    class Factory {
    public:
        static bool register_producer(const std::string& name, std::function<std::shared_ptr<T>(void) > producer);
        static std::shared_ptr<T> produce(const std::string& name);
        static void producers(std::list<std::string>& result);

    private:
        static std::unordered_map<std::string, std::function<std::shared_ptr<T> (void) > > producers_;
    };

    template<typename T>
    class ProducerIface {
    public:
        virtual std::shared_ptr<T> produce(void) const { return NULL; };
    private:
        // Don't allow instantiation.
        ProducerIface() {}
    };

    template<typename TBase, typename TDerived>
    class Producer:
        public ProducerIface <TBase> {
    public:
        std::shared_ptr<TBase> produce(void) const
        {
            return std::make_shared<TDerived>();
        }
    };
};
#endif
