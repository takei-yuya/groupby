#ifndef __GROUPBY_HPP__
#define __GROUPBY_HPP__

#include <cstddef>
#include <string>
#include <vector>
#include <utility>

std::vector<std::pair<size_t, size_t> > Split(const std::string& input, const std::string& chars);

class GroupBy {
public:
  struct Option {
  public:
    enum get_key_mode_t {
      GET_KEY_MODE_CHARS,
      GET_KEY_MODE_FIELDS,
    };

    Option()
      : get_key_mode(GET_KEY_MODE_FIELDS),
      key_length(0), field_length(1), n_skip(0), delimiters("\t "), join_delimiter("\t") {}

    get_key_mode_t get_key_mode;
    size_t key_length;
    size_t field_length;
    size_t n_skip;
    std::string delimiters;
    std::string join_delimiter;
  };

  GroupBy(const Option& option)
    : prev_key_(""), buffers_(0), option_(option) {}

  void Append(const std::string& line);
  const std::vector<std::string>& Buffers() const;
  void Flush();

private:
  std::string GetKey(const std::string& line);

private:
  std::string prev_key_;
  std::vector<std::string> buffers_;
  Option option_;
};


#endif//__GROUPBY_HPP__
