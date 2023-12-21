/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 11:10:17 by tmoumni           #+#    #+#             */
/*   Updated: 2023/12/21 11:11:26 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channels.hpp"

Channels::Channels()
{
}

Channels::~Channels()
{
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
