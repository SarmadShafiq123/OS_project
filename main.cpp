#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>

using namespace std;

class SimpleShell {
private:
  vector<string> history;
  map<string, string> aliases;
  string prompt = "shell>";

public:
  SimpleShell() {
    aliases["ls"] = "ls";
    aliases["cls"] = "clear";
  }

  void run() {
    cout << "========================================\n";
    cout << "          Welcome to Simple Shell        \n";
    cout << "========================================\n";
    cout << "  Available commands:                   \n";
    cout << "    help    - Show available commands   \n";
    cout << "    exit    - Quit the shell            \n";
    cout << "    clear   - Clear the terminal screen \n";
    cout << "========================================\n\n";

    string input;
    while (true) {
      cout << getCurrentDirectory() + " " + prompt + " ";
      getline(cin, input);
      if (input.empty())
        continue;

      history.push_back(input);
      vector<string> commands = splitCommands(input);
      for (string &cmd : commands) {
        executeCommand(cmd);
      }
    }
  }

private:
  vector<string> splitCommands(const string &input) {
    vector<string> commands;
    stringstream ss(input);
    string command;
    while (getline(ss, command, '&')) {
      if (!command.empty()) {
        commands.push_back(command);
      }
    }
    return commands;
  }

  void executeCommand(string command) {
    if (aliases.find(command) != aliases.end()) {
      command = aliases[command];
    }

    if (command == "exit") {
      cout << "Exiting shell...\n";
      exit(0);
    } else if (command == "clear") {
      if (system("clear") == -1) {
        cerr << "Error: Failed to clear the terminal.\n";
      }
      return;
    } else if (command == "history") {
      showHistory();
      return;
    } else if (command == "help") {
      showHelp();
      return;
    } else if (command == "pwd") {
      cout << getCurrentDirectory() + "\n";
      return;
    } else if (command.rfind("cd ", 0) == 0) {
      changeDirectory(command.substr(3));
      return;
    } else if (command.rfind("echo ", 0) == 0) {
      cout << command.substr(5) + "\n";
      return;
    } else if (command.rfind("touch ", 0) == 0) {
      createFile(command.substr(6));
      return;
    } else if (command.rfind("cat ", 0) == 0) {
      displayFile(command.substr(4));
      return;
    } else if (command.rfind("mkdir ", 0) == 0 ||
               command.rfind("rmdir ", 0) == 0 ||
               command.rfind("rm ", 0) == 0 ||
               command.rfind("cp ", 0) == 0 ||
               command.rfind("mv ", 0) == 0) {
      if (system(command.c_str()) == -1) {
        cerr << "Error: Command execution failed.\n";
      }
      return;
    }

    auto start = chrono::high_resolution_clock::now();
    int result = system(command.c_str());
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if (result == -1) {
      cout << "Error: Command not found!\n";
    } else {
      cout << "\nCommand executed in " << duration.count() << " seconds.\n";
    }
  }

  void showHistory() {
    cout << "Command History:\n";
    for (size_t i = 0; i < history.size(); ++i) {
      cout << i + 1 << ". " << history[i] << "\n";
    }
  }

  void showHelp() {
    cout << "\nAvailable commands:\n";
    cout << "  help     - Show available commands\n";
    cout << "  exit     - Quit the shell\n";
    cout << "  clear    - Clear the terminal screen\n";
    cout << "  pwd      - Print current directory\n";
    cout << "  cd <dir> - Change directory\n";
    cout << "  ls       - List directory contents\n";
    cout << "  history  - Show command history\n";
    cout << "  echo <msg> - Print a message\n";
    cout << "  mkdir <dir> - Create a directory\n";
    cout << "  rmdir <dir> - Remove a directory\n";
    cout << "  rm <file>   - Remove a file\n";
    cout << "  cp <src> <dest> - Copy a file\n";
    cout << "  mv <src> <dest> - Move a file\n";
    cout << "  touch <file> - Create an empty file\n";
    cout << "  cat <file> - Display file contents\n\n";
  }

  string getCurrentDirectory() {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer))) {
      return string(buffer);
    }
    return "";
  }

  void changeDirectory(const string &path) {
    if (chdir(path.c_str()) == 0) {
      cout << "Changed directory to: " << getCurrentDirectory() << "\n";
    } else {
      cout << "Error: Unable to change directory.\n";
    }
  }

  void createFile(const string &filename) {
    ofstream file(filename);
    if (file) {
      cout << "File created: " << filename << "\n";
    } else {
      cerr << "Error: Unable to create file.\n";
    }
  }

  void displayFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
      cerr << "Error: Unable to open file.\n";
      return;
    }
    string line;
    while (getline(file, line)) {
      cout << line << "\n";
    }
  }
};

int main() {
  SimpleShell shell;
  shell.run();
  return 0;
}