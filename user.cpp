// #include "channel.hpp"

// std::string	findUsername(std::string msg);
// std::string	findRealname(std::string msg);


// void	user(Server *server, int const client_fd, cmd_struct cmd_infos)
// {
// 	Client&		client		= retrieveClient(server, client_fd);
// 	std::string	username	= findUsername(cmd_infos.message);
// 	std::string realname	= findRealname(cmd_infos.message);

// 	if (username.empty() || realname.empty())
// 		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
// 	else if (client.isRegistrationDone() == true)
// 		addToClientBuffer(server, client_fd, ERR_ALREADYREGISTERED(client.getNickname()));
// 	else
// 	{
// 		client.setUsername(username);
// 		client.setRealname(realname);
// 		client.setNbInfo(+1);
// 	}
// }

// std::string	findUsername(std::string msg)
// {
// 	std::string user;
// 	user.clear();

// 	if (msg.empty() == false && msg[0] == ' ')
// 		msg.erase(0, 1);
// 	if (msg.empty() == false)
// 	{
// 		if (msg.find_first_of(' ') != std::string::npos)
// 			user.insert(0, msg, 0, msg.find(' '));
// 		else
// 			user.insert(0, msg, 0, std::string::npos);
// 	}
// 	return (user);
// }

// std::string	findRealname(std::string msg)
// {
// 	std::string real;
// 	real.clear();

// 	if (msg.empty() == false && msg[0] == ' ')
// 		msg.erase(0, 1);
// 	if (msg.empty())
// 		return (real);

// 	int space_count = 0;
// 	int	begin_pos = 0;
// 	for (size_t i = 0; i < msg.size(); i++)
// 	{
// 		if (msg[i] == ' ')
// 			space_count++;
// 		if (space_count == 3)
// 		{
// 			begin_pos = i;
// 			break;
// 		}
// 	}
// 	if (space_count >= 3)
// 		real.insert(0, msg, begin_pos + 1, std::string::npos);
// 	if (real[0] == ':')
// 		real.erase(0, 1);
// 	return (real);
// }

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "channel.hpp"

// Mock definitions for error and reply messages
#define ERR_NEEDMOREPARAMS(nick, cmd) "461 " + nick + " " + cmd + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(nick) "462 " + nick + " :You may not reregister"

// Function declarations
std::string findUsername(const std::string& msg);
std::string findRealname(const std::string& msg);

// Command handler for USER
void user(Server *server, int const client_fd, cmd_struct cmd_infos) {
    Client& client = retrieveClient(server, client_fd);
    std::string username = findUsername(cmd_infos.message);
    std::string realname = findRealname(cmd_infos.message);

    if (username.empty() || realname.empty()) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
    } else if (client.isRegistrationDone()) {
        addToClientBuffer(server, client_fd, ERR_ALREADYREGISTERED(client.getNickname()));
    } else {
        client.setUsername(username);
        client.setRealname(realname);
        client.setNbInfo(+1);
    }
}

// Extracts the username from the message
std::string findUsername(const std::string& msg) {
    std::stringstream ss(msg);
    std::string username;

    // Skip command keyword and extract the username
    std::string temp;
    if (ss >> temp >> username) {
        // Username should be non-empty and skip if it's one of the placeholders (0 *)
        if (username == "0" || username == "*") {
            username.clear();
        }
    }
    return username;
}

// Extracts the realname from the message
std::string findRealname(const std::string& msg) {
    std::stringstream ss(msg);
    std::string realname, temp;

    // Skip to the fourth parameter
    int paramCount = 0;
    while (ss >> temp && paramCount < 3) {
        paramCount++;
    }
    // Everything after the 3rd space is considered the realname
    std::getline(ss, realname);
    if (!realname.empty() && realname[0] == ':') {
        realname.erase(0, 1);  // Remove leading colon if present
    }
    return realname;
}

// Mock classes and functions
class Client {
private:
    std::string nickname;
    std::string username;
    std::string realname;
    int client_fd;
    bool registration_done = false;
    int nb_info = 0;

public:
    Client(int fd, const std::string& nick) : client_fd(fd), nickname(nick) {}

    std::string getNickname() const { return nickname; }
    std::string getUsername() const { return username; }
    std::string getRealname() const { return realname; }
    bool isRegistrationDone() const { return registration_done; }
    void setNickname(const std::string& nick) { nickname = nick; }
    void setUsername(const std::string& user) { username = user; }
    void setRealname(const std::string& real) { realname = real; }
    void setNbInfo(int value) { nb_info += value; }
    void completeRegistration() { registration_done = true; }
};

class Server {
private:
    std::map<const int, Client> clients;

public:
    std::map<const int, Client>& getClients() { return clients; }
    void addClient(int fd, const Client& client) { clients[fd] = client; }
};

Client& retrieveClient(Server* server, int client_fd) {
    return server->getClients()[client_fd];
}

void addToClientBuffer(Server* server, int client_fd, const std::string& message) {
    std::cout << "Server response to client " << client_fd << ": " << message << std::endl;
}

struct cmd_struct {
    std::string name;
    std::string message;
};

// Main function for testing the USER command
int main() {
    Server server;

    // Adding some clients
    Client client1(1, "User1");
    server.addClient(1, client1);

    Client client2(2, "User2");
    server.addClient(2, client2);

    cmd_struct user_cmd;

    // Test case 1: Successful USER command
    std::cout << "\nTest 1: Valid USER command\n";
    user_cmd.name = "USER";
    user_cmd.message = "User1 0 * :Real Name";
    user(&server, 1, user_cmd);  // Expect successful set of username and realname

    // Test case 2: Missing parameters
    std::cout << "\nTest 2: Missing parameters\n";
    user_cmd.message = "User1 0 *";
    user(&server, 1, user_cmd);  // Expect ERR_NEEDMOREPARAMS

    // Test case 3: Already registered user
    std::cout << "\nTest 3: Already registered user\n";
    client1.completeRegistration();
    user_cmd.message = "User1 0 * :Real Name After Registration";
    user(&server, 1, user_cmd);  // Expect ERR_ALREADYREGISTERED

    // Test case 4: Empty username and realname
    std::cout << "\nTest 4: Empty username and realname\n";
    user_cmd.message = "";
    user(&server, 2, user_cmd);  // Expect ERR_NEEDMOREPARAMS

    // Test case 5: Realname with spaces
    std::cout << "\nTest 5: Realname with spaces\n";
    client2.setNickname("NewNick");
    user_cmd.message = "NewNick 0 * :Real Name With Spaces";
    user(&server, 2, user_cmd);  // Expect success with realname set as "Real Name With Spaces"

    return 0;
}
