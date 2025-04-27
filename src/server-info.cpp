//
// Created by plush-jill on 4/25/25.
//

#include "server-info.hpp"

uint32_t ServerInfo::get_data_size() const {
    return m_data_size;
}
std::filesystem::path ServerInfo::get_data_path() const {
    return m_data_path;
}
bool ServerInfo::get_is_running() const {
    return m_is_running;
}

void ServerInfo::set_data_size(const uint32_t m_data_size) {
    this->m_data_size = m_data_size;
}
void ServerInfo::set_data_path(const std::filesystem::path& data_path) {
    m_data_path = data_path;
}
void ServerInfo::set_is_running(const bool m_is_running) {
    this->m_is_running = m_is_running;
}
