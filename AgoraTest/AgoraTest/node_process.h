#ifndef NODE_PROCESS_H
#define NODE_PROCESS_H

#include <string>
#include <functional>

class INodeProcess
{
public:
    virtual ~INodeProcess() {}

    virtual void Monitor(std::function<void (INodeProcess *)> callback) = 0;
    virtual int GetProcessId() = 0;

    static bool getCurrentModuleFileName(std::string &targetPath);
    static INodeProcess *OpenNodeProcess(int pid);
};

#endif