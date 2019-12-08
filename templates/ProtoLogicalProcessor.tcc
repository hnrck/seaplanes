/**
 * @file ProtoLogicalProcessor.tcc
 * @brief Implementation of Proto-Logical processor template.
 * @author Henrick Deschamps <henrick [dot] deschamps [at] isae [dot] fr>
 * @version 1.0.0
 * @date July 2016
 */

template <typename T> static inline void fileWrite(std::ofstream *p_os, T t) {
  (*p_os) << t;
}

template <typename T>
void Seaplanes::ProtoLogicalProcessor::consoFileWrite(T t) {
  fileWrite(&__federate_conso_, t);
}

template <typename T, typename... Args>
void Seaplanes::ProtoLogicalProcessor::consoFileWrite(T t, Args &&... args) {
  consoFileWrite(t);
  consoFileWrite(", ");
  consoFileWrite(args...);
}

template <typename T>
void Seaplanes::ProtoLogicalProcessor::prodFileWrite(T t) {
  fileWrite(&__federate_prod_, t);
}

template <typename T, typename... Args>
void Seaplanes::ProtoLogicalProcessor::prodFileWrite(T t, Args &&... args) {
  prodFileWrite(t);
  prodFileWrite(", ");
  prodFileWrite(args...);
}
