#include "NetworkConnection.h"

NetworkConnection::NetworkConnection()
{
  //ctor
}

NetworkConnection::~NetworkConnection()
{
  //dtor
}

void sendMove(void)
{
	static std::string oldMsg;
	while(!quit)
	{
		sf::Packet packetSend;
		globalMutex.lock();
		packetSend << msgSend;
		globalMutex.unlock();

		socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		socket.receive(packetReceive);
		if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
		{
			std::cout << msg << std::endl;
			oldMsg = msg;
		}
	}
}

void Server(void)
{
	sf::TcpListener listener;
	listener.listen(PORT);
	listener.accept(socket);
	std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
}

bool Client(void)
{
	if(socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
	{
		std::cout << "Connected\n";
		return true;
	}
	return false;
}

void GetInput(std::string move_)
{
	std::string move = move_;
	if(move == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = move;
	globalMutex.unlock();
}


int main(int argc, char* argv[])
{
	sf::Thread* thread = 0;

	Client();

	thread = new sf::Thread(&sendMove);
	thread->launch();

	while(!quit)
	{
		GetInput();
	}

	if(thread)
	{
		thread->wait();
		delete thread;
	}
	return 0;
}
