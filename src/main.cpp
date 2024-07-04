#include <iostream>
#include <set>
#include <string>

std::set<std::string> supportedCommands = {
  "echo", "cd", "pwd", "ls"
};

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::string input;
  std::getline(std::cin, input);

  if (!supportedCommands.contains(input)) {
    std::cout << input << ": command not found" << std::endl;
  }
}
