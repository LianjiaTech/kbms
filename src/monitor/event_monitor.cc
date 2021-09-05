/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-21 11:19:18 AM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:36:03 AM
 @Modified By: dongzhixiong
************************************************************************/
#include "event_monitor.h"
#include "utils/logging.h"

namespace kbms {

    EventMonitor::EventMonitor(struct event_base *evbase, const Func &handler) : evbase_(evbase), handler_(handler) {
        memset(pipe_fds_, 0, sizeof(pipe_fds_[0]) * 2);
    }

    EventMonitor::~EventMonitor() {

    }

    bool EventMonitor::Initialize() {

        assert(pipe_fds_[0] == 0);

        if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_fds_) < 0) {
            LOGGER_ERROR << "creat socketpair fail";
            return false;
        }
        if (evutil_make_socket_nonblocking(pipe_fds_[0]) < 0 || evutil_make_socket_nonblocking(pipe_fds_[1]) < 0) {
            LOGGER_ERROR << "make socket fail";
            return false;
        }
        event_set(&notify_event_, pipe_fds_[1], EV_READ | EV_PERSIST, &EventMonitor::EventHandler, this);


        event_base_set(evbase_, &notify_event_);

        if (-1 == event_add(&notify_event_, nullptr)) {
            LOGGER_ERROR << "add event error";
            return false;
        }
        return true;
    }

    void EventMonitor::EventHandler(evutil_socket_t fd, short /* which */, void *v) {
        EventMonitor *em = (EventMonitor *) v;
        char buf[24];
        int n = 0;
        if ((n = ::recv(em->pipe_fds_[1], buf, sizeof(buf), 0)) > 0) {
            em->handler_();
        }
    }

    void EventMonitor::EventNotify() {
        char buf[1] = {};
        if (::send(pipe_fds_[0], buf, sizeof(buf), 0) < 0) {
            return;
        }

    }

}
