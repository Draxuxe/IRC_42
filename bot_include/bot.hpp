/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleferra <aleferra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:47:29 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/23 16:54:01 by aleferra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <iostream>
#include <map>
#include <list>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <csignal>
#include <ctime>


#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

#define MAX_BUFLEN 4000

class Bot {
    public :
        Bot();
        Bot(char **argv);
        ~Bot();
        
        void ft_aff_ban();
        std::list <std::string>splitMsg(std::string mess, char token);
        
        bool coconnect(char **argv);
        void sendMsg();
        void joinChannel();
        void botUp();
        bool findCmd(std::string mess);
        void sendCmdToChannel();
        bool findBanWord(std::string mess);
        void sendBanToChannel();
        void sendWelcome(std::string mess);
        bool findJoin(std::string mess);
        void sendJoin();
        
        void setServ(std::string info);
        void setPort(std::string info);
        void setPass(std::string info);
        void setName(std::string info);
        void setSocket(int info);
        void setCmd(std::string info);
        void setChannel(std::string info);
        
        std::string getServ();
        std::string getPort();
        std::string getPass();
        std::string getName();
        int getSocket();
        std::string getCmd();
        std::string getChannel();
    
        void help();
        void love();
        void egg();
        bool banWord();
        void lfilloux();
        void aleferra();
        void cmd();
        void sendTheFuckingMess();
        void closeBot();
    
    private :
        int _socketServ;
        std::string _server;
        std::string _port;
        std::string _pass;
        std::string _name;
        std::string _cmd;
        std::string _next;
        std::string _channel;
        std::list <std::string> _msg;
        std::list <std::string> _ban;
        struct timeval _tv;
        long _time;
        bool _newMsg;
        std::string _banName;
        struct addrinfo *res;
        struct addrinfo hints;
};

#endif