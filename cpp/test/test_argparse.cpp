#include <gtest/gtest.h>
#include "argparse/argparse.h"

class SimpleArgparser : public ::testing::Test
{
protected:
  static void SetUpTestSuite()
  {
    auto& f1 = argparse.add_flag("rf1", '1', "one");
    f1.add_help("First flag for the root command");
    auto& f2 = argparse.add_flag("rf2", '2', "two");
    f2.add_help("Second flag for the root command, requires param");
    f2.require_arg();
  }
  static amocli::ArgParse argparse;
};

amocli::ArgParse SimpleArgparser::argparse;

TEST_F(SimpleArgparser, Positive1)
{
  const char* args[] = {"program", "-1", "-2", "value"};
  char** argv = const_cast<char**>(args);
  EXPECT_TRUE(argparse.parse(4, argv));
  EXPECT_TRUE(argparse.is_set("rf1"));
  EXPECT_TRUE(argparse.is_set("rf2"));
  std::string val;
  EXPECT_TRUE(argparse.value("rf2", val));
  EXPECT_EQ(val, "value");
}

TEST_F(SimpleArgparser, MissingParam)
{
  const char* args[] = {"program", "-1", "-2"};
  char** argv = const_cast<char**>(args);
  EXPECT_FALSE(argparse.parse(3, argv));
}

TEST_F(SimpleArgparser, PositionalRootCommand)
{
  const char* args[] = {"program", "-1", "-2", "value", "pos0", "pos1"};
  char** argv = const_cast<char**>(args);
  EXPECT_TRUE(argparse.parse(6, argv));
  EXPECT_TRUE(argparse.is_set("rf1"));
  EXPECT_TRUE(argparse.is_set("rf2"));
  EXPECT_EQ(argparse[0], "pos0");
  EXPECT_EQ(argparse[1], "pos1");
}

class SubCommandArgParser : public ::testing::Test
{
protected:
  static void SetUpTestSuite()
  {
    auto& f1 = argparse.add_flag("rf1", '1', "one");
    f1.add_help("First flag for the root command");
    auto& f2 = argparse.add_flag("rf2", '2', "two");
    f2.add_help("Second flag for the root command, requires param");
    auto& cmd1 = argparse.add_subcommand("cmd1");
    auto& f3 = cmd1.add_flag("cf1", 'a', "ay");
    f3.add_help("First command for the first subcommand");
    auto& f4 = cmd1.add_flag("cf2", 'b', "be");
    f4.add_help("second command for the first subcommand");
    f4.require_arg();
    auto& cmd2 = argparse.add_subcommand("cmd2");
    auto& f5 = cmd2.add_flag("c2f1", 'A', "AY");
    f5.add_help("First command for the second subcommand");
  }
  static amocli::ArgParse argparse;
};

amocli::ArgParse SubCommandArgParser::argparse;

TEST_F(SubCommandArgParser, CommandParsing)
{
  const char* args[] = {"program", "-1", "cmd1", "-b", "value", "-a"};
  char** argv = const_cast<char**>(args);
  EXPECT_TRUE(argparse.parse(6, argv));
  EXPECT_TRUE(argparse.is_set("rf1"));
  std::string val = argparse.flag_value("cf2");
  EXPECT_EQ(val, "value");
  EXPECT_EQ(argparse["cf2"], "value");
}

TEST_F(SubCommandArgParser, CombinedArgs)
{
  const char* args[] = {"program", "-1", "cmd1", "-abvalue"};
  char** argv = const_cast<char**>(args);
  EXPECT_TRUE(argparse.parse(4, argv));
  EXPECT_TRUE(argparse("rf1"));
  EXPECT_TRUE(argparse("cf1"));
  EXPECT_TRUE(argparse("cf2"));
  EXPECT_EQ(argparse["cf2"], "value");
}

TEST_F(SubCommandArgParser, Positionals)
{
  const char* args[] = {"program", "-1", "cmd1", "-abvalue", "pos0", "pos1"};
  char** argv = const_cast<char**>(args);
  EXPECT_TRUE(argparse.parse(6, argv));
  EXPECT_TRUE(argparse("rf1"));
  EXPECT_TRUE(argparse("cf1"));
  EXPECT_TRUE(argparse("cf2"));
  EXPECT_EQ(argparse["cf2"], "value");
  EXPECT_EQ(argparse[0], "pos0");
  EXPECT_EQ(argparse[1], "pos1");
}
