#ifndef AMOCLI_SUBCOMMAND_H__
#define AMOCLI_SUBCOMMAND_H__

#include <string>
#include "pole.h"

namespace amocli
{

class Flag;

class SubCommand : public Pole
{
public:
  SubCommand();

  void add_help(const std::string& help);
  const std::string& help();

private:
  std::string help_;
};

} // namespace

#endif // AMOCLI_SUBCOMMAND_H__
