/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 14:46:12 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/24 12:21:00 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serv.hpp"

bool end = false;

Serv::Serv(char *pass, int port) : _pass(pass), _port(port)
{
    _tv.tv_sec = 0;
    _tv.tv_usec = 0;
    _s_addr_in.sin_family = AF_INET;// AF_INET == IPv4 domain
    _s_addr_in.sin_port = htons(this->_port); // htons convert the port number
    _s_addr_in.sin_addr.s_addr = inet_addr("10.11.12.5"); //Change param with the pc ip address
    _cli_size = sizeof(this->_s_addr_in);
    _bots.clear();
    return;
}

Serv::Serv(const Serv &serv) : _pass(serv._pass), _port(serv._port), _s_addr_in(serv._s_addr_in), _cli_size(serv._cli_size), _tv(serv._tv)
{
    return;
}

Serv::~Serv(void)
{
    return;
}

Serv &Serv::operator=(const Serv &serv)
{
    this->_pass = serv._pass;
    this->_port = serv._port;
    this->_tv = serv._tv;
    this->_s_addr_in = serv._s_addr_in;
    this->_cli_size = serv._cli_size;
    return (*this);
}

void Serv::setSocket(int socket)
{
    this->_socket = socket;
}

int Serv::getPort(void) const
{
    return (this->_port);
}

std::string Serv::getPass(void) const
{
    return (this->_pass);
}

int Serv::getSocket(void) const
{
    return (this->_socket);
}

sockaddr_in Serv::getSockAddr(void) const
{
    return (this->_s_addr_in);
}

socklen_t Serv::getCliSize(void) const
{
    return (this->_cli_size);
}

std::list<User> &Serv::getListBots()
{
    return (this->_bots);
}

std::list<Channel> &Serv::getChannels()
{
    return (this->Channels);
}

bool Serv::initServ(void)
{
    // Socket creation, binding and listening
    //  We gonna use a TCP connection cause its more reliable than UDP
    setSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)); // SOCK_STREAM == TCP connection type, IPPROTO_TCP == TCP Protocol
    if (bind(this->_socket, (sockaddr *)&(this->_s_addr_in), sizeof(this->_s_addr_in)) == -1) // bind == connect a socket to a port number
    {
        std::cerr << "Cannot bind the socket." << std::endl;
        close(this->_socket);
        return (false);
    }
    if (listen(this->_socket, SOMAXCONN) == -1) // listen == wait for connexions
    {
        std::cerr << "Cannot listen" << std::endl;
        return (false);
    }
    std::cout << "The best IRC server is now open!" << std::endl;
    return (true);
}

int Serv::acceptClients(char *host, char *service)
{
    int socketClient = accept(_socket, (sockaddr *)&_s_addr_in, &_cli_size); // accept == accept connexions on a socket
    if (socketClient < 0)
    {
        std::cerr << "Accept failed." << std::endl;
        close(socketClient);
        close(_socket);
        return (-1);
    }
    if (getnameinfo((sockaddr *)&_s_addr_in, sizeof(_s_addr_in), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
        std::cout << "New connection on " << host << " on port " << service << "." << std::endl;
    else
    {
        std::cerr << "Cannot get server info." << std::endl;
        return (-1);
    }
    return (socketClient);
}

void Serv::connectNewUser(std::map<int, User> &Users, int socketClient, fd_set &use, char *host)
{
    User newRandom;

    newRandom.setSocket(socketClient);
    newRandom.setHost(host);
    Users[socketClient] = newRandom;
    FD_SET(Users[socketClient].getSocket(), &use);
}

void sigNcHandler(int sig, siginfo_t *info, void *context)
{
    (void) info;
    (void) context;
    (void) sig;
    end = true;
}

void setSignal(void)
{
    struct sigaction sigNc;
    
    memset(&sigNc, 0, sizeof(sigNc));
    sigaddset(&sigNc.sa_mask, SIGKILL);
    sigNc.sa_sigaction = &sigNcHandler;
    sigaction(SIGINT, &sigNc, NULL);
}

void Serv::launchServ(void)
{
    char    host[NI_MAXHOST];
    char    service[NI_MAXHOST];

    int socketClient;

    if (!initServ())
        return;
    FD_ZERO(&use);
    FD_SET(_socket, &use);
    setSignal();
    while (1)
    {
        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXHOST);

        writing = use;
        reading = use;
        if (select(FD_SETSIZE, &reading, &writing, NULL, &_tv) == -1)
        {
            std::cerr << "select() error" << std::endl;
            return;
        }
        if (end == true)
            closeServ();
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &reading))
            {
                if (i == _socket)
                {
                    socketClient = acceptClients(host, service); // accept == accept connexions on a socket
                    if (socketClient < 0)
                        return ;
                    connectNewUser(Users, socketClient, use, host);
                }
                else
                    readMessageClient(Users, Users[i], i, use, *this);
            }
            writing = use;
            if (FD_ISSET(i, &writing))
                findCmd(Channels ,Users, i, use);
        }
    }
    close(_socket); 
}

void Serv::closeServ(void)
{
    Users.clear();
    Channels.clear();
    _bots.clear();
    for (int i = 0; i < FD_SETSIZE; i++)
        if (FD_ISSET(i, &use))
            close(i);
    exit(1);
}

void Serv::findCmd(std::list<Channel> &Channels, std::map<int, User> &Users, int index, fd_set &use)
{
    if (Users[index].getCmd().empty() || Users[index].getEnded() != true)
        return ;
    std::cout << YEL << "Cmd we received: " << Users[index].getCmd() << WHT;
    Cmds cmds;
    cmds.executeCmd(Users, *this, index, Channels, use);
}

void Serv::botConnection(User &user, std::list<Channel> &Channels)
{
    std::list <Channel>::iterator ite;
    for(ite = Channels.begin(); ite != Channels.end(); ite ++)
    {
        std::string msg = "JOIN #" + (*ite).getChannelName() + "\r\n";
        send(user.getSocket(), msg.data(), msg.size(), 0);
    }
    _bots.push_back(user);
}