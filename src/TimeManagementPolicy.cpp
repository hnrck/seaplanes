//! \file    ITimeManagementPolicy.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Time management interface implementation.

#include <memory>

#include <ITimeManagementPolicy.h>
#include <ProtoLogicalProcessor.h>
#include <TimeManagementPolicyTimeStep.h>

namespace Seaplanes {

TimeManagementPolicy::TimeManagementPolicy(ProtoLogicalProcessor &lp)
    : __lp_(lp) {}

void TimeManagementPolicy::initializing() {
  if (getLP().getAskTimeRegulator()) {
    getLP().enableTimeRegulation();
  }
  if (getLP().getAskTimeConstrained()) {
    getLP().enableTimeConstrained();
  }
  getLP().enableAsynchronousDelivery();
}

void TimeManagementPolicy::timeAdvance() { getLP().timeAdvanceRequest(__dt_); }

void TimeManagementPolicy::deactivating() {
  if (getLP().getAskTimeRegulator()) {
    getLP().disableTimeRegulation();
  }
  if (getLP().getAskTimeConstrained()) {
    getLP().disableTimeConstrained();
  }
  getLP().disableAsynchronousDelivery();
}

auto TimeManagementPolicy::setDt(SeaplanesTime dt) -> void { __dt_ = dt; }

auto TimeManagementPolicy::getLP() -> ProtoLogicalProcessor & { return __lp_; }

} // namespace Seaplanes
