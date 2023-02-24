/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:22:24 by lfilloux          #+#    #+#             */
/*   Updated: 2023/02/16 16:47:46 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cmds.hpp"

void Cmds::part(User &user, std::map<int, User> &Users, std::list<Channel> &Channels)
{
    std::string args = getArgs(user.getCmd());
    if (args.empty())
    {
        messageServDispatcher(user, 461);
        return ;
    }
    std::list<std::string> channelsName = parseChannels(getArgs(user.getCmd()));
    (*Channels.begin()).checkChan(Users, user, Channels, channelsName);
}