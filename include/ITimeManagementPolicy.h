/**
 * @file    ITimeManagementPolicy.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Time management interface for using with the Federates.
 *
 * Time management interface allowing easiest time management policy develepment
 * for using with federates. Only one type of time management policy should be
 * used for a federation for now. For complete description of time management
 * policies, see the different files concerned.
 */

#ifndef ITIMEMANAGEMENTPOLICY_H
#define ITIMEMANAGEMENTPOLICY_H

#include <memory>

namespace Seaplanes {

/**
 * @brief Type for ITimeManagementPolicy unique pointer.
 */
using UpITimeManagementPolicy = std::unique_ptr<class ITimeManagementPolicy>;

/**
 * @brief Time management policy interface.
 */
class ITimeManagementPolicy {
public:
  /**
   * @brief ITimeManagementPolicy pure destructor.
   */
  virtual ~ITimeManagementPolicy() = default;

  /**
   * @brief ITimeManagementPolicy copy constructor disabled
   */
  ITimeManagementPolicy(const ITimeManagementPolicy &) = delete;

  /**
   * @brief ITimeManagementPolicy copy assignement disabled
   */
  void operator=(const ITimeManagementPolicy &) = delete;

  /**
   * @brief ITimeManagementPolicy move constructor disabled
   */
  ITimeManagementPolicy(ITimeManagementPolicy &&) = delete;

  /**
   * @brief ITimeManagementPolicy move assignement disabled
   */
  void operator=(ITimeManagementPolicy &&) = delete;

  /**
   * @brief Initializing of time management policy.
   */
  virtual void initializing() = 0;

  /**
   * @brief time advance of time management policy.
   */
  virtual void timeAdvance() = 0;

  /**
   * @brief Deactivating of time management policy.
   */
  virtual void deactivating() = 0;

protected:
  class ProtoLogicalProcessor &__lp_; /**< Reference to the concerned LP. */

  /**
   * @brief ITimeManagementPolicy constructor, protected so the
   * timeManagementPolicies can use it while federates use the AbstractFactory
   * builder.
   * @param lp Reference to the LP concerned by the time management
   * policy.
   */
  explicit ITimeManagementPolicy(class ProtoLogicalProcessor & /* lp */);
};

} // namespace Seaplanes

#endif // ITIMEMANAGEMENTPOLICY_HH
