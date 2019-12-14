//! \file    LogicalProcessorResults.cpp
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Attribute manipulation implementation.

#include <LogicalProcessorResults.h>

namespace Seaplanes {

Results &Results::get_logger() { static auto &&logger = Results(); };

void Results::log(const std::string &message, Destination destination) {}

} // namespace Seaplanes
