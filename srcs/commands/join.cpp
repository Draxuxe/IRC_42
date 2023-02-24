/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:17:40 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/23 15:38:47 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

bool isInUserList(std::list<std::string> &chans, Channel &actualChan)
{
    std::list<std::string>::iterator it;
    for (it = chans.begin(); it != chans.end(); it ++)
    {
        if (actualChan.getChannelName() == *it)
            return (true);
    }
    return (false);
}

bool isABot(User &user, Serv &serv)
{
    std::list<User>::iterator it;
    if (serv.getListBots().empty())
        return (false);
    for (it = serv.getListBots().begin(); it != serv.getListBots().end(); it ++)
    {
        if (it->getSocket() == user.getSocket())
            return (true);
    }
    return (false);
}

void Cmds::channelAlreadyExist(std::map<int, User> &Users, User &user, Channel &c, Serv &serv)
{
    if (c.isInList(user.getUsername(), c.getListBan()) == true)
    {
        messageServDispatcher(user, 474);
        return ;
    }
    std::list<User>::iterator ite;
    std::list<User> &userMapChannel = c.getListUser();
    for (ite = userMapChannel.begin(); ite != userMapChannel.end(); ite ++)
    {
        if (ite->getNickname().compare(user.getNickname()) == 0)
        {
            if (isInUserList(user.getListChans(), c) == true)
                return ;
            userMapChannel.erase(ite);
            break ;
        }
    }
    userMapChannel.push_back(user);
    user.getListChans().push_back(c.getChannelName());
    std::string names = c.listUsers(Users);
	std::string mess = ":" + user.getNickname() + " JOIN :#" + c.getChannelName() + "\r\n";
    mess += ":127.0.0.1 353 = #" + c.getChannelName() + " :" + names + "\r\n";
	mess += ":127.0.0.1 366 = #" + c.getChannelName() + " :End of /NAMES list\r\n";
    announceOnChannel(c, mess);
}

void Cmds::createNewChannel(User &user, std::string nameChannel, std::list<Channel> &Channels, int pos, Serv &serv)
{
    Channel newChannel(nameChannel, user);
    newChannel.setBots(serv.getListBots());
    Channels.push_back(newChannel);
	std::string mess = ":" + user.getNickname() + " JOIN :#" + nameChannel + "\r\n";
	mess += ":127.0.0.1 353 = #" + nameChannel + " : @" + user.getNickname() + "\r\n";
	mess += ":127.0.0.1 366 = #" + nameChannel + " :End of /NAMES list\r\n";
	sendMessageToUser(user.getSocket(), mess);
    if (!newChannel.getListBot().empty())
        sendMessageToBot(newChannel.getListBot(), "JOIN :#" + nameChannel + "\r\n");
}

void Cmds::join(std::map<int, User> &Users, User &user, std::string cmd, std::list<Channel> &Channels, Serv &serv)
{
    if (getCmdArgs(user.getCmd()).empty())
    {
        messageServDispatcher(user, 431);
        return ;
    }
    std::list<std::string> chansToJoin = parseChannels(getArgs(user.getCmd()));
    std::list<std::string>::iterator actual;
    std::string channel;
    int nbChan = 0;
	for (actual = chansToJoin.begin(); actual != chansToJoin.end(); actual ++)
    {
        if (actual->find("#") == std::string::npos)
            channel = actual->substr(actual->find(' ') + 1);
        else
            channel = actual->substr(actual->find('#') + 1);
        std::list<Channel>::iterator ite;
        for (ite = Channels.begin(); ite != Channels.end(); ite ++)
        {
            if ((*ite).getChannelName() == channel)
            {
                channelAlreadyExist(Users, user, (*ite), serv);
                user.getListChans().push_back(channel);
                break ;
            }
            nbChan ++;
        }
        if (ite == Channels.end())
        {
            createNewChannel(user, channel, Channels, nbChan, serv);
            user.getListChans().push_back(channel);
        }
    }
}