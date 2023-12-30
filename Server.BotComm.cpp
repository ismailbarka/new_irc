#include "Server.hpp"

std::string getTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    return std::string(buffer);
}

void Server::handleBotCommand(std::string params, int i, struct pollfd _pfds[])
{

    if(params == "TIME" || params == "TIME\n")
    {
		std::string time = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " :" + getTime() + "\n";
		std::cout << time << std::endl;
        // std::string str = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " " + params + "\n";
        send(_pfds[i].fd, time.c_str(), time.length(), 0);
    }
    else if(params == "MAN" || params == "MAN\n")
    {
            std::string man[14] = {"*************************************************************************************",
                                    "*_-_Set_UserName_:_USER_+_(your_username)__________________________________________*",
                                    "*_-_Set_NickName___:_NICK_+_(your_nickname)________________________________________*",
                                    "*_-_Join_a_channel_:_JOIN_+_#(channel_name_________________________________________*",
                                    "*_-_KICK_-_Eject_a_client_from_the_channe__________________________________________*",
                                    "*_-_INVITE_-_Invite_a_client_to_a_channel__________________________________________*",
                                    "*_-_TOPIC_-_Change_or_view_the_channel_topic_______________________________________*",
                                    "*_-_MODE_-_Change_the_channel's_mode:______________________________________________*",
                                    "*___+_i:_Set/remove_Invite-only_channel____________________________________________*",
                                    "*___+_t:_Set/remove_the_restrictions_of_the_TOPIC_command_to_channel_operators_____*",
                                    "*___+_k:_Set/remove_the_channel_key_(password)_____________________________________*",
                                    "*___+_o:_Give/take_channel_operator_privilege______________________________________*",
                                    "*___+_l:_Set/remove_the_user_limit_to_channel______________________________________*",
                                    "************************************************************************************",
                                    };
        for (size_t j = 0; j < 14; j++)
        {
            man[j] = ":BOT PRIVMSG " + ClientsMap[_pfds[i].fd].getNickname() + " " + man[j] + "\n";
            std::cout << man[j] << std::endl;
        }
        for (size_t j = 0; j < 14; j++)
        {
            send(_pfds[i].fd, man[j].c_str(), man[j].length(), 0);
        }
    }
    else
    {
        std::string str = "421 " + params + " :Unknown BOOT command\r\n";
        send(_pfds[i].fd,str.c_str(), str.length(), 0);
    }
}