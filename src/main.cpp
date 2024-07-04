#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sys/stat.h>

std::set<std::string> supportedCommands = {
  "echo", "cd", "pwd", "ls", "exit", "type",
  "cat", ""
};

std::set<std::string> builtinCommands = {
  "exit","type"
};

bool running = true;
int exit_code = 0;

void segment_query(std::vector<std::string>& query, std::string input, char separator);
std::string find_command(std::vector<std::string> pathDirectories, std::string command);

int main() {
  const char* path = std::getenv("PATH");
  std::vector<std::string> pathDirectories;

  segment_query(pathDirectories, path, ':');

  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (running) {
    std::cout << "$ ";

    std::string input;
    std::vector<std::string> query;

    std::getline(std::cin, input);

    segment_query(query, input, ' ');

    std::string command = query[0];
    if (!supportedCommands.contains(command)) {
      std::cout << command << ": command not found\n";
    }

    if (command == "exit") {
      running = false;
      if (query.size() == 1)
        exit_code = 0;
      else
        exit_code = stoi(query[1]);
      break;
    } else if (command == "echo") {
        for (int i = 1; i < query.size(); i++) {
          std::cout << query[i];
          if (i != query.size() - 1)
            std::cout << ' ';
          else
            std::cout << '\n';
        }
    } else if (command == "type") {
      const std::string &command2 = query[1];
      std::string command_path;
      command_path = find_command(pathDirectories, command2);
      if (!command_path.empty()) {
        std::cout << command2 << " is " << command_path << '\n';
      } else if (builtinCommands.contains(command2)) {
        std::cout << command2 << " is a shell builtin\n";
      } else {
        std::cout << command2 << ": not found\n";
      }
    }

  }
  return exit_code;
}

void segment_query(std::vector<std::string>& query, std::string input, char separator) {
    int inputLength = input.size();
    int leftIndex = 0;
    for (int i = 0; i < inputLength; i++) {
        if (input[i] == separator) {
          query.push_back(input.substr(leftIndex, i - leftIndex));
          leftIndex = i + 1;
        }
    }
    query.push_back(input.substr(leftIndex, inputLength - leftIndex));
}
std::string find_command(std::vector<std::string> pathDirectories, std::string command) {
  if (command.empty())
    return "";

  for (const auto &dir : pathDirectories) {
    std::string file = dir;
    if (!file.empty() && file.back() != '/') {
      file += '/';
    }
    file += command;

    struct stat sb;
    if (stat(file.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR))
      return file;
  }

  return "";
}
