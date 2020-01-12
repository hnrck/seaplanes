//! \file    SeaplanesProtoLogicalProcessor.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July 2016
//! \brief   Common interface to federates implementation.

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>

#include <ProtoLogicalProcessor.h>
#include <TimeManagementPolicyTimeStep.h>

#ifndef FEDERATION_SYNC_POINT_NAME
#define "syncPoint"
#endif // FEDERATION_SYNC_POINT_NAME

using std::clog;
using std::move;
using std::ofstream;
using std::ostream;
using std::string;
using std::stringstream;
using std::to_string;

namespace Seaplanes {

ProtoLogicalProcessor::ProtoLogicalProcessor(
    Name federation_name, Name federate_name, Name federation_file,
    double time_limit, double timestep, double lookahead, ostream *p_log_stream)
    : __synchro_point_name_(FEDERATION_SYNC_POINT_NAME),
      __federation_name_(move(federation_name)),
      __federate_name_(move(federate_name)),
      __federation_file_(move(federation_file)),
      __rti_amb_(RTI::RTIambassador()), __certi_message_buffer_(0),
      __ask_time_regulator_(true), __ask_time_constrained_(true),
      __is_time_regulator_(false), __is_time_constrained_(false),
      __has_time_advance_grant_(false), __local_time_(0_s),
      __time_step_(timestep), __lookahead_(lookahead),
      __time_limit_(time_limit), __is_creator_(false), __step_number_(0),
      __sync_reg_success_(false), __sync_reg_failed_(false), __in_pause_(false),
      __up_time_management_policy_(
          TimeManagementPolicyFactory::create<TimeManagementPolicyTimeStep>(
              *this)),
      __logger_(Logger::get_instance(p_log_stream)),
      __uav_index_(0), __up_rav_tags_(VecUpTag()),
      __sp_object_classes_(VecSpObject()),
      __sp_subscribed_objects_(VecSpObjectInstanceSubscribed()),
      __up_published_objects_(VecUpObjectInstancePublished()),
      __map_sp_subscribed_objects_(MapHandleSpObjectInstanceSubscribed()),
      __timer_simu_start_(0), __timer_simu_end_(0) {}

ProtoLogicalProcessor::ProtoLogicalProcessor(Name federate_name,
                                             double timeLimit, double timeStep,
                                             double lookahead)
    : ProtoLogicalProcessor(FEDERATION_NAME, move(federate_name),
                            FEDERATION_FILE, timeLimit, timeStep, lookahead) {}

ProtoLogicalProcessor::~ProtoLogicalProcessor() noexcept {}

inline double ProtoLogicalProcessor::getSimulationTimeElapsed() const {
  const auto elapsedTime = difftime(__timer_simu_end_, __timer_simu_start_);
  return (elapsedTime);
}

void ProtoLogicalProcessor::bindAttribute(
    UpObjectInstancePublished const &up_instance,
    SpAttribute const &sp_attribute) {
  up_instance->addAttribute(sp_attribute);
}

void ProtoLogicalProcessor::bindAttribute(
    UpObjectInstanceSubscribed const &up_instance,
    SpAttribute const &sp_attribute) {
  up_instance->addAttribute(sp_attribute);
}

void ProtoLogicalProcessor::addObjectClass(SpObject sp_object) {
  __sp_object_classes_.push_back(sp_object);
}

void ProtoLogicalProcessor::addPublishedObject(
    UpObjectInstancePublished up_instance) {
  __up_published_objects_.push_back(move(up_instance));
}

void ProtoLogicalProcessor::addSubscribedObject(
    SpObjectInstanceSubscribed sp_instance) {
  __sp_subscribed_objects_.push_back(move(sp_instance));
}

inline void ProtoLogicalProcessor::creationPhase() {
  __logger_.log(Logger::Level::NOTICE, __func__);
  federationCreation();
  federationJoin();
}

inline void ProtoLogicalProcessor::federationCreation() {
  __logger_ << __func__;
  try {
    __rti_amb_.createFederationExecution(__federation_name_.c_str(),
                                         __federation_file_.c_str());
    __is_creator_ = true;
  } catch (const RTI::FederationExecutionAlreadyExists &) {
    __logger_.log(Logger::Level::NOTICE,
                  "Federation already exists, nothing to do.");
  }
}

inline void ProtoLogicalProcessor::federationJoin() {
  __logger_ << __func__;
  __rti_amb_.joinFederationExecution(__federate_name_.c_str(),
                                     __federation_name_.c_str(), this);
}

void ProtoLogicalProcessor::initializationPhase() {
  __logger_.log(Logger::Level::NOTICE, __func__);
  attributesAndObjectsHandlesFetching();
  declarationOfPublicationAndSubscription();
  initializingTimeManagementPolicy();
  synchronization();
  registeringObjects();
}

inline void ProtoLogicalProcessor::attributesAndObjectsHandlesFetching() {
  __logger_ << __func__;

  __logger_ << "nb objects " + to_string(__sp_object_classes_.size());
  for (auto &sp_object_class : __sp_object_classes_) {
    // Fetching handles.
    __logger_ << "object " + sp_object_class->getName();
    sp_object_class->setHandle(&__rti_amb_);
    __logger_ << "object handle " + to_string(sp_object_class->getHandle());
  }

  for (auto &up_published_object : __up_published_objects_) {
    __logger_ << "object instance published " + up_published_object->getName();
    up_published_object->setAttributesHandles(&__rti_amb_);
    __logger_ << "object instance published handle " +
                     to_string(up_published_object->getHandle());
  }

  for (auto &sp_subscribed_object : __sp_subscribed_objects_) {
    __logger_ << "object instance subscribed " +
                     sp_subscribed_object->getName();
    sp_subscribed_object->setAttributesHandles(&__rti_amb_);
    __logger_ << "object instance subscribed handle " +
                     to_string(sp_subscribed_object->getHandle());
    sp_subscribed_object->initAttributesMap();
  }
}

inline void ProtoLogicalProcessor::declarationOfPublicationAndSubscription() {
  __logger_ << __func__;

  __logger_ << "subscribing objects";
  for (auto &sp_subscribed_object : __sp_subscribed_objects_) {
    __logger_ << "subscribing: " + sp_subscribed_object->getName();
    sp_subscribed_object->subscribeObjectClassAttributes(&__rti_amb_);
  }

  __logger_ << "publishing objects";
  for (auto &up_publishedObject : __up_published_objects_) {
    __logger_ << "publishing: " + up_publishedObject->getName();
    up_publishedObject->publishObjectClass(&__rti_amb_);
  }
}

inline void ProtoLogicalProcessor::initializingTimeManagementPolicy() {
  __logger_ << __func__;
  __up_time_management_policy_->initializing();
}

inline void ProtoLogicalProcessor::synchronization() {
  __logger_ << __func__;
  __logger_.log(Logger::Level::NOTICE,
                "Synchro point name: " + __synchro_point_name_);

  if (__is_creator_) {
    std::cout << "Press enter to register synchro point." << std::endl;
    std::cin.get();

    __logger_ << "Registering synchro point " + __synchro_point_name_;
    __rti_amb_.registerFederationSynchronizationPoint(
        __synchro_point_name_.c_str(), "");

    while (!__sync_reg_success_ && !__sync_reg_failed_) {
      __rti_amb_.tick2();
    }

    if (__sync_reg_failed_) {
      __logger_.log(Logger::Level::ERROR, "Error, synchronization failed.");
    }
    std::cout << "Press enter to start the simulation." << std::endl;
    std::cin.get();

    setInPause();
    __rti_amb_.synchronizationPointAchieved(__synchro_point_name_.c_str());
    while (!__in_pause_) {
      __logger_.log(Logger::Level::NOTICE, "Waiting for next phase.");
      __rti_amb_.tick2();
    }

  } else {
    while (!__in_pause_) {
      __logger_.log(Logger::Level::NOTICE,
                    "Waiting for synchronization point announcement.");
      __rti_amb_.tick2();
    }

    __rti_amb_.synchronizationPointAchieved(__synchro_point_name_.c_str());

    __logger_.log(Logger::Level::NOTICE, "Synchronization point achieved.");

    while (!__in_pause_) {
      __logger_.log(Logger::Level::NOTICE, "Waiting for next phase.");
      __rti_amb_.tick2();
    }
  }

  resetSyncRegSuccess();
  resetSyncRegFailed();
}

inline void ProtoLogicalProcessor::registeringObjects() {
  __logger_ << __func__;

  for (auto &up_published_object : __up_published_objects_) {
    __logger_ << "Registering: " + up_published_object->getName();
    up_published_object->registering(&__rti_amb_);
  }

  for (auto &sp_subscribed_object : __sp_subscribed_objects_) {
    __logger_ << "Waiting for registering of: " +
                     sp_subscribed_object->getName();
    sp_subscribed_object->waitRegistering(&__rti_amb_);
  }

  for (auto &sp_subscribed_object : __sp_subscribed_objects_) {
    __logger_ << sp_subscribed_object->getName() + " discovered ? " +
                     to_string(sp_subscribed_object->getDiscovered());
  }
}

inline void ProtoLogicalProcessor::simulationLoopPhase() {
  __logger_.log(Logger::Level::NOTICE, __func__);

  time(&__timer_simu_start_);

  while (__local_time_ < __time_limit_) {
    ++__step_number_;
    updatesReception();
    logPreLocalsCalculation();
    localsCalculation(); // Specialized by the federate.
    logPostLocalsCalculation();
    // so the method could be pure.
    updatesSending();
    timeAdvance();
  }

  time(&__timer_simu_end_);

  __logger_.log(Logger::Level::NOTICE,
                "Simulation ended in " + to_string(getSimulationTimeElapsed()) +
                    "s.");
}

inline void ProtoLogicalProcessor::updatesReception() {
  __logger_ << __func__;
  // Nothing to do.
}

inline void ProtoLogicalProcessor::updatesSending() {
  __logger_ << __func__;
  stringstream tag;
  const auto timeStamp = __local_time_ + __lookahead_;

  tag << __federate_name_ << "." << __uav_index_;

  __logger_ << "preUAV\t" + tag.str() + "\t( " +
                   to_string(__local_time_.get_s()) + ",\t" +
                   to_string(timeStamp.get_s()) + ")";

  for (auto &up_published_object : __up_published_objects_) {
    // might need a small improvement later.
    up_published_object->updateAttributeValues(
        &__rti_amb_, &__certi_message_buffer_, timeStamp.get_us(), tag.str());
  }

  __uav_index_++;
}

inline void ProtoLogicalProcessor::timeAdvance() {
  __logger_ << __func__;
  __up_time_management_policy_->timeAdvance();
}

inline void ProtoLogicalProcessor::endingPhase() {
  __logger_.log(Logger::Level::NOTICE, __func__);

  deletingRegisteredObjects();
  deactivatingTimeManagementPolicy();
}

inline void ProtoLogicalProcessor::deletingRegisteredObjects() {
  __logger_ << __func__;

  for (auto &sp_subscribed_object : __sp_subscribed_objects_) {
    sp_subscribed_object->unsubscribe(&__rti_amb_);
  }

  for (auto &up_published_object : __up_published_objects_) {
    up_published_object->publishObjectClass(&__rti_amb_);
    up_published_object->unpublish(&__rti_amb_);
  }
}

inline void ProtoLogicalProcessor::deactivatingTimeManagementPolicy() {
  __logger_ << __func__;
  __up_time_management_policy_->deactivating();
}

inline void ProtoLogicalProcessor::deletingPhase() {
  __logger_.log(Logger::Level::NOTICE, __func__);
  federationLeaving();
  federationDestruction();
}

inline void ProtoLogicalProcessor::federationLeaving() {
  __logger_ << __func__;
  __rti_amb_.resignFederationExecution(
      RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
}

inline void ProtoLogicalProcessor::federationDestruction() {
  __logger_ << __func__;
  if (__is_creator_) {
    bool federatesCurrentlyJoined = true;
    while (federatesCurrentlyJoined) {
      try {
        __rti_amb_.destroyFederationExecution(__federation_name_.c_str());
        federatesCurrentlyJoined = false;
      } catch (const RTI::FederatesCurrentlyJoined &) {
        __logger_.log(Logger::Level::NOTICE, "Federates currently joined.");
      }
      sleep(1);
    }
    __logger_.log(Logger::Level::NOTICE, "Federation destruction.");
  } else {
    __logger_.log(Logger::Level::NOTICE, "Out of federation.");
  }
}

inline void ProtoLogicalProcessor::logPreLocalsCalculation() {
  for (const auto &up_rav_tag : __up_rav_tags_) {
    __logger_ << "sRAV\t" + (*up_rav_tag) + "\t(" +
                     to_string(__local_time_.get_s()) + ")";
  }
  __up_rav_tags_.clear();
}

inline void ProtoLogicalProcessor::logPostLocalsCalculation() {
  __logger_ << "sUAV\t" + __federate_name_ + "." + to_string(__uav_index_) +
                   "\t(" + to_string(__local_time_.get_s()) + ")";
}

void ProtoLogicalProcessor::printProgression() const {
  static const auto width = 70U;
  const auto currProgression = static_cast<unsigned int>(
      (width * __local_time_.get_s()) / __time_limit_.get_s());

  std::cout << "[";
  for (auto it = 0U; it < width; ++it) {
    if (it < currProgression) {
      std::cout << "=";
    } else if (it == currProgression) {
      std::cout << ">";
    } else {
      std::cout << " ";
    }
  }
  std::cout << "] " << (currProgression / width) * 100 << " %\r";
  std::cout.flush();
}

void ProtoLogicalProcessor::timeRegulationEnabled(
    const RTI::FedTime & /* time */) noexcept {
  __logger_ << __func__;
  __is_time_regulator_ = true;
}

void ProtoLogicalProcessor::timeConstrainedEnabled(
    const RTI::FedTime & /* time */) noexcept {
  __logger_ << __func__;
  __is_time_constrained_ = true;
}

void ProtoLogicalProcessor::timeAdvanceGrant(
    const RTI::FedTime &time) noexcept {
  __logger_ << __func__;
  const auto fedtime = static_cast<RTIfedTime>(time);
  __local_time_.set_us(static_cast<unsigned long long int>(fedtime.getTime()));
  __logger_ << "TAG " + to_string(__local_time_.get_s());
  __has_time_advance_grant_ = true;
}

void ProtoLogicalProcessor::synchronizationPointRegistrationSucceeded(
    const char /* label */[]) noexcept {
  __logger_ << __func__;
  __sync_reg_success_ = true;
}

void ProtoLogicalProcessor::synchronizationPointRegistrationFailed(
    const char /* label */[]) noexcept {
  __logger_ << __func__;
  __sync_reg_failed_ = true;
}

void ProtoLogicalProcessor::announceSynchronizationPoint(
    const char /* label */[], const char /* tag */[]) noexcept {
  __logger_ << __func__;
  __in_pause_ = true;
}

void ProtoLogicalProcessor::federationSynchronized(
    const char /* label */[]) noexcept {
  __logger_ << __func__;
  __in_pause_ = false;
}

void ProtoLogicalProcessor::run() {
  __logger_.log(Logger::Level::NOTICE, __func__);

  try {
    creationPhase();
    initializationPhase();
    simulationLoopPhase();
    endingPhase();
    deletingPhase();
  } catch (const RTI::Exception &e) {
    __logger_.log(Logger::Level::ERROR, "RTI exception " + string(e._name) +
                                            "( " + string(e._reason) + " ).");
  } catch (...) {
    __logger_.log(Logger::Level::ERROR, "Unknown");
  }
}

void ProtoLogicalProcessor::setAskTimeRegulator() {
  __ask_time_regulator_ = true;
}

void ProtoLogicalProcessor::setDoesNotAskTimeRegulator() {
  __ask_time_regulator_ = false;
}

SeaplanesTime ProtoLogicalProcessor::getLocalTime() const {
  return (__local_time_);
}

SeaplanesTime ProtoLogicalProcessor::getTimeStep() const {
  return (__time_step_);
}

bool ProtoLogicalProcessor::getAskTimeRegulator() const {
  return (__ask_time_regulator_);
}

void ProtoLogicalProcessor::setAskTimeConstrained() {
  __ask_time_constrained_ = true;
}

void ProtoLogicalProcessor::setDoesNotAskTimeConstrained() {
  __ask_time_constrained_ = false;
}

bool ProtoLogicalProcessor::getAskTimeConstrained() const {
  return (__ask_time_constrained_);
}

void ProtoLogicalProcessor::setIsTimeRegulator() {
  __is_time_regulator_ = true;
}

void ProtoLogicalProcessor::setIsNotTimeRegulator() {
  __is_time_regulator_ = false;
}

bool ProtoLogicalProcessor::getIsTimeRegulator() const {
  return (__is_time_regulator_);
}

void ProtoLogicalProcessor::setIsTimeConstrained() {
  __is_time_constrained_ = true;
}

void ProtoLogicalProcessor::setIsNotTimeConstrained() {
  __is_time_constrained_ = false;
}

bool ProtoLogicalProcessor::getIsTimeConstrained() const {
  return (__is_time_constrained_);
}

void ProtoLogicalProcessor::setTimeAdvanceGrant() {
  __has_time_advance_grant_ = true;
}

void ProtoLogicalProcessor::setTimeAdvanceNotGrant() {
  __has_time_advance_grant_ = false;
}

bool ProtoLogicalProcessor::getTimeAdvanceGrant() const {
  return (__has_time_advance_grant_);
}

void ProtoLogicalProcessor::setSyncRegSuccess() { __sync_reg_success_ = true; }

void ProtoLogicalProcessor::resetSyncRegSuccess() {
  __sync_reg_success_ = false;
}

bool ProtoLogicalProcessor::getSyncRegSuccess() const {
  return (__sync_reg_success_);
}

void ProtoLogicalProcessor::setSyncRegFailed() { __sync_reg_failed_ = true; }

void ProtoLogicalProcessor::resetSyncRegFailed() { __sync_reg_failed_ = false; }

bool ProtoLogicalProcessor::getSyncRegFailed() const {
  return (__sync_reg_failed_);
}

void ProtoLogicalProcessor::setInPause() { __in_pause_ = true; }

void ProtoLogicalProcessor::setNotInPause() { __in_pause_ = false; }

bool ProtoLogicalProcessor::getInPause() const { return (__in_pause_); }

void ProtoLogicalProcessor::setIsCreator() { __is_creator_ = true; }

bool ProtoLogicalProcessor::getIsCreator() const { return (__is_creator_); }

unsigned long long ProtoLogicalProcessor::getStepNumber() const {
  return __step_number_;
}

void ProtoLogicalProcessor::enableTimeRegulation() {
  __rti_amb_.enableTimeRegulation(
      static_cast<RTIfedTime>(__local_time_.get_us()),
      static_cast<RTIfedTime>(__lookahead_.get_us()));

  while (!__is_time_regulator_) {
    __rti_amb_.tick2();
  }
}

void ProtoLogicalProcessor::enableTimeConstrained() {
  __rti_amb_.enableTimeConstrained();

  while (!__is_time_constrained_) {
    __rti_amb_.tick2();
  }
}

void ProtoLogicalProcessor::enableAsynchronousDelivery() {
  __rti_amb_.enableAsynchronousDelivery();
}

void ProtoLogicalProcessor::disableTimeRegulation() {
  __rti_amb_.disableTimeRegulation();
  __is_time_regulator_ = false;
}
void ProtoLogicalProcessor::disableTimeConstrained() {
  __rti_amb_.disableTimeConstrained();
  __is_time_constrained_ = false;
}

void ProtoLogicalProcessor::disableAsynchronousDelivery() {
  __rti_amb_.disableAsynchronousDelivery();
}

void ProtoLogicalProcessor::timeAdvanceRequest(const SeaplanesTime dt) {
  const auto requested_time = __local_time_ + dt;
  const auto tar = static_cast<RTIfedTime>(requested_time.get_us());

  __logger_ << "TAR " + to_string(requested_time.get_s());

  __rti_amb_.timeAdvanceRequest(tar);

  while (!__has_time_advance_grant_) {
    __rti_amb_.tick2();
    sched_yield();
  }

  __has_time_advance_grant_ = false;
}

void ProtoLogicalProcessor::discoverObjectInstance(
    RTI::ObjectHandle object_handle, RTI::ObjectClassHandle object_class_handle,
    const char name[]) noexcept {
  __logger_ << __func__;
  __logger_ << "Discovering object " + string(name);

  for (auto &sp_subscribed_object : __sp_subscribed_objects_) {
    sp_subscribed_object->tryToDiscover(name, object_class_handle,
                                        object_handle);
    __map_sp_subscribed_objects_[sp_subscribed_object->getHandle()] =
        sp_subscribed_object;
  }
}

void ProtoLogicalProcessor::reflectAttributeValues(
    const RTI::ObjectHandle handle,
    const RTI::AttributeHandleValuePairSet &values,
    const char /* tag */[]) noexcept {
  try {
    auto sp_subscribed_object = __map_sp_subscribed_objects_.at(handle);
    sp_subscribed_object->reflectAttributeValues(values);
  } catch (const std::out_of_range &) {
    // Due to reception of attribute with same name, but for different
    // instances, we need to check if the given attribute received is the one
    // awaited. If not, it won't be in the map. Nothing to do, but if you want
    // to harden the code, please help yourself.
    __logger_.log(Logger::Level::ERROR, "Out of range");
  }
}

void ProtoLogicalProcessor::reflectAttributeValues(
    const RTI::ObjectHandle handle,
    const RTI::AttributeHandleValuePairSet &values, const RTI::FedTime &time,
    const char tag[],
    const RTI::EventRetractionHandle /* event_reaction_handle */) noexcept {
  auto fedtime = static_cast<RTIfedTime>(time);

  __up_rav_tags_.emplace_back(new Tag(tag));

  reflectAttributeValues(handle, values, tag);

  __logger_ << "postRAV\t" + Tag(tag) + "\t(" +
                   to_string(__local_time_.get_s()) + ",\t" +
                   to_string(fedtime.getTime() * SeaplanesTime::us2s) + ")";
}

} // namespace Seaplanes
