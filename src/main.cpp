#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <map>

std::set<std::string> supportedCommands = {
  "echo", "cd", "pwd", "ls", "exit", "type",
  "cat"
};

enum commandTypes{builtin, bin, SIZE};

std::map<std::string, commandTypes> commandTypesMap = {
  {"echo", builtin}, {"exit", builtin},
  {"cat", bin}, {"type", builtin}
};

bool running = true;
int exit_code = 0;

void segment_query(std::vector<std::string>& query, std::string input);

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (running) {
    std::cout << "$ ";

    std::string input;
    std::vector<std::string> query;

    std::getline(std::cin, input);

    segment_query(query, input);

    std::string command = query[0];
    if (!supportedCommands.contains(command)) {
      std::cout << command << ": command not found" << std::endl;
    }

    if (command =="exit") {
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
      std::string command2 = query[1];
      if (!supportedCommands.contains(command2)) {
        std::cout << command2 << ": not found" << std::endl;
        break;
      }
      commandTypes type = commandTypesMap[command2];
      if (type == bin) {
        std::cout << command2 << " is /bin/" << command2 << std::endl;
      } else if (type == builtin) {
        std::cout << command2 << " is a shell builtin" << std::endl;
      }
    }
  }
  return exit_code;
}

void segment_query(std::vector<std::string>& query, std::string input) {
    int inputLength = input.size();
    int leftIndex = 0;
    for (int i = 0; i < inputLength; i++) {
        if (input[i] == ' ') {
          query.push_back(input.substr(leftIndex, i - leftIndex));
          leftIndex = i + 1;
        }
    }
    query.push_back(input.substr(leftIndex, inputLength - leftIndex));
}
