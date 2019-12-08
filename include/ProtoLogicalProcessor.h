/**
 * @file    ProtoLogicalProcessor.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July 2016
 * @brief   Proto-Logical processor that can schedule an ordered set of models.
 */

#ifndef PROTOLOGICALPROCESSOR_H
#define PROTOLOGICALPROCESSOR_H

#include <ctime>
#include <fstream>
#include <map>
#include <vector>

#include <LogicalProcessorCommon.h>
#include <LogicalProcessorLogger.h>
#include <LogicalProcessorObjectClass.h>
#include <LogicalProcessorObjectInstancePublished.h>
#include <LogicalProcessorObjectInstanceSubscribed.h>
#include <SeaplanesTime.h>
#include <TimeManagementPolicyFactory.h>

namespace Seaplanes {

/**
 * @brief Type for tag unique pointer.
 */
using UpTag = std::unique_ptr<Tag>;

/**
 * @brief Type for tag unique pointer vector.
 */
using VecUpTag = std::vector<UpTag>;

/**
 * @brief Type for vector of object shared pointers.
 */
using VecSpObject = std::vector<SpObject>;

/**
 * @brief Type for object instance shared poiner.
 */
using SpObjectInstanceSubscribed = std::shared_ptr<ObjectInstanceSubscribed>;

/**
 * @brief Type for vector of object instance published shared pointers.
 */
using VecSpObjectInstanceSubscribed = std::vector<SpObjectInstanceSubscribed>;

/**
 * @brief Type for map of object handle <-> object instance subscribde shared
 * pointers.
 */
using MapHandleSpObjectInstanceSubscribed =
    std::map<RTI::ObjectHandle, SpObjectInstanceSubscribed>;

/**
 * @brief Type for object instance published unique pointers vector.
 */
using VecUpObjectInstancePublished = std::vector<UpObjectInstancePublished>;

/**
 * @brief Common interface to federates.
 */
class ProtoLogicalProcessor : public NullFederateAmbassador {
public:
  /**
   * @brief Create a new federate.
   * @param  federation_name The name of the federation.
   * @param  federate_name The name of the federate.
   * @param  federate_file The FED file.
   * @param  time_limit The time limit (in seconds).
   * @param  timestep The time step (in seconds).
   * @param  lookahead The lookahead (in seconds).
   * @param  log_filename The federate log filename.
   * @return The new federate.
   */
  ProtoLogicalProcessor(Name /* federation_name */, Name /* federate_name */,
                        Name /* federation_file */, double /* time_limit */,
                        double /* timestep */, double /* lookahead */,
                        Name /* log_filename */ = std::string());

  /**
   * @brief Create a new federate.
   * @param  federate_name The name of the federate.
   * @param  time_limit The time limit (in seconds).
   * @param  timestep The time step (in seconds).
   * @param  lookahead The lookahead (in seconds).
   * @return The new federate.
   */
  ProtoLogicalProcessor(Name /* federate_name */, double /* time_limit */,
                        double /* timestep */, double /* lookahead */);

  /**
   * @brief Federate desctructor.
   */
  ~ProtoLogicalProcessor() noexcept override;
  // TODO(h.deschamps) Refactor and handle exception.
  // ~ProtoLogicalProcessor() noexcept override;

  /**
   * @brief Federate copy constructor (disabled).
   */
  ProtoLogicalProcessor(const ProtoLogicalProcessor &) = delete;

  /**
   * @brief Federate move constructor (disabled).
   */
  ProtoLogicalProcessor(const ProtoLogicalProcessor &&) = delete;

  /**
   * @brief Federate copy assignement (disabled).
   */
  void operator=(const ProtoLogicalProcessor &) = delete;

  /**
   * @brief Federate move assignement (disabled).
   */
  void operator=(ProtoLogicalProcessor &&) = delete;

  /**
   * @brief Set a name for the log file.
   * @param name  The log file name.
   */
  void setLogFilename(Name /* name */);

  /**
   * @brief Set a name for the prod file.
   * @param name  The prod file name.
   */
  void setProdFilename(Name /* name */);

  /**
   * @brief Set a name for the conso file.
   * @param name  The conso file name.
   */
  void setConsoFilename(Name /* name */);

  /**
   * @brief Get local time.
   * @return
   */
  SeaplanesTime getLocalTime() const;

  /**
   * @brief Get timestep.
   * @return
   */
  SeaplanesTime getTimeStep() const;

  /**
   * @brief Get if asking time regulator or not.
   * @return True if asking for time regulator.
   */
  bool getAskTimeRegulator() const;

  /**
   * @brief Get if ask constrained or not.
   * @return True if asking for time constrained.
   */
  bool getAskTimeConstrained() const;

  /**
   * @brief Enable time regulation.
   */
  void enableTimeRegulation();

  /**
   * @brief Enable time constrained.
   */
  void enableTimeConstrained();

  /**
   * @brief Enable asynchronous delivery.
   */
  void enableAsynchronousDelivery();

  /**
   * @brief Disable time regulation.
   */
  void disableTimeRegulation();

  /**
   * @brief Disable time constriction.
   */
  void disableTimeConstrained();

  /**
   * @brief Disable asynchronous delivery.
   */
  void disableAsynchronousDelivery();

  /**
   * @brief TAR
   * @param  dt the delta of time for next step.
   */
  void timeAdvanceRequest(SeaplanesTime /* dt */);

  /**
   * @brief Discover object instance.
   * @param object_handle The object handle.
   * @param object_class_handle The class of the object.
   * @param name The object name.
   */
  void discoverObjectInstance(RTI::ObjectHandle /* object_handle */,
                              RTI::ObjectClassHandle /* object_class_handle */,
                              const char /* name */[]) noexcept final;

  /**
   * @brief RAV.
   * @param handle The object handle.
   * @param values The attributes containing the new value.
   * @param tag A TAG.
   */
  void
  reflectAttributeValues(RTI::ObjectHandle /* handle */,
                         const RTI::AttributeHandleValuePairSet & /* values */,
                         const char /* tag */[]) noexcept final;

  /**
   * @brief RAV
   * @param object_handle The object handle.
   * @param values The attributes containing the new value.
   * @param time Fedtime.
   * @param tag A TAG.
   * @param event_retraction_handle An handle for event reaction.
   */
  void reflectAttributeValues(
      RTI::ObjectHandle /* object_handle */,
      const RTI::AttributeHandleValuePairSet & /* values */,
      const RTI::FedTime & /* time */, const char /* tag */[],
      RTI::EventRetractionHandle /* event_retraction_handle */) noexcept final;

  /**
   * @brief Time regulation callback.
   * @param time The time.
   */
  void timeRegulationEnabled(const RTI::FedTime & /* time */) noexcept final;

  /**
   * @brief Time constriction callback.
   * @param time The time.
   */
  void timeConstrainedEnabled(const RTI::FedTime & /* time */) noexcept final;

  /**
   * @brief TAG callback.
   * @param time The time.
   */
  void timeAdvanceGrant(const RTI::FedTime & /* time */) noexcept final;

  /**
   * @brief Synchronization point callback.
   * @param label The synchronization point name.
   */
  void synchronizationPointRegistrationSucceeded(
      const char /* label */[]) noexcept final;

  /**
   * @brief Synchronization point failed callback.
   * @param  label The synchronization point name.
   */
  void synchronizationPointRegistrationFailed(
      const char /* label */[]) noexcept final;

  /**
   * @brief Synchronization point announcement.
   * @param  label The synchronization point name.
   * @param  tag A tag
   */
  void announceSynchronizationPoint(const char /* label */[],
                                    const char /* tag */[]) noexcept final;

  /**
   * @brief Federation is synchronized callback.
   * @param  label The synchronization point name.
   */
  void federationSynchronized(const char /* label */[]) noexcept final;

  /**
   * @brief Run
   */
  void run();

protected:
  /**
   * @brief Compute the time elapse during simulation.
   * @return The time elapsed in seconds.
   */
  double getSimulationTimeElapsed() const;

  /**
   * @brief Bind an attribute to a published object instance.
   * @param up_instance An unique pointer to the object instance.
   * @param sp_attribute A shared pointer to the attribute.
   */
  void bindAttribute(UpObjectInstancePublished const & /* up_instance */,
                     SpAttribute const & /* sp_attribute */);

  /**
   * @brief Bind an attribute to a subscribed object instance.
   * @param  up_instance An unique pointer to the object instance.
   * @param  sp_attribute A shared pointer to the attribute.
   */
  void bindAttribute(UpObjectInstanceSubscribed const & /* up_instance */,
                     SpAttribute const & /* sp_attribute */);

  /**
   * @brief A an object class to the LP object class collection.
   * @param  sp_object The object instance.
   */
  void addObjectClass(SpObject /* sp_object */);

  /**
   * @brief Add an object instance to publish.
   * @param up_instance The object instance.
   */
  void addPublishedObject(UpObjectInstancePublished /* up_instance */);

  /**
   * @brief Add an object instance to subscribe.
   * @param sp_instance The object instance.
   */
  void addSubscribedObject(SpObjectInstanceSubscribed /* sp_instance */);

  /**
   * @brief The creation phase function.
   */
  void creationPhase();

  /**
   * @brief The Federation creation function. @see
   * Seaplanes::ProtoLogicalProcessor::creationPhase.
   */
  void federationCreation();

  /**
   * @brief Join the federation. @see
   * Seaplanes::ProtoLogicalProcessor::creationPhase.
   */
  void federationJoin();

  /**
   * @brief Initialize the federation.
   */
  virtual void initializationPhase();

  /**
   * @brief Initialize the log and dump values files
   */
  void initFiles();

  /**
   * @brief  Fetch attributes and object handles. @see
   * Seaplanes::ProtoLogicalProcessor::initializationPhase
   */
  void attributesAndObjectsHandlesFetching();

  /**
   * @brief Declare publication and subscription. @see
   * Seaplanes::ProtoLogicalProcessor::initializationPhase
   */
  void declarationOfPublicationAndSubscription();

  /**
   * @brief Initialize time managemeent policy. @see
   * Seaplanes::ProtoLogicalProcessor::initializationPhase
   */
  void initializingTimeManagementPolicy();

  /**
   * @brief Synchronization. @see
   * Seaplanes::ProtoLogicalProcessor::initializationPhase
   */
  void synchronization();

  /**
   * @brief Register objects. @see
   * Seaplanes::ProtoLogicalProcessor::initializationPhase
   */
  void registeringObjects();

  /**
   * @brief Simulation loop.
   */
  void simulationLoopPhase();

  /**
   * @brief Time advance, depending of the policy. @see
   * Seaplanes::ProtoLogicalProcessor::simulationLoopPhase
   */
  void timeAdvance();

  /**
   * @brief Update recepted values. @see
   * Seaplanes::ProtoLogicalProcessor::simulationLoopPhase
   */
  void updatesReception();

  /**
   * @brief Local core calculation. @see
   * Seaplanes::ProtoLogicalProcessor::simulationLoopPhase
   */
  virtual void localsCalculation() = 0;

  /**
   * @brief Update the values and send them. @see
   * Seaplanes::ProtoLogicalProcessor::simulationLoopPhase
   */
  void updatesSending();

  /**
   * @brief Ending.
   */
  void endingPhase();

  /**
   * @brief Deleting registration. @see
   * Seaplanes::ProtoLogicalProcessor::endingPhase
   */
  void deletingRegisteredObjects();

  /**
   * @brief Deactivation time management. @see
   * Seaplanes::ProtoLogicalProcessor::endingPhase
   */
  void deactivatingTimeManagementPolicy();

  /**
   * @brief Delete the federate (and eventually the federation).
   */
  void deletingPhase();

  /**
   * @brief Leave the federation. @see
   * Seaplanes::ProtoLogicalProcessor::deletingPhase
   */
  void federationLeaving();

  /**
   * @brief Desctroy the federation (if creator). @see
   * Seaplanes::ProtoLogicalProcessor::deletingPhase
   */
  void federationDestruction();

  /**
   * @brief Close the log files. @see
   * Seaplanes::ProtoLogicalProcessor::deletingPhase
   */
  void closeLogs();

  /**
   * @brief Initialize the log files.
   */
  void initLogs();

  /**
   * @brief Initialize the dump files.
   */
  virtual void initDumpFiles();

  /**
   * @brief Log activity before local calculation.
   */
  void logPreLocalsCalculation();

  /**
   * @brief Log activity after local calculation.
   */
  void logPostLocalsCalculation();

  /**
   * @brief Dump values in files.
   */
  virtual void dumpValuesInFiles();

  /**
   * @brief Print a progression bar.
   */
  void printProgression() const;

  /**
   * @brief Set to true the asking of time regulation.
   */
  void setAskTimeRegulator();

  /**
   * @brief Set not to ask time regulator.
   */
  void setDoesNotAskTimeRegulator();

  /**
   * @brief Set to true the asking of time constriction.
   */
  void setAskTimeConstrained();

  /**
   * @brief Set not to ask time constriction.
   */
  void setDoesNotAskTimeConstrained();

  /**
   * @brief Set is time regulator.
   */
  void setIsTimeRegulator();

  /**
   * @brief Set is not time regulator.
   */
  void setIsNotTimeRegulator();

  /**
   * @brief Get if time regulator or not.
   * @return True if time regulator, else false.
   */
  bool getIsTimeRegulator() const;

  /**
   * @brief Set is time constrained.
   */
  void setIsTimeConstrained();

  /**
   * @brief Set is not time constrained.
   */
  void setIsNotTimeConstrained();

  /**
   * @brief Get if federate is time constrained or not.
   * @return True if time constrained, else false.
   */
  bool getIsTimeConstrained() const;

  /**
   * @brief Set TAG.
   */
  void setTimeAdvanceGrant();

  /**
   * @brief Set not TAG.
   */
  void setTimeAdvanceNotGrant();

  /**
   * @brief Get TAG.
   * @return True if TAG, else false.
   */
  bool getTimeAdvanceGrant() const;

  /**
   * @brief Set synchro success.
   */
  void setSyncRegSuccess();

  /**
   * @brief Reset synchro success.
   */
  void resetSyncRegSuccess();

  /**
   * @brief Get if synchro is a success or not.
   * @return True if success, false, if unknown.
   */
  bool getSyncRegSuccess() const;

  /**
   * @brief Set synchro failed.
   */
  void setSyncRegFailed();

  /**
   * @brief Reset synchro failed.
   */
  void resetSyncRegFailed();

  /**
   * @brief Get if synchro failed or not.
   * @return True if synchro failed, false if unknown.
   */
  bool getSyncRegFailed() const;

  /**
   * @brief Set in pause.
   */
  void setInPause();

  /**
   * @brief Set not in pause.
   */
  void setNotInPause();

  /**
   * @brief Get if in pause or not.
   * @return True if in pause, else false.
   */
  bool getInPause() const;

  /**
   * @brief Set if creator.
   */
  void setIsCreator();

  /**
   * @brief Get if creator or not.
   * @return True if creator, else false.
   */
  bool getIsCreator() const;

  /**
   * @brief Write in conso file.
   * @param  t something to write.
   */
  template <typename T> void consoFileWrite(T /* t */);

  /**
   * @brief Write in conso file.
   * @param  t Something to write.
   * @param  args Other things to write.
   */
  template <typename T, typename... Args>
  void consoFileWrite(T /* t */, Args &&... /* args */);

  /**
   * @brief Write a new line in conso file.
   */
  void consoFileNewLine();

  /**
   * @brief Write in prod file.
   * @param  t Something to write.
   */
  template <typename T> void prodFileWrite(T /* t */);

  /**
   * @brief Write in prod file.
   * @param  t Something to write.
   * @param  args Other things to write.
   */
  template <typename T, typename... Args>
  void prodFileWrite(T /* t */, Args &&... /* args */);

  /**
   * @brief Write a new line in prod file.
   */
  void prodFileNewLine();

  /**
   * @brief get current step index.
   * @return The step index
   */
  unsigned long long getStepNumber() const;

private:
  // TODO(Henrick) Check for methods to make private.
  Name __synchro_point_name_; /**< The name of the synchronization point. */
  Name __federation_name_;    /**< The name of the federation. */
  Name __federate_name_;      /**< The name of the federate. */
  Name __federation_file_;    /**< The file used for the federation */

  /** The RTI Ambassador. @see RTI::RTIambassador. */
  RTI::RTIambassador __rti_amb_;

  /**< The CERTI message buffer. @see libhla::MessageBuffer. */
  libhla::MessageBuffer __certi_message_buffer_;

  bool __ask_time_regulator_;   /**< True if the Federate ask to be time
                                   regulator, else False. */
  bool __ask_time_constrained_; /**< True if the Federate ask to be time
                                   constrained, else False. */
  bool __is_time_regulator_;   /**< True if the Federate is time regulator, else
                                  False. */
  bool __is_time_constrained_; /**< True if the Federate is time constrained,
                                  else False. */
  bool __has_time_advance_grant_; /**< True if the Federate has time advance
                                     grant, else False. */

  SeaplanesTime __local_time_; /**< Current time of the federate. */
  SeaplanesTime __time_step_;  /**< Duration of a timeStep. */
  SeaplanesTime __lookahead_;  /**< Duration of the lookahead. */
  SeaplanesTime __time_limit_; /**< Time limit. */

  bool __is_creator_; /**< True if the Federate is creator, else False. */

  unsigned long long __step_number_; /**< Current step index. */

  /*
   * Federate synchronization attributes
   */
  bool __sync_reg_success_; /**< True if registration is OK, else false. */
  bool __sync_reg_failed_;  /**< True if registration failed, else false. */
  bool __in_pause_;         /**< True if in pause, else false. */

  /** The strategy for time managemeent policy. */
  UpITimeManagementPolicy __up_time_management_policy_;

  /*
   * Federate LOG, consomate and product values dump. If NULL (default value),
   * does not do * anything. Set to NULL during federate initialization if
   * filename are empty.
   */
  Logger &__logger_;

  /* Todo(h.deschamps) refactor */
  Name __federate_conso_filename_; /**< The name of the consumed values file. */
  std::ofstream __federate_conso_; /**< The consumed values file. */

  Name __federate_prod_filename_; /**< The name of the product values file. */
  std::ofstream __federate_prod_; /**< The product values file. */

  unsigned int __uav_index_; /**< Index of UAV for context. */
  VecUpTag __up_rav_tags_;   /**< Index of RAVs for context. */

  /** Vector of object classes. @see Object. */
  VecSpObject __sp_object_classes_;

  /** Vector of subscribed object instances. @see
   * Seaplanes::ProtoLogicalProcessor::ObjectInstanceSubscribed. */
  VecSpObjectInstanceSubscribed __sp_subscribed_objects_;

  /** Vector of published object instances. @see
   * Seaplanes::ProtoLogicalProcessor::ObjectInstancePublished. */
  VecUpObjectInstancePublished __up_published_objects_;

  /** Map on subscribedObjects for better access to subscribed objects. */
  MapHandleSpObjectInstanceSubscribed __map_sp_subscribed_objects_;

  std::time_t __timer_simu_start_; /**< Timer set when simulation start. */
  std::time_t __timer_simu_end_;   /**< Time set when simulation end.  */
};

// Templates implementation
#include <ProtoLogicalProcessor.tcc>

} // namespace Seaplanes

#endif // PROTOLOGICALPROCESSOR_H
