#pragma once

#include <stdexcept>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

////todo: These are C libs. See if I can just use <filesystem>
#include <sys/types.h>  /* check dir existence */
#include <sys/stat.h>   /* check dir existence */

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

namespace Noodle
{

// For brevity
using std::function,
      std::string,
      std::vector,
      std::unordered_map,
      std::unordered_set;
namespace fs = std::filesystem;

using Name      = string;
using Names     = unordered_set<Name>; // Unique and iterable.
using NameList  = vector<Name>; // Unique and ordered.

}   // namespace Noodle