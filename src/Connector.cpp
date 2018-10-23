#include "Connector.h"

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

