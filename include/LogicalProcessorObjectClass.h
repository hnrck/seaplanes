/**
 * @file    LogicalProcessorObject.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   SEAPlanes Object manipulation.
 *
 * This class provides a simple way to use object class.
 */

#ifndef LOGICALPROCESSOROBJECT_H
#define LOGICALPROCESSOROBJECT_H

#include <memory>
#include <string>

#include <LogicalProcessorCommon.h>

namespace Seaplanes {

/**
 * @brief Type for object shared pointer.
 */
using SpObject = std::shared_ptr<class Object>;

class Object final {
public:
  /**
   * @brief  Object factory builder.
   * @param  name The name of the object.
   * @return The object in a shared pointer.
   */
  static SpObject create(Name /* name */);

  /**
   * @brief Object name getter.
   * @return The name of the object.
   */
  Name getName() const;

  /**
   * @brief Object handle setter.
   * @param  handle The object class handle
   */
  void setHandle(RTI::ObjectClassHandle /* handle */);

  /**
   * @brief Object handle setter, handle is generated from RTIA.
   * @param  p_rtia A pointer to the RTIA.
   */
  void setHandle(RTI::RTIambassador * /* p_rtia */);

  /**
   * @brief Object handle getter.
   * @return The object handle.
   */
  RTI::ObjectClassHandle getHandle() const;

private:
  RTI::ObjectClassHandle __handle_; /**< Object handle. */
  Name __name_;                     /**< Object name. */

  /**
   * @brief Object constructor.
   * @param  name The name of the object.
   * @return The object.
   */
  explicit Object(Name /* name */);
};

} // namespace Seaplanes

#endif // LOGICALPROCESSOROBJECT_H
