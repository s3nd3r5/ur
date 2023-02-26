#ifndef UR_LOG_H
#define UR_LOG_H

#include "helper.hpp"
#include <ctime>
#include <iostream>
#include <string>
/**
 * A very simple stdio/stderr logger
 */
namespace Log {

// if we want debug to action
// will be noop fork, but cleaner for a simple program
#ifdef DEBUG
static const bool is_debug = true;
#else
static const bool is_debug = false;
#endif
// log message constants
static const std::string L_INFO = "[INFO]";
static const std::string L_ERROR = "[ERROR]";
static const std::string L_WARN = "[WARN]";
static const std::string L_DEBUG = "[DEBUG]";

// wrap <string> to_string to keep all semantics within the log namespace
//
// UR SPECIFIC CODE
std::string
str(GameState gs)
{
  switch (gs) {
    case WAITING:
      return "WAITING";
    case ROLLING:
      return "ROLLING";
    case PASSING:
      return "PASSING";
    case PLACING:
      return "PLACING";
    case GAME_OVER:
      return "GAME_OVER";
    default:
      return "Unknown state: " + std::to_string(gs);
  }
}

// END UR SPECIFIC CODE
std::string
str(int i)
{
  return std::to_string(i);
}

std::string
str(long i)
{
  return std::to_string(i);
}

std::string
str(long long i)
{
  return std::to_string(i);
}

std::string
str(unsigned i)
{
  return std::to_string(i);
}

std::string
str(unsigned long i)
{
  return std::to_string(i);
}

std::string
str(unsigned long long i)
{
  return std::to_string(i);
}

std::string
str(float i)
{
  return std::to_string(i);
}

std::string
str(double i)
{
  return std::to_string(i);
}

std::string
str(long double i)
{
  return std::to_string(i);
}
// end to_string

// helper functions
std::string
now()
{
  std::time_t current_time;
  std::time(&current_time);
  char now[25];
  std::strftime(now, sizeof(now), "%FT%T %Z", std::localtime(&current_time));

  return std::string(now);
}
// end helper fns

// log functions
void
log(std::string msg, std::string level)
{
  std::clog << now() << "\t" << level << "\t" << msg << std::endl;
}

void
log_d(std::string msg)
{
  if (is_debug) {
    log(msg, L_DEBUG);
  }
}
// end log fns

// DEBUG
void
debug()
{
  log_d("");
}

void
debug(int msg)
{
  log_d(str(msg));
}

void
debug(long msg)
{
  log_d(str(msg));
}

void
debug(long long msg)
{
  log_d(str(msg));
}

void
debug(unsigned msg)
{
  log_d(str(msg));
}

void
debug(unsigned long msg)
{
  log_d(str(msg));
}

void
debug(unsigned long long msg)
{
  log_d(str(msg));
}

void
debug(float msg)
{
  log_d(str(msg));
}

void
debug(double msg)
{
  log_d(str(msg));
}

void
debug(long double msg)
{
  log_d(str(msg));
}

void
debug(std::string msg)
{
  log_d(msg);
}

// L_INFO
void
info()
{
  log("", L_INFO);
}

void
info(int msg)
{
  log(str(msg), L_INFO);
}

void
info(long msg)
{
  log(str(msg), L_INFO);
}

void
info(long long msg)
{
  log(str(msg), L_INFO);
}

void
info(unsigned msg)
{
  log(str(msg), L_INFO);
}

void
info(unsigned long msg)
{
  log(str(msg), L_INFO);
}

void
info(unsigned long long msg)
{
  log(str(msg), L_INFO);
}

void
info(float msg)
{
  log(str(msg), L_INFO);
}

void
info(double msg)
{
  log(str(msg), L_INFO);
}

void
info(long double msg)
{
  log(str(msg), L_INFO);
}

void
info(std::string msg)
{
  log(msg, L_INFO);
}

// L_WARN
void
warn()
{
  log("", L_WARN);
}

void
warn(int msg)
{
  log(str(msg), L_WARN);
}

void
warn(long msg)
{
  log(str(msg), L_WARN);
}

void
warn(long long msg)
{
  log(str(msg), L_WARN);
}

void
warn(unsigned msg)
{
  log(str(msg), L_WARN);
}

void
warn(unsigned long msg)
{
  log(str(msg), L_WARN);
}

void
warn(unsigned long long msg)
{
  log(str(msg), L_WARN);
}

void
warn(float msg)
{
  log(str(msg), L_WARN);
}

void
warn(double msg)
{
  log(str(msg), L_WARN);
}

void
warn(long double msg)
{
  log(str(msg), L_WARN);
}

void
warn(std::string msg)
{
  log(msg, L_WARN);
}

// L_ERROR
void
error()
{
  log("", L_ERROR);
}

void
error(int msg)
{
  log(str(msg), L_ERROR);
}

void
error(long msg)
{
  log(str(msg), L_ERROR);
}

void
error(long long msg)
{
  log(str(msg), L_ERROR);
}

void
error(unsigned msg)
{
  log(str(msg), L_ERROR);
}

void
error(unsigned long msg)
{
  log(str(msg), L_ERROR);
}

void
error(unsigned long long msg)
{
  log(str(msg), L_ERROR);
}

void
error(float msg)
{
  log(str(msg), L_ERROR);
}

void
error(double msg)
{
  log(str(msg), L_ERROR);
}

void
error(long double msg)
{
  log(str(msg), L_ERROR);
}

void
error(std::string msg)
{
  log(msg, L_ERROR);
}
}
#endif
