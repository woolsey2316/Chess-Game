#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include <iostream>
#include <SFML/Network.hpp>

const unsigned short PORT = 5000;
const std::string IPADDRESS("192.168.0.100");

std::string msgSend;

sf::TcpSocket socket;
sf::Mutex globalMutex;
bool quit = false;

class NetworkConnection
{
  public:
    NetworkConnection();
    virtual ~NetworkConnection();

  protected:

  private:
};

#endif // NETWORKCONNECTION_H
