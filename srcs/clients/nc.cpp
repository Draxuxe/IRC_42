/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nc.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 14:02:41 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/22 15:32:03 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nc.hpp"

std::string getCommand(std::string line)
{
    if (line.empty() || line.find_first_of(" ") == std::string::npos)
        return ("");
    size_t len = line.find_first_of(" ");
    std::string str(line.substr(0, len));
    return (str);
}

std::string getArgs(std::string cmd)
{
    if (cmd.empty() || cmd.find_first_of(" ") == std::string::npos)
        return ("");
    size_t len = (cmd.find_first_of(" ") + 1) - cmd.length();
    std::string str(cmd.substr(cmd.find_first_of(" ") + 1, len));
    return (str);
}

bool checkRightsNc(User &user, Serv &server)
{
    if (user.getPass() != server.getPass())
    {
        sendMessageToUser(user.getSocket(), ":irc You didn't registered a Pass to connect to the server. (use /PASS)\r\n");
        return (false);
    }
    else if (user.getUsername().empty())
    {
        sendMessageToUser(user.getSocket(), ":irc You didn't registered a Username. (use /USER)\r\n");
        return (false);
    }
    else if (user.getNickname().empty())
    {
        sendMessageToUser(user.getSocket(), ":irc You didn't registered a Nickname. (use /NICK)\r\n");
        return (false);
    }
    return (true);
}

bool askForNcConnection(User &user, Serv &server, std::map<int, User> &Users)
{
    std::string cmd = getCommand(user.getCmd());
    if (user.getCmd().empty() || user.getEnded() == false)
        return (false);
    if (user.getPass() == "None" && user.getUsername().empty() && user.getNickname().empty())
    {
        std::string str = ":irc Hello! Since you connected via NC, you need to enter the commands /PASS, /USER and /NICK and be validated by the server so you can connect to our server!\r\n";
        sendMessageToUser(user.getSocket(), str);
    }
    if (cmd == "/PASS" || cmd == "PASS")
        user.setPass(getArgs(user.getCmd()));
    else if (cmd == "/NICK" || cmd == "NICK")
        user.setNickname(user.checkForNicknameInUse(Users, getArgs(user.getCmd())));
    else if (cmd == "/USER" || cmd == "USER")
        user.setUsername(user.checkForUsernameInUse(Users, getArgs(user.getCmd())));
    else
        messageServDispatcher(user, 451);
    if (checkRightsNc(user, server) == true && user.getPass() == server.getPass())
    {
        messageServDispatcher(user, 001);
        user.setAuthorization(Users);
        user.setCmd("");
        user.unsetEnd();
        return (true);
    }
    return (false);
}

std::string findUsername(std::string line)
{
    std::string newLine;

    if (line.find(":") < 0)
        return (line);
    else
        newLine = line.substr(line.find(":") + 1);
    return (newLine);
}

int findNextLine(std::string line, int start)
{
    for (int i = start; i < (int)line.size(); i ++)
    {
        if (line[i] == 10 || (line[i] == '\r' && line[i + 1] == '\n'))
            return (i - start);
    }
    return (start);
}

bool ncCommand(User &user, std::map<int, User> &Users, Serv &server)
{
    int indexLine = 0;
    if (user.getCmd().empty())
        return (false);
    while (indexLine < (int)user.getCmd().size())
    {
        std::string singleLine = user.getCmd().substr(indexLine, findNextLine(user.getCmd(), indexLine));
        std::string cmd = getCommand(singleLine);
        if (cmd == "/PASS" || cmd == "PASS")
        {
            user.setPass(getArgs(singleLine));
            if (!user.getPass().empty() && server.getPass().compare(user.getPass()) != 0)
            {
                std::cerr << RED << "User " << user.getUsername() << " tried to connect with a wrong password." << WHT << std::endl;
                messageServDispatcher(user, 464);
            }
            else if (user.getPass().empty())
                std::cerr << RED << "User " << user.getUsername() << " tried to connect without a password." << WHT << std::endl;
        }
        else if (cmd == "/NICK" || cmd == "NICK")
            user.setNickname(user.checkForNicknameInUse(Users, getArgs(singleLine)));
        else if (cmd == "/USER" || cmd == "USER")
            user.setUsername(user.checkForUsernameInUse(Users, findUsername(getArgs(singleLine))));
        indexLine += (int)singleLine.size() + 2;
    }
    if (user.getPass() == server.getPass() && !user.getNickname().empty() && !user.getUsername().empty())
    {
        messageServDispatcher(user, 001);
        user.setAuthorization(Users);
        user.setConnection();
        return (true);
    }
    return (false);
}

std::string parseNcCmd(User &user, std::string cmd)
{
    std::string newCmd = cmd;

    if (cmd.empty())
        return (cmd);
    if (cmd.find("\r\n") != std::string::npos)
    {
        newCmd = cmd.substr(0, cmd.find("\r\n"));
        user.setEnd();
    }
    else if (cmd.find(10) != std::string::npos)
    {
        newCmd = cmd.substr(0, cmd.find(10));
        user.setEnd();
    }
    if (newCmd.data()[0] == '/')
        newCmd = newCmd.substr(1);
    return (newCmd);
}