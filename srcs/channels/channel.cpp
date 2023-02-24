/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:26:49 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/23 14:38:44 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/channel.hpp"

Channel::Channel(void) : _channelName("None")
{
	return ;
}

Channel::Channel(std::string nameChannel, User &firstUser)
{
	_channelName = nameChannel;
	_UsersChannel.push_back(firstUser);
	_OpesChannel.push_back(firstUser);
}

Channel::~Channel(void)
{
	return;
};

Channel &Channel::operator=(const Channel &chan)
{
	this->_channelName = chan._channelName;
	this->_UsersChannel = chan._UsersChannel;
	this->_OpesChannel = chan._OpesChannel;
	this->_BanChannel = chan._BanChannel;
	this->_Bot = chan._Bot;
	return (*this);
}

void Channel::setBots(std::list<User> &bots)
{
	for (std::list<User>::iterator start = bots.begin(); start != bots.end(); start ++)
		_Bot.push_back(*start);
}

void Channel::addNewUserToChannel(User &user, std::string nameAdd)
{
	if (user.getUsername().compare(nameAdd) == 0)
		return ;
	_UsersChannel.push_back(user);
}

void Channel::removeUserToChannel(User &user, std::string nameRemove)
{
	std::list<User>::iterator ite;
	for (ite = getListUser().begin(); ite != getListUser().end(); ite ++)
	{
		if (ite->getUsername().compare(nameRemove) == 0)
		{
			_UsersChannel.erase(ite);
			return;
		}
	}
}

std::string Channel::getChannelName(void)
{ 
	return (this->_channelName);
}

void Channel::setChannelName(std::string chan)
{
	this->_channelName = chan;
}

std::list<User> &Channel::getListUser()
{
	return (this->_UsersChannel);
}

std::list<User> &Channel::getListOpe()
{
	return (this->_OpesChannel);
}

std::list<User> &Channel::getListBan()
{
	return (this->_BanChannel);
}

std::list<User> &Channel::getListBot()
{
	return (this->_Bot);
}


void Channel::sendMessAllChannel(User &userActual, std::string mess)
{
	std::list<User>::iterator ite;
	for (ite = _UsersChannel.begin(); ite != _UsersChannel.end(); ite ++)
		sendMessageToUser(ite->getSocket(), mess);
	return ;
}

void Channel::affListOpes()
{
	std::cout << BLU << "List opes" << WHT << std::endl;
	std::list<User>::iterator ite;
	for (ite = _OpesChannel.begin(); ite != _OpesChannel.end(); ite++)
		std::cout << ite->getNickname() << std::endl;
}

void Channel::affListUsers()
{
	std::cout << BLU << "List users" << WHT << std::endl;
	std::list<User>::iterator ite;
	for (ite = _UsersChannel.begin(); ite != _UsersChannel.end(); ite++)
		std::cout << ite->getNickname() << std::endl;
}

void Channel::affListBan()
{
	std::cout << BLU << "List ban" << WHT << std::endl;
	std::list<User>::iterator ite;
	for (ite = _BanChannel.begin(); ite != _BanChannel.end(); ite++)
		std::cout << ite->getNickname() << std::endl;
}

void Channel::affListBot()
{
	std::cout << BLU << "List ban" << WHT << std::endl;
	std::list<User>::iterator ite;
	for (ite = this->getListBot().begin(); ite != this->getListBot().end(); ite++)
		std::cout << (*ite).getNickname() << std::endl;
}

void Channel::addNewOpeToChannel(std::map<int,User> &Users, User &user, std::string nameAdd)
{
	affListUsers();
	if (user.getNickname().compare(nameAdd) == 0)
		return ;
	std::list<User>::iterator ite;
	for (ite = _OpesChannel.begin(); ite != _OpesChannel.end(); ite ++)
	{
		if (ite->getNickname().compare(nameAdd) == 0)
			return ;
	}
	for (ite = _UsersChannel.begin(); ite != _UsersChannel.end(); ite ++)
	{
		if (ite->getNickname().compare(nameAdd) == 0)
		{
			_OpesChannel.push_back(*ite);
			std::string names = this->listUsers(Users);
			std::string mess = ":127.0.0.1 353 = #" + this->getChannelName() + " :" + names + "\r\n";
			mess += ":127.0.0.1 366 = #" + this->getChannelName() + " :End of /NAMES list\r\n";
		    announceOnChannel(*this, mess);
			return ;
		}
	}
}

void Channel::removeOpeToChannel(std::map<int, User> &Users, User &user, std::string actName)
{
	if (user.getUsername().compare(actName) == 0)
		return ;
	std::list<User>::iterator ite;
	for (ite = _OpesChannel.begin(); ite != _OpesChannel.end(); ite ++)
	{
		if (ite->getNickname().compare(actName) == 0)
		{
			_OpesChannel.erase(ite);
			std::string names = this->listUsers(Users);
			std::string mess = ":127.0.0.1 353 = #" + this->getChannelName() + " :" + names + "\r\n";
			mess += ":127.0.0.1 366 = #" + this->getChannelName() + " :End of /NAMES list\r\n";
		    announceOnChannel(*this, mess);
			return ;
		}
	}
}

bool Channel::isInList(std::string name, std::list<User> li)
{
	std::list<User>::iterator ite;
	for (ite = li.begin(); ite != li.end(); ite ++)
	{
		if (ite->getNickname().compare(name) == 0)
			return (true);
	}
	return (false);
}

void Channel::eraseFromList(std::string name, std::list<User> &li)
{
	std::list<User>::iterator ite;
	for (ite = li.begin(); ite != li.end(); ite++)
	{
		if (ite->getNickname().compare(name) == 0)
		{
			li.erase(ite);
			if (li.empty())
				li.clear();
			return ;
		}
	}
}

User &Channel::isUser(std::string name)
{
	std::list<User>::iterator ite;
	for (ite = _UsersChannel.begin(); ite != _UsersChannel.end(); ite++)
	{
		if (name.compare(ite->getUsername()) == 0)
			return (*ite);
	}
	return (*ite);
}

void Channel::banUser(User &u, std::string name)
{
	if (u.getUsername() == name)
		return ;
	if (isInList(name, _UsersChannel) == true)
	{
		std::string mess = ":" + u.getNickname() + "!" + u.getUsername() + "@" + u.getHost() + " KICK #" + this->getChannelName() + " " + name + "\r\n";
        this->sendMessAllChannel(u, mess);
		_BanChannel.push_back(isUser(name));
		if (isInList(name, _OpesChannel) == true)
			eraseFromList(name, _OpesChannel);
		if (isInList(name, _UsersChannel) == true)
			eraseFromList(name, _UsersChannel);
	}
}

void Channel::unbanUser(User &u, std::string name)
{
	if (isInList(name, _BanChannel) == true)
		eraseFromList(name, _BanChannel);
}

std::string Channel::listUsers(std::map<int, User> &Users)
{
	std::string names;
	std::map<int, User>::iterator ite;
	std::list<User>::iterator itebis;
	for (ite = Users.begin(); ite != Users.end(); ite++)
	{
		if (ite->second.userIsInChan(ite->second.getListChans(), _channelName))
		{
			names += " ";
			for (itebis = _OpesChannel.begin(); itebis != _OpesChannel.end(); itebis++)
			{
				if (ite->second.getUsername().compare(itebis->getUsername()) == 0)
					names += "@";
			}
			names += ite->second.getNickname();
		}
	}
	return (names);
}

int findReasons(std::string args)
{
    for (int index = 0; args.data()[index]; index ++)
    {
        if (args.data()[index] == ' ')
            return (index);
    }
    return (-1);
}

std::string makeCommandOutput(User &user, std::string cmdName, std::string channelToQuit)
{   
    std::string mess = ":" + user.getNickname() + cmdName + channelToQuit;
    int reasons = findReasons(getArgs(user.getCmd()));
    if (reasons > 0)
        mess += getArgs(user.getCmd()).substr(reasons);
    mess += "\r\n";
    return (mess);
}

void Channel::removeUserFromChan(Channel &c, std::map<int, User> &Users, User &user, std::list<Channel> &Channels, std::string channelToQuit)
{
    std::string mess = makeCommandOutput(user, " PART ", channelToQuit);
    announceOnChannel(c, mess);
	c.eraseFromList(user.getNickname(), c.getListUser());
    if (c.isInList(user.getNickname(), c.getListOpe()) == true)
    {
	    c.eraseFromList(user.getNickname(), c.getListOpe());
        if (c.getListOpe().empty() == true && c.getListUser().empty() == false)
        {
            std::list<User>::iterator newOp = c.getListUser().begin();
			c.getListOpe().push_back(*newOp);
            sendMessagesToAllChannel(c, newOp->getNickname() + " is now the new Operator of the channel.");
        }
    }
    if (c.getListUser().empty())
    {
        deleteChan(c, Channels);
        return ;
    }
	for (std::list<std::string>::iterator ite = user.getListChans().begin(); ite != user.getListChans().end(); ite ++)
	{
		if (c.getChannelName() == (*ite))
		{
			user.getListChans().erase(ite);
			break ;
		}
	}
    std::string names = c.listUsers(Users);
    mess = ":127.0.0.1 353 = #" + c.getChannelName() + " :" + names + "\r\n";
	mess += ":127.0.0.1 366 = #" + c.getChannelName() + " :End of /NAMES list\r\n";
    announceOnChannel(c, mess);
}

void Channel::checkUserInChan(std::map<int, User> &Users, User &user, std::list<std::string>::iterator chan, std::list<Channel> &Channels)
{
    std::list<Channel>::iterator ite;
    if ((*chan).data()[0] != '#')
    {
        messageServDispatcher(user, 403);
        return ;
    }
    for (ite = Channels.begin(); ite != Channels.end(); ite ++)
    {
        if ((*ite).getChannelName() == (*chan).substr(1))
        {
            std::list<User> li = ite->getListUser();
            if (ite->isInList(user.getNickname(), li) == true)
                removeUserFromChan(*ite, Users, user, Channels, *chan);
            else
                messageServDispatcher(user, 441);
            return ;
        }
    }
    if (ite == Channels.end())
        messageServDispatcher(user, 403);
}

void Channel::checkChan(std::map<int, User> &Users, User &user, std::list<Channel> &Channels, std::list<std::string> &channelsName)
{
    std::list<std::string>::iterator chan;
    for (chan = channelsName.begin(); chan != channelsName.end(); chan ++)
        checkUserInChan(Users, user, chan, Channels);
}

void sendMessageToChan(std::string chan, User &user, std::list<Channel> &Channels)
{
    std::list<Channel>::iterator end = Channels.end();
    std::list<Channel>::iterator j;
    for (j = Channels.begin(); j != end; j ++)
    {
        if ((*j).getChannelName() == chan)
        {
            std::list<User> usersInChan = j->getListUser();
            if (isInChan(usersInChan, user) == true)
            {
                std::string str = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHost() + " " + user.getCmd() + "\r\n";
                std::list<User>::iterator end = usersInChan.end();
                for (std::list<User>::iterator i = usersInChan.begin(); i != end; i ++)
                {
                    if (i->getConnection() == true && i->getSocket() != user.getSocket())
                        send(i->getSocket(), str.data(), str.size(), 0);
                }
			}
            else if ((*j).isInList(user.getUsername(), (*j).getListBan()) == true)
                messageServDispatcher(user, 474);
            else
                messageServDispatcher(user, 442);
            break ;
        }
    }
}

void Channel::deleteChan(Channel &c, std::list<Channel> &Channels)
{
    std::list<Channel>::iterator ite;
	for (ite = Channels.begin(); ite != Channels.end(); ite++)
	{
		if (ite->getChannelName().compare(c.getChannelName()) == 0)
        {
			ite->getListBan().clear();
			ite->getListUser().clear();
			ite->getListOpe().clear();
			Channels.erase(ite);
			if (Channels.empty())
				Channels.clear();
            return ;
        }
	}
}

bool isInChan(std::list<User> &usersInChan, User &user)
{
    std::list<User>::iterator end = usersInChan.end();
    for (std::list<User>::iterator j = usersInChan.begin(); j != end; j ++)
    {
        if (j->getConnection() == true && j->getUsername() == user.getUsername())
            return (true);
    }
    return (false);
}

void Channel::infoChannel()
{
	this->affListBan();
	this->affListOpes();
	this->affListUsers();
}
