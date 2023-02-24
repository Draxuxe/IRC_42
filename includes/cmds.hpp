/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 15:41:01 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/23 14:19:42 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_HPP
# define CMDS_HPP

#include "serv.hpp"

enum allCmds{
	JOIN, // Create or join a channel
    NICK, // Set nickname
    PRIVMSG, // Send a msg to someone or a channel
    PART, // Quit a channel
    QUIT, // Quit the server
    CAP,
    PING, // PING PONG
    OP, // Mode (add or remove operators and ban and uunban)
	KICK, //Kick someone
	PASS, //Pass to set
	USER, //Set Username
	BOT, //Bot connection
	NOTHING
};

class Serv;
class User;
class Channel;

class Cmds
{
	public :
		Cmds();
		~Cmds();
		Cmds(const Cmds &cmd);
		Cmds &operator=(const Cmds &cmd);
	
		//Commands functions
		void quit(std::map<int, User> &Users, std::list<Channel> &Channels, User &user, fd_set &use);
		void join(std::map<int, User> &Users, User &user, std::string cmd, std::list<Channel> &Channels, Serv &serv);
		void createNewChannel(User &user, std::string nameChannel, std::list<Channel> &Channels, int pos, Serv &serv);
		void channelAlreadyExist(std::map<int, User> &Users, User &user, Channel &c, Serv &serv);
		void pong(User &user);
		void nick(std::map<int, User> &Users, User &user, std::list<Channel> &Channels);
		void doOp(std::map<int, User> &Users, User &user, Channel &channel, std::string, std::list<std::string> &listNames);
		void op(std::map<int, User> &Users, User &user, std::list<Channel> &Channels);
		void privmsg(User &user, std::map<int, User> &Users, std::list<Channel> &Channels);
        void kick(User &user, std::list<Channel> &channels);
        void doKick(User &u, Channel &c, std::string who, std::string why);
		void part(User &user, std::map<int, User> &Users, std::list<Channel> &Channels);
		void ncConnection(User &user, std::map<int, User> &Users, Serv &serv);
		void errorCmd(User &user, std::map<int, User> &Users, Serv &serv);

		//Utils for the commands
		allCmds getCmdName(std::string cmd);
		std::string getCmdArgs(std::string cmd);
		std::string getChannelName(std::string cmd);
		std::string getArgCmd(std::string cmd);
		std::string getWhy(std::string cmd);
		std::list<std::string> parseChannels(std::string channels);
		void executeCmd(std::map<int ,User> &Users, Serv &serv, int index, std::list<Channel> &Channels, fd_set &use);
		void unsetCmd(User &user);
	
	private :
		std::map<allCmds, std::string> _mymap;
};

#endif