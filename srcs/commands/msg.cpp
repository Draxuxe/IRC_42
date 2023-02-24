/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 16:06:30 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/15 11:51:30 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

bool firstTimeChat(User &user, int targetSocket, std::map<int, User> &Users)
{
    if ((*user.getPrivMessages().find(targetSocket) != *user.getPrivMessages().end()) == true)
        return (false);
    user.getPrivMessages().insert(targetSocket);
    Users[targetSocket].getPrivMessages().insert(user.getSocket());
    return (true);
}

void sendPrivMessageToUser(std::string target, User &user, std::map<int, User> &Users)
{
    std::string str = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHost() + " " + user.getCmd() + "\r\n";
    std::map<int, User>::iterator end = Users.end();
    for (std::map<int, User>::iterator i = Users.begin(); i != end; i ++)
    {
        if (i->second.getNickname() == target)
        {
            if (i->second.getConnection() == true)
            {
                send(i->first, str.data(), str.size(), 0);
                if (firstTimeChat(user, i->second.getSocket(), Users) == true)
                    send(user.getSocket(), str.data(), str.size(), 0);
                return ;
            }
        }
    }
    messageServDispatcher(user, 401);
    return ;
}


void Cmds::privmsg(User &user, std::map<int, User> &Users, std::list<Channel> &Channels)
{
    std::string args = getCmdArgs(user.getCmd());
    int separation = args.find(" ");
    std::string target = args.substr(0, separation);
    if (args.empty())
    {
        messageServDispatcher(user, 412);
        return ;
    }
    else if (args.find("#") != std::string::npos)
        sendMessageToChan(target.substr(1, separation), user, Channels);
    else
        sendPrivMessageToUser(target, user, Users);
    return ;
}