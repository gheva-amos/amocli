#include "flag.h"

namespace amocli
{

Flag::Flag() :
  short_{'\0'}, long_{""}, require_arg_{false}, is_set_{false}
{
}

Flag::Flag(char short_flag, const std::string& long_flag) :
  short_{short_flag}, long_{long_flag}, require_arg_{false}, is_set_{false}
{
}

void Flag::require_arg()
{
  require_arg_ = true;
}

bool Flag::requires_arg()
{
  return require_arg_;
}

void Flag::add_help(const std::string& help)
{
  help_ = help;
}

const std::string& Flag::help()
{
  return help_;
}

void Flag::set()
{
  is_set_ = true;
}

void Flag::set(const std::string& val)
{
  value_ = val;
  is_set_ = true;
}

bool Flag::is_set()
{
  return is_set_;
}

bool Flag::matches(char c)
{
  return short_ == c;
}

bool Flag::matches(const std::string& s)
{
  return long_ == s;
}

const std::string& Flag::value()
{
  return value_;
}

char& Flag::short_flag()
{
  return short_;
}

std::string& Flag::long_flag()
{
  return long_;
}

} // namespace
