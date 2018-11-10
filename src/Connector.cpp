#include "Connector.h"
#include <string>
#include <iostream>
#include <sstream>

void Connector::connectToEngine(char* path)
{
   pipin_w = pipin_r = pipout_w = pipout_r = NULL;
   secattr.nLength = sizeof(secattr);
   secattr.bInheritHandle = TRUE;
   secattr.lpSecurityDescriptor = NULL;

   CreatePipe(&pipout_r, &pipout_w, &secattr, 0);
   CreatePipe(&pipin_r, &pipin_w, &secattr, 0);

   startinfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
   startinfo.wShowWindow = SW_HIDE;
   startinfo.hStdInput = pipin_r;
   startinfo.hStdOutput = pipout_w;
   startinfo.hStdError = pipout_w;

   CreateProcess(NULL, path, NULL, NULL, TRUE,0, NULL, NULL, &startinfo, &procinfo);
}

std::string Connector::getCompMove(std::string position, Computer comp)
{
	std::string str;
	position = "position startpos moves "+position
      +"\ngo depth "+comp.getDepth()+"\n"
      + "Skill Level "+comp.getSkillLevel()+"\n"
      + "movetime "+comp.getMoveTime()+"\n";

	WriteFile(pipin_w, position.c_str(), position.length(),&writ, NULL);
    Sleep(100);

    PeekNamedPipe(pipout_r, buffer,sizeof(buffer), &read, &available, NULL);
    do
    {
    ZeroMemory(buffer, sizeof(buffer));
		if(!ReadFile(pipout_r, buffer, sizeof(buffer), &read, NULL) || !read) break;
        buffer[read] = 0;
		str+=(char*)buffer;
    }
    while(read >= sizeof(buffer));

    int n = str.find("bestmove");

    if (n!=-1) {
      return str.substr(n+9,4);
    }

    return "error";
}

std::vector<std::array<std::string, 2>> Connector::getScore(std::string position, Computer comp)
{
	std::string str;
	std::string messageToEngine;

	messageToEngine += "position startpos moves "+position+"\n"
      + "go depth "+comp.getDepth()+"\n"
      + "setoption name MultiPV value 5\n";

	WriteFile(pipin_w, messageToEngine.c_str(), messageToEngine.length(),&writ, NULL);
    Sleep(100);

    PeekNamedPipe(pipout_r, buffer,sizeof(buffer), &read, &available, NULL);
    do
    {
    ZeroMemory(buffer, sizeof(buffer));
		if(!ReadFile(pipout_r, buffer, sizeof(buffer), &read, NULL) || !read) break;
        buffer[read] = 0;
		str+=(char*)buffer;
    }
    while(read >= sizeof(buffer));

    std::vector<std::array<std::string, 2>> positions;

    size_t scoreIndex = str.find("score cp ", 0)+9;
    size_t moveListIndex = str.find("multipv 1 pv ", 0);
    int centipawns = 0;
    size_t linenumber = 0;
    size_t previousScoreIndex = 0;
    size_t endofLineIndex = 0;

    //finding all possible move variations recommended by engine
    while(scoreIndex > previousScoreIndex)
    {
        moveListIndex = str.find(" pv ", moveListIndex+1)+4;
        if (scoreIndex!=-1) {
          std::string num = str.substr(scoreIndex,5);
          if (num[0] == '-' || (num[0] >= '1' && num[0] <= '9')) {
            for (int i = 0; i < 6; ++i) {
              if (num[i] >= '1' && num[i] <= '9' ) {
                if (num[0]== '-') {
                    continue;
                }

              } else {
              centipawns = std::stoi(num.substr(0,i-1));
              break;
              }
            }
          }
        }
        endofLineIndex = str.substr(moveListIndex,50).find("\n");
        if (moveListIndex!=-1) {
          positions.push_back(std::array<std::string,2>{
            std::to_string(centipawns),
            str.substr(moveListIndex,endofLineIndex)});
        }
        previousScoreIndex = scoreIndex;
        scoreIndex = str.find("score cp ", scoreIndex+1)+9;
    }
    return positions;
}

void Connector::closeConnection()
{
	WriteFile(pipin_w, "quit\n", 5,&writ, NULL);
    if(pipin_w != NULL) CloseHandle(pipin_w);
    if(pipin_r != NULL) CloseHandle(pipin_r);
    if(pipout_w != NULL) CloseHandle(pipout_w);
    if(pipout_r != NULL) CloseHandle(pipout_r);
    if(procinfo.hProcess != NULL) CloseHandle(procinfo.hProcess);
    if(procinfo.hThread != NULL) CloseHandle(procinfo.hThread);
}

Connector::Connector(){
  secattr = {0};
  startinfo = {0};
  procinfo = {0};
}

