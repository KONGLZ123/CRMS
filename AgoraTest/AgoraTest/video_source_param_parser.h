#ifndef VIDEO_SOURCE_PARAM_PARSER
#define VIDEO_SOURCE_PARAM_PARSER

#include <string>
#include <unordered_map>
#include <vector>

class VideoSourceParamParser
{
public:
    VideoSourceParamParser();
    ~VideoSourceParamParser();

    bool initialize(const std::string &cmdline);
    std::string getParameter(const std::string &key);

private:
    std::unordered_map<std::string, std::string> m_params;
    std::vector<std::string> m_switchs;
};

#endif