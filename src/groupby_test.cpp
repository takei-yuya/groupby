#include <gtest/gtest.h>

#include "groupby.hpp"

TEST(Split, simple)
{
  std::string str("foo bar baz");
  std::vector<std::pair<size_t, size_t> > expected;
  expected.push_back(std::make_pair(0, 3));
  expected.push_back(std::make_pair(4, 7));
  expected.push_back(std::make_pair(8, 11));

  std::vector<std::pair<size_t, size_t> > actual = Split(str, " ");
  EXPECT_EQ(expected, actual);
}

TEST(Split, delimiters)
{
  std::string str("foo bar\tbaz \thoge");

  std::vector<std::pair<size_t, size_t> > expected;
  expected.push_back(std::make_pair(0, 3));
  expected.push_back(std::make_pair(4, 7));
  expected.push_back(std::make_pair(8, 11));
  expected.push_back(std::make_pair(12, 12));
  expected.push_back(std::make_pair(13, 17));

  std::vector<std::pair<size_t, size_t> > actual = Split(str, " \t");
  EXPECT_EQ(expected, actual);
}

TEST(Split, vector_string)
{
  std::string str("foo bar\tbaz \thoge");

  std::vector<std::string> expected;
  expected.push_back("foo");
  expected.push_back("bar");
  expected.push_back("baz");
  expected.push_back("");
  expected.push_back("hoge");

  std::vector<std::pair<size_t, size_t> > poses = Split(str, " \t");
  std::vector<std::string> actual;
  for (std::vector<std::pair<size_t, size_t> >::const_iterator it = poses.begin();
       it != poses.end(); ++it) {
    actual.push_back(std::string(str.begin() + it->first, str.begin() + it->second));
  }
  EXPECT_EQ(expected, actual);
}

TEST(Split, one)
{
  std::vector<std::pair<size_t, size_t> > expected;
  expected.push_back(std::make_pair(0, 11));

  std::vector<std::pair<size_t, size_t> > actual = Split("foo bar\tbaz", "/");
  EXPECT_EQ(expected, actual);
}

TEST(GroupBy, fields)
{
  GroupBy::Option option;
  option.get_key_mode = GroupBy::Option::GET_KEY_MODE_FIELDS;
  option.n_skip = 1;
  option.field_length = 2;
  option.delimiters = " ";
  option.join_delimiter = "\t";

  GroupBy group_by(option);
  group_by.Append("a 2 3 a");
  group_by.Append("b 2 3 b");
  group_by.Append("c 2 3");
  group_by.Append("d 2 33 d");
  group_by.Append("e 2");
  group_by.Append("f  2");
  group_by.Append("g");
  group_by.Append("h");
  group_by.Append("i 2");

  std::vector<std::string> expected;
  expected.push_back("a 2 3 a\tb 2 3 b\tc 2 3");
  expected.push_back("d 2 33 d");
  expected.push_back("e 2");
  expected.push_back("f  2");
  expected.push_back("g\th");
  expected.push_back("i 2");

  EXPECT_EQ(expected, group_by.Buffers());
}

TEST(GroupBy, chars)
{
  GroupBy::Option option;
  option.get_key_mode = GroupBy::Option::GET_KEY_MODE_CHARS;
  option.n_skip = 2;
  option.key_length = 3;
  option.delimiters = " ";
  option.join_delimiter = "\t";

  GroupBy group_by(option);
  group_by.Append("a 2 3 a");
  group_by.Append("b 2 3 b");
  group_by.Append("c 2 3");
  group_by.Append("d 2 33 d");
  group_by.Append("e 2");
  group_by.Append("f  2");
  group_by.Append("g");
  group_by.Append("h");
  group_by.Append("i 2");

  std::vector<std::string> expected;
  expected.push_back("a 2 3 a\tb 2 3 b\tc 2 3\td 2 33 d");
  expected.push_back("e 2");
  expected.push_back("f  2");
  expected.push_back("g\th");
  expected.push_back("i 2");

  EXPECT_EQ(expected, group_by.Buffers());
}

