/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 10:26:43 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/24 12:31:12 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serv.hpp"

void messageServDispatcher(User &user, int code)
{
    std::string str;

    switch (code)
    {
        case 001:
            str += ":irc 001 ";
            str += user.getNickname();
            str += " Hello and Welcome to our IRC.\r\n";
            break ;
        case 401:
            str += ":irc 401 Error No such nick on this server.\r\n";
            break ;
        case 403:
            str += ":irc 403 Error No such channel.\r\n";
            break ;
        case 409:
            str += ":irc 409 Error No such origin.\r\n";
            break ;
        case 412:
            str += ":irc 412 Error There is no text to send.\r\n";
            break ;
        case 431:
            str += ":irc 431 Error No Nickname given.\r\n";
            break ;       
        case 433:
            str += ":irc 433 Error Nickname already in use.\r\n";
            break ;
        case 441:
            str += ":irc 441 Error User is not on channel.\r\n";
            break ;
        case 442:
            str += ":irc 442 Error You are not on channel.\r\n";
            break ;
        case 451:
            str += ":irc 451 Error Not registered.\r\n";
            break ;
        case 461:
            str += ":irc 461 Error Need more params.\r\n";
            break ;
        case 464:
            str += ":irc 464 Error Connection refused incorrect password.\r\n";
            break ;
        case 472:
            str += ":irc 472 Error This is un unknown mode.\r\n";
            break ;
        case 474:
            str += ":irc 474 Error You are banned from this channel.\r\n";
            break ;
        case 482:
            str += ":irc 482 Error You are not a channel operator.\r\n";
            break ;
        default :
            str += ":irc 421 Error No such command.\r\n";
            break ;
    }
    sendMessageToUser(user.getSocket(), str);
}

void sendMessageToAll(std::map<int, User> &Users, User &user, std::string str)
{
    std::map<int, User>::iterator end = Users.end();
    for (std::map<int, User>::iterator j = Users.begin(); j != end; j ++)
    {
        if (j->second.getConnection() == true && user.getNickname() != j->second.getNickname()
            && j->second.getNickname() != "None")
        {
            std::string newStr = ":irc 001 " + j->second.getNickname() + " " + str;
            send(j->second.getSocket(), newStr.data(), newStr.size(), 0);
        }
    }
}

void sendMessageToUser(int userSocket, std::string str)
{
    send(userSocket, str.data(), str.size(), 0);
    return;
}

bool checkEndString(std::string buffer)
{
    std::string::size_type size = buffer.size();
    return((size >= 2 && buffer[size - 2] == '\r' && buffer[size - 1] == '\n') || buffer[size - 1] == 10);
}

int readUntilAll(User &user, std::string &buffer)
{
    buffer.clear();
    char line[4096];
    int n;

    memset(line, 0, 4096);
    while ((n = recv(user.getSocket(), line, 4096, 0)) && n != -1)
    {
        buffer += line;
        if (line[n] == '\0' || checkEndString(buffer))
            return (buffer.size());
        memset(line, 0, 4096);
    }
    return (n);
}

void leaveAllChans(User &user, std::list<Channel> &Channels)
{
    bool in = false;
	std::list<Channel>::iterator channel = Channels.begin();
	std::list<Channel>::iterator channelEnd = Channels.end();
    while (channel != channelEnd)
	{
		if (channel->isInList(user.getNickname(), channel->getListUser()))
		{
            in = true;
			channel->eraseFromList(user.getNickname(), channel->getListUser());
		}
        if (in == true)
        {
            std::string deco = ":" + user.getNickname() + " QUIT\r\n";
            std::list<User>::iterator ite;
            for (ite = channel->getListUser().begin(); ite != channel->getListUser().end(); ite++)
                send(ite->getSocket(), deco.data(), deco.size(), 0);
            in = false;
        }
        channel ++;
	}
}

void readMessageClient(std::map<int, User> &Users, User &user, int index, fd_set &use, Serv &serv)
{
    std::string str;
    std::string buffer;

    int n = readUntilAll(user, buffer);
    if (n < 0) //N < 0 error in recv()
    {
        std::cout << RED << user.getNickname() << " quitted: an error had occured." << WHT << std::endl;
        FD_CLR(user.getSocket(), &use);
        int closingSocket = user.getSocket();
        Users.erase(index);
        close(closingSocket);
        return ;
    }
    if (n == 0) //n == end of connection
    {
        std::cout << RED << user.getNickname() << " disconnected from the server." << WHT << std::endl;
        leaveAllChans(user, serv.getChannels());
        if (isABot(user, serv) == true)
            serv.getListBots().clear();
        user.unsetAuthorization();
        FD_CLR(user.getSocket(), &use);
        int closingSocket = user.getSocket();
        Users.erase(index);
        close(closingSocket);
        return ;
    }
    user.setCmd(user.getCmd() + buffer);
    if (user.getCmd().find("\r\n") != std::string::npos)
        user.setEnd();
    else if (user.getCmd().find("\n") != std::string::npos)
        user.setEnd();
    return ;
}

void sendMessagesToAllChannel(Channel &channel, std::string message)
{
    std::string completeLine;
    std::list<User>::iterator ite;
    std::list<User> users = channel.getListUser();
    for (ite = users.begin(); ite != users.end(); ite++)
    {
        completeLine = ":server!server@" + ite->getHost() + " PRIVMSG #" + channel.getChannelName() + " " + message + "\r\n";
        send(ite->getSocket(), completeLine.data(), completeLine.size(), 0);
    }
}

void sendMessageToBot(std::list<User> &bots, std::string mess)
{
    std::list<User>::iterator ite;
    for (ite = bots.begin(); ite != bots.end(); ite++)
        send((*ite).getSocket(), mess.data(), mess.size(), 0);
}

void announceOnChannel(Channel &channel, std::string message)
{
    std::list<User>::iterator ite;
    std::list<User> users = channel.getListUser();
    for (ite = users.begin(); ite != users.end(); ite++)
        send(ite->getSocket(), message.data(), message.size(), 0);
}
