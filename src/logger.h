#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace Lithium
{

    class Logger
    {
    public:
        static void Init();

        static void Debug(const std::string& msg);
        static void Info(const std::string& msg);
        static void Warn(const std::string& msg);
        static void Error(const std::string& msg);
        static void Fatal(const std::string& msg);
    private:
        static std::shared_ptr<spdlog::logger> m_Logger;
    };
}
