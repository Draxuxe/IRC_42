/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 13:27:37 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/17 15:35:54 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/user.hpp"

User::User(void) : _username(""), _nickname(""), _pwd("None"), _port(0), _socket(0), _connected(false), _cmd(""), _host(""), _authorized(false), _endString(false)
{
    _chans.clear();
    return ;
}

User::~User(void)
{
    return ;
}

User::User(const User &user) : _username(user._username), _nickname(user._nickname), _pwd(user._pwd), _port(user._port), _socket(user._socket), _connected(user._connected), _cmd(user._cmd), _host(user._host), _privateMessages(user._privateMessages), _authorized(user._authorized), _chans(user._chans)
{
    return ;
}

User &User::operator=(const User &user)
{
    this->_username = user._username;
    this->_nickname = user._nickname;
    this->_pwd = user._pwd;
    this->_port = user._port;
    this->_socket = user._socket;
    this->_connected = user._connected;
    this->_authorized = user._authorized;
    this->_cmd = user._cmd;
    this->_host = user._host;
    this->_privateMessages = user._privateMessages;
    this->_endString = user._endString;
    this->_chans = user._chans;
    return (*this);
}

// -------------------------Setter 
void User::setUsername(std::string username)
{
    this->_username = username;     
}

void User::setNickname(std::string nickname)
{
    this->_nickname = nickname;     
}

void User::setSocket(int socket)
{
    this->_socket = socket;
}

void User::setPass(std::string pass)
{
    this->_pwd = pass;
}

void User::setConnection(void)
{
    this->_connected = true;
}

void User::setAuthorization(std::map<int, User> &Users)
{
    this->_authorized = true;
    sendMessageToAll(Users, *this, _nickname + " just arrived on the Serv.\r\n");
}

void User::setCmd(std::string cmd)
{
    this->_cmd = cmd;
}

void User::setHost(char *host)
{
    this->_host = host;
}

void User::setEnd(void)
{
    this->_endString = true;
}

void User::unsetEnd(void)
{
    this->_endString = false;
}

// -------------------------Getter
std::string User::getUsername()
{
    return (this->_username);
}

std::string User::getUsername() const
{
    return (this->_username);
}

std::string User::getPass()
{
    return (this->_pwd);
}

std::string User::getNickname()
{
    return (this->_nickname);
}

int User::getSocket(void)
{
    return (this->_socket);
}

std::string User::getCmd(void)
{
    return (this->_cmd);
}

bool User::getConnection(void)
{
    return (this->_connected);
}

bool User::getAuthorization(void)
{
    return (this->_authorized);
}

void User::unsetConnection(void)
{
    this->_connected = false;
}

void User::unsetAuthorization(void)
{
    this->_authorized = false;
}

std::string User::getHost(void)
{
    return (this->_host);
}

std::set<int> &User::getPrivMessages(void)
{
    return (this->_privateMessages);
}

bool User::getEnded(void)
{
    return (this->_endString);
}

std::list<std::string> &User::getListChans(void)
{
    return (this->_chans);
}

//--------------------------Fct
std::string itoa(int num)
{
    std::string str;
    std::string newStr;

    while (num != 0)
    {
        int rem = num % 10;
        str += (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }
    for (std::string::reverse_iterator ite = str.rbegin(); ite != str.rend(); ite ++)
        newStr += *ite;
    return (newStr);
}

std::string User::checkForUsernameInUse(std::map<int, User> &Users, std::string username)
{
    int suffixe = 1;
    std::string newUsername = username;

    std::map<int, User>::iterator end = Users.end();
    for (std::map<int, User>::iterator j = Users.begin(); j != end; j ++)
    {
        if (j->second.getUsername() == newUsername)
        {
            newUsername = username + itoa(suffixe);
            suffixe ++;
        }
    }
    return (newUsername);
}

std::string User::checkForNicknameInUse(std::map<int, User> &Users, std::string nickname)
{
    int suffixe = 1;
    std::string newNickname = nickname;

    std::map<int, User>::iterator end = Users.end();
    for (std::map<int, User>::iterator j = Users.begin(); j != end; j ++)
    {
        if (j->second.getNickname() == newNickname)
        {
            newNickname = nickname + itoa(suffixe);
            suffixe ++;
        }
    }
    return (newNickname);
}

bool User::userIsInChan(std::list<std::string> &chans, std::string channel)
{
    std::list<std::string>::iterator ite;
    for (ite = chans.begin(); ite != chans.end(); ite++)
    {
        if ((*ite) == channel)
            return (true);
    }
    return (false);
}