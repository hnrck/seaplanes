//! \file    TimeManagementPolicyTimeStep.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Timestep time management specialization.

#ifndef TIMEMANAGEMENTPOLICYTIMESTEP_H
#define TIMEMANAGEMENTPOLICYTIMESTEP_H

#include <ITimeManagementPolicy.h>
#include <TimeManagementPolicy.h>

namespace Seaplanes {

//! \brief Type for TimeManagementPolicyTimeStep unique pointer.
using UpTimeManagementPolicyTimeStep =
    std::unique_ptr<class TimeManagementPolicyTimeStep>;

//! \brief Timestep time management policy.
class TimeManagementPolicyTimeStep final : public ITimeManagementPolicy {
public:
  ~TimeManagementPolicyTimeStep() override = default;

  //! \brief Initializing of time management policy.
  auto initializing() -> void final;

  //! \brief time advance of time management policy.
  auto timeAdvance() -> void final;

  //! \brief Deactivating of time management policy.
  auto deactivating() -> void final;

  //! \brief Set next time for time advancement.
  auto setDt(SeaplanesTime /* dt */) -> void final;

  //! \brief Access logical processor.
  auto getLP() -> ProtoLogicalProcessor & final;

private:
  friend ITimeManagementPolicy;
  friend UpTimeManagementPolicyTimeStep
  std::make_unique<TimeManagementPolicyTimeStep>(
      Seaplanes::ProtoLogicalProcessor &);

  UpTimeManagementPolicy
      __up_time_management_policy_; //!< base time management policy.

  //! \brief TimeManagementPolicyTimeStep constructor, protected so the
  //! timeManagementPolicies can use it while federates use the factory builder.
  //! \param lp Reference to the LP concerned by the time management policy.
  explicit TimeManagementPolicyTimeStep(ProtoLogicalProcessor & /* lp */);
};

} // namespace Seaplanes

#endif // TIMEMANAGEMENTPOLICYTIMESTEP_H
