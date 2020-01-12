//! \file    TimeManagementPolicyTimeStep.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Timestep time management specialization implementation.

#include <ProtoLogicalProcessor.h>
#include <TimeManagementPolicyTimeStep.h>

namespace Seaplanes {

TimeManagementPolicyTimeStep::TimeManagementPolicyTimeStep(
    ProtoLogicalProcessor &lp)
    : __up_time_management_policy_(std::make_unique<TimeManagementPolicy>(lp)) {
}

void TimeManagementPolicyTimeStep::initializing() {
  __up_time_management_policy_->initializing();
}

void TimeManagementPolicyTimeStep::timeAdvance() {
  setDt(getLP().getTimeStep());
  __up_time_management_policy_->timeAdvance();
}

void TimeManagementPolicyTimeStep::deactivating() {
  __up_time_management_policy_->deactivating();
}

auto TimeManagementPolicyTimeStep::setDt(SeaplanesTime dt) -> void {
  __up_time_management_policy_->setDt(dt);
}

auto TimeManagementPolicyTimeStep::getLP() -> ProtoLogicalProcessor & {
  return __up_time_management_policy_->getLP();
}

} // namespace Seaplanes
