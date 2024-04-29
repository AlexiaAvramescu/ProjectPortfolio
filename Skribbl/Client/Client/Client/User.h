#pragma once
#include <qstring.h>

using GameHistory = std::pair<std::string, std::string>;

class User
{
public: 
    
    User();
    void SetUsername(const std::string& username);
    void SetLobbyCode(const std::string& lobbyCode);
    void SetAdmin();

    QString GetUsername() const;
    std::optional<QString> GetLobbyCode();

    void SetGameHistory(const std::vector<GameHistory>& history);

    std::vector<GameHistory> GetGameHistory();

private:
    QString m_username;
    bool m_isAdmin;
    std::optional<QString> m_lobbyCode;
    std::vector<GameHistory> m_gamesHistory;
};

