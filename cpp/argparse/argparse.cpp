#include "argparse.h"
#include <iostream>

namespace amocli
{

SubCommand& ArgParse::add_subcommand(const std::string& name)
{
  subcommands_[name] = SubCommand();
  return subcommands_[name];
}

bool ArgParse::parse(int argc, char** argv)
{
  // argv[0] = program name
  for (int i{1}; i < argc; ++i)
  {
    if (!is_opt(argv[i]) && subcommands_.contains(argv[i]))
    {
      command_ = &subcommands_[argv[i]];
      for (int j{i+1}; j < argc; ++j)
      {
        if (!command_->do_parse(argc, argv, j))
        {
          return false;
        }
      }
      return true;
    }
    if (!do_parse(argc, argv, i))
    {
      return false;
    }
  }
  return true;
}

bool ArgParse::is_flag_set(const std::string& name)
{
  if (!is_set(name))
  {
    return command_->is_set(name);
  }
  return true;
}

const std::string ArgParse::flag_value(const std::string& name)
{
  std::string ret;
  if (value(name, ret))
  {
    return ret;
  }
  if (command_->value(name, ret))
  {
    return ret;
  }
  return "";
}

std::string ArgParse::operator[](const std::string& name)
{
  return flag_value(name);
}

bool ArgParse::operator()(const std::string& name)
{
  return is_flag_set(name);
}

std::string ArgParse::operator[](size_t index)
{
  std::string ret;
  if (command_ && command_->at(index, ret))
  {
    return ret;
  }
  if (!command_)
  {
    if (at(index, ret))
    {
      return ret;
    }
  }
  return "";
}

} // namespace
