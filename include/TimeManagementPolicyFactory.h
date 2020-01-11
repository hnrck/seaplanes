//! \file    TimeManagementPolicyFactory.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Time management interface for using with the Federates.
//!
//! Time management policy factory allow the generation of a new time management
//! policy, based on the strategy will.

#ifndef TIMEMANAGEMENTPOLICYFACTORY_H
#define TIMEMANAGEMENTPOLICYFACTORY_H

#include <memory>

#include <ITimeManagementPolicy.h>

namespace Seaplanes {

//! \brief Time management policy factory.
class TimeManagementPolicyFactory final {
public:
  TimeManagementPolicyFactory() = delete;
  ~TimeManagementPolicyFactory() = delete;
  TimeManagementPolicyFactory(const TimeManagementPolicyFactory &) = delete;
  void operator=(const TimeManagementPolicyFactory &) = delete;
  TimeManagementPolicyFactory(TimeManagementPolicyFactory &&) = default;
  TimeManagementPolicyFactory &
  operator=(TimeManagementPolicyFactory &&) = default;

  //! \brief TimeManagementPolicy generator.
  //! \template class T The type of time management policy. \see Time management
  //! policies.
  //! \param federate A reference to the federate concerned by the time
  //! management policy.
  //! \return unique pointer to the constructed time management policy.
  template <class T>
  static UpITimeManagementPolicy
  create(class ProtoLogicalProcessor & /*federate*/);
};

} // namespace Seaplanes

#endif // TIMEMANAGEMENTPOLICYFACTORY_HH
