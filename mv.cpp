#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <string.h>

using std::cout;
using std::endl;
using std::string;
using std::cerr;

static void showUsage(string name);
static void moveFile(string oldName, string newName);

int main(const int argc, const char * argv [], const char * envp []) {
  if (argc < 3) {
    showUsage(argv[0]);
    return EXIT_FAILURE;
  }
  // Declare variables for command line args
  for (int i=1; i<argc; ++i) {
    string arg = argv[i];
    //  User requests for help
    if ((arg == "-h") || (arg == "--help")) {
      showUsage(argv[0]);
      return EXIT_SUCCESS;
    }
  }
  string oldName = argv[1];
  string newName = argv[2];
  moveFile(oldName, newName);
  return EXIT_SUCCESS;
} // main

/** Displays proper usage */
static void showUsage(string name) {
  cerr << "Usage: " << endl 
       << name << " SOURCE-FILE TARGET-FILE" << endl
       << name << " SOURCE-FILE TARGET-DIR/" << endl
       << "Options:\n"
       << "\t-h,--help\t\tShow this help message\n"
       << endl;
}


/** Copies files and removes the originals */
static void moveFile(string oldName, string newName) {
  bool newNameIsDir = false;
  bool oldNameExist = false;
  bool newNameExist= false;
  struct stat sb;
  if(oldName.compare(newName) == 0) {
    exit( EXIT_SUCCESS);
  }
  if(newName[newName.size()-1] == '/'){
    newNameIsDir = true;
    newName = newName.substr(0,newName.size()-1);
  }
  newNameExist = (stat(strdup(newName.c_str()), &sb) == 0);
  oldNameExist = (stat(strdup(oldName.c_str()), &sb) == 0);

  if(newNameIsDir) {
    if(!newNameExist) {
      perror("Destination folder does not exist");
      exit(EXIT_FAILURE);
    }
  }
  if(newNameExist && (stat(strdup(newName.c_str()), &sb)==0) && S_ISDIR(sb.st_mode)){
    newNameIsDir = true;
  }
  if(!oldNameExist) {
    perror("Source does not exist");
    exit(EXIT_FAILURE);
  } 
  if(newNameIsDir) {
    newName = newName + '/' + oldName;
  }
  if(rename(strdup(oldName.c_str()), strdup(newName.c_str())) != 0) {
      perror("rename");
      exit(EXIT_FAILURE);
  }
}

