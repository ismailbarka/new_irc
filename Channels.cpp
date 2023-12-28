/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 11:10:17 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/28 10:27:56 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channels.hpp"

Channels::Channels()
{
	topic = "no topic yet\n";
}

Channels::~Channels()
{
}

Channels::Channels(bool _havePassword)
{
	havePassword = _havePassword;
	topic = "no topic yet\n";
	mode = "+t";
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
	if (mode[0] == '+')
	mode += _mode;
	else
	{
		int i = 1;
		while (mode[i] != '\0')
		{
			if (mode[i] == _mode[1])
			{
				mode.erase(i,1);
				break;
			}
			i++;
		}
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
