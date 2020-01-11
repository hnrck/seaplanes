//! \file    SeaplanesObjectInstancePublished.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Published object instance manipulation implementation.

#include <LogicalProcessorObjectInstancePublished.h>

using std::make_unique;
using std::string;

namespace Seaplanes {

ObjectInstancePublished::ObjectInstancePublished(Name name, SpObject sp_object)
    : ObjectInstance(move(name), move(sp_object)) {}

UpObjectInstancePublished ObjectInstancePublished::create(Name name,
                                                          SpObject sp_object) {
  return make_unique<ObjectInstancePublished>(move(name), sp_object);
}

void ObjectInstancePublished::publishObjectClass(RTI::RTIambassador *rtiAmb) {
  rtiAmb->publishObjectClass(__sp_object_->getHandle(),
                             *__up_instance_attributes_);
}

void ObjectInstancePublished::registering(RTI::RTIambassador *rtiAmb) {
  __handle_ = rtiAmb->registerObjectInstance(__sp_object_->getHandle(),
                                             __name_.c_str());
  __up_instance_object_.reset(
      RTI::AttributeSetFactory::create(__sp_attributes_.size()));
}

void ObjectInstancePublished::updateAttributeValues(
    RTI::RTIambassador *rtiAmb,
    libhla::MessageBuffer *certiMessagebuffer __attribute__((unused)),
    const RTIfedTime &time, const string &tag) {
  __up_instance_object_->empty();

  for (auto &sp_attribute : __sp_attributes_) {
    auto value = sp_attribute->getValue<double>();
#if USE_CERTI_MESSAGE_BUFFER
    certiMessagebuffer->reset();
    certiMessagebuffer->write_double(value);
    certiMessagebuffer->updateReservedBytes();
    instance_object_->add(attribute->getHandle(),
                          static_cast<char *>((*certiMessagebuffer)(0)),
                          certiMessagebuffer->size());
#else  // USE_CERTI_MESSAGE_BUFFER
    // dependant. (For instance 64bits LE).
    auto *const messageBuffer = reinterpret_cast<char *>(&value);
    __up_instance_object_->add(sp_attribute->getHandle(), messageBuffer,
                               sizeof(double));
#endif // USE_CERTI_MESSAGE_BUFFER
  }

  rtiAmb->updateAttributeValues(getHandle(), *__up_instance_object_, time,
                                tag.c_str());
}

void ObjectInstancePublished::updateAttributeValues(
    RTI::RTIambassador *rtiAmb, libhla::MessageBuffer *certiMessagebuffer,
    const RTIfedTime &time) {
  updateAttributeValues(rtiAmb, certiMessagebuffer, time, __name_);
}

void ObjectInstancePublished::unpublish(RTI::RTIambassador *rtiAmb) {
  rtiAmb->unpublishObjectClass(__sp_object_->getHandle());
}

} // namespace Seaplanes
