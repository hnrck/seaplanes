/**
 * @file    LogicalProcessorObjectInstanceSubscribed.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Logical processor Subscribed object instance manipulation.
 *
 * This class provides a simple way to use subscribed object instance.
 */

#ifndef LOGICALPROCESSOROBJECTINSTANCESUBSCRIBED_H
#define LOGICALPROCESSOROBJECTINSTANCESUBSCRIBED_H

#include <map>
#include <memory>
#include <string>

#include <LogicalProcessorCommon.h>
#include <LogicalProcessorObjectInstance.h>

namespace Seaplanes {

/**
 * @brief Type for object instance pusblished unique pointer.
 */
using UpObjectInstanceSubscribed =
    std::unique_ptr<class ObjectInstanceSubscribed>;

/**
 * @brief Type for Map Handles <-> Attribute shared pointers.
 */
using MapHandleSpAttribute = std::map<RTI::AttributeHandle, SpAttribute>;

class ObjectInstanceSubscribed final : public ObjectInstance {
public:
  /* TODO(h.deschamps) Object instance factory */
  /**
   * @brief Subscribed object instance factory builder.
   * @param name The name of the object.
   * @param sp_object The object which the object instance belongs to.
   * @return The subscribed object instance in unique pointer.
   */
  static UpObjectInstanceSubscribed create(Name /* name */,
                                           SpObject /* sp_object */);

  /**
   * @brief Init the attributes map.
   */
  void initAttributesMap();

  /**
   * @brief Subscribe to the object class attributes.
   * @param p_rtia A pointer to the RTIA.
   */
  void subscribeObjectClassAttributes(RTI::RTIambassador * /* p_rtia */);

  /**
   * @brief Wait for registering.
   * @param p_rtia A pointer to the RTIA.
   */
  void waitRegistering(RTI::RTIambassador * /* p_rtia */);

  /**
   * @brief Unsubscribe to the object class attributes.
   * @param p_rtia A pointer to the RTIA.
   */
  void unsubscribe(RTI::RTIambassador * /* p_rtia */);

  /**
   * @brief Try to discover.
   * @param  &name The name of the object.
   * @param  &object_class_handle The handle of the object class.
   * @param  &object_handle The handle of object instance.
   */
  void tryToDiscover(const Name & /* name */,
                     const RTI::ObjectClassHandle & /* object_class_handle */,
                     const RTI::ObjectHandle & /* object_handle */);

  /**
   * @brief Reflects the attributes values.
   * @param  &values The values to reflect.
   */
  void
  reflectAttributeValues(const RTI::AttributeHandleValuePairSet & /* values */);

private:
  friend UpObjectInstanceSubscribed std::make_unique<ObjectInstanceSubscribed>(
      Name &&, std::shared_ptr<Seaplanes::Object> &);

  MapHandleSpAttribute __map_sp_attributes_; /**< Map on attribute for faster
                                                access when in RAV. */

  /**
   * @brief Subscribed object instance constructor.
   * @param  name The name of the object.
   * @param  sp_object The object which the object instance belongs to.
   * @return The subscribed object instance.
   */
  ObjectInstanceSubscribed(Name /* name */, SpObject /* sp_object */);
};

} // namespace Seaplanes

#endif // LOGICALPROCESSOROBJECTINSTANCESUBSCRIBED_H
