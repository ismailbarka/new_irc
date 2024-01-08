/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoumni <tmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 11:02:19 by tmoumni           #+#    #+#             */
/*   Updated: 2024/01/08 12:17:58 by tmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
}

Client::Client(struct pollfd client)
{
	pFd = client;
	nickName = "";
	userName = "";
	_client_host = "";
	isAutonticated = false;
	correctPassword = false;
}

Client::~Client()
{
}

void Client::setPollfd(struct pollfd _pFd)
{
	pFd = _pFd;
}

int Client::getfd()
{
	return pFd.fd;
}

void Client::setNickname(std::string _nickName)
{
	nickName = _nickName;
}

void Client::setuserName(std::string _userName)
{
	userName = _userName;
}

void Client::setRealName(std::string _realName)
{
	realName = _realName;
}

std::string Client::getNickname()
{
	return nickName;
}

std::string Client::getUserName()
{
	return userName;
}

std::string Client::getRealName()
{
	return realName;
}

void Client::setIsAutonticated()
{
	if (nickName.length() > 0 && userName.length() > 0 && correctPassword == true)
		isAutonticated = true;
}

bool Client::getIsAutonticated()
{
	return isAutonticated;
}

void Client::setClientIp(char *ip)
{
	strcpy(ipAddress, ip);
}

void Client::setCorrectPassWord(bool _correctPassword)
{
	correctPassword = _correctPassword;
}

bool Client::getCorrectPassWord()
{
	return correctPassword;
}

void Client::setClientHost(std::string host)
{
	_client_host = host;
}

std::string Client::getClientHost() const
{
	return _client_host;
}

