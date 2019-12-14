//! \file    SeaplanesObjectInstance.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Object instance manipulation implementation.

#include <LogicalProcessorObjectInstance.h>

using std::move;

namespace Seaplanes {

ObjectInstance::ObjectInstance(Name name, SpObject sp_object)
    : __name_(move(name)), __sp_object_(move(sp_object)), __handle_(0),
      __up_instance_object_(nullptr), __up_instance_attributes_(nullptr),
      __sp_attributes_(VecSpAttribute()), __discovered_(false) {}

Name ObjectInstance::getName() const { return (__name_); }

void ObjectInstance::setHandle(const RTI::ObjectHandle handle) {
  __handle_ = handle;
}

RTI::ObjectHandle ObjectInstance::getHandle() const { return (__handle_); }

RTI::ObjectClassHandle ObjectInstance::getObjectClassHandle() const {
  return (__sp_object_->getHandle());
}

void ObjectInstance::addAttribute(SpAttribute sp_attribute) {
  __sp_attributes_.push_back(sp_attribute);
}

void ObjectInstance::setAttributesHandles(RTI::RTIambassador *p_rtia) {
  __up_instance_attributes_.reset(
      RTI::AttributeHandleSetFactory::create(__sp_attributes_.size()));
  for (const SpAttribute &sp_attribute : __sp_attributes_) {
    sp_attribute->setHandle(p_rtia, __sp_object_->getHandle());
    __up_instance_attributes_->add(sp_attribute->getHandle());
  }
}

void ObjectInstance::setDiscovered() { __discovered_ = true; }

void ObjectInstance::unsetDiscovered() { __discovered_ = false; }

bool ObjectInstance::getDiscovered() const { return (__discovered_); }

} // namespace Seaplanes
