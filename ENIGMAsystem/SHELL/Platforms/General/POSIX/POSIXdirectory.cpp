#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Universal_System/estring.h"

#include <limits.h>
#include <unistd.h>
#include <string>

using std::string;

using enigma_user::file_exists;
using enigma_user::directory_exists;

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

namespace enigma_user {

bool set_working_directory(string dname) {
  if (chdir((char *)dname.c_str()) == 0) {
    char buffer[PATH_MAX + 1]; 
    if (getcwd(buffer, PATH_MAX + 1) != NULL) {
      working_directory = add_slash(buffer);
      return true;
    }
  }

  return false;
}

// converts a relative path to absolute if the path exists
std::string filename_absolute(std::string fname) {
  if (file_exists(fname) || directory_exists(fname)) {
    char rpath[PATH_MAX];
    char *ptr = realpath(fname.c_str(), rpath);
    if (ptr != NULL) { return rpath; }
  } return "";
}

std::string filename_path_join(std::string prefix, std::string suffix) {
  if (prefix.back() != '/') prefix += "/";
  return prefix + suffix;
}

std::string directory_contents(std::string dname) {
  if (string_replace_all(dname, " ", "") == "") dname = ".";
  if (directory_exists(dname)) {
    std::string rpath = filename_absolute(dname);
    for (string item = file_find_first(rpath, fa_all); item != ""; item = file_find_next())
      res += filename_path_join(rpath, item) + "\n";
    file_find_close();
    res.pop_back();
    return res;
  }
  return "";
}

string environment_get_variable(string name) {
  char *env = getenv(name.c_str());
  return env ? env : "";
}

// deletes the environment variable if set to empty string
bool environment_set_variable(const string &name, const string &value) {
  if (value == "") return (unsetenv(name.c_str()) == 0);
  return (setenv(name.c_str(), value.c_str(), 1) == 0);
}

} // namespace enigma_user
