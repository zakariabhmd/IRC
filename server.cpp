/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabahmi <zbabahmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 00:27:58 by mboudrio          #+#    #+#             */
/*   Updated: 2024/12/14 03:09:18 by zbabahmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool Server::Signal_status = 0;

void Server::cleanupServer()
{
    Close_filedescriptors();

    for (size_t i = 0; i < Channels.size(); ++i) {
        Channel* channel = Channels[i];

        for (std::vector<Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) {
            Client* client = *it;
            channel->remove_admin(client);
            channel->remove_user(client);
            channel->remove_Invited(client);
        }

        delete channel;
    }

    Channels.clear();

    for (size_t i = 0; i < Clients.size(); ++i) {
        Client* client = Clients[i];

        delete client;
    }

    Clients.clear();
}

void Server::Signal_Handler(int signum)
{
    if (signum == 2)
        std::cout << " Signal " << " SIGINT " << " received. " << std::endl;
    else if (signum == 3 )
        std::cout << " Signal " << " SIGQUIT " << " received. " << std::endl;
    Signal_status = true;
}

void Server::socket_creation()
{
    // socket(domaine , type , protocol);
    Serverfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (Serverfd < 0)
    {
        throw(std::runtime_error(" ---> Socket Creation stage failed "));
    }
}

sockaddr_in Server:: socket_infos()
{
    // 2 - defining server address  i can use setsockopt also for this
    sockaddr_in serveradd;
    std::memset(&serveradd, 0, sizeof(serveradd)); // ensure initialisation and clean memory
    serveradd.sin_family = AF_INET ; // specify addr family;
    serveradd.sin_port = htons(this->Port) ; // specify port number; htons is used to ensure that port numbers are represented in the correct byte order when transmitted over the network
    // why order matters ? -->   When different computers communicate over a network, they need to agree on how multi-byte values (like port numbers and IP addresses) are represented. Since different computer architectures may use different byte orders, a common standard (network byte order) is used to ensure compatibility.
    serveradd.sin_addr.s_addr = INADDR_ANY ; // it is used when we dont want to bind our network to any particular IP and make it listen to all available IPs
    int enabling_reuse = 1;
    int infos_status = setsockopt(Serverfd, SOL_SOCKET, SO_REUSEADDR, &enabling_reuse, sizeof(enabling_reuse));
    if (infos_status < 0)
        throw(std::runtime_error(" ---> failed to make the local address reusable "));

    return serveradd;
}
void Server::socket_non_blocking()
{
    int non_blocking_status = fcntl(Serverfd, F_SETFL, O_NONBLOCK);
    if (non_blocking_status < 0)
    {
        throw(std::runtime_error(" ---> failed to make the socket non blocking "));
    }
}
void Server::socket_Binding(sockaddr_in serveradd)
{
    int bind_status = bind(Serverfd , (struct sockaddr*) &serveradd , sizeof(serveradd));
    if (bind_status < 0)
    {
        throw(std::runtime_error(" ---> Binding stage Failed ..."));
    }
}

void Server::socket_listening()
{
    // 4 - listen for incoming connections
    int listening_status = listen(Serverfd ,  SOMAXCONN);
    if ( listening_status < 0)
    {
        throw(std::runtime_error(" ---> listening stage Failed ..."));
    }
}

void Server::server_socket_polling()
{
    struct pollfd Poll;

    Poll.fd = Serverfd;
    Poll.events = POLLIN;
    Poll.revents = 0;

    fds.push_back(Poll);
}

void Server::client_socket_polling(int client_fd)
{
    struct pollfd NPoll;

    NPoll.fd = client_fd;
    NPoll.events = POLLIN;
    NPoll.revents = 0;
}

void Server::Launching_server(int port, std::string password)
{
    this->Port = port;
    this->Password = password;

    // Socket Operations :
    socket_creation();
    sockaddr_in freemon = socket_infos();
    socket_non_blocking();
    socket_Binding(freemon);
    socket_listening();
    server_socket_polling();

    std::cout << " Your Server Sir is in the Listening mode waiting for incoming connections  ... waiting to accept them" << std::endl;

    Server_cycle();
}

void Server::Server_cycle()
{
    while(get_Signal_Status() == false)
    {
        if((poll(fds.data(), fds.size(), -1) == -1) && get_Signal_Status() == false)
        {
            throw(std::runtime_error(" ---> poll stage failed "));
        }

        size_t i = 0;
        while( i < fds.size())
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == Serverfd)
                    socket_Accepting();
                else
                    socket_receiving(fds[i].fd);
            }
            i++;
        }
    }
    // Close_filedescriptors();
}


void Server::socket_Accepting()
{
    Client *client = new Client();
    socklen_t len = sizeof(client->clientadd);

    int coming_fd = accept(Serverfd, (sockaddr *)&(client->clientadd), &len);

    if (coming_fd == -1)
        throw(std::runtime_error(" ---> accept stage failed "));

    int non_blocking_status = fcntl(coming_fd, F_SETFL, O_NONBLOCK);
    if (non_blocking_status < 0)
        throw(std::runtime_error(" ---> failed to make the socket non blocking "));


    struct pollfd NPoll;

    NPoll.fd = coming_fd;
    NPoll.events = POLLIN;
    NPoll.revents = 0;
    client->setfd(coming_fd);
    client->setIPaddress(inet_ntoa(client->clientadd.sin_addr));
    Clients.push_back(client);
    fds.push_back(NPoll);

    std::cout << " ---> Client connected " << client->getIPaddress() << std::endl;
}

std::string Server::getServerIP()
{
    struct sockaddr_in server_address;
    socklen_t address_len = sizeof(server_address);

    if (getsockname(Serverfd, (struct sockaddr*)&server_address, &address_len) == -1)
    {
        throw(std::runtime_error(" ---> Failed to get server IP address"));
    }

    return std::string(inet_ntoa(server_address.sin_addr));
}



void Server::socket_receiving(int client_fd)
{
    char buffer[1024];
    std::vector<std::string> cmd;
    Client *client = getClient(client_fd);
    memset(buffer, 0, sizeof(buffer));

    if (!client)
        return;

    int r = recv(client_fd, buffer, sizeof(buffer), 0);
    RawData = buffer;
    if (r <= 0)
    {
        std::cerr << " --> receiving stage failed ... " << std::endl;
        // cleanupServer();
        Remove_Client(client_fd);
        close(client_fd);
        return;
        // delete client;
    }
    else
    {
        std::cout << " Received Data :  " << getRawData() << std::endl;
        std::cout << client->get_clientfd() << std::endl;
        process_received_data(getRawData(), client_fd);
    }
}




void Server::process_received_data(const std::string& raw_data, int client_fd)
{
    Client* cli = getClient(client_fd);

    cli->buffer += raw_data;

    size_t pos;
    while ((pos = cli->buffer.find("\r\n")) != std::string::npos || (pos = cli->buffer.find("\n")) != std::string::npos)
    {
        std::string command = cli->buffer.substr(0, pos);

        Parcing_and_Executing(client_fd, command);

        if (cli->buffer[pos] == '\r' && pos + 1 < cli->buffer.size() && cli->buffer[pos + 1] == '\n')
            cli->buffer.erase(0, pos + 2); // Remove "\r\n"
        else
            cli->buffer.erase(0, pos + 1); // Remove "\n"
    }

}

std::vector<std::string>  Server::split_received_Buffer(std::string str)
{
    std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

void Server::Parcing_and_Executing(int fd, std::string buffer)
{
    executing_commands(fd , buffer);
}

Channel* Server::create_channel(Client *cl, std::string name, std::string pass)
{
    std::string serverIP = getServerIP();
    Channel *chan = new Channel(name, pass);
    chan->addAdmin(cl);
    chan->addUser(cl, pass,serverIP);
    chan->getMembers();
    return chan;
}

void Channel::addAdmin(Client* client)
{
        client->setoperatorstatus(true);
        admins.push_back(client);
}

void Channel::addInvited(Client* client) {
    invites.push_back(client);
}

void Server::addChannel(Channel *chan) {
    Channels.push_back(chan);
}

std::vector <std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}
void Server::executing_commands(int fd, std::string &cmd)
{

    size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);

    Client *client = getClient(fd);
    std::vector<std::string> splited_cmd = split_cmd(cmd);

    if (cmd.empty())
        return;
    else if (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass")
    {
        PASS(client, cmd);
    }
    else if (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick")
    {
        NICK(client, cmd);
    }
    else if (splited_cmd[0] == "USER" || splited_cmd[0] == "user")
    {
        USER(client, cmd);
    }
    else if (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit")
    {
        QUIT(client, cmd);
    }
    else if (client->getregistred())
    {
        if (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join")
        {
            JOIN(client, cmd);
        }
        else if (splited_cmd[0] == "WHO" || splited_cmd[0] == "who")
        {
            WHO(client, cmd);
        }
        else if (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick")
        {
            KICK(client, cmd);
        }
        else if (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic")
        {
            TOPIC(client, cmd);
        }
        else if (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite")
        {
            INVITE(client, cmd);
        }
        else if (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode")
        {
            MODE(client, cmd);
        }
        else if (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg")
        {
            PRIVMSG(client , cmd);
        }
        else if (splited_cmd[0] == "PART" || splited_cmd[0] == "part")
        {
            PART(client , cmd);
        }
        else if (splited_cmd[0] == "BOT" || splited_cmd[0] == "bot")
        {
            Bot_call(client, cmd);
        }
        else
        {
            std::string msg_to_reply =  ":" + client->getIPaddress() + ERR_UNKNOWNCOMMAND(client->getnickname(), splited_cmd[0]);
            ssendMessage(msg_to_reply , client->get_clientfd() );
        }
    }
    else if (!client->getregistred() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
    {
        std::string msg_to_reply =  ":" + client->getIPaddress() + ERR_NOTREGISTERED(client->getnickname());
        ssendMessage(msg_to_reply , client->get_clientfd() );
    }
}

Client* Server::findClientByNick(const std::string& nickname)
{
    for(size_t i = 0; i < Clients.size(); i++)
    {
        if (Clients[i]->getnickname() == nickname)
            return Clients[i];
    }
    return NULL;
}

void Server::ssendMessage(std::string message, int destination_fd)
{
    size_t sent = 0;
    size_t msg_len = message.size();
    char *msg = (char *)message.c_str();
    while(sent < msg_len)
    {
        int i = send(destination_fd, msg + sent, msg_len - sent, 0);
        if(i == -1)
            return ;
        sent += i;
    }
}