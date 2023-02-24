/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:21:37 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/22 12:19:35 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

void updateList(User &user, Channel &chan, std::list<User> &list)
{
    std::list<User>::iterator it;
    for (it = list.begin(); it != list.end(); it ++)
    {
        if (it->getUsername() == user.getUsername())
        {
            list.erase(it);
            list.push_back(user);
            break ;
        }
    }
}

void updateChannels(std::map<int, User> &Users, User &user, std::list<Channel> &Channels, std::string str)
{
    std::list<Channel>::iterator ite;
    for (ite = Channels.begin(); ite != Channels.end(); ite ++)
    {
        if (ite->isInList(user.getNickname(), ite->getListUser()) || ite->isInList(user.getNickname(), ite->getListOpe()))
            announceOnChannel(*ite, str);
        updateList(user, *ite, ite->getListUser());
        updateList(user, *ite, ite->getListOpe());
        updateList(user, *ite, ite->getListBan());
    }
}

void Cmds::nick(std::map<int, User> &Users, User &user, std::list<Channel> &Channels)
{
    if (getCmdArgs(user.getCmd()).empty())
    {
        messageServDispatcher(user, 431);
        return ;
    }
    std::string newName = getCmdArgs(user.getCmd());
    std::map<int, User>::iterator end = Users.end();
    for (std::map<int, User>::iterator j = Users.begin(); j != end; j ++)
    {
        if (j->second.getNickname().compare(newName) == 0)
        {
            messageServDispatcher(user, 433);
            return ;
        }
    }
    std::string oldName = user.getNickname();
    user.setNickname(newName);
    std::string response = ":" + oldName + "!" + user.getUsername() + "@" + user.getHost() + " NICK " + user.getNickname() + "\r\n";
    std::string msg = oldName + " is now known as " + user.getNickname() + "\r\n";
    sendMessageToAll(Users, user, msg);
    std::string myMsg = ":irc 001 " + user.getNickname() + " You are now known as " + user.getNickname() + "\r\n";
    sendMessageToUser(user.getSocket(), myMsg);
    updateChannels(Users, user, Channels, response);
    return ;
}