#pragma once

// this is a precompiled header. In here is where we want to place LARGE dependencies that we're using a lot, so we don't want them being compiled each time we build.
// Only place things here that will not be modified as recompiling this header will take time.

// Additionally, be wary of readability, as removing the context of all your includes can make it difficult to determine what dependencies a file has, which can make code reuse more difficult.

#include <GL/glew.h>

#include <iostream>
#include <fstream>

// data structures

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>