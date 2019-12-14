//! \file    SeaplanesAttribute.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Attribute manipulation implementation.

#include <LogicalProcessorAttribute.h>

namespace Seaplanes {

Attribute::Attribute(Name name)
    : __name_(move(name)), __handle_(0), __fresh_(false), __value_({0}) {}

SpAttribute Attribute::create(Name name) {
  return SpAttribute(new Attribute(move(name)));
}

Name Attribute::getName() const { return (__name_); }

template <> void Attribute::setValue(int value) {
  __fresh_ = true;
  __value_.i = value;
}

template <> void Attribute::setValue(double value) {
  __fresh_ = true;
  __value_.d = value;
}

template <> void Attribute::setValue(bool value) {
  __fresh_ = true;
  __value_.b = value;
}

void Attribute::setValue(int value) { setValue<int>(value); }

void Attribute::setValue(double value) { setValue<double>(value); }

void Attribute::setValue(bool value) { setValue<bool>(value); }

template <> int Attribute::getValue() const { return (__value_.i); }

template <> double Attribute::getValue() const { return (__value_.d); }

template <> bool Attribute::getValue() const { return (__value_.b); }

template <> int Attribute::getFreshValue() const {
  if (!__fresh_) {
    throw(AttributeNoFreshValue());
  }
  __fresh_ = false;
  return (__value_.i);
}

template <> double Attribute::getFreshValue() const {
  if (!__fresh_) {
    throw(AttributeNoFreshValue());
  }
  __fresh_ = false;
  return (__value_.d);
}

template <> bool Attribute::getFreshValue() const {
  if (!__fresh_) {
    throw(AttributeNoFreshValue());
  }
  __fresh_ = false;
  return (__value_.b);
}

void Attribute::setHandle(RTI::AttributeHandle attributeHandle) {
  __handle_ = attributeHandle;
}

void Attribute::setHandle(RTI::RTIambassador *rtiAmb,
                          const RTI::ObjectClassHandle &objectClassHandle) {
  setHandle(rtiAmb->getAttributeHandle(__name_.c_str(), objectClassHandle));
}

RTI::AttributeHandle Attribute::getHandle() const { return (__handle_); }

} // namespace Seaplanes
