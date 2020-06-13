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

#ifndef __EMITTER_IFACE_HH__
#define __EMITTER_IFACE_HH__
#include <cstddef>
#include <string>
#include "specification.hh"
#include "record.hh"
#include <ostream>
namespace csv {

    /// An interface class to emit records to an output stream.
    //
    /// This class can be subclassed to suppport JSON, YAML, and other
    /// output formats.
    ///
    /// Subclasses are managed through the csv::Factory<EmitterIface> template
    /// instantiation, and should never be instantiated directly.
    ///
    /// The call flow for an instantiation is like follow.
    ///
    /// \code
    /// emitter_obj->begin();
    /// emitter_obj->emit_record();
    /// emitter_obj->emit_record();
    /// emitter_obj->emit_record();
    /// ...
    /// emitter_obj->end();
    /// \endcode
    ///
    /// The begin() call is used to setup the object for record
    /// emission, writing headers, etc.
    ///
    /// Each record is written out through their own call to emit_record()
    ///
    /// The end() call is used to write data footers and clean up.
    class EmitterIface {

    public:

        /// Default virtual destructor
        virtual ~EmitterIface(void) = default;

        /// Emit record headers.
        //
        /// This method is to be redefined to emit necessary header
        /// data to \a output and prepare for the emission of records
        /// throug emit_record().
        //
        /// @param output The output file stream to emit data to
        /// @param config Additional configuration data.
        /// @param specification  Record specification.
        //
        /// @return true - Header data was successfully emitted.
        /// @return false - Header data could not be emitted.
        ///
        virtual bool begin(std::ostream& output,
                           const std::string& config,
                           const csv::Specification& specification) { return false; };

        /// Emit a single record to an output stream.
        //
        /// This method is to be redefined to emit the single provided
        /// record in the desired output format to the \a output file
        /// stream.
        ///
        /// The specification contains the format of the provided record.
        /// Specifically Specification::fields() will provide the name and
        /// data type for each field retrieved through Record::fields().
        ///
        /// A typical sequence to write out a single record would be:
        /// \code
        ///     auto field_type_iter { specification.fields().begin() };
        ///     for(const auto& field: record.fields()) {
	///
	///         // Convert record field to string.
        ///         // The name of the field is available in field_type->name_
	///         switch(field_type_iter->type_) {
	///           case csv::FieldType::INT64:
	///             write_field(std::to_string(std::get<int64_t>(field)));
	///             break;
	///
	///           case csv::FieldType::DOUBLE:
	///             write_field(std::to_string(std::get<double>(field)));
	///             break;
	///
	///           case csv::FieldType::STRING:
	///             write_field(std::get<std::string>(field));
	///             break;
	///         }
	///         ++field_type_iter;
        ///     }
        /// \endcode
        ///
        /// The record iterator field is advanced in lockstep with the
        /// specification field_type_iter, enabling the code to determine
        /// of the name and data type of each field being written.
        ///
        /// @param output The output file stream to emit therecord to to.
        /// @param specification  Record specification.
        /// @param record The record to emit.
        //
        /// @return true - Record was successfully emitted.
        /// @return false - Record data could not be emitted.
        virtual bool emit_record(std::ostream& output,
                                 const csv::Specification& specification,
                                 const class Record& record) { return false; }

        /// Emit footer data and clean up.
        //
        /// Allows for emitter to write out footer data and clean up.
        /// This method should \b not close \a output.
        ///
        /// @return true - Footer data was successfully emitted.
        /// @return false - Footer data could not be emitted.
        ///
        virtual bool end(std::ostream& output,
                         const csv::Specification& specification) { return false; };

    protected:
        /// Default constructor 
        //
        /// Protected status means that the EmitterIface class cannot
        /// be directly instantiated.
        EmitterIface(void) = default;

    private:
    };
};
#endif
