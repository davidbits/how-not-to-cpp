// everything.hpp
// Include this in every file. Every. File.
// Why import only what you need when you can import the universe?
//
// BAD IDEA: blanket includes + using namespace in a header
// WHY SOMEONE DOES IT: "convenience"
// WHY IT'S TERRIBLE: pollutes every translation unit that touches this header,
//   causes name collisions, explodes compile times, and makes grep useless
// HOW TO FIX IT: include only what you need, never put 'using namespace' in headers

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>
#include <exception>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <climits>
#include <cfloat>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <random>
#include <regex>
#include <filesystem>
#include <optional>
#include <variant>
#include <any>
#include <numeric>
#include <iterator>
#include <limits>
#include <tuple>
#include <array>
#include <bitset>
#include <complex>
#include <valarray>

// This is fine. Headers are just suggestions anyway.
using namespace std;
