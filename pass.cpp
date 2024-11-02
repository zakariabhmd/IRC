#include "channel.hpp"

std::string	retrievePassword(std::string msg_to_parse);

int		pass(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client&		client		= retrieveClient(server, client_fd);
	std::string	password	= retrievePassword(cmd_infos.message);


	if (cmd_infos.message.empty() == true || password.empty() == true)
	{
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client.getNickname(), cmd_infos.name));
		return (FAILURE);
	}
	else if (server->getPassword() != password)
	{
		addToClientBuffer(server, client_fd, ERR_PASSWDMISMATCH(client.getNickname()));
		if (client.isRegistrationDone() == false)
			client.setNbInfo(-1);
		return (FAILURE);
	}
	else
	{
		client.setNbInfo(+1);
		return (SUCCESS);
	}
}

std::string	retrievePassword(std::string msg_to_parse)
{
	std::string	password;
	size_t i = 0;

	password.clear();

	while (msg_to_parse[i] && msg_to_parse[i] == ' ')
		i++;
	while (msg_to_parse[i] && msg_to_parse[i] != ' ')
		password += msg_to_parse[i++];
	return (password);
}