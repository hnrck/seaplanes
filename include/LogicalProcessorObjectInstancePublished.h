/**
 * @file    LogicalProcessorObjectInstancePublished.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Logical processor Published object instance manipulation.
 *
 * This class provides a simple way to use published object instance.
 */

#ifndef LOGICALPROCESSOROBJECTINSTANCEPUBLISHED_H
#define LOGICALPROCESSOROBJECTINSTANCEPUBLISHED_H

#include <memory>
#include <string>

#include <LogicalProcessorCommon.h>
#include <LogicalProcessorObjectInstance.h>

namespace Seaplanes {

/**
 * @brief Type for object instance pusblished unique pointer.
 */
using UpObjectInstancePublished =
    std::unique_ptr<class ObjectInstancePublished>;

class ObjectInstancePublished final : public ObjectInstance {
public:
  ~ObjectInstancePublished() override = default;

  /**
   * @brief Published object instance factory builder.
   * @param  name The name of the object.
   * @param  sp_object The object which the object instance belongs to.
   * @return The Published object instance in unique pointer.
   */
  static UpObjectInstancePublished create(Name /* name */,
                                          SpObject /* sp_object */);

  /**
   * @brief Publish the object class attributes.
   * @param p_rtia A pointer to the RTIA.
   */
  void publishObjectClass(RTI::RTIambassador * /* p_rtia */);

  /**
   * @brief Register the published object instance.
   * @param p_rtia A pointer to the RTIA.
   */
  void registering(RTI::RTIambassador * /* p_rtia */);

  /**
   * @brief Update the published object instance attributes values.
   * @param p_rtia A pointer to the RTIA.
   * @param p_message_buffer A pointer to a buffer to send the values.
   * @param time The timestamp.
   * @param tag A tag.
   */
  void updateAttributeValues(RTI::RTIambassador * /* p_rtia */,
                             libhla::MessageBuffer * /* p_message_buffer */,
                             const RTIfedTime & /* time */,
                             const Tag & /* tag */);

  /**
   * @brief Update the published object instance attributes values.
   * @param p_rtia A pointer to the RTIA.
   * @param p_message_buffer A pointer to a buffer to send the values.
   * @param time The timestamp.
   */
  void updateAttributeValues(RTI::RTIambassador * /* p_rtia */,
                             libhla::MessageBuffer * /* p_message_buffer */,
                             const RTIfedTime & /* time */);

  /**
   * @brief Unpublish the published object instance.
   * @param p_rtia A pointer to the RTIA.
   */
  void unpublish(RTI::RTIambassador * /* p_rtia */);

private:
  friend UpObjectInstancePublished std::make_unique<ObjectInstancePublished>(
      Name &&, std::shared_ptr<Seaplanes::Object> &);

  /**
   * @brief Published object instance constructor.
   * @param name The name of the object.
   * @param sp_object The object which the object instance belongs to.
   * @return The published object instance.
   */
  ObjectInstancePublished(Name /* name */, SpObject /* sp_object */);
};

} // namespace Seaplanes

#endif // LOGICALPROCESSOROBJECTINSTANCEPUBLISHED_H
