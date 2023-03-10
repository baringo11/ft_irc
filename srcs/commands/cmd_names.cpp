#include "main.hpp"

void	cmd_names_inChannel(Message &message, std::string channelName)
{
	channelUsersMap *channelUsersMap = message.get_server().getChannelUsersMap(channelName);
	channelUsersMap::iterator it;
	std::string	line;

	if (!channelUsersMap)
	{
		message.send_numeric(" 366 ", findAndReplace(Message::numericsMap[RPL_ENDOFNAMES], "<channel>", channelName));
		return ;
	}	
	it = channelUsersMap->begin();

	line = (it->second == 2 ? "@" : "") + it->first->get_nick();
	for (it++; it != channelUsersMap->end(); it++)
		line += (it->second == 2 ? " @" : " ") + it->first->get_nick();

	message.send_numeric(" 353 ", line, ("= " + channelName + " :"));
	message.send_numeric(" 366 ", findAndReplace(Message::numericsMap[RPL_ENDOFNAMES], "<channel>", channelName));
}

void	cmd_names(Message &message)
{
	Server			&server = message.get_server();
	UserMap			&userMap = server.getUserMap();
	UserMapIterator	it;
	std::string		line;

	if (message.get_params().empty())
	{
		it = userMap.begin();
		line = (it->second->isOper() ? "@" : "") + it->second->get_nick();
		for (it++ ; it != userMap.end(); it++)
			line += (it->second->isOper() ? " @" : " ") + it->second->get_nick();
		message.send_numeric(" 353 ", line, ("= " + server.getServerName() + " :"));
		message.send_numeric(" 366 ", findAndReplace(Message::numericsMap[RPL_ENDOFNAMES], "<channel>", server.getServerName()));
	}
	else
		cmd_names_inChannel(message, message.get_params()[0]);
}
