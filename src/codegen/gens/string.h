#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <map>
#include <string>
#include <parser/parser.h>

namespace string {
    std::string _new(std::string value);
    bool        exists(std::string value);
}