/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 13:28:25 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/23 15:39:06 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include "serv.hpp"
#include "color.hpp"

class Serv;

class User
{
    private:
        std::string _username;
        std::string _nickname;
        std::string _pwd;
        int _port;
        int _socket;
        bool _connected;
        std::string _cmd;
        std::string _host;
        std::set<int> _privateMessages;
        bool _authorized;
        bool _endString;
        std::list<std::string> _chans;

    public:
        User();
        ~User();
        User(const User &user);
        User &operator=(const User &user);

        void setSocket(int socket);
        void setPass(std::string pass);
        void setConnection();
        void setAuthorization(std::map<int, User> &Users);
        void setUsername(std::string username);
        void setNickname(std::string nickname);
        void setCmd(std::string cmd);
        void setHost(char *host);
        void setEnd();

        std::string getUsername();
        std::string getUsername() const;
        std::string getPass();
        std::string getNickname();
        std::string getCmd();
        int getSocket();
        bool getConnection();
        bool getAuthorization();
        bool getEnded();
        std::string getHost();
        std::set<int> &getPrivMessages();
        std::list<std::string> &getListChans();

        void checkPass(std::string pass);
        std::string checkForUsernameInUse(std::map<int, User> &Users, std::string username);
        std::string checkForNicknameInUse(std::map<int, User> &Users, std::string username);

        bool userIsInChan(std::list<std::string> &chans, std::string channel);

        void unsetConnection();
        void unsetAuthorization();
        void unsetEnd();
};

bool isABot(User &user, Serv &serv);

#endif