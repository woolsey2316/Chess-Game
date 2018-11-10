#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <Computer.h>
#include <vector>
#include <array>

class Connector{
 private:
  SECURITY_ATTRIBUTES secattr = {0};
  STARTUPINFO startinfo = {0};
  PROCESS_INFORMATION procinfo = {0};
  HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
  BYTE buffer[2048];
  DWORD writ, excode, read, available;
 public:
  Connector();
  void connectToEngine(char* path);
  void closeConnection();
  std::string getCompMove(std::string position, Computer comp);
  std::vector<std::array<std::string, 2>> getScore(std::string position, Computer comp);
};
#endif
