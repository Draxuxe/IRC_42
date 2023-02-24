/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:54:08 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/22 12:13:49 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

void Cmds::doKick(User &u, Channel &c, std::string who, std::string why)
{
    std::string mess;
    if (u.getUsername().compare(who) == 0)
        return ;
    else if (c.isInList(u.getUsername(), c.getListUser()) == false)
    {
        messageServDispatcher(u, 441);
        return ;
    }
    std::list<User>::iterator ite;
    for (ite = c.getListUser().begin(); ite != c.getListUser().end(); ite ++)
    {
        if (ite->getUsername().compare(who) == 0)
        {
            if (why.length() > 0)
                mess = ":" + u.getNickname() + "!" + u.getUsername() + "@" + u.getHost() + " KICK #" + c.getChannelName() + " " + who + " " + why + "\r\n"; 
            else
                mess = ":" + u.getNickname() + "!" + u.getUsername() + "@" + u.getHost() + " KICK #" + c.getChannelName() + " " + who + "\r\n";
            c.sendMessAllChannel(u, mess);
        }
    }
    c.removeUserToChannel(u, who);
}

void Cmds::kick(User &user, std::list<Channel> &Channels)
{
    std::string channelName = getChannelName(user.getCmd());
    std::string who = getArgCmd(user.getCmd());
    if (user.getCmd().empty() || channelName.empty() || who.empty())
    {
        messageServDispatcher(user, 461);
        return ;
    }
    std::string why = getWhy(user.getCmd());
    std::list<Channel>::iterator ite;
    for (ite = Channels.begin(); ite != Channels.end(); ite ++)
    {
        if (channelName.compare(ite->getChannelName()) == 0)
        {
            if (ite->isInList(user.getUsername(), ite->getListOpe()) == false)
            {
                messageServDispatcher(user, 482);
                return ;
            }
            doKick(user, (*ite), who, why);
        }
    }
    if (ite == Channels.end())
        messageServDispatcher(user, 403);
}