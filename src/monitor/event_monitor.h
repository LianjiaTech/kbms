/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-21 11:09:46 AM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:35:37 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_MONITOR_EVENT_MONITOR_H_
#define KBMS_MONITOR_EVENT_MONITOR_H_

#include <iostream>
#include <functional>
#include <event.h>
#include <event2/event_compat.h>
#include <event2/event_struct.h>
#include <cassert>
#include <cstring>

namespace kbms {
    class EventMonitor {
    public:
        typedef std::function<void()> Func;
    public:
        EventMonitor(struct event_base *evbase, const Func &handler);

        virtual ~EventMonitor();

        bool Initialize();

        void EventNotify();

    private:
        static void EventHandler(evutil_socket_t fd, short /* which */, void *v);

    private:
        struct event notify_event_;
        struct event_base *evbase_;
        evutil_socket_t pipe_fds_[2];
        Func handler_;
    };

}
#endif // KBMS_MONITOR_EVENT_MONITOR_H_
