/**
 * @file    ITimeManagementPolicy.cc
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Time management interface implementation.
 */

#include <memory>

#include <TimeManagementPolicyFactory.h>
#include <TimeManagementPolicyTimeStep.h>

using std::make_unique;
using std::unique_ptr;

namespace Seaplanes {

template <>
UpITimeManagementPolicy
TimeManagementPolicyFactory::create<TimeManagementPolicyTimeStep>(
    class ProtoLogicalProcessor &federate) {
  return make_unique<TimeManagementPolicyTimeStep>(federate);
}

} // namespace Seaplanes
