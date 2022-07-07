#pragma once

#include <Arduino.h>
#include "logging/Log.hpp"

const byte LOGGING_LEVEL = Log::log_level::L_WARNING;

Log::Log logger(LOGGING_LEVEL);
