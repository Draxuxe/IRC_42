/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleferra <aleferra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 16:07:06 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/22 18:43:35 by aleferra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

bool noFlag(std::string cmd)
{
    int condition = 0;

    for (int i = 0; i < (int)cmd.length(); i ++)
    {
        if (cmd.data()[i] == '+' || cmd.data()[i] == '#' || cmd.data()[i] == '-')
            condition ++;
    }
    if (condition < 2)
        return (false);
    return (true);
}

int find_first_name(std::string cmd)
{
    int whiteSpaceNb = 0;

    for (int i = 0; i < (int)cmd.length(); i ++)
    {
        if (whiteSpaceNb == 2)
            return (i);
        else if (cmd.data()[i] == ' ')
            whiteSpaceNb++;
    }
    return (-1);
}

std::string operationMode(std::string cmd)
{
    std::string ope;
    if (cmd.find("+") != std::string::npos)
        ope = cmd.substr(cmd.find('+'));
    else if (cmd.find("-") != std::string::npos)
        ope = cmd.substr(cmd.find('-'));
    else if (cmd.find("+") == std::string::npos || cmd.find("-") == std::string::npos)
        return ("");
    ope = ope.substr(0, ope.find(" "));
    return ope;
}
std::string isMode(std::string ope)
{
    if (ope.empty() || ope == "")
        return ("+o");
    std::string realOpe;
    char mode = ope.data()[1];

    realOpe = ope.data()[0];
    realOpe += ope.data()[1];
    for (int i = 1; i < (int)ope.length(); i ++)
    {
        if (ope.data()[i] != mode && ope.data()[i] != ' ')
            return ("");
        else if (ope.data()[i] == ' ')
            break ;
    }
    return (realOpe);
}

std::list<std::string> getNames(std::string cmd)
{
    std::list<std::string> listNames;
    std::string name;

    int end = cmd.length();
    int start = find_first_name(cmd);
    if (start < 0)
        return (listNames);
    for (int i = start; cmd.data()[i] && i < end; i ++)
    {
        if (cmd.data()[i] == ' ')
        {
            listNames.push_back(name);
            name.clear();
            continue ;
        }
        name += cmd.data()[i];
    }
    if (!name.empty())
        listNames.push_back(name);
    return (listNames);
}

void Cmds::doOp(std::map<int, User> &Users, User &user, Channel &channel, std::string cmd, std::list<std::string> &listNames)
{
    std::string ope = isMode(operationMode(cmd));
    if (noFlag(user.getCmd()) == false)
        return ;
    else if (ope.empty() || ope == "")
        messageServDispatcher(user, 472);
    std::string actName = cmd.substr(cmd.find_last_of(" ") + 1);
    if (channel.isInList(user.getNickname(), channel.getListOpe()) == false && channel.isInList(user.getUsername(), channel.getListBot()) == false)
    {
        messageServDispatcher(user, 482);
        return ;
    }
    for (std::list<std::string>::iterator it = listNames.begin(); it != listNames.end(); it++)
    {
        if (channel.isInList(*it, channel.getListUser()) == false)
            messageServDispatcher(user, 441);
        else if (ope == "+o")
            channel.addNewOpeToChannel(Users, user, *it);
        else if (isMode(ope) == "-o")
            channel.removeOpeToChannel(Users, user, *it);
        else if (isMode(ope) == "+b")
            channel.banUser(user, *it);
        else if (isMode(ope) == "-b")
            channel.unbanUser(user, *it);
        else if (isMode(ope) == "-i")
            channel.infoChannel();
    }
}

void Cmds::op(std::map<int, User> &Users, User &user, std::list<Channel> &Channels)
{
    if (user.getCmd().empty())
    {
        messageServDispatcher(user, 461);
        return ;
    }
    else if (user.getCmd().find("#") == std::string::npos)
        return ;
    std::string cmd = user.getCmd().substr(user.getCmd().find("#") + 1);
    std::string nameChannel = cmd.substr(0, cmd.find(" "));
    std::list<Channel>::iterator ite;
    std::list<std::string> names = getNames(cmd);
    for (ite = Channels.begin(); ite != Channels.end(); ite ++)
    {
        if (nameChannel == (*ite).getChannelName())
            return doOp(Users, user, (*ite), cmd, names);
    }
    messageServDispatcher(user, 403);
}