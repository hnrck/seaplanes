/**
 * @file    TimeManagementPolicyTimeStep.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Timestep time management specialization.
 */

#ifndef TIMEMANAGEMENTPOLICYTIMESTEP_H
#define TIMEMANAGEMENTPOLICYTIMESTEP_H

#include <ITimeManagementPolicy.h>

namespace Seaplanes {

/**
 * @brief Type for TimeManagementPolicyTimeStep unique pointer.
 */
using UpTimeManagementPolicyTimeStep =
    std::unique_ptr<class TimeManagementPolicyTimeStep>;

/**
 * @brief Timestep time management policy.
 */
class TimeManagementPolicyTimeStep final : public ITimeManagementPolicy {
public:
  ~TimeManagementPolicyTimeStep() override = default;

  /**
   * @brief Initializing of time management policy.
   */
  void initializing() final;

  /**
   * @brief time advance of time management policy.
   */
  void timeAdvance() final;

  /**
   * @brief Deactivating of time management policy.
   */
  void deactivating() final;

private:
  friend ITimeManagementPolicy;
  friend UpTimeManagementPolicyTimeStep
  std::make_unique<TimeManagementPolicyTimeStep>(
      Seaplanes::ProtoLogicalProcessor &);

  /**
   * @brief TimeManagementPolicyTimeStep constructor, protected so the
   * timeManagementPolicies can use it while federates use the factory builder.
   *
   * @param lp Reference to the LP concerned by the time management
   * policy.
   */
  explicit TimeManagementPolicyTimeStep(ProtoLogicalProcessor & /* lp */);
};

} // namespace Seaplanes

#endif // TIMEMANAGEMENTPOLICYTIMESTEP_H
