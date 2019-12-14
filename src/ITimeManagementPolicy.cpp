//! \file    ITimeManagementPolicy.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Time management interface implementation.

#include <memory>

#include <ITimeManagementPolicy.h>
#include <TimeManagementPolicyTimeStep.h>

namespace Seaplanes {

ITimeManagementPolicy::ITimeManagementPolicy(ProtoLogicalProcessor &lp)
    : __lp_(lp) {}

} // namespace Seaplanes
