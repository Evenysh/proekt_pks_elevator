#pragma once

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

namespace ConsoleLog {

struct LogState {
    std::mutex mtx;
    int uiDepth = 0;
    bool deferLogs = false;
    std::vector<std::string> deferred;
};

inline LogState& state() {
    static LogState s;
    return s;
}

#ifdef ELEVATOR_QUIET_CONSOLE
// Режим unit-тестов: не писать в std::cout из фоновых потоков (иначе ctest / пайп
// может заблокироваться при полном буфере — зависание на «Start 1: …»).
inline void beginUiInput() {
    std::lock_guard<std::mutex> lock(state().mtx);
    if (++state().uiDepth == 1) state().deferLogs = true;
}

inline void endUiInput() {
    std::lock_guard<std::mutex> lock(state().mtx);
    if (--state().uiDepth < 0) state().uiDepth = 0;
    if (state().uiDepth == 0) {
        state().deferred.clear();
        state().deferLogs = false;
    }
}

class UiInputSession {
public:
    UiInputSession() { beginUiInput(); }
    ~UiInputSession() { endUiInput(); }
    UiInputSession(const UiInputSession&) = delete;
    UiInputSession& operator=(const UiInputSession&) = delete;
};

template <typename Fn>
inline void sync(Fn&& fn) {
    std::lock_guard<std::mutex> lock(state().mtx);
    if (state().deferLogs) {
        std::ostringstream oss;
        fn(oss);
        state().deferred.push_back(oss.str());
    } else {
        (void)fn;
    }
}

template <typename Fn>
inline void syncUi(Fn&& fn) {
    (void)fn;
}

#else

inline void beginUiInput() {
    std::lock_guard<std::mutex> lock(state().mtx);
    if (++state().uiDepth == 1) state().deferLogs = true;
}

inline void endUiInput() {
    std::lock_guard<std::mutex> lock(state().mtx);
    if (--state().uiDepth < 0) state().uiDepth = 0;
    if (state().uiDepth == 0) {
        if (!state().deferred.empty()) {
            std::cout << "\n  ----- события системы (пока ожидался ввод) -----\n";
            for (const auto& chunk : state().deferred) std::cout << chunk;
            state().deferred.clear();
            std::cout << "  ----- конец блока -----\n\n";
        }
        state().deferLogs = false;
    }
    std::cout.flush();
}

class UiInputSession {
public:
    UiInputSession() { beginUiInput(); }
    ~UiInputSession() { endUiInput(); }
    UiInputSession(const UiInputSession&) = delete;
    UiInputSession& operator=(const UiInputSession&) = delete;
};

template <typename Fn>
inline void sync(Fn&& fn) {
    std::lock_guard<std::mutex> lock(state().mtx);
    if (state().deferLogs) {
        std::ostringstream oss;
        fn(oss);
        state().deferred.push_back(oss.str());
    } else {
        fn(std::cout);
        std::cout.flush();
    }
}

template <typename Fn>
inline void syncUi(Fn&& fn) {
    std::lock_guard<std::mutex> lock(state().mtx);
    fn(std::cout);
    std::cout.flush();
}

#endif

}  // namespace ConsoleLog
