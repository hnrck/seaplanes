//! \file    SeaplanesTime.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    September, 2016
//! \brief   Time manipulation class. Allows modifying and using time without
//! considering its representation.

#ifndef SEAPLANESTIME_H
#define SEAPLANESTIME_H

#include <iostream>
#include <string>

namespace Seaplanes {

//! \brief Time overflow error.
class ErrorTimeOverflow final : public std::exception {
  const char *what() const noexcept final;
};

//! \brief Time precision loss. If casted in double, time will be false.
class ErrorTimePrecisionLoss final : public std::exception {
  const char *what() const noexcept final;
};

//! \brief Time class, allowing manipulating time without explicit
//! multiplications and casts when changing units.
//!
//! List of tolerated literals: _us, _ms, _s.
//! When creating time, if literal is precised, the conversion will be handled.
//! For instance Time(1_us) -> us_ <= 1
//!              Time(0.5_ms) -> us_ <= 50
//!              Time(10_s) -> us_ <= 10000000
class SeaplanesTime final {
public:
  //! Explicit conversion ratio, if needed.

  //! Conversion ratio from microseconds to seconds.
  static constexpr double us2s = 1e-6;

  //! Conversion ratio from milliseconds to seconds.
  static constexpr double ms2s = 1e-3;

  //! Conversion ratio from microseconds to milliseconds.
  static constexpr double us2ms = 1e-3;

  //! Conversion ratio from milliseconds to microseconds.
  static constexpr double ms2us = 1e3;

  //! Conversion ratio from seconds to microseconds.
  static constexpr double s2us = 1e6;

  //! Conversion ratio from seconds to milliseconds.
  static constexpr double s2ms = 1e3;

  //! \brief Time default constructor.
  //! \param  us Time in microseconds.
  //! \return An object Time.
  explicit constexpr SeaplanesTime(unsigned long long /*us*/);

  //! \brief Time alternative constructor.
  //! \param  s Time in seconds.
  //! \return An object Time.
  explicit constexpr SeaplanesTime(double /*s*/);

  //! \brief Time alternative constructor.
  //! \param t_cstr Time in c-string, second by default. nothing, s, ms and us
  //! accepted. i.e. 1s, 10ms, 1...
  //! \return An object Time.
  explicit SeaplanesTime(char * /*t_cstr*/);

  //! \brief Time alternative constructor.
  //! \param t_str Time in string, second by default. nothing, s, ms and us
  //! accepted. i.e. 1s, 10ms, 1...
  //! \return An object Time.
  explicit SeaplanesTime(std::string & /*t_str*/);

  //! \brief Time user-defined literal us.
  //! \param  us Time in microseconds.
  friend constexpr SeaplanesTime operator"" _us(unsigned long long /*us*/);

  //! \brief Time user-defined literal ms.
  //! \param  ms Time in milliseconds.
  friend constexpr SeaplanesTime operator"" _ms(long double /*ms*/);

  //! \brief Time user-defined literal ms.
  //! \param  ms Time in milliseconds.
  friend constexpr SeaplanesTime operator"" _ms(unsigned long long /*ms*/);

  //! \brief Time user-defined literal s.
  //! \param  s Time in seconds.
  friend constexpr SeaplanesTime operator"" _s(long double /*s*/);

  //! \brief Time user-defined literal s.
  //! \param  s Time in seconds.
  friend constexpr SeaplanesTime operator"" _s(unsigned long long /*s*/);

  //! \brief Copy constructor.
  //! \param  t Time to copy.
  SeaplanesTime(const SeaplanesTime & /* t */) = default;

  //! \brief Move constructor.
  //! \param  t Time to move.
  SeaplanesTime(SeaplanesTime && /* t */) noexcept(true) = default;

  ~SeaplanesTime() = default;

  //! \brief Copy assignment operator.
  //! \param  t Time to copy.
  SeaplanesTime &operator=(const SeaplanesTime & /* t */) = default;

  //! \brief Move assignment operator.
  //! \param  t Time to move.
  SeaplanesTime &operator=(SeaplanesTime && /* t */) noexcept(true) = default;

  //! \brief Time setter.
  //! \param  t Time to copy.
  void set(const SeaplanesTime & /*t*/);

  //! \brief Explict time setter.
  //! \param  us Time to set in microseconds.
  void set_us(unsigned long long /*us*/);

  //! \brief Explicit time setter.
  //! \param  ms Time to set in milliseconds.
  void set_ms(double /*ms*/);

  //! \brief Explicit time setter.
  //! \param  ms Time to set in milliseconds.
  void set_s(double /*s*/);

  //! \brief Time getter.
  //! \return  Time in microseconds.
  unsigned long long get() const;

  //! \brief Explicit time getter.
  //! \return  Time in microseconds.
  unsigned long long get_us() const;

  //! \brief Explicit time getter.
  //! \return  Time in milliseconds.
  double get_ms() const;

  //! \brief Explicit time getter.
  //! \return  Time in seconds.
  double get_s() const;

  //! \brief Time adder.
  //! \param  t Time to add.
  void add(const SeaplanesTime & /*t*/) noexcept(false);

  //! \brief Explicit time adder.
  //! \param  t Time to add in microseconds.
  void add_us(unsigned long long /*us*/) noexcept(false);

  //! \brief Explicit time adder.
  //! \param  t Time to add in microseconds.
  void add_ms(long double /*ms*/) noexcept(false);

  //! \brief Explicit time adder.
  //! \param  t Time to add in microseconds.
  void add_s(long double /*s*/) noexcept(false);

  //! \brief += overloading.
  //! \param  t Time to add.
  //! \return Time to assign plus time to add.
  SeaplanesTime &operator+=(const SeaplanesTime & /*t*/) noexcept(false);

  //! \brief division and assignment overloading.
  //! \param  t Time to divide with.
  //! \return The division by the given time.
  //! \warning The division will always be imprecise. Manipulate this method
  //! carefully.
  SeaplanesTime &operator/=(const SeaplanesTime & /*t*/);

  //! \brief + overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return The addition of the two given times.
  friend SeaplanesTime operator+(SeaplanesTime /*lhs*/,
                                 const SeaplanesTime & /*rhs*/) noexcept(false);

  //! \brief division overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return The division of the first given time by the second one.
  //! \warning The division will always be imprecise. Manipulate this method
  //! carefully.
  friend SeaplanesTime operator/(SeaplanesTime /*lhs*/,
                                 const SeaplanesTime & /*rhs*/);

  //! \brief < overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return True if first time is strictly inferior to the second one, else
  //! false.
  friend bool operator<(const SeaplanesTime & /*lhs*/,
                        const SeaplanesTime & /*rhs*/);

  //! \brief > overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return True if first time is strictly superrior to the second one, else
  //! false.
  friend bool operator>(const SeaplanesTime & /*lhs*/,
                        const SeaplanesTime & /*rhs*/);

  //! \brief <= overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return True if first time is inferior or equal to the second one, else
  //! false.
  friend bool operator<=(const SeaplanesTime & /*lhs*/,
                         const SeaplanesTime & /*rhs*/);

  //! \brief >= overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return True if first time is superrior or equal to the second one, else
  //! false.
  friend bool operator>=(const SeaplanesTime & /*lhs*/,
                         const SeaplanesTime & /*rhs*/);

  //! \brief == overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return True if first time is equal to the second one, else false.
  friend bool operator==(const SeaplanesTime & /*lhs*/,
                         const SeaplanesTime & /*rhs*/);

  //! \brief != overloading.
  //! \param  lhs One time.
  //! \param  rhs Another time.
  //! \return True if first time is different to the second one, else false.
  friend bool operator!=(const SeaplanesTime & /*lhs*/,
                         const SeaplanesTime & /*rhs*/);

  //! \brief << overloading.
  //! \param  stream The stream to write in
  //! \param  t      The time to write.
  //! \return the stream
  friend std::ostream &operator<<(std::ostream & /*stream*/,
                                  const SeaplanesTime & /*t*/);

private:
  SeaplanesTime();
  unsigned long long us_; //!< The time in microseconds.
};

#include <TimeLiterals.tcc>

} // namespace Seaplanes

#endif // SEAPLANESTIME_H
