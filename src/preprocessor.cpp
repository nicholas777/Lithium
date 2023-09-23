#include "preprocessor.h"
#include "logger.h"

#include <stddef.h>
#include <iostream>

namespace Pirut
{

    std::string PreProcessor::PreProcess()
    {
        std::string result;
        
        int delta = 0;
        int lastCommentEnd = -1;

        for (size_t i = 0; i < m_Input.size(); i++)
        {
            std::cout << m_Input[i] << std::endl;
            if (m_Input[i] == '/')
            {
                if (m_Input[i+1] == '/')
                {
                    size_t newLine = m_Input.find('\n', i+1);

                    if (newLine != std::string::npos)
                    {
                        i = newLine;
                        continue;
                    }

                    break;
                }
                else if (m_Input[i+1] == '*')
                {
                    size_t commentEnd = m_Input.find("*/", i+1);
                    if (commentEnd != std::string::npos)
                    {
                        lastCommentEnd = commentEnd + 2;
                        i = commentEnd + 1;
                        continue;
                    }

                    continue;
                }

            }

            result += m_Input[i];
        }

        return result;
    }

}
