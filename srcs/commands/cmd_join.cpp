#include "main.hpp"

int		cmd_join(Message &message)
{
	User *user = message.get_user();
	Server &server = message.get_server();
	Channel *newChannel;
	pairAddChannel pair;
	std::string line;

	if (user->get_channels().size() < MAXCHANNELS)
	{
		newChannel = new Channel(server, user, message.get_params()[0]);
		pair = server.addChannel(newChannel);
		if (pair.second == false)
		{
			delete newChannel;
			if (pair.first->second->add_user(user) == false)
				return -1;
		}
		user->add_channel(pair.first->second);
	}
	else
		return message.send_numeric(" 405 ", findAndReplace(Message::numericsMap[ERR_TOOMANYCHANNELS], "<channel name>", message.get_params()[0]));

	line = user->get_mask() + " JOIN :" + pair.first->second->get_name() + "\n";
	pair.first->second->send(line, -1);

	cmd_names_inChannel(message, pair.first->second->get_name());

	return -1;
}