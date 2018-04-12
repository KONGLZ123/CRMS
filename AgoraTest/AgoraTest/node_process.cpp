#include <windows.h>
#include <thread>
#include "node_process.h"
#include "node_log.h"

class NodeProcessWinImpl : public INodeProcess
{
public:
    NodeProcessWinImpl(HANDLE hProcess, int pid);
    ~NodeProcessWinImpl();

    virtual void Monitor(std::function<void(INodeProcess *)> callback);
    virtual int GetProcessId();

private:
    int m_pid;
    HANDLE m_hProcess;
};

NodeProcessWinImpl::NodeProcessWinImpl(HANDLE hProcess, int pid)
    : m_pid(pid)
    , m_hProcess(hProcess)
{
    LOG_ENTER;
    LOG_LEAVE;
}

NodeProcessWinImpl::~NodeProcessWinImpl()
{
    LOG_ENTER;
    LOG_LEAVE;
}

void NodeProcessWinImpl::Monitor(std::function<void(INodeProcess*)> callback)
{
    LOG_ENTER;

    HANDLE hp = m_hProcess;
    INodeProcess * pProcess = static_cast<INodeProcess *>(this);
    auto thr = std::thread([hp, pProcess, callback] {
        if (hp != NULL) {
            LOG_INFO("%s, waiting for process\n", __FUNCTION__);
            ::WaitForSingleObject(hp, INFINITE);
        }
        else {
            LOG_ERROR("%s, monitor process fail\n", __FUNCTION__);
        }
        if (pProcess) {
            callback(pProcess);
        }
    });
    thr.detach();

    LOG_LEAVE;
}

int NodeProcessWinImpl::GetProcessId()
{
    return m_pid;
}

bool INodeProcess::getCurrentModuleFileName(std::string & targetPath)
{
    HMODULE module = NULL;
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&getCurrentModuleFileName, &module)) {
        return false;
    }

    char path[MAX_PATH] = { 0 };
    if (!GetModuleFileName(module, (LPWSTR)path, MAX_PATH)) {
        return false;
    }
    targetPath.assign(path);

    return false;
}

INodeProcess * INodeProcess::OpenNodeProcess(int pid)
{
    HANDLE hProcess = ::OpenProcess(SYNCHRONIZE, FALSE, pid);
    if (!hProcess) {
        LOG_ERROR("%s, OpenProcess fail\n", __FUNCTION__);
        return nullptr;
    }

    auto *pProcess = new NodeProcessWinImpl(hProcess, pid);
    if (!pProcess) {
        CloseHandle(hProcess);
        return nullptr;
    }

    return pProcess;
}


