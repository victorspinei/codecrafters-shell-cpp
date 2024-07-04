#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <filesystem>

namespace fs = std::filesystem;

std::set<std::string> supportedCommands = {
  "echo", "cd", "pwd", "ls", "exit", "type",
  ""
};

std::set<std::string> builtinCommands = {
  "echo","exit","type", "pwd", "cd"
};

bool running = true;
int exit_code = 0;
std::string working_directory;

void segment_query(std::vector<std::string>& query, std::string input, char separator);
std::string find_command(std::vector<std::string> pathDirectories, std::string command);
bool valid_directory(std::string path);

int main() {
  const char* path = std::getenv("PATH");
  std::vector<std::string> pathDirectories;

  segment_query(pathDirectories, path, ':');

  working_directory = fs::current_path();

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
    std::string command_path;
    command_path = find_command(pathDirectories, command);

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
      std::string command2_path;
      command2_path = find_command(pathDirectories, command2);
      if (builtinCommands.contains(command2)) {
        std::cout << command2 << " is a shell builtin\n";
      } else if (!command2_path.empty()) {
        std::cout << command2 << " is " << command2_path << '\n';
      } else {
        std::cerr << command2 << ": not found\n";
      }
    } else if (command == "pwd") {
      std::cout << working_directory << '\n';
    } else if (command == "cd") {
      fs::path directory_path;

      if (query.size() == 1) {
        // No argument, change to home directory
        const char* home = std::getenv("HOME");
        if (home) {
          directory_path = home;
        } else {
          std::cerr << "cd: HOME not set\n";
          continue;
        }
      } else {
        directory_path = query[1];
      }

      fs::path new_path;
      if (directory_path.is_absolute()) {
        new_path = directory_path;
      } else {
        new_path = working_directory / directory_path;
      }

      if (valid_directory(new_path.string())) {
        working_directory = fs::canonical(new_path);
      } else {
        std::cerr << "cd: " << new_path.string() << ": No such file or directory\n";
      }

    } else if (!command_path.empty()) {
      system(input.c_str());
    } else if (!supportedCommands.contains(command)) {
      std::cerr << command << ": command not found\n";
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
bool valid_directory(std::string path) {
  struct stat sb;
  if (stat(path.c_str(), &sb) == 0)
    return true;
  return false;
}
