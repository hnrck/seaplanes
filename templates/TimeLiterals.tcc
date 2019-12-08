constexpr Seaplanes::SeaplanesTime::SeaplanesTime(unsigned long long us)
    : us_(us) {}

constexpr Seaplanes::SeaplanesTime::SeaplanesTime(double s)
    : us_(static_cast<unsigned long long int>(s * s2us)) {}

constexpr Seaplanes::SeaplanesTime operator"" _us(unsigned long long us) {
  return Seaplanes::SeaplanesTime{us};
}

constexpr Seaplanes::SeaplanesTime operator"" _ms(long double ms) {
  return Seaplanes::SeaplanesTime{
      static_cast<unsigned long long>(ms * Seaplanes::SeaplanesTime::ms2us)};
}

constexpr Seaplanes::SeaplanesTime operator"" _ms(unsigned long long ms) {
  return Seaplanes::SeaplanesTime{
      static_cast<unsigned long long>(ms * Seaplanes::SeaplanesTime::ms2us)};
}

constexpr Seaplanes::SeaplanesTime operator"" _s(long double s) {
  return Seaplanes::SeaplanesTime{
      static_cast<unsigned long long>(s * Seaplanes::SeaplanesTime::s2us)};
}

constexpr Seaplanes::SeaplanesTime operator"" _s(unsigned long long s) {
  return Seaplanes::SeaplanesTime{
      static_cast<unsigned long long>(s * Seaplanes::SeaplanesTime::s2us)};
}
