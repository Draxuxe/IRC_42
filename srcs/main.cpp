/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfilloux <lfilloux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 14:09:21 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/17 12:21:46 by lfilloux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cctype>
#include "../includes/user.hpp"
#include "../includes/serv.hpp"

/*
Parse the Port given as first parameter
return
    true = port valid;
    false = port invalid;
*/
bool    parsePort(char *s)
{
    for (int i=0; s[i]; i++)
    {
        if (!std::isdigit(s[i]))
            return (false);
    }
    return (true);
}

bool ft_parse(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Wrong arg numbers: './ircserv <port> <pwd>'." << std::endl;
        return (false);
    }
    if (!parsePort(argv[1]))
    {
        std::cerr << "The port must be correct and only numbers." << std::endl;
        return (false);
    }
    if (!argv[2][0])
    {
        std::cerr << "The password cannot be empty." << std::endl;
        return (false);
    }
    return (true);    
}

int main(int ac, char **av)
{
    if (!ft_parse(ac, av))
        return(false);
    Serv server(av[2], std::atoi(av[1]));
    //Launching Server
    server.launchServ();
    return (0);
}