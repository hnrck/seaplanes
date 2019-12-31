//! \file    LogicalProcessorAttribute.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Logical processor Attribute manipulation.
//!
//! This class provides a simple way to use attribute.

#ifndef LOGICALPROCESSORATTRIBUTE_H
#define LOGICALPROCESSORATTRIBUTE_H

#include <memory>
#include <string>

#include <LogicalProcessorCommon.h>

namespace Seaplanes {

//! \brief Type for attribute shared pointer.
using SpAttribute = std::shared_ptr<class Attribute>;

//! \brief No fresh value error.
//!
//! When fresh value is asked from attribute, and there is none, this exception
//! must be raised.
//!
class AttributeNoFreshValue final : public std::exception {};

//! \brief Attribute class
class Attribute final {
public:
  //! \brief Attribute factory builder.
  //! \param  name The name of the attribute.
  //! \return The attribute in a shared pointer.
  static SpAttribute create(Name /* name */);

  //! \brief Name getter
  //! \return The name of the attribute.
  Name getName() const;

  //! \brief Value setter.
  //! \param  value The value to set.
  template <typename T> void setValue(T /* value */);

  //! \brief Implicit integer value setter.
  //! \param  value The value to set.
  void setValue(int /* value */);

  //! \brief Implicit double value setter.
  //! \param  newValue The value to set.
  void setValue(double /* value */);

  //! \brief Implicit boolean value setter.
  //! \param  value The value to set.
  void setValue(bool /* value */);

  //! \brief Value getter.
  //! \return The value.
  template <typename T> T getValue() const;

  //! \brief Value getter, checking if the value is fresh or not.
  //! \return The value.
  template <typename T> T getFreshValue() const noexcept(false);

  //! \brief Attribute handle setter.
  //! \param  attributeHandle The attribute handle
  void setHandle(RTI::AttributeHandle /*attributeHandle*/);

  //! \brief Attribute handle setter, handle is generated from RTIA.
  //! \param  &objectClassHandle The handle of the object to which belong the
  //! attribute.
  //! \param  *rtiAmb The RTIA.
  void setHandle(RTI::RTIambassador * /*rtiAmb*/,
                 const RTI::ObjectClassHandle & /*objectClassHandle*/);

  //! \brief Attribute handle getter.
  //! \return The attribute handle.
  RTI::AttributeHandle getHandle() const;

private:
  Name __name_;                   //!< Attribute name.
  RTI::AttributeHandle __handle_; //!< Attribute handle, generated during
                                  //!< attribute initialization
  mutable bool __fresh_;          //!< True if value asked is fresh, else false.
  union {
    int i;
    double d;
    bool b;
  } __value_; //!< Attribute value, which can have different representations.

  //! \brief Attribute constructor.
  //! \param  name The name of the attribute.
  //! \return The attribute.
  explicit Attribute(Name /* name */);
};

} // namespace Seaplanes

#endif // LOGICALPROCESSORATTRIBUTE_H
