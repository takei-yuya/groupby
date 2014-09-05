#include "groupby.hpp"

#include <iostream>

std::vector<std::pair<size_t, size_t> > Split(const std::string& input, const std::string& chars)
{
  std::vector<std::pair<size_t, size_t> > results;
  size_t begin = 0;
  size_t end;
  while (true) {
    end = input.find_first_of(chars, begin);
    if (end == std::string::npos) {
      results.push_back(std::make_pair(begin, input.length()));
      return results;
    }
    results.push_back(std::make_pair(begin, end));
    begin = end + 1;
  }
}

void GroupBy::Append(const std::string& line)
{
  std::string key = GetKey(line);
  if (buffers_.empty() || key != prev_key_) {
    prev_key_ = key;
    buffers_.push_back(line);
  } else {
    buffers_.back() += option_.join_delimiter + line;
  }
}

const std::vector<std::string>& GroupBy::Buffers() const {
  return buffers_;
}

void GroupBy::Flush()
{
  buffers_.clear();
}

std::string GroupBy::GetKey(const std::string& line)
{
  switch(option_.get_key_mode) {

  case Option::GET_KEY_MODE_CHARS:
    {
      size_t begin, end;
      if (option_.n_skip < line.size()) {
        begin = option_.n_skip;
      } else {
        begin = line.size();
      }

      if (option_.n_skip + option_.key_length < line.size()) {
        end = option_.n_skip + option_.key_length;
      } else {
        end = line.size();
      }

      return std::string(line.begin() + begin, line.begin() + end);
    }

  case Option::GET_KEY_MODE_FIELDS:
    {
      std::vector<std::pair<size_t, size_t> > poses = Split(line, option_.delimiters);
      size_t begin, end;
      if (option_.n_skip < poses.size()) {
        begin = poses[option_.n_skip].first;
      } else {
        begin = line.length();
      }

      if (option_.n_skip + option_.field_length - 1 < poses.size()) {
        end = poses[option_.n_skip + option_.field_length - 1].second;
      } else {
        end = line.length();
      }

      return std::string(line.begin() + begin, line.begin() + end);
    }
  }
}
