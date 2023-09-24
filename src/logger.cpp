#include "logger.h"
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Lithium
{
    std::shared_ptr<spdlog::logger> Logger::m_Logger; 

    void Logger::Init()
    {
        m_Logger = spdlog::stdout_color_mt("Pirut");
        m_Logger->set_level(spdlog::level::debug);
        
        m_Logger->set_pattern("%^%l:%$ %v");
    }

    void Logger::Debug(const std::string& msg)
    {
        m_Logger->debug(msg);
    }

    void Logger::Info(const std::string& msg)
    {
        m_Logger->info(msg);
    }

    void Logger::Warn(const std::string& msg)
    {
        m_Logger->warn(msg);
    }

    void Logger::Error(const std::string& msg)
    {
        m_Logger->error(msg);
    }

    void Logger::Fatal(const std::string& msg)
    {
        m_Logger->critical(msg);
    }

}
