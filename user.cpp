#include "channel.hpp"

std::string	findUsername(std::string msg);
std::string	findRealname(std::string msg);


void	user(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client&		client		= retrieveClient(server, client_fd);
	std::string	username	= findUsername(cmd_infos.message);
	std::string realname	= findRealname(cmd_infos.message);

	if (username.empty() || realname.empty())
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
	else if (client.isRegistrationDone() == true)
		addToClientBuffer(server, client_fd, ERR_ALREADYREGISTERED(client.getNickname()));
	else
	{
		client.setUsername(username);
		client.setRealname(realname);
		client.setNbInfo(+1);
	}
}

std::string	findUsername(std::string msg)
{
	std::string user;
	user.clear();

	if (msg.empty() == false && msg[0] == ' ')
		msg.erase(0, 1);
	if (msg.empty() == false)
	{
		if (msg.find_first_of(' ') != std::string::npos)
			user.insert(0, msg, 0, msg.find(' '));
		else
			user.insert(0, msg, 0, std::string::npos);
	}
	return (user);
}

std::string	findRealname(std::string msg)
{
	std::string real;
	real.clear();

	if (msg.empty() == false && msg[0] == ' ')
		msg.erase(0, 1);
	if (msg.empty())
		return (real);

	int space_count = 0;
	int	begin_pos = 0;
	for (size_t i = 0; i < msg.size(); i++)
	{
		if (msg[i] == ' ')
			space_count++;
		if (space_count == 3)
		{
			begin_pos = i;
			break;
		}
	}
	if (space_count >= 3)
		real.insert(0, msg, begin_pos + 1, std::string::npos);
	if (real[0] == ':')
		real.erase(0, 1);
	return (real);
}