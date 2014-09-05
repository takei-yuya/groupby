#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <getopt.h>

#include "groupby.hpp"

std::string Escape(const std::string& line)
{
  std::string escaped;
  for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
    switch(*it) {
    case '\t':
      escaped += "\\t";
      break;
    case '\n':
      escaped += "\\n";
      break;
    case '\r':
      escaped += "\\r";
      break;
    default:
      escaped += *it;
    }
  }
  return escaped;
}

void Usage(int argc, char** argv, const GroupBy::Option& option) {
  std::cout
    << "Usage: " << argv[0] << " [OPTIONS]" << std::endl
    << "Read lines from stdin and group lines by key." << std::endl
    << "" << std::endl
    << "  -w, --width=N              Use N chars as key[" << option.key_length << "]." << std::endl
    << "  -f, --fields=N             Use N fields as key[" << option.field_length << "]." << std::endl
    << "  -s, --skip=N               Skip N chars/fields[" << option.n_skip << "]." << std::endl
    << "  -d, --delimiters=DELIM     Use DELIM as field delimiter[\"" << Escape(option.delimiters) << "\"]." << std::endl
    << "  -j, --join-delimiter=DELIM Join lines by DELIM[\"" << Escape(option.join_delimiter) << "\"]." << std::endl
    << "      --help                 Display this help and exit." << std::endl;
};

int main(int argc, char** argv)
{
  GroupBy::Option option;

  while (1) {
    static struct option long_options[] = {
      { "width",            required_argument, 0, 'w' },
      { "fields",           required_argument, 0, 'f' },
      { "skip",             required_argument, 0, 's' },
      { "delimiters",       required_argument, 0, 'd' },
      { "join-delimiter",   required_argument, 0, 'j' },
      { "help",             no_argument,       0, '?' },
      { 0,                 0 ,                 0, 0 },
    };

    int c = getopt_long(argc, argv, "w:d:j:f:s:", long_options, NULL);
    if (c == -1) {
      break;
    }

    switch(c) {
    case 'w':
      option.get_key_mode = GroupBy::Option::GET_KEY_MODE_CHARS;
      option.key_length = std::atoi(optarg);
      break;

    case 'f':
      option.get_key_mode = GroupBy::Option::GET_KEY_MODE_FIELDS;
      option.field_length = std::atoi(optarg);
      break;

    case 's':
      option.n_skip = std::atoi(optarg);
      break;

    case 'd':
      option.get_key_mode = GroupBy::Option::GET_KEY_MODE_FIELDS;
      option.delimiters = optarg;
      break;

    case 'j':
      option.join_delimiter = optarg;
      break;

    case '?':
      Usage(argc, argv, option);
      return 0;
      break;

    default:
      Usage(argc, argv, option);
      return -1;
    }
  }

  if (optind < argc) {
    Usage(argc, argv, option);
    return -1;
  }

  argc -= optind;
  argv += optind;

  GroupBy group_by(option);
  while (std::cin) {
    std::string line;
    if (std::getline(std::cin, line).eof()) {
      break;
    }
    group_by.Append(line);
  }

  const std::vector<std::string>& buffers = group_by.Buffers();
  for (std::vector<std::string>::const_iterator it = buffers.begin(); it != buffers.end(); ++it) {
    std::cout << *it << std::endl;
  }

  return 0;
}
