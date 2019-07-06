/** Copyright (C) 2019 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"

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
    if (ptr != NULL) {
      if (directory_exists(ptr)) return add_slash(ptr);
      if (file_exists(ptr)) return ptr;
    }
  } 
  return "";
}

std::string filename_join(std::string prefix, std::string suffix) {
  if (directory_exists(filename_absolute(prefix)))
    return filename_absolute(prefix) + suffix;
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
