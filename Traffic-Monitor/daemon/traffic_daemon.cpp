#include "traffic_daemon.h"

#include <QString>

#include <thread>
#include <csignal>

#include <unistd.h>
#include <syslog.h>
#include <execinfo.h>
#include <sys/types.h>

bool TrafficDaemon::quit_ = false;

TrafficDaemon::TrafficDaemon(int argc, char* argv[])
{
    this->trafficMonitor_ = TrafficMonitor(this->trafficStorage_.read());
}

TrafficDaemon::~TrafficDaemon()
{
    ;
}

void TrafficDaemon::startDaemon()
{
    struct sigaction sigActTrace;

    sigActTrace.sa_flags     = SA_SIGINFO;
    sigActTrace.sa_sigaction = this->signalTrace_;

    sigemptyset(&sigActTrace.sa_mask);

    sigaction(SIGFPE,  &sigActTrace, 0);  // FPU error
    sigaction(SIGILL,  &sigActTrace, 0);  // instruction error
    sigaction(SIGSEGV, &sigActTrace, 0);  // memory error
    sigaction(SIGBUS,  &sigActTrace, 0);  // bus error

    std::signal(SIGQUIT, this->signalQuit_);
    std::signal(SIGINT,  this->signalQuit_);
    std::signal(SIGTERM, this->signalQuit_);

    syslog(LOG_NOTICE, "[Traffic-Daemon] Started pid(%d)\n", getpid());

    for (;;)
    {
        std::this_thread::yield();

        this->getTraffic_();

        if (TrafficDaemon::quit_)
        {
            break;
        }
    }

    syslog(LOG_NOTICE, "[Traffic-Daemon] Stopped\n");
    return;
}

void TrafficDaemon::signalTrace_(int sig, siginfo_t *si, void *ptr)
{
    void* errorAddr = nullptr;
    void* trace[16] = { 0 };

    syslog(LOG_NOTICE, "[Traffic-Daemon] Signal: %s, Addr: 0x%0.16X\n", strsignal(sig), si->si_addr);

#if __WORDSIZE == 64 // 64 bit ОС
    errorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_RIP];
#else
    // получим адрес инструкции которая вызвала ошибку
    errorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];
#endif // __WORDSIZE

    auto traceSize = backtrace(trace, 16);

    trace[1] = errorAddr;

    char** messages = backtrace_symbols(trace, traceSize);
    if (messages)
    {
        syslog(LOG_NOTICE, "[Traffic-Daemon] == Backtrace ==\n");

        for (auto i = 1; i < traceSize; i++)
        {
            syslog(LOG_NOTICE, "%s\n", messages[i]);
        }

        syslog(LOG_NOTICE, "[Traffic-Daemon] == End Backtrace ==\n");
        free(messages);
    }

    syslog(LOG_NOTICE, "[Traffic-Daemon] Aborted\n");

    exit(-1);
}

void TrafficDaemon::signalQuit_(int sig)
{
    TrafficDaemon::quit_ = true;
}

void TrafficDaemon::getTraffic_()
{
    auto usage = this->trafficMonitor_.getNetworkUsage();
    this->trafficStorage_.write(usage);
}
