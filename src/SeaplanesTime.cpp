//! \file Time.cc
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date September 2016
//! \brief Time manipulation class implementation.

#include <SeaplanesTime.h>

using std::ostream;
using std::stod;
using std::string;

namespace Seaplanes {

SeaplanesTime::SeaplanesTime(char *t_cstr) : SeaplanesTime() {
  auto t_str = string(t_cstr);
  auto converter = 1ULL;

  if (t_str.find("us") != string::npos) {
    t_str.erase(t_str.find("us"), 2);
  } else if (t_str.find("ms") != string::npos) {
    converter = static_cast<unsigned long long>(ms2us);
    t_str.erase(t_str.find("ms"), 2);
  } else if (t_str.find('s') != string::npos) {
    converter = static_cast<unsigned long long>(s2us);
    t_str.erase(t_str.find('s'), 1);
  } else {
    converter = static_cast<unsigned long long>(s2us);
  }

  if (t_str.find('_') != string::npos) {
    t_str.erase(t_str.find('_'));
  }

  us_ = static_cast<unsigned long long int>(stod(t_str) * converter);
}

SeaplanesTime::SeaplanesTime(string &t_str) : SeaplanesTime() {
  auto converter = 1.;

  if (t_str.find("us") != string::npos) {
    t_str.erase(t_str.find("us"), 2);
  } else if (t_str.find("ms") != string::npos) {
    converter = ms2us;
    t_str.erase(t_str.find("ms"), 2);
  } else if (t_str.find('s') != string::npos) {
    converter = s2us;
    t_str.erase(t_str.find('s'), 1);
  } else {
    converter = s2us;
  }

  if (t_str.find('_') != string::npos) {
    t_str.erase(t_str.find('_'));
  }

  us_ = static_cast<unsigned long long int>(stod(t_str) * converter);
}

void SeaplanesTime::set(const SeaplanesTime &t) { us_ = t.us_; }

void SeaplanesTime::set_us(unsigned long long us) { us_ = us; }

void SeaplanesTime::set_ms(double ms) {
  us_ = static_cast<unsigned long long int>(ms * ms2us);
}

void SeaplanesTime::set_s(double s) {
  us_ = static_cast<unsigned long long int>(s * s2us);
}

unsigned long long SeaplanesTime::get() const { return (get_us()); }

unsigned long long SeaplanesTime::get_us() const { return (us_); }

double SeaplanesTime::get_ms() const { return (us_ * us2ms); }

double SeaplanesTime::get_s() const { return (us_ * us2s); }

void SeaplanesTime::add(const SeaplanesTime &t) {
  const auto tmp_us = us_ + t.us_;

  if (tmp_us < us_) {
    throw(ErrorTimeOverflow());
  }

  if (tmp_us > (1UL << 53U)) {
    throw(ErrorTimePrecisionLoss());
  }

  us_ = tmp_us;
}

void SeaplanesTime::add_us(unsigned long long us) {
  const auto tmp_us = us_ + us;

  if (tmp_us < us_) {
    throw(ErrorTimeOverflow());
  }

  if (tmp_us > (1UL << 53U)) {
    throw(ErrorTimePrecisionLoss());
  }

  us_ = tmp_us;
}

void SeaplanesTime::add_ms(long double ms) {
  add_us(static_cast<unsigned long long>(ms * ms2us));
}

void SeaplanesTime::add_s(long double s) {
  add_us(static_cast<unsigned long long>(s * s2us));
}

SeaplanesTime &SeaplanesTime::operator+=(const SeaplanesTime &t) {
  add_us(t.get());
  return (*this);
}

SeaplanesTime &SeaplanesTime::operator/=(const SeaplanesTime &t) {
  us_ /= t.us_;
  return (*this);
}

SeaplanesTime::SeaplanesTime() : us_(0) {}

SeaplanesTime operator+(SeaplanesTime lhs, const SeaplanesTime &rhs) {
  lhs += rhs;
  return (lhs);
}

SeaplanesTime operator/(SeaplanesTime lhs, const SeaplanesTime &rhs) {
  lhs /= rhs;
  return (lhs);
}

bool operator<(const SeaplanesTime &lhs, const SeaplanesTime &rhs) {
  return (lhs.us_ < rhs.us_);
}

bool operator>(const SeaplanesTime &lhs, const SeaplanesTime &rhs) {
  return (rhs < lhs);
}

bool operator<=(const SeaplanesTime &lhs, const SeaplanesTime &rhs) {
  return (!(lhs > rhs));
}

bool operator>=(const SeaplanesTime &lhs, const SeaplanesTime &rhs) {
  return (!(lhs < rhs));
}

bool operator==(const SeaplanesTime &lhs, const SeaplanesTime &rhs) {
  return (lhs.us_ == rhs.us_);
}

bool operator!=(const SeaplanesTime &lhs, const SeaplanesTime &rhs) {
  return (!(lhs == rhs));
}

ostream &operator<<(ostream &stream, const SeaplanesTime &t) {
  stream << t.get_s() << 's';
  return stream;
}

} // namespace Seaplanes
