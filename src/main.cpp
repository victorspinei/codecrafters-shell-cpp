#include <iostream>
#include <set>
#include <vector>
#include <string>

std::set<std::string> supportedCommands = {
  "echo", "cd", "pwd", "ls", "exit"
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

    if (command == "exit") {
      running = false;
      if (query.size() == 1)
        exit_code = 0;
      else
        exit_code = stoi(query[1]);
      break;
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
