//! \file    TimeManagementPolicy.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Time management interface for using with the Federates.
//!
//! Time management interface allowing easiest time management policy
//! develepment for using with federates. Only one type of time management
//! policy should be used for a federation for now. For complete description of
//! time management policies, see the different files concerned.

#ifndef TIMEMANAGEMENTPOLICY_H
#define TIMEMANAGEMENTPOLICY_H

#include <TimeManagementPolicy.h>

namespace Seaplanes {

using UpTimeManagementPolicy = std::unique_ptr<class TimeManagementPolicy>;

//! \brief Time management policy interface.
class TimeManagementPolicy final : public ITimeManagementPolicy {
public:
  //! \brief TimeManagementPolicy pure destructor.
  ~TimeManagementPolicy() override = default;

  //! \brief TimeManagementPolicy copy constructor disabled
  TimeManagementPolicy(const TimeManagementPolicy &) = delete;

  //! \brief TimeManagementPolicy copy assignement disabled
  auto operator=(const TimeManagementPolicy &) = delete;

  //! \brief TimeManagementPolicy move constructor, default
  TimeManagementPolicy(TimeManagementPolicy &&) = default;

  //! \brief TimeManagementPolicy move assignement disabled
  auto operator=(TimeManagementPolicy &&) = delete;

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
  friend UpTimeManagementPolicy
  std::make_unique<TimeManagementPolicy>(Seaplanes::ProtoLogicalProcessor &);

  class ProtoLogicalProcessor &__lp_; //!< Reference to the concerned LP.
  SeaplanesTime __dt_{0_s};           //!< Reference to the next time.

  //! \brief TimeManagementPolicy constructor, protected so the
  //! timeManagementPolicies can use it while federates use the AbstractFactory
  //! builder.
  //! \param lp Reference to the LP concerned by the time management
  //! policy.
  explicit TimeManagementPolicy(class ProtoLogicalProcessor & /* lp */);
};

} // namespace Seaplanes

#endif // ITIMEMANAGEMENTPOLICY_HH
