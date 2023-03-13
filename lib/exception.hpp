#pragma once

class exception
{
public:
    exception(const char *_msg = "") : msg(_msg) {}
    exception(const exception &) = default;
    exception(exception &&) = default;
    exception &operator=(const exception &) = default;
    exception &operator=(exception &&) = default;
    virtual ~exception() {}
    virtual const char *what() { return msg; }

private:
    const char *msg;
};
