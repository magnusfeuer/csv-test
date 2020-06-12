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

#ifndef __FACTORY_IMPL_HH__
#define __FACTORY_IMPL_HH__


#include "factory.hh"

template <typename T>
bool csv::Factory<T>::register_producer(const std::string& name, std::function<std::shared_ptr<T> (void) > producer)
{
    std::cout << &producers_ << ": Registering producer " << name << " as " << typeid(T).name() << std::endl;
    producers_.insert(std::pair<std::string, std::function<std::shared_ptr<T> (void)> >(name, producer));
    return true;
}


template <typename T>
std::shared_ptr<T> csv::Factory<T>::produce(const std::string& name)
{
    auto pub_iter(producers_.find(name));

    if (pub_iter == producers_.end())
        return 0;

    std::cout << "Producing an instance of " << name << std::endl;
    return pub_iter->second();
}

template <typename T>
void csv::Factory<T>::producers(std::list<std::string>& result)
{
    result.clear();

    for(auto& codec: producers_)
        result.push_back(codec.first);
}

#endif
