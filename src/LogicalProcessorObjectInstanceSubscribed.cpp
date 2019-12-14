//! \file    SeaplanesObjectInstanceSubscribed.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Subscribed object instance manipulation implementation.

#include <LogicalProcessorObjectInstanceSubscribed.h>

using std::make_unique;

namespace Seaplanes {

#if !USE_CERTI_MESSAGE_BUFFER
// If the CERTI message buffer is not used, a buffer is declared for reception.
#define MESSAGE_BUFFER_LENGTH 256
char messageBuffer[MESSAGE_BUFFER_LENGTH];
#endif // USE_CERTI_MESSAGE_BUFFER

ObjectInstanceSubscribed::ObjectInstanceSubscribed(Name name,
                                                   SpObject sp_object)
    : ObjectInstance(move(name), move(sp_object)),
      __map_sp_attributes_(MapHandleSpAttribute()) {}

UpObjectInstanceSubscribed
ObjectInstanceSubscribed::create(Name name, SpObject sp_object) {
  return make_unique<ObjectInstanceSubscribed>(move(name), sp_object);
}

void ObjectInstanceSubscribed::initAttributesMap() {
  for (auto &sp_attribute : __sp_attributes_) {
    __map_sp_attributes_[sp_attribute->getHandle()] = sp_attribute;
  }
}

void ObjectInstanceSubscribed::subscribeObjectClassAttributes(
    RTI::RTIambassador *rtiAmb) {
  rtiAmb->subscribeObjectClassAttributes(__sp_object_->getHandle(),
                                         *__up_instance_attributes_);
}

void ObjectInstanceSubscribed::waitRegistering(RTI::RTIambassador *rtiAmb) {
  while (!getDiscovered()) {
    rtiAmb->tick2();
  }
}

void ObjectInstanceSubscribed::unsubscribe(RTI::RTIambassador *rtiAmb) {
  rtiAmb->unsubscribeObjectClass(__sp_object_->getHandle());
}

void ObjectInstanceSubscribed::tryToDiscover(
    const Name &name, const RTI::ObjectClassHandle &objectClassHandle,
    const RTI::ObjectHandle &objectHandle) {
  if (objectClassHandle == __sp_object_->getHandle()) {
    // if (name_.compare(objectName) == 0) {
    if (__name_ == name) {
      setHandle(objectHandle);
      setDiscovered();
    }
  }
}

void ObjectInstanceSubscribed::reflectAttributeValues(
    const RTI::AttributeHandleValuePairSet &receivedAttributes) {
  RTI::ULong valueLength;
  RTI::AttributeHandle attributeHandle;

#if USE_CERTI_MESSAGE_BUFFER
  libhla::MessageBuffer buffer;
#endif // USE_CERTI_MESSAGE_BUFFER

  for (auto i = 0U; i < receivedAttributes.size(); ++i) {
    attributeHandle = receivedAttributes.getHandle(i);
    valueLength = receivedAttributes.getValueLength(i);
#if USE_CERTI_MESSAGE_BUFFER
    buffer.resize(valueLength);
    buffer.reset();
    receivedAttributes.getValue(i, static_cast<char *>(buffer(0)), valueLength);
    buffer.assumeSizeFromReservedBytes();
#else  // USE_CERTI_MESSAGE_BUFFER
    receivedAttributes.getValue(i, static_cast<char *>(messageBuffer),
                                valueLength);
#endif // USE_CERTI_MESSAGE_BUFFER

#if USE_CERTI_MESSAGE_BUFFER
    const double value = buffer.read_double();
    // buffer.read_doubles(&value, 0);
    mapped_attributes_[attributeHandle]->setValue(value);
#else  // USE_CERTI_MESSAGE_BUFFER
    // generic.
    auto *const p_value = reinterpret_cast<double *>(messageBuffer);
    __map_sp_attributes_[attributeHandle]->setValue(*p_value);
#endif // USE_CERTI_MESSAGE_BUFFER
  }
}

} // namespace Seaplanes
