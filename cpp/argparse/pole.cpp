#include "pole.h"
#include <stdexcept>
#include <iostream>

namespace amocli
{

Flag& Pole::add_flag(const std::string& name, char short_flag, const std::string& long_flag)
{
  if (short_flags_.contains(short_flag))
  {
    throw std::runtime_error("Flag already used: " + short_flag);
  }
  if (long_flags_.contains(long_flag))
  {
    throw std::runtime_error("Flag already used: " + long_flag);
  }

  flags_[name].short_flag() = short_flag;
  flags_[name].long_flag() = long_flag;

  short_to_flag_[short_flag] = &flags_[name];
  long_to_flag_[long_flag] = &flags_[name];

  short_flags_.insert(short_flag);
  long_flags_.insert(long_flag);
  return flags_[name];
}

bool Pole::is_opt(const std::string& opt)
{
  return !opt.empty() && opt[0] == '-';
}

bool Pole::is_short_opt(const std::string& opt)
{
  return opt.size() >= 2 && opt[0] == '-' && opt[1] != '-';
}

bool Pole::is_long_opt(const std::string& opt)
{
  return opt.size() >= 3 && opt[0] == '-' && opt[1] == '-';
}

bool Pole::do_parse(int argc, char** argv, int& index)
{
  if (is_long_opt(argv[index]))
  {
    std::string flag_str = std::string{argv[index]}.substr(2);
    if (!long_flags_.contains(flag_str))
    {
      std::cerr << "unknown option " << flag_str << std::endl;
      return false;
    }

    auto flag = long_to_flag_[flag_str];
    return handle_flag(flag, argc, argv, index);
  }
  std::string opt{argv[index]};
  if (!is_opt(opt))
  {
    positionals_.push_back(opt);
    return true;
  }
  else
  {
    if (opt.size() == 2) // -a
    {
      auto flag = short_to_flag_[opt[1]];
      return handle_flag(flag, argc, argv, index);
    }
    for (size_t i{1}; i < opt.size(); ++i)
    {
      auto flag = short_to_flag_[opt[i]];
      if (flag->requires_arg())
      {
        if ((i + 1) == opt.size())
        {
          return handle_flag(flag, argc, argv, index);
        }
        else
        {
          flag->set(opt.substr(i+1));
          return true;
        }
      }
      flag->set();
    }
  }
  return true;
}

bool Pole::handle_flag(Flag* flag, int argc, char** argv, int& index)
{
  if (flag->requires_arg())
  {
    index += 1;
    if (index == argc)
    {
      std::cerr << argv[index - 1] << " requires an argument" << std::endl;
      return false;
    }
    flag->set(argv[index]);
  }
  else
  {
    flag->set();
  }
  return true;
}

bool Pole::is_set(const std::string& name)
{
  if (flags_.contains(name))
  {
    return flags_[name].is_set();
  }
  return false;
}

bool Pole::value(const std::string& name, std::string& ret)
{
  if (!is_set(name))
  {
    std::cerr << name << " is not set" << std::endl;
    return false;
  }
  ret = flags_[name].value();
  return true;
}

bool Pole::at(size_t index, std::string& ret)
{
  if (index >= positionals_.size())
  {
    return false;
  }
  ret = positionals_[index];
  return true;
}

} // namespace
