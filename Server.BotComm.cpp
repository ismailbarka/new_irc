#include "Server.hpp"

void Server::handleBotCommand(std::string params, int i, struct pollfd _pfds[])
{
    std::string tr = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " :";
    if (params == "TIME" || params == "TIME\n")
    {
        std::time_t nowTime = std::time(NULL);
        char timeChar[6];
        std::strftime(timeChar, sizeof(timeChar),"%H:%M", std::localtime(&nowTime));
        std::string timestr = "" + std::string(timeChar) + "\n";
        std::cout << timestr;
        std::string msg = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " Current_Time_is:" + timestr;
        send(_pfds[i].fd, msg.c_str(), msg.length(), 0);
    }
    else if (params == "MAN" || params == "MAN\n")
    {
		std::string man = tr +  "************************************************************************************\r\n";
					man += tr + "*_-_Set_UserName_:_USER_+_(your_username)__________________________________________*\r\n";
					man += tr + "*_-_Set_NickName___:_NICK_+_(your_nickname)________________________________________*\r\n";
					man += tr + "*_-_Join_a_channel_:_JOIN_+_#(channel_name_________________________________________*\r\n";
					man += tr + "*_-_KICK_-_Eject_a_client_from_the_channe__________________________________________*\r\n";
					man += tr + "*_-_INVITE_-_Invite_a_client_to_a_channel__________________________________________*\r\n";
					man += tr + "*_-_TOPIC_-_Change_or_view_the_channel_topic_______________________________________*\r\n";
					man += tr + "*_-_MODE_-_Change_the_channel's_mode:______________________________________________*\r\n";
					man += tr + "*___+_i:_Set/remove_Invite-only_channel____________________________________________*\r\n";
					man += tr + "*___+_t:_Set/remove_the_restrictions_of_the_TOPIC_command_to_channel_operators_____*\r\n";
					man += tr + "*___+_k:_Set/remove_the_channel_key_(password)_____________________________________*\r\n";
					man += tr + "*___+_o:_Give/take_channel_operator_privilege______________________________________*\r\n";
					man += tr + "*___+_l:_Set/remove_the_user_limit_to_channel______________________________________*\r\n";
					man += tr + "************************************************************************************\r\n";
            std::cout << man;
            send(_pfds[i].fd, man.c_str(), man.length(), 0);
    }
    else
    {
        std::string str = tr + " Unknown BOT command: " + params + "\r\n";
        send(_pfds[i].fd,str.c_str(), str.length(), 0);
    }
}