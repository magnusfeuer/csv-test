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

#ifndef __FACTORY_HH__
#define __FACTORY_HH__
#include <memory>
#include <list>
#include <string>
#include <unordered_map>
#include <iostream>
#include <functional>

namespace csv {

    /// A generic and static factory.
    //
    /// Template instantiations of this class allows subclasses of \c T to
    /// be instantiated. Subclass registration is done automatically
    /// without the need to write any code outside the subclass itself.
    ///
    /// A subclass producer can automatically be registered by adding the following
    /// line of code to the subclass' source file.
    ///
    /// \code
    /// bool factory_registration_ =
    /// csv::Factory<BASECLASS>::
    ///     register_producer("my_name",
    ///                       [](void) -> std::shared_ptr<BASECLASS> {
    ///                           return std::make_shared<SUBCLASS>();
    ///                       });
    /// \endcode
    ///
    /// The \c factory_registration_ variable will be dynamically
    /// created at program startup and initialized to the dummy return
    /// value of the \c csv::Factory<BASECLASS>::register_producer()
    /// call.
    ///
    /// The provided lambda function simply creates a shared pointer
    /// to a new instance of the given subclass, and returns a
    /// baseclass-converted pointer to the instance.
    ///
    /// There is currently no way to provide arguments to the constructor
    /// of a fabricated object.
    ///
    template <typename T>
    class Factory {
    public:
        /// Register a new producer.
        //
        /// This method is invoked by instantiated Producer templates to add support for
        /// creating new object types.
        ///
        /// The \a producer argument is a function that is called to
        /// create a new object of a given subclass to \c T.
        ///
        /// The function \a producer, which returns a shared pointer
        /// to the baseclass \c T is invoked by produce() when the
        /// value of \a name is provided. See class documentation for details.
        ///
        /// @param name - The symbolic name to tie to the the class. 
        /// @param producer - The function to call to create a new intsance of the class.
        ///
        /// @return true - The producer was registered under \a name.
        ///
        static bool register_producer(const std::string& name,
                                      std::function<std::shared_ptr<T>(void) > producer);

        /// Produce a new instance of the named class.
        //
        /// This function will map the provided name to a producer function, previously
        /// registered with a register_producer() call, and invoke it to create
        /// a new instance of the given class.
        ///
        /// @param name The name of the class, as provided to register_producer(), to instantiate.
        ///
        /// @return Shared pointer - A new instance of the given class.
        /// @return NULL - The class \a name has not been registered.
        ///
        static std::shared_ptr<T> produce(const std::string& name);


        /// Return a list of all currenty registered classes
        //
        /// The provided \a result vector will be cleared before the registered
        /// classes are added to it.
        ///
        /// @param result - The vector to fill with the name of all supported classes.
        ///
        /// @return n/a
        ///
        static void producers(std::list<std::string>& result);

    private:
        static std::unordered_map<std::string, std::function<std::shared_ptr<T> (void) > > producers_;
    };
};
#endif
