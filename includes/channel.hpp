/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleferra <aleferra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:26:51 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/22 17:17:26 by aleferra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <map>
#include "serv.hpp"
#include "message.hpp"

class User;

class Channel {
	public:
		Channel();
		Channel(std::string nameChannel, User &firstUser);
		~Channel();
		//Channel(const Channel &c);
		Channel &operator=(const Channel &chan);

		void addNewUserToChannel(User &newUser, std::string nameAdd);
		void removeUserToChannel(User &oldUser, std::string nameRemove);
		void addNewOpeToChannel(std::map<int, User> &Users, User &newUser, std::string nameAdd);
		void removeOpeToChannel(std::map<int, User> &Users, User &oldUser, std::string actName);
		
		void banUser(User &u, std::string actName);
		void unbanUser(User &u, std::string actName);
		
		bool isInList(std::string name, std::list<User> li);
		
		User &isUser(std::string name);
		void eraseFromList(std::string name, std::list<User> &li);
		
		std::list<User> &getListUser();
		std::list<User> &getListOpe();
		std::list<User> &getListBan();
		std::list<User> &getListBot();

		std::string	getChannelName();

		void sendMessAllChannel(User &userActu, std::string mess);
		void setChannelName(std::string chan);
		void setBots(std::list<User> &bots);
		

		std::string listUsers(std::map<int, User> &Users);
		void affListOpes();
		void affListUsers();
		void affListBan();
		void affListBot();
		void infoChannel();
		
		void removeUserFromChan(Channel &c, std::map<int, User> &Users, User &user, std::list<Channel> &Channels, std::string channelToQuit);
		void checkUserInChan(std::map<int, User> &Users, User &user, std::list<std::string>::iterator chan, std::list<Channel> &Channels);
		void checkChan(std::map<int, User> &Users, User &user, std::list<Channel> &Channels, std::list<std::string> &channelsName);
		void deleteChan(Channel &c, std::list<Channel> &Channels);

	private:
		std::string		_channelName;
		std::list<User> _UsersChannel;
		std::list<User> _OpesChannel;
		std::list<User> _BanChannel;
        std::list<User> _Bot;
};

	void sendMessageToChan(std::string chan, User &user, std::list<Channel> &Channels);
	bool isInChan(std::list<User> &usersInChan, User &user);

#endif