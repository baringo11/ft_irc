#include "Channel.hpp"

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

Channel::Channel(Server& server, User* creator, std::string name) : _server(server), _creator(creator), _channelName(name)
{
	this->_usersMap[_creator] = 2;

	(void)this->_server.getPassword(); // tmp para el flag -Wall unused variable
}

Channel::~Channel()
{}

bool	Channel::add_user(User *newUser)
{
	return this->_usersMap.insert(std::make_pair(newUser, 0)).second;
}

void	Channel::delete_user(User *user)
{
	this->_usersMap.erase(user);
	
	std::vector<Channel *>::iterator it;
	for (it = user->get_channels().begin(); it != user->get_channels().end(); it++)
	{
		if(*(it) == this)
		{
			user->get_channels().erase(it);
			break;
		}
	}

	if (this->_usersMap.empty())
	{
		this->_server.getChannelsMap().erase(this->_server.getChannelsMap().find(this->get_name()));
		std::cout << "Channel " << this->get_name() << " deleted" << std::endl;
		delete this;
	}
}

void	Channel::send(std::string str, int userFd)
{
	for (channelUsersMap::iterator it = this->_usersMap.begin(); it != this->_usersMap.end(); it++)
	{
		if (!userFd || userFd != it->first->get_fd())
			send_all(it->first->get_fd(), str.c_str());
	}
}