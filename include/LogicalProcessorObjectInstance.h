//! \file    LogicalProcessorObjectInstance.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Logical processor Object instance manipulation.
//!
//! This class provides a simple way to use object instance.

#ifndef LOGICALPROCESSOROBJECTINSTANCE_H
#define LOGICALPROCESSOROBJECTINSTANCE_H

#include <memory>
#include <string>
#include <vector>

#include <LogicalProcessorAttribute.h>
#include <LogicalProcessorCommon.h>
#include <LogicalProcessorObjectClass.h>

#ifndef USE_CERTI_MESSAGE_BUFFER
// USE_CERTI_MESSAGE_BUFFER. If set to 1, use CERTI message buffer to send
// attribute values.
#define USE_CERTI_MESSAGE_BUFFER 0
#endif // USE_CERTI_MESSAGE_BUFFER

namespace Seaplanes {

//! \brief Type for RTI attribute handle value pair set unique pointer.
using UpRTIAttributeHandleValuePairSet =
    std::unique_ptr<RTI::AttributeHandleValuePairSet>;

//! \brief Type for RTI attribute handle set unique pointer.
using UpRTIAttributeHandleSet = std::unique_ptr<RTI::AttributeHandleSet>;

//! \brief Type for vector of attribute shared pointers.
using VecSpAttribute = std::vector<SpAttribute>;

class ObjectInstance {
public:
  //! \brief Default virtual destructor
  virtual ~ObjectInstance() = default;

  //! \brief Object instance name getter.
  //! \return The name of the object instance.
  Name getName() const;

  //! \brief Object instance handle setter.
  //! \param handle The object instance handle
  void setHandle(RTI::ObjectHandle /* handle */);

  //! \brief Object handle getter.
  //! \return The object handle.
  RTI::ObjectHandle getHandle() const;

  //! \brief Object instantiation object handle getter.
  //! \return The object handle of the object instance object.
  RTI::ObjectClassHandle getObjectClassHandle() const;

  //! \brief Add an attribute to the object instance.
  //! \param sp_attribute A shared pointer to the attribute to add.
  void addAttribute(SpAttribute /* sp_attribute */);

  //! \brief Set the handles of the object instance attributes
  //! \param  p_rtia A pointer to the RTIA
  void setAttributesHandles(RTI::RTIambassador * /* p_rtia */);

  //! \brief Set the object instance as discovered.
  void setDiscovered();

  //! \brief Set the object instance as not discovered.
  void unsetDiscovered();

  //! \brief Get if the object instance is discovered or not.
  //! \return True if discovered, else false.
  bool getDiscovered() const;

protected:
  Name __name_;

  //! The object which the object instance belongs to.
  SpObject __sp_object_;

  //! The handle of the object instance.
  RTI::ObjectHandle __handle_;

  //! An unique pointer used to pair value.
  UpRTIAttributeHandleValuePairSet __up_instance_object_;

  //! An unique pointer used to pair attributes.
  UpRTIAttributeHandleSet __up_instance_attributes_;

  //! Attributes the object instance use.
  VecSpAttribute __sp_attributes_;

  //! True if the object instance is discovered, else false.
  bool __discovered_;

  //! \brief Object instance constructor.
  //! \param  name The name of the object.
  //! \param  object The object which the object instance belongs to.
  //! \return The object instance.
  ObjectInstance(Name /* name */, SpObject /* sp_object */);
};

} // namespace Seaplanes

#endif // LOGICALPROCESSOROBJECTINSTANCE_H
