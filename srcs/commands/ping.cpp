/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:21:16 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/13 15:21:32 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

void Cmds::pong(User &user)
{
    std::string response;
    std::string args = getCmdArgs(user.getCmd());
    if (args.empty())
    {
        messageServDispatcher(user, 409);
        return ;
    }
    response = ":irc PONG irc " + args + "\r\n";
    sendMessageToUser(user.getSocket(), response);
}