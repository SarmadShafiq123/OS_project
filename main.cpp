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
    #ifdef _WIN32
        aliases["ls"] = "dir";
        aliases["clear"] = "cls";
    #else
        aliases["ls"] = "ls";
        aliases["clear"] = "clear";
    #endif
  }

  void run() {
    cout << "========================================\n";
    cout << "          Welcome to Simple Shell        \n";
    cout << "========================================\n";
    showHelp();

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
      system("clear");
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
    } else if (command.rfind("writecpp ", 0) == 0) {
      writeCppFile(command.substr(9));
      return;
    } else if (command.rfind("compile ", 0) == 0) {
      compileCppFile(command.substr(8));
      return;
    } else if (command.rfind("run ", 0) == 0) {
      runCppFile(command.substr(4));
      return;
    }

    system(command.c_str());
  }

  void showHistory() {
    cout << "Command History:\n";
    for (size_t i = 0; i < history.size(); ++i) {
      cout << i + 1 << ". " << history[i] << "\n";
    }
  }

  void showHelp() {
    cout << "\nAvailable commands:\n";
    cout << "  help       - Show available commands\n";
    cout << "  exit       - Quit the shell\n";
    cout << "  clear      - Clear the terminal screen\n";
    cout << "  pwd        - Print current directory\n";
    cout << "  cd <dir>   - Change directory\n";
    cout << "  ls         - List directory contents\n";
    cout << "  history    - Show command history\n";
    cout << "  echo <msg> - Print a message\n";
    cout << "  mkdir <dir> - Create a directory\n";
    cout << "  touch <file> - Create an empty file\n";
    cout << "  cat <file> - Display file contents\n";
    cout << "  writecpp <file> - Create & edit a C++ file\n";
    cout << "  compile <file> - Compile a C++ file\n";
    cout << "  run <file> - Execute a compiled C++ program\n\n";
  }

  string getCurrentDirectory() {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer))) {
      return string(buffer);
    }
    return "";
  }

  void changeDirectory(const string &path) {
    chdir(path.c_str());
  }

  void createFile(const string &filename) {
    ofstream file(filename);
    file.close();
  }

  void displayFile(const string &filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
      cout << line << "\n";
    }
  }

  void writeCppFile(const string &filename) {
    ofstream file(filename);
    cout << "Enter C++ code (type ':wq' on a new line to save & exit):\n";
    string line;
    while (true) {
      getline(cin, line);
      if (line == ":wq") break;
      file << line << "\n";
    }
  }

  void compileCppFile(const string &filename) {
    string outputFile = filename.substr(0, filename.find_last_of('.')) + "_out";
    string command = "g++ " + filename + " -o " + outputFile;
    system(command.c_str());
  }

  void runCppFile(const string &filename) {
    string command = "./" + filename + "_out";
    system(command.c_str());
  }
};

int main() {
  SimpleShell shell;
  shell.run();
  return 0;
}