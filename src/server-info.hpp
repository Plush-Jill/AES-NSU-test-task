//
// Created by plush-jill on 4/25/25.
//

#ifndef SERVER_STATE_HPP
#define SERVER_STATE_HPP
#include <filesystem>

/**
 * @class ServerInfo
 * @brief Класс для хранения данных, полученных с сервера, не обязательно в том "сыром" виде, полученном с сервера.
 *
 */
class ServerInfo {
private:
    bool m_is_running;
    bool m_data_size;
    std::filesystem::path m_data_path;

public:
    ServerInfo() : m_is_running{}, m_data_size{} {}

    [[nodiscard]] bool                  get_data_size       () const;
    [[nodiscard]] std::filesystem::path get_data_path       () const;
    [[nodiscard]] bool                  get_is_running      () const;

    void                                set_data_size       (bool m_data_size);
    void                                set_data_path       (const std::filesystem::path& data_path);
    void                                set_is_running      (bool m_is_running);
};



#endif //SERVER_STATE_HPP
