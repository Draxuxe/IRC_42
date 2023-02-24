/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nc.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 14:01:10 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/17 15:15:31 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NC_HPP
# define NC_HPP

#include <iostream>
#include <string>
#include "serv.hpp"

class User;
class Serv;

std::string getCommand(std::string line);
std::string getArgs(std::string cmd);
bool askForNcConnection(User &user, Serv &server, std::map<int, User> &Users);
bool askNcConnection(User &user, Serv &server, std::map<int, User> &Users);
bool checkRightsNc(User &user, Serv &server, std::map<int, User> &Users);
std::string parseNcCmd(User &user, std::string cmd);
bool ncCommand(User &user, std::map<int, User> &Users, Serv &server);
int findNextLine(std::string line, int start);

#endif