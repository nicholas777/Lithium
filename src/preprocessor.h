#pragma once

#include <string>

namespace Pirut
{

    class PreProcessor
    {
    public:
        PreProcessor(const std::string& input)
            : m_Input(input) {};

        ~PreProcessor() = default;

        std::string PreProcess();
    private:
        std::string m_Input;
    };

}
