#pragma once

#include "sniffcraft/enums.hpp"
#include "sniffcraft/LogItem.hpp"

#include <protocolCraft/enums.hpp>
#include <protocolCraft/Message.hpp>
#include <protocolCraft/Utilities/Json.hpp>

#include <thread>
#include <mutex>
#include <fstream>
#include <memory>
#include <queue>
#include <chrono>
#include <set>
#include <ctime>
#include <condition_variable>

class Logger
{
public:
    Logger(const std::string &conf_path);
    ~Logger();
    void Log(const std::shared_ptr<ProtocolCraft::Message> msg, const ProtocolCraft::ConnectionState connection_state, const Endpoint origin);

private:
    void LogConsume();
    void LoadConfig(const std::string& path);
    void LoadPacketsFromJson(const ProtocolCraft::Json::Value& value, const ProtocolCraft::ConnectionState connection_state);
    std::string OriginToString(const Endpoint origin) const;
    Endpoint SimpleOrigin(const Endpoint origin) const;

private:
    std::chrono::time_point<std::chrono::system_clock> start_time;

    std::thread log_thread;
    std::mutex log_mutex;
    std::condition_variable log_condition;
    std::queue<LogItem> logging_queue;

    std::string logconf_path;
    std::ofstream log_file;
    bool is_running;
    bool log_to_console;
    bool log_raw_bytes;

    std::time_t last_time_checked_log_file;
    std::time_t last_time_log_file_modified;

    std::map<std::pair<ProtocolCraft::ConnectionState, Endpoint>, std::set<int> > ignored_packets;
    std::map<std::pair<ProtocolCraft::ConnectionState, Endpoint>, std::set<int> > detailed_packets;
};
