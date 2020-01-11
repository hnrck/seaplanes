//! \file    SeaplanesObject.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Object manipulation implementation.

#include <LogicalProcessorObjectClass.h>

namespace Seaplanes {

Object::Object(Name name) : __handle_(0), __name_(move(name)) {}

SpObject Object::create(Name name) {
  return (SpObject(new Object(move(name))));
}

Name Object::getName() const { return (__name_); }

void Object::setHandle(RTI::ObjectClassHandle handle) { __handle_ = handle; }

void Object::setHandle(RTI::RTIambassador *p_rtia) {
  setHandle(p_rtia->getObjectClassHandle(__name_.c_str()));
}

RTI::ObjectClassHandle Object::getHandle() const { return (__handle_); }

} // namespace Seaplanes
