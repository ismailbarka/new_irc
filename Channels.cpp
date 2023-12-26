/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 11:10:17 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/26 12:12:30 by tmoumni          ###   ########.fr       */
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
}
void Channels::addClient(int _clientFd)
{
	std::vector<int>::iterator it = clientsFd.begin();
	while(it != clientsFd.end() && *it != _clientFd)
		it++;
	if (it == clientsFd.end())
		clientsFd.push_back(_clientFd);
}

std::vector<int> Channels::getClientFd()
{
	return clientsFd;
}

std::vector<int>  & Channels::getClientsFd()
{
	return clientsFd;
}

void Channels::setMode(std::string _mode)
{
	mode = _mode;
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
