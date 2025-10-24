#ifndef AMOCLI_ARGPARSE_H__
#define AMOCLI_ARGPARSE_H__

#include <unordered_map>
#include <string>
#include "pole.h"
#include "subcommand.h"

namespace amocli
{

class ArgParse : public Pole
{
public:
  SubCommand& add_subcommand(const std::string& name);
  bool parse(int argc, char** argv);
  bool is_flag_set(const std::string& name);
  const std::string flag_value(const std::string& name);
  std::string operator[](const std::string& name);
  bool operator()(const std::string& name);
private:
  std::unordered_map<std::string, SubCommand> subcommands_;
  SubCommand* command_;
};

} // namespace

#endif // AMOCLI_ARGPARSE_H__
