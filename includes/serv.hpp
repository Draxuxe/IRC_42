/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 14:10:41 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/23 15:47:44 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <map>
#include <list>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <csignal>

#include "user.hpp"
#include "channel.hpp"
#include "message.hpp"
#include "cmds.hpp"
#include "nc.hpp"
#include "signal.hpp"

class User;
class Channel;

class Serv
{
    private :
        std::string _pass;
        int _port;
        struct sockaddr_in _s_addr_in;
        socklen_t   _cli_size;
        int _socket;
        struct timeval _tv;
        fd_set use;
        fd_set writing;
        fd_set reading;
        std::list<User> _bots;
        std::map<int, User> Users;
        std::list<Channel> Channels;

    public :
        Serv(char *pass, int port);
        ~Serv();

        Serv(const Serv &serv);
        Serv &operator=(const Serv &serv);

        void setSocket(int socket);

        int getPort() const;
        std::string getPass() const;
        int getSocket() const;
        sockaddr_in getSockAddr() const;
        socklen_t getCliSize() const;
        std::list<User> &getListBots();
        std::list<Channel> &getChannels();

        bool initServ();
        void launchServ();
        void closeServ();
        int acceptClients(char *host, char *service);

        void connectNewUser(std::map<int, User> &Users, int, fd_set &use, char *host);
        void findCmd(std::list<Channel> &Channels, std::map<int, User> &Users, int index, fd_set &use);

        void botConnection(User &user, std::list<Channel> &Channels);
};

#endif