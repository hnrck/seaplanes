/**
 * @file    TimeManagementPolicyTimeStep.cc
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Timestep time management specialization implementation.
 */

#include <iostream>

#include <ProtoLogicalProcessor.h>
#include <TimeManagementPolicyTimeStep.h>

namespace Seaplanes {

TimeManagementPolicyTimeStep::TimeManagementPolicyTimeStep(
    ProtoLogicalProcessor &lp)
    : ITimeManagementPolicy(lp) {}

void TimeManagementPolicyTimeStep::initializing() {
  if (__lp_.getAskTimeRegulator()) {
    __lp_.enableTimeRegulation();
  }
  if (__lp_.getAskTimeConstrained()) {
    __lp_.enableTimeConstrained();
  }
  __lp_.enableAsynchronousDelivery();
}

void TimeManagementPolicyTimeStep::timeAdvance() {
  const auto dt = __lp_.getTimeStep();
  __lp_.timeAdvanceRequest(dt);
}

void TimeManagementPolicyTimeStep::deactivating() {
  if (__lp_.getAskTimeRegulator()) {
    __lp_.disableTimeRegulation();
  }
  if (__lp_.getAskTimeConstrained()) {
    __lp_.disableTimeConstrained();
  }
  __lp_.disableAsynchronousDelivery();
}

} // namespace Seaplanes
