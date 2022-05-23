
#pragma once
#include "../PipeInfo.hpp"
#include <stdio.h>
#include <string>
#include <unistd.h>

class SimpleLogger {

public:
  explicit SimpleLogger(bool isLogInfoEnable, bool isLogErrorEnable)
      : isLogInfoEnable_(isLogInfoEnable), isLogErrorEnable_(isLogErrorEnable) {
  }

  void LG_INF(const std::string &info) const;
  void LG_INF(const std::string &info, const int &intInfo) const;

  void LG_ERR(const std::string &info) const;
  void LG_ERR(const std::string &info, const int &intInfo) const;

private:
  void logString(const std::string &log) const;

  bool isLogInfoEnable_;
  const std::string infoString = " >> INFO  - ";
  bool isLogErrorEnable_;
  const std::string errorString = " >> ERROR - ";
  bool isRealTime{true};
};
