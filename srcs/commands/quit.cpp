/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 16:19:50 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/24 11:37:34 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"
    
void leaveAll(std::map<int, User> &Users, User &user, std::list<Channel> &Channels)
{
    bool emptyChan = false;
    bool in = false;
	std::list<Channel>::iterator chanToDel;
	std::list<Channel>::iterator channel = Channels.begin();
	std::list<Channel>::iterator channelEnd = Channels.end();
    while (channel != channelEnd)
	{
		if (channel->isInList(user.getNickname(), channel->getListOpe()))
        {
			channel->eraseFromList(user.getNickname(), channel->getListOpe());
        }
		if (channel->isInList(user.getNickname(), channel->getListUser()))
		{
            in = true;
			channel->eraseFromList(user.getNickname(), channel->getListUser());
			if (channel->getListUser().empty())
			{
				emptyChan = true;
				chanToDel = channel;
			}
            else if (channel->getListUser().empty() == false && channel->getListOpe().empty())
            {
                std::list<User>::iterator it;
                for (it = channel->getListUser().begin(); it != channel->getListUser().end(); it ++)
                {
                    if (channel->getListBot().empty())
                    {
                        channel->getListOpe().push_back((*it));
                        break ;
                    }
                    else if (channel->isInList(it->getNickname(), channel->getListBot()) == false)
                    {
                        channel->getListOpe().push_back((*it));
                        break ;
                    }
                }
            }
		}
        if (in == true)
        {
            std::string deco = ":" + user.getNickname() + " QUIT\r\n";
            std::list<User>::iterator ite;
            for (ite = channel->getListUser().begin(); ite != channel->getListUser().end(); ite++)
                send(ite->getSocket(), deco.data(), deco.size(), 0);
            in = false;
        }
        channel++;
		if (emptyChan)
		{
			Channels.erase(chanToDel);
			emptyChan = false;
		}
	}
}

void Cmds::quit(std::map<int, User> &Users, std::list<Channel> &Channels, User &user, fd_set &use)
{
    std::string deco = user.getNickname() + " disconnected from the server.";
	std::cout << RED << deco << WHT << std::endl;
    deco += "\r\n";
    sendMessageToAll(Users, user, deco);
    leaveAll(Users, user, Channels);
    sendMessageToUser(user.getSocket(), deco);
    FD_CLR(user.getSocket(), &use);
    int socket = user.getSocket();
    close(socket);
    Users.erase(socket);
    if (Users.empty())
        Users.clear();
}