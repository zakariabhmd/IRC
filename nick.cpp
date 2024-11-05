// #include "channel.hpp"

// std::string	retrieveNickname(std::string msg_to_parse);
// static bool	containsInvalidCharacters(std::string nickname);

// void	nick(Server *server, int const client_fd, cmd_struct cmd_infos)
// {
// 	std::string nickname	= retrieveNickname(cmd_infos.message);
// 	Client&		client		= retrieveClient(server, client_fd);

// 	if (client.isRegistrationDone() == false)
// 	{
// 		client.setNickname(nickname);
// 		client.setOldNickname(nickname);
// 		client.setNbInfo(+1);
// 	}

// 	if (nickname.empty()) {
// 		addToClientBuffer(server, client_fd, ERR_NONICKNAMEGIVEN(client.getNickname()));
// 		if (client.isRegistrationDone() == false)
// 			client.setNbInfo(-1);
// 	}
// 	else if (containsInvalidCharacters(nickname)) {
// 		addToClientBuffer(server, client_fd,  ERR_ERRONEUSNICKNAME(client.getNickname(), nickname));
// 		if (client.isRegistrationDone() == false)
// 			client.setNbInfo(-1);
// 	}
// 	else if (isAlreadyUsed(server, client_fd, nickname) == true) {
// 			addToClientBuffer(server, client_fd, ERR_NICKNAMEINUSE(client.getNickname(), nickname));
// 		if (client.isRegistrationDone() == false)
// 			client.setNbInfo(-1);
// 	} else {

// 		if (client.isRegistrationDone() == true)
// 		{
// 			client.setOldNickname(client.getNickname());
// 			std::cout << "[Server] Nickname change registered. Old nickname is now : " << client.getOldNickname() << std::endl;

// 			client.setNickname(nickname);

// 		}
// 	}
// 	addToClientBuffer(server, client_fd, RPL_NICK(client.getOldNickname(), client.getUsername(), client.getNickname()));
// }

// std::string	retrieveNickname(std::string msg_to_parse)
// {
// 	std::string nickname;

// 	nickname.clear();
// 	if (msg_to_parse.empty())
// 		return (nickname);
// 	if (msg_to_parse.empty() == false && msg_to_parse[0] == ' ')
// 		msg_to_parse.erase(0, 1);
// 	if (msg_to_parse.empty() == false && msg_to_parse.find(' '))
// 	{
// 		char *str = const_cast<char *>(msg_to_parse.data());
// 		nickname = strtok(str, " ");
// 	}
// 	if (nickname.empty())
// 		nickname.clear();
// 	return (nickname);
// }

// static bool	isForbidden(char c)
// {
// 	if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!'
// 		|| c == '@' || c == '.')
// 		return (true);
// 	else
// 		return (false);
// }

// static bool	containsInvalidCharacters(std::string nickname)
// {
// 	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
// 		return (true);

// 	for (size_t i = 0; i < nickname.size(); i++) {
// 			if (isForbidden(nickname[i]) == true)
// 				return (true);
// 	}
// 	return (false);
// }

// bool	isAlreadyUsed(Server *server, int client_fd, std::string new_nickname)
// {
// 	std::map<const int, Client>&			client_list	= server->getClients();
// 	std::map<const int, Client>::iterator	client		= client_list.begin();

// 	while (client != client_list.end())
// 	{
// 		if (client->second.getClientFd() != client_fd \
// 			&& client->second.getNickname() == new_nickname)
// 			return (true);
// 		client++;
// 	}
// 	return (false);
// }

// #include <iostream>
// #include <string>
// #include <map>
// #include "channel.hpp"

// // Mock definitions for error and reply messages
// #define ERR_NONICKNAMEGIVEN(nick) "431 " + nick + " :No nickname given"
// #define ERR_ERRONEUSNICKNAME(nick, attempted) "432 " + nick + " :Erroneous nickname " + attempted
// #define ERR_NICKNAMEINUSE(nick, attempted) "433 " + nick + " :Nickname is already in use " + attempted
// #define RPL_NICK(old_nick, user, new_nick) ":" + old_nick + "!" + user + "@localhost NICK " + new_nick

// // Mock Server class
// class Server {
// private:
//     std::map<const int, Client> clients;
// public:
//     std::map<const int, Client>& getClients() { return clients; }
//     void addClient(int fd, const Client& client) { clients[fd] = client; }
// };

// // Mock Client class
// class Client {
// private:
//     std::string nickname;
//     std::string old_nickname;
//     std::string username;
//     int client_fd;
//     bool registration_done;
//     int nb_info;

// public:
//     Client(int fd, const std::string& user) : client_fd(fd), username(user), registration_done(false), nb_info(0) {}

//     std::string getNickname() const { return nickname; }
//     std::string getOldNickname() const { return old_nickname; }
//     std::string getUsername() const { return username; }
//     int getClientFd() const { return client_fd; }
//     bool isRegistrationDone() const { return registration_done; }
//     void setNickname(const std::string& nick) { nickname = nick; }
//     void setOldNickname(const std::string& nick) { old_nickname = nick; }
//     void setNbInfo(int value) { nb_info += value; }
//     void completeRegistration() { registration_done = true; }
// };

// // Mock functions to simulate server behavior
// Client& retrieveClient(Server* server, int client_fd) {
//     return server->getClients()[client_fd];
// }

// void addToClientBuffer(Server* server, int client_fd, const std::string& message) {
//     std::cout << "Server response to client " << client_fd << ": " << message << std::endl;
// }

// // Main function to test the NICK command
// int main() {
//     Server server;

//     // Adding some clients
//     Client client1(1, "User1");
//     client1.setNickname("User1");
//     server.addClient(1, client1);

//     Client client2(2, "User2");
//     client2.setNickname("User2");
//     server.addClient(2, client2);

//     // Mock command structure
//     cmd_struct nick_cmd;

//     // Test cases
//     std::cout << "\nTest 1: Valid nickname change\n";
//     nick_cmd.message = "NewNick";
//     nick(&server, 1, nick_cmd);  // Expect success, nickname change

//     std::cout << "\nTest 2: Nickname already in use\n";
//     nick_cmd.message = "User2";
//     nick(&server, 1, nick_cmd);  // Expect ERR_NICKNAMEINUSE

//     std::cout << "\nTest 3: Erroneous nickname with invalid characters\n";
//     nick_cmd.message = "Invalid!Nick";
//     nick(&server, 1, nick_cmd);  // Expect ERR_ERRONEUSNICKNAME

//     std::cout << "\nTest 4: Empty nickname\n";
//     nick_cmd.message = "";
//     nick(&server, 1, nick_cmd);  // Expect ERR_NONICKNAMEGIVEN

//     std::cout << "\nTest 5: Valid nickname after registration\n";
//     client1.completeRegistration();  // Mark client1 as registered
//     nick_cmd.message = "RegisteredNick";
//     nick(&server, 1, nick_cmd);  // Expect successful nickname change

//     return 0;
// }

#include <iostream>
#include <string>
#include <map>
#include <cstring>

// Mock definitions for error and reply messages
#define ERR_NONICKNAMEGIVEN(nick) "431 " + nick + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick, attempted) "432 " + nick + " :Erroneous nickname " + attempted
#define ERR_NICKNAMEINUSE(nick, attempted) "433 " + nick + " :Nickname is already in use " + attempted
#define RPL_NICK(old_nick, user, new_nick) ":" + old_nick + "!" + user + "@localhost NICK " + new_nick

// Mock Server class
class Server {
private:
    std::map<int, Client> clients;
public:
    std::map<int, Client>& getClients() { return clients; }
    void addClient(int fd, const Client& client) { clients[fd] = client; }
};

// Mock Client class
class Client {
private:
    std::string nickname;
    std::string old_nickname;
    std::string username;
    int client_fd;
    bool registration_done;

public:
    Client(int fd, const std::string& user) : client_fd(fd), username(user), registration_done(false) {}

    std::string getNickname() const { return nickname; }
    std::string getOldNickname() const { return old_nickname; }
    std::string getUsername() const { return username; }
    int getClientFd() const { return client_fd; }
    bool isRegistrationDone() const { return registration_done; }
    void setNickname(const std::string& nick) { old_nickname = nickname; nickname = nick; }
    void completeRegistration() { registration_done = true; }
};

// Mock functions to simulate server behavior
Client& retrieveClient(Server* server, int client_fd) {
    return server->getClients()[client_fd];
}

void addToClientBuffer(Server* server, int client_fd, const std::string& message) {
    std::cout << "Server response to client " << client_fd << ": " << message << std::endl;
}

// Function to check if a nickname has invalid characters
bool containsInvalidCharacters(const std::string& nickname) {
    if (nickname.empty() || nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
        return true;

    for (char c : nickname) {
        if (isspace(c) || c == ',' || c == '*' || c == '?' || c == '!' || c == '@' || c == '.')
            return true;
    }
    return false;
}

// Function to check if a nickname is already used by another client
bool isAlreadyUsed(Server* server, int client_fd, const std::string& new_nickname) {
    const auto& clients = server->getClients();
    for (const auto& [fd, client] : clients) {
        if (fd != client_fd && client.getNickname() == new_nickname) {
            return true;
        }
    }
    return false;
}

// The NICK command handler
void nick(Server *server, int client_fd, const std::string& message) {
    Client& client = retrieveClient(server, client_fd);
    std::string nickname = message;

    if (nickname.empty()) {
        addToClientBuffer(server, client_fd, ERR_NONICKNAMEGIVEN(client.getNickname()));
        return;
    }

    if (containsInvalidCharacters(nickname)) {
        addToClientBuffer(server, client_fd, ERR_ERRONEUSNICKNAME(client.getNickname(), nickname));
        return;
    }

    if (isAlreadyUsed(server, client_fd, nickname)) {
        addToClientBuffer(server, client_fd, ERR_NICKNAMEINUSE(client.getNickname(), nickname));
        return;
    }

    client.setNickname(nickname);
    addToClientBuffer(server, client_fd, RPL_NICK(client.getOldNickname(), client.getUsername(), client.getNickname()));
}

// Main function to test the NICK command
int main() {
    Server server;

    // Adding some clients
    Client client1(1, "User1");
    client1.setNickname("User1");
    server.addClient(1, client1);

    Client client2(2, "User2");
    client2.setNickname("User2");
    server.addClient(2, client2);

    // Test cases
    std::cout << "\nTest 1: Valid nickname change\n";
    nick(&server, 1, "NewNick");  // Expect success, nickname change

    std::cout << "\nTest 2: Nickname already in use\n";
    nick(&server, 1, "User2");  // Expect ERR_NICKNAMEINUSE

    std::cout << "\nTest 3: Erroneous nickname with invalid characters\n";
    nick(&server, 1, "Invalid!Nick");  // Expect ERR_ERRONEUSNICKNAME

    std::cout << "\nTest 4: Empty nickname\n";
    nick(&server, 1, "");  // Expect ERR_NONICKNAMEGIVEN

    std::cout << "\nTest 5: Valid nickname after registration\n";
    client1.completeRegistration();  // Mark client1 as registered
    nick(&server, 1, "RegisteredNick");  // Expect successful nickname change

    return 0;
}
