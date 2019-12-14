//! \file    LogicalProcessorResults.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Logical processor Attribute manipulation.
//!
//! This class provides a simple way to use attribute.

#ifndef LOGICALPROCESSORRESULTS_H
#define LOGICALPROCESSORRESULTS_H

#include <memory>
#include <string>

namespace Seaplanes {

using UpResults = std::unique_ptr<class Results>;

class Results {
protected:
  std::string __filename_;

  explicit Results(std::string /* filename */);

public:
  virtual ~Results() = default;
  Results(const Results &) = delete;
  void operator=(const Results &) = delete;
  Results(Results &&) = delete;
  void operator=(Results &&) = delete;

  virtual void operator<<(std::string /* message */);
};

class ProdResults final : public Results {
private:
  friend class ResultsFactory;
  explicit ProdResults(std::string /* filename */);

public:
  ~ProdResults() override = default;
  ProdResults(const ProdResults &) = delete;
  void operator=(const ProdResults &) = delete;
  ProdResults(ProdResults &&) = delete;
  void operator=(ProdResults &&) = delete;

  virtual void operator<<(std::string /* message */);
};

class ResultsFactory final {
public:
  template <class T> static UpResults create();
};

} // namespace Seaplanes

#endif // LOGICALPROCESSORRESULTS_H
