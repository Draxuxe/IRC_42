/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 10:24:42 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/23 15:41:40 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <string>
#include "user.hpp"

class User;
class Channel;
class Serv;

void messageServDispatcher(User &user, int code);
void sendMessageToUser(int userSocket, std::string str);
void sendMessageToAll(std::map<int, User> &Users, User &user, std::string str);
void sendMessagesToAllChannel(Channel &channel, std::string message);
void sendMessageToBot(std::list<User> &bots, std::string mess);
void readMessageClient(std::map<int, User> &Users, User &user, int index, fd_set &use, Serv &serv);
void announceOnChannel(Channel &channel, std::string message);
std::string makeCommandOutput(User &user, std::string cmdName, std::string channelToQuit);


#endif