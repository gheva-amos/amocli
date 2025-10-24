#include "subcommand.h"

namespace amocli
{

SubCommand::SubCommand()
{
}

void SubCommand::add_help(const std::string& help)
{
  help_ = help;
}

const std::string& SubCommand::help()
{
  return help_;
}

} // namespace

