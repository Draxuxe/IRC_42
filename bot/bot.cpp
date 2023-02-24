/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleferra <aleferra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 17:57:34 by aleferra          #+#    #+#             */
/*   Updated: 2023/02/23 16:55:05 by aleferra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bot_include/bot.hpp"

bool end = false;

Bot::Bot(char **argv)
{
    this->setServ(argv[1]);
    this->setPort(argv[2]);
    this->setPass(argv[3]);
    this->setName(argv[4]);
	res = NULL;
	_tv.tv_sec = 0;
	_tv.tv_usec = 0;
	_newMsg = false;
}

Bot::~Bot()
{
	freeaddrinfo(res);
    _msg.clear();
	return ;
}

void	Bot::ft_aff_ban()
{	
	std::list<std::string>::iterator ite;
	for (ite = _ban.begin(); ite != _ban.end(); ite++)
		std::cout << (*ite) << std::endl;
}
	
void    Bot::sendMsg()
{
    
    _msg.push_back("PASS " + this->getPass() + "\r\n");
    _msg.push_back("CAP LS 302\r\n");
    _msg.push_back("NICK " + this->getName() + "\r\n");
    _msg.push_back("USER " + this->getName() + " 0 * :" + this->getName() + "\r\n");
    _msg.push_back("BOT " + this->getName() + "\r\n");
	sendTheFuckingMess();
	_msg.clear();
}

bool	Bot::coconnect(char **argv)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(argv[1], argv[2], &hints, &res) != 0) {
		std::cerr << "Can't get address." << std::endl;
		return (false);
	}
	setSocket(socket(res->ai_family, res->ai_socktype, res->ai_protocol));
	if (this->getSocket() == -1) {
		std::cerr << "Socket is invalid." << std::endl;
		return (false);
	}
	if (connect(this->getSocket(), res->ai_addr, (int)res->ai_addrlen) != 0) {
		std::cerr << "Can not connect to server." << std::endl;
		close(this->getSocket());
		this->setSocket(-1);
		return (false);
	}
	return (true);
}

void	Bot::joinChannel()
{
    _msg.push_back("JOIN " + _channel + "\r\n");
	_newMsg = true;
}

bool	Bot::findCmd(std::string mess)
{
	_next = "";
	if (mess.find('#') == std::string::npos)
		return (false);
	mess = mess.substr(mess.find("#"));
	if (mess.find('!') == std::string::npos)
		return (false);
	std::string channel = mess.substr(mess.find("#"));
	channel = channel.substr(0, channel.find(" "));
	this->setChannel(channel);
	std::string cmd = mess.substr(mess.find("!"));
	if (cmd.find(" ") == std::string::npos)
		this->setCmd(cmd.substr(0, cmd.length() - 1));
	else
	{
		_next = cmd.substr(cmd.find(" ") + 1);
		this->setCmd(cmd.substr(0, cmd.find(" ")));
	}
	return (true);
}

void	Bot::sendCmdToChannel()
{
	if (this->getCmd().compare("!help") == 0)
		this->help();
	else if(this->getCmd().compare("!love") == 0)
		this->love();
	else if(this->getCmd().compare("!ban-word") == 0)
		this->banWord();
	else if(this->getCmd().compare("!aleferra") == 0)
		this->aleferra();
	else if(this->getCmd().compare("!lfilloux") == 0)
		this->lfilloux();
	else if(this->getCmd().compare("!cmd") == 0)
		this->cmd();	
	else if(this->getCmd().compare("!egg") == 0)
		this->egg();
	this->setCmd("");
	this->setChannel("");
}

void	Bot::egg()
{
	
	std::string start = "PRIVMSG " + this->getChannel() + " :";
	_msg.push_back(start + "8=====/)\r\n");
	_newMsg = true;
}

std::list<std::string>	Bot::splitMsg(std::string msg, char token)
{
	size_t start;
	std::list<std::string> newSplit;
	for (size_t i = 0; i < msg.size(); i++) {
        while (msg[i] == token && i < msg.size())
			i++;
		if (i == 0)
			start = i;
		else
			start = i - 1;
		while (msg[i] != token && i < msg.size())
			i++;
		newSplit.push_back(msg.substr(start, i - start));
		while (msg[i] == token && i < msg.size())
			i++;
    }
	std::list<std::string>::iterator ite;
	return newSplit;
}


bool	Bot::findBanWord(std::string msg)
{
	if (msg.empty())
		return (false);
	if (msg.find("PRIVMSG") == std::string::npos)
		return (false);
	msg = msg.substr(0, msg.size() - 1);
	if (msg.find("#") != std::string::npos)
	{
		std::string channel = msg.substr(msg.find("#"));
		if (channel.find(" ") != std::string::npos)
			_channel = channel.substr(0, channel.find(" "));
	}
	if (msg.find("!") != std::string::npos || msg.find("@") != std::string::npos)
	{
		std::string banName = msg.substr(msg.find("!") + 1);
		_banName = banName.substr(0, banName.find("@"));
	}
	if (_ban.size() <= 0)
		return (false);
	if (msg.find_last_of(":") != std::string::npos)
		msg = msg.substr(msg.find_last_of(":") + 1);
	std::list<std::string> _splitmsg = splitMsg(msg, ' ');
	std::list<std::string>::iterator ite; 
	std::list<std::string>::iterator iteBis; 
	for (ite = _splitmsg.begin(); ite != _splitmsg.end(); ite++)
	{
		for (iteBis = _ban.begin(); iteBis != _ban.end(); iteBis++)
		{
			if ((*ite).compare(*iteBis) == 0)
				return (true);
		}
	}
	return (false);
}

void	Bot::sendBanToChannel()
{
	std::cout << getChannel() << " " << _banName << std::endl;
	_msg.push_back("MODE " + this->getChannel() + " +b " + _banName + "\r\n");
	_newMsg = true;
}

bool	Bot::findJoin(std::string msg)
{
	msg = msg.substr(0, msg.size() - 1);
	if (msg.find("JOIN") == std::string::npos)
		return (false);
	if (msg.compare(0, 4, "JOIN") == 0)
	{	
		_channel = msg.substr(msg.find("#") + 1, msg.find(" ") - msg.find("#") - 1);
		return (true);
	}
	sendWelcome(msg);
	return (false);
}

void	Bot::sendJoin()
{
	_msg.push_back("JOIN " + _channel + "\r\n");
	_newMsg = true;
}

void	Bot::sendWelcome(std::string msg)
{
	_channel = msg.substr(msg.find("#"));
	_channel = _channel.substr(0, _channel.find("\r"));
	_msg.push_back("PRIVMSG " + _channel + " :Welcome to the channel, I'm the Bot, to know what command I can do use: [!help].\r\n");
	_newMsg = true;
}

void	Bot::sendTheFuckingMess()
{
	std::list <std::string>::iterator ite; 
	for(ite = _msg.begin(); ite != _msg.end(); ite++)
	{
		send(this->getSocket(), (*ite).data(), (*ite).size(), 0);
		usleep(1000);
	}
}

void sigNcHandler(int sig, siginfo_t *info, void *context)
{
    (void) info;
    (void) context;
    (void) sig;
    end = true;
}

void setSignal(void)
{
    struct sigaction sigNc;
    
    memset(&sigNc, 0, sizeof(sigNc));
    sigaddset(&sigNc.sa_mask, SIGKILL);
    sigNc.sa_sigaction = &sigNcHandler;
    sigaction(SIGINT, &sigNc, NULL);
}

void Bot::closeBot()
{
	_msg.clear();
	_ban.clear();
	_server.clear();
	_port.clear();
	_pass.clear();
	_name.clear();
	_cmd.clear();
	_next.clear();
	_channel.clear();
	_banName.clear();
	free(res);
	exit(1);
}

void	Bot::botUp()
{
	int		iRecv = 1;
	char	recvbuff[MAX_BUFLEN];
	fd_set	use;
	fd_set	writ;
	fd_set	read;
	
	FD_ZERO(&use);
	FD_SET(this->getSocket(), &use);
	setSignal();
	do {
		writ = use;
		read = use;	
		if (select(FD_SETSIZE, &read, &writ, NULL, &_tv) == -1)
		{
			std::cerr << "select() error" << std::endl;
            return;
		}
		if (end == true)
			closeBot();
		if (FD_ISSET(this->getSocket(), &read))
		{
			memset(&recvbuff, '\0', sizeof(recvbuff));
			iRecv = recv(this->getSocket(), recvbuff, sizeof(recvbuff), 0);
			if (iRecv > 0)
			{
				std::string mess = recvbuff;
				std::list <std::string> messList = splitMsg(mess, '\n');
				for (std::list<std::string>::iterator ite = messList.begin(); ite != messList.end(); ite++)
				{
					if (findJoin(*ite) == true)
						sendJoin();
					if (findBanWord(*ite) == true)
						sendBanToChannel();
					if (findCmd(*ite) == true)
						sendCmdToChannel();
				}
			}
			else if (iRecv == 0)
				std::cout <<  "End of connection." << std::endl;
			else
				std::cout << "Revc failed." << std::endl;
		}
		if (FD_ISSET(this->getSocket(), &writ))
		{
			if (_newMsg == true)
			{
				sendTheFuckingMess();
				_newMsg = false;
			}
			_msg.clear();
		}
	} while (iRecv > 0);
	close(this->getSocket());
}

void Bot::setServ(std::string info)
{
    this->_server = info;
}

void Bot::setPort(std::string info)
{
    this->_port = info;
}

void Bot::setPass(std::string info)
{
    this->_pass = info;
}

void Bot::setName(std::string info)
{
    this->_name = info;
}

void Bot::setSocket(int info)
{
    this->_socketServ = info;
}

void Bot::setCmd(std::string info)
{
    this->_cmd = info;
}

void Bot::setChannel(std::string info)
{
    this->_channel = info;
}

std::string Bot::getServ(){return this->_server;}
std::string Bot::getPort(){return this->_port;};
std::string Bot::getPass(){return this->_pass;};
std::string Bot::getName(){return this->_name;};
int Bot::getSocket(){return this->_socketServ;};
std::string Bot::getChannel(){return this->_channel;};
std::string Bot::getCmd(){return this->_cmd;};

void	Bot::help()
{
	_msg.push_back("PRIVMSG " + this->getChannel() + " :My commands:\r\n");
	_msg.push_back("PRIVMSG " + this->getChannel() + " :-!cmd [List server commands].\r\n");
	_msg.push_back("PRIVMSG " + this->getChannel() + " :-!help [All bot commands].\r\n");
	_msg.push_back("PRIVMSG " + this->getChannel() + " :-!love [Do love <3].\r\n");
	_msg.push_back("PRIVMSG " + this->getChannel() + " :-!ban-word {banned word} [If you use those words you are automatically banned from the channel, limit 3 words].\r\n");
	_msg.push_back("PRIVMSG " + this->getChannel() + " :-!lfilloux ['~'].\r\n");
	_msg.push_back("PRIVMSG " + this->getChannel() + " :-!aleferra [-_-].\r\n");
	_newMsg = true;
}

void	Bot::love()
{
	
	std::string start = "PRIVMSG " + this->getChannel() + " :";
	_msg.push_back(start + " ,d88b d88b.\r\n");
	_msg.push_back(start + "d88888v8888b.\r\n");
	_msg.push_back(start + "8888888888888\r\n");
	_msg.push_back(start + "'Y888888888P\r\n");
	_msg.push_back(start + "  'Y88888P\r\n");
	_msg.push_back(start + "   'Y888P\r\n");
	_msg.push_back(start + "     'Y'\r\n");
	_newMsg = true;
}

void	Bot::lfilloux()
{
	std::string start = "PRIVMSG " + this->getChannel() + " :";
	_msg.push_back(start + "This men is super-man, if you have a problem HE is THE answer.\r\n");
	_newMsg = true;
}

void	Bot::aleferra()
{
	std::string start = "PRIVMSG " + this->getChannel() + " :";
	_msg.push_back(start + "Sometimes he is a bit aigris but what a good dev and friend, HE is able of doing ANYTHING!\r\n");
	_newMsg = true;
}

bool	Bot::banWord()
{
	_next = _next.substr(0,  _next.length() - 1);
	if (_next.length() <= 0)
		return (false);
	if (_ban.size() > 3)
		return (false);
	_ban.push_back(_next);
	return (true);
}

void	Bot::cmd()
{
	std::string start = "PRIVMSG " + this->getChannel() + " :";
	_msg.push_back(start + "All commands of our server:\r\n");
	_msg.push_back(start + "/JOIN #[Channel name] -> Join a channel.\r\n");
	_msg.push_back(start + "/NICK [New nickname] -> Change your nickname.\r\n");
	_msg.push_back(start + "/PRIVMSG [Contact] [Message] -> Private message.\r\n");
	_msg.push_back(start + "/QUIT -> Quit the Server.\r\n");
	_msg.push_back(start + "/PART #[Channel name] -> Quit channel.\r\n");
	_msg.push_back(start + "/KICK [Nickname] [Reason] -> Kick guys.\r\n");
	_msg.push_back(start + "/BAN [Nickname] -> Kick and forbidden acces to the channel (ban).\r\n");
	_newMsg = true;
}

int main(int argc, char **argv)
{
    if (argc < 4 || argc > 5)
	{
		std::cout << "[IP] [PORT] [PASS] [BOTNAME]" << std::endl;
        return 1;
	}
    Bot happy(argv);
    if (happy.coconnect(argv) == 0)
		return (-1);
    happy.sendMsg();
	happy.botUp();
    return 0;
}