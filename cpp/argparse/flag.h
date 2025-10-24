#ifndef AMOCLI_FLAG_H__
#define AMOCLI_FLAG_H__

#include <string>

namespace amocli
{

class Flag
{
public:
  Flag();
  Flag(char short_flag, const std::string& long_flag);
  void require_arg();
  bool requires_arg();

  void add_help(const std::string& help);
  const std::string& help();

  void set();
  void set(const std::string& val);
  bool is_set();

  bool matches(char c);
  bool matches(const std::string& s);

  char& short_flag();
  std::string& long_flag();

  const std::string& value();
private:
  char short_;
  std::string long_;
  bool require_arg_;
  bool is_set_;
  std::string value_;
  std::string help_;
};

} // namespace

#endif // AMOCLI_FLAG_H__
