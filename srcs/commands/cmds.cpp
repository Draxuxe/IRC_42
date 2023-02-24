/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:41:38 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/22 15:31:46 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

Cmds::Cmds()
{
	_mymap[JOIN] = "JOIN";
	_mymap[NICK] = "NICK";
	_mymap[PRIVMSG] = "PRIVMSG";
	_mymap[PART] = "PART";
	_mymap[QUIT] = "QUIT";
	_mymap[CAP] = "CAP";
	_mymap[PING] = "PING";
	_mymap[OP] = "MODE";
	_mymap[KICK] = "KICK";
    _mymap[PASS] = "PASS";
    _mymap[USER] = "USER";
	_mymap[BOT] = "BOT";
	_mymap[NOTHING] = "NOTHING";
	return;
}

Cmds::~Cmds()
{
	return ;
}

Cmds::Cmds(const Cmds &cmd)
{
	return ;
}

Cmds &Cmds::operator=(const Cmds &cmd)
{
	return (*this);
}

std::list<std::string> Cmds::parseChannels(std::string channels)
{
    std::list<std::string> listChannels;
    std::string chan;

    int end = channels.length();
    for (int i = 0; channels.data()[i] && i < end; i ++)
    {
        if (channels.data()[i] == ',')
        {
            listChannels.push_back(chan);
            chan.clear();
            continue ;
        }
        else if (channels.data()[i] == ' ')
        {
            listChannels.push_back(chan);
            chan.clear();
            break ;
        }
        chan += channels.data()[i];
    }
    if (!chan.empty())
        listChannels.push_back(chan);
    return (listChannels);
}

allCmds Cmds::getCmdName(std::string line)
{
	std::map <allCmds, std::string>::iterator ite;
	for (ite=_mymap.begin(); ite != _mymap.end(); ite++ )
	{
		if (!line.compare(0, ite->second.length(), ite->second))
			return (ite->first);
	}
	return (NOTHING);
}

std::string Cmds::getCmdArgs(std::string cmd)
{
    if (cmd.empty() || cmd.find_first_of(" ") == std::string::npos)
        return (0);
    size_t len = (cmd.find_first_of(" ") + 1) - cmd.length();
    std::string str(cmd.substr(cmd.find_first_of(" ") + 1, len));
    return (str);
}

std::string Cmds::getArgCmd(std::string cmd)
{
    std::string arg = cmd.substr(cmd.find("#") + 1);
    arg = arg.substr(arg.find_first_of(" ") + 1);
    if (arg.find(" ") != std::string::npos)
        arg = arg.substr(0, arg.find(" "));
    return arg;
}

std::string Cmds::getChannelName(std::string cmd)
{ 
    std::string channel = cmd.substr(cmd.find("#") + 1);
    channel = channel.substr(0, channel.find(" "));
    return channel;
}

std::string Cmds::getWhy(std::string cmd)
{
    std::string why;
    if (cmd.find(":") == std::string::npos)
        return why;
    why = cmd.substr(cmd.find(":") + 1);
    return (why);
}

void Cmds::errorCmd(User &user, std::map<int, User> &Users, Serv &serv)
{
    if (getCommand(user.getCmd()) == "CAP")
        return ;
    else
        askForNcConnection(user, serv, Users);
}

void Cmds::unsetCmd(User &user)
{
    if (user.getEnded() == true)
    {
        user.setCmd("");
        user.unsetEnd();
    }
}

void Cmds::executeCmd(std::map<int, User> &Users, Serv &serv, int index, std::list<Channel> &Channels, fd_set &use)
{
    if (!Users[index].getCmd().empty() && Users[index].getAuthorization() == true && Users[index].getEnded() == true)
    {
        if (Users[index].getCmd()[0] == '/')
            Users[index].setCmd(Users[index].getCmd().substr(1));
        Users[index].setCmd(Users[index].getCmd().substr(0, findNextLine(Users[index].getCmd(), 0)));
        allCmds nameCmd = getCmdName(Users[index].getCmd());
        switch (nameCmd)
        {
            case JOIN:
                join(Users, Users[index], Users[index].getCmd(), Channels, serv);
                unsetCmd(Users[index]);
                break ;
            case NICK:
                nick(Users, Users[index], Channels);
                unsetCmd(Users[index]);
                break ;
            case PRIVMSG:
                privmsg(Users[index], Users, Channels);
                unsetCmd(Users[index]);
                break ;
            case PART:
                part(Users[index], Users, Channels);
                unsetCmd(Users[index]);
                break ;
            case QUIT:
                quit(Users, Channels, Users[index], use);
                break ;
            case PING:
                pong(Users[index]);
                unsetCmd(Users[index]);
                break ;
            case OP:
                op(Users, Users[index], Channels);
                unsetCmd(Users[index]);
                break ;
            case KICK:
                kick(Users[index], Channels);
                unsetCmd(Users[index]);
                break ;
            case BOT:
                serv.botConnection(Users[index], Channels);
                unsetCmd(Users[index]);
                break ;
            default :
                unsetCmd(Users[index]);
                break ;
        }
    }
    else if (!Users[index].getCmd().empty() && Users[index].getAuthorization() == false && Users[index].getEnded() == true)
    {
        if (Users[index].getCmd()[0] == '/')
            Users[index].setCmd(Users[index].getCmd().substr(1));
        allCmds nameCmd = getCmdName(Users[index].getCmd());
        switch (nameCmd)
        {
            case NICK:
                ncCommand(Users[index], Users, serv);
                unsetCmd(Users[index]);
                break ;
            case PASS:
                ncCommand(Users[index], Users, serv);
                unsetCmd(Users[index]);
                break ;
            case USER:
                ncCommand(Users[index], Users, serv);
                unsetCmd(Users[index]);
                break ;
            case CAP:
                ncCommand(Users[index], Users, serv);
                unsetCmd(Users[index]);
                break ;
            default :
                errorCmd(Users[index], Users, serv);
                unsetCmd(Users[index]);
                break ;
        }
    }
}