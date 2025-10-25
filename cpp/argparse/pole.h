#ifndef AMOCLI_POLE_H__
#define AMOCLI_POLE_H__

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include "flag.h"

namespace amocli
{

class Pole
{
public:
  virtual ~Pole() = default;

  Flag& add_flag(const std::string& name, char short_flag, const std::string& long_flag);

  bool do_parse(int argc, char** argv, int& index);
  bool is_set(const std::string& name);
  bool value(const std::string& name, std::string& ret);
  bool at(size_t index, std::string& ret);
protected:
  std::unordered_map<std::string, Flag> flags_;
  std::vector<std::string> positionals_;

  bool is_opt(const std::string& opt);
  bool is_short_opt(const std::string& opt);
  bool is_long_opt(const std::string& opt);
private:
  std::unordered_map<char, Flag*> short_to_flag_;
  std::unordered_map<std::string, Flag*> long_to_flag_;
  std::unordered_set<char> short_flags_;
  std::unordered_set<std::string> long_flags_;
  bool handle_flag(Flag* flag, int argc, char** argv, int& index);
};

} // namespace

#endif // AMOCLI_POLE_H__
