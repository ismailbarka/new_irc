/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 11:10:17 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/31 10:32:49 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channels.hpp"

Channels::Channels()
{
	topic = "Hi new user, welcome to the channel\r\n";
	mode = "t";
}

Channels::~Channels()
{
}

Channels::Channels(bool _havePassword)
{
	havePassword = _havePassword;
	topic = "Hi new user, welcome to the channel\r\n";
	mode = "t";
}

void Channels::addClient(int _clientFd)
{
	std::vector<int>::iterator it = clientsFd.begin();
	while(it != clientsFd.end() && *it != _clientFd)
		it++;
	if (it == clientsFd.end())
		clientsFd.push_back(_clientFd);
}

// std::vector<int> Channels::getClientFd()
// {
// 	return clientsFd;
// }

// std::vector<int>  & Channels::getClientsFd()
// {
// 	return clientsFd;
// }

void Channels::setMode(std::string _mode)
{
	int i = 0;
	while (_mode[i])
	{
		int j = 0;
		while (mode[j])
		{
			if (_mode[0] == '+')
			{
				if (_mode[i] == mode[j])
					break;
				else if (_mode[i] != mode[j] && mode[j + 1] == '\0')
					mode += _mode[i];
				j++;
			}
			else if (_mode[0] == '-')
			{
				if (_mode[i] == mode[j])
				{
					mode.erase(j, 1);
					break;
				}
				j++;
			}
		}
		i++;
	}
}

void Channels::setKey(std::string _key)
{
	password = _key;
}

void Channels::setLimit(std::string _limit)
{
	limit = _limit;
}

std::string Channels::getMode()
{
	return mode;
}

void Channels::removeClient(int _clientFd)
{
	std::vector<int>::iterator it = clientsFd.begin();
	while(it != clientsFd.end() && *it != _clientFd)
		it++;
	if (it != clientsFd.end())
		clientsFd.erase(it);
}

bool Channels::isOperator(int _clientFd)
{
	std::vector<Client>::iterator it = operators.begin();
	while (it != operators.end())
	{
		if (it->getfd() == _clientFd)
			return true;
		it++;
	}
	return false;
}
