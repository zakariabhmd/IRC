// #include "channel.hpp"
// #include <iostream>
// #include <string>

// // Mock definitions for error messages
// #define ERR_NEEDMOREPARAMS(nickname, command) "461 " + nickname + " :Not enough parameters for " + command
// #define ERR_PASSWDMISMATCH(nickname) "464 " + nickname + " :Password mismatch"
// #define FAILURE 0
// #define SUCCESS 1

// std::string	retrievePassword(std::string msg_to_parse);

// int		pass(Server *server, int const client_fd, cmd_struct cmd_infos)
// {
// 	Client&		client		= retrieveClient(server, client_fd);
// 	std::string	password	= retrievePassword(cmd_infos.message);


// 	if (cmd_infos.message.empty() == true || password.empty() == true)
// 	{
// 		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
// 		return (FAILURE);
// 	}
// 	else if (server->getPassword() != password)
// 	{
// 		addToClientBuffer(server, client_fd, ERR_PASSWDMISMATCH(client.getNickname()));
// 		if (client.isRegistrationDone() == false)
// 			client.setNbInfo(-1);
// 		return (FAILURE);
// 	}
// 	else
// 	{
// 		client.setNbInfo(+1);
// 		return (SUCCESS);
// 	}
// }

// std::string	retrievePassword(std::string msg_to_parse)
// {
// 	std::string	password;
// 	size_t i = 0;

// 	password.clear();

// 	while (msg_to_parse[i] && msg_to_parse[i] == ' ')
// 		i++;
// 	while (msg_to_parse[i] && msg_to_parse[i] != ' ')
// 		password += msg_to_parse[i++];
// 	return (password);
// }


// // Mock Server class
// class Server {
// private:
//     std::string password;
// public:
//     Server(const std::string& pass) : password(pass) {}
//     std::string getPassword() const { return password; }
// };

// // Mock Client class
// class Client {
// private:
//     std::string nickname;
//     bool registrationDone;
//     int nbInfo;
// public:
//     Client(const std::string& nick) : nickname(nick), registrationDone(false), nbInfo(0) {}

//     std::string getNickname() const { return nickname; }
//     bool isRegistrationDone() const { return registrationDone; }
//     void setNbInfo(int value) { nbInfo += value; }
//     int getNbInfo() const { return nbInfo; }
// };

// // Mock cmd_struct for command information
// struct cmd_struct {
//     std::string name;
//     std::string message;
// };

// // Mock functions to simulate server behavior
// Client& retrieveClient(Server* server, int client_fd) {
//     static Client client("TestUser"); // Static to persist across calls for simplicity
//     return client;
// }

// void addToClientBuffer(Server* server, int client_fd, const std::string& message) {
//     std::cout << "Server response to client " << client_fd << ": " << message << std::endl;
// }

// // Your existing code: retrievePassword and pass function
// std::string retrievePassword(std::string msg_to_parse) {
//     std::string password;
//     size_t i = 0;

//     password.clear();
//     while (msg_to_parse[i] && msg_to_parse[i] == ' ') i++;
//     while (msg_to_parse[i] && msg_to_parse[i] != ' ') password += msg_to_parse[i++];
//     return password;
// }

// int pass(Server* server, int const client_fd, cmd_struct cmd_infos) {
//     Client& client = retrieveClient(server, client_fd);
//     std::string password = retrievePassword(cmd_infos.message);

//     if (cmd_infos.message.empty() || password.empty()) {
//         addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
//         return FAILURE;
//     } else if (server->getPassword() != password) {
//         addToClientBuffer(server, client_fd, ERR_PASSWDMISMATCH(client.getNickname()));
//         if (!client.isRegistrationDone()) {
//             client.setNbInfo(-1);
//         }
//         return FAILURE;
//     } else {
//         client.setNbInfo(+1);
//         return SUCCESS;
//     }
// }

// // Main function to test
// int main() {
//     Server server("correct_password"); // Server with a set password
//     int client_fd = 1; // Mock client file descriptor

//     // Test cases
//     cmd_struct test1 = {"PASS", "correct_password"};
//     std::cout << "\nTest 1: Correct password\n";
//     pass(&server, client_fd, test1); // Expect SUCCESS

//     cmd_struct test2 = {"PASS", "wrong_password"};
//     std::cout << "\nTest 2: Wrong password\n";
//     pass(&server, client_fd, test2); // Expect FAILURE with ERR_PASSWDMISMATCH

//     cmd_struct test3 = {"PASS", ""};
//     std::cout << "\nTest 3: Missing password\n";
//     pass(&server, client_fd, test3); // Expect FAILURE with ERR_NEEDMOREPARAMS

//     cmd_struct test4 = {"PASS", " "};
//     std::cout << "\nTest 4: Blank password\n";
//     pass(&server, client_fd, test4); // Expect FAILURE with ERR_NEEDMOREPARAMS

//     return 0;
// }

#include <iostream>
#include <string>

// Mock definitions for error messages
#define ERR_NEEDMOREPARAMS(nickname, command) "461 " + nickname + " :Not enough parameters for " + command
#define ERR_PASSWDMISMATCH(nickname) "464 " + nickname + " :Password mismatch"
#define FAILURE 0
#define SUCCESS 1

// Mock Server class
class Server {
private:
    std::string password;
public:
    Server(const std::string& pass) : password(pass) {}
    std::string getPassword() const { return password; }
};

// Mock Client class
class Client {
private:
    std::string nickname;
    bool registrationDone;
    int nbInfo;
public:
    Client(const std::string& nick) : nickname(nick), registrationDone(false), nbInfo(0) {}

    std::string getNickname() const { return nickname; }
    bool isRegistrationDone() const { return registrationDone; }
    void setNbInfo(int value) { nbInfo += value; }
    int getNbInfo() const { return nbInfo; }
};

// Mock cmd_struct for command information
struct cmd_struct {
    std::string name;
    std::string message;
};

// Mock functions to simulate server behavior
Client& retrieveClient(Server* server, int client_fd) {
    static Client client("TestUser"); // Static to persist across calls for simplicity
    return client;
}

void addToClientBuffer(Server* server, int client_fd, const std::string& message) {
    std::cout << "Server response to client " << client_fd << ": " << message << std::endl;
}

// Function to extract password from message
std::string retrievePassword(std::string msg_to_parse) {
    std::string password;
    size_t i = 0;

    while (msg_to_parse[i] && msg_to_parse[i] == ' ')
        i++;
    while (msg_to_parse[i] && msg_to_parse[i] != ' ')
        password += msg_to_parse[i++];

    return password;
}

// Function to process the PASS command
int pass(Server *server, int const client_fd, cmd_struct cmd_infos) {
    Client& client = retrieveClient(server, client_fd);
    std::string password = retrievePassword(cmd_infos.message);

    if (cmd_infos.message.empty() || password.empty()) {
        addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
        return FAILURE;
    } else if (server->getPassword() != password) {
        addToClientBuffer(server, client_fd, ERR_PASSWDMISMATCH(client.getNickname()));
        if (!client.isRegistrationDone()) {
            client.setNbInfo(-1);
        }
        return FAILURE;
    } else {
        client.setNbInfo(+1);
        return SUCCESS;
    }
}

// Main function to test
int main() {
    Server server("correct_password"); // Server with a set password
    int client_fd = 1; // Mock client file descriptor

    // Test cases
    cmd_struct test1 = {"PASS", "correct_password"};
    std::cout << "\nTest 1: Correct password\n";
    pass(&server, client_fd, test1); // Expect SUCCESS

    cmd_struct test2 = {"PASS", "wrong_password"};
    std::cout << "\nTest 2: Wrong password\n";
    pass(&server, client_fd, test2); // Expect FAILURE with ERR_PASSWDMISMATCH

    cmd_struct test3 = {"PASS", ""};
    std::cout << "\nTest 3: Missing password\n";
    pass(&server, client_fd, test3); // Expect FAILURE with ERR_NEEDMOREPARAMS

    cmd_struct test4 = {"PASS", " "};
    std::cout << "\nTest 4: Blank password\n";
    pass(&server, client_fd, test4); // Expect FAILURE with ERR_NEEDMOREPARAMS

    return 0;
}
