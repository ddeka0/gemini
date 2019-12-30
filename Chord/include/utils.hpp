#pragma once
#include "common.hpp"
#include <openssl/sha.h>

unsigned int sha256(const std::string str);
