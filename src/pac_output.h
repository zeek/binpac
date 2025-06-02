#ifndef pac_output_h
#define pac_output_h

#include <cstdarg>
#include <cstdio>
#include <string>

class OutputException {
public:
    OutputException(const char* arg_msg);
    ~OutputException();
    const char* errmsg() const { return msg.c_str(); }

protected:
    std::string msg;
};

class Output {
public:
    Output(std::string filename);
    ~Output();

    int println(const char* fmt, ...);
    int print(const char* fmt, ...);

    int indent() const { return indent_; }

    void inc_indent() { ++indent_; }
    void dec_indent() { --indent_; }

protected:
    int print(const char* fmt, va_list ap);

    FILE* fp;
    int indent_;
};

#endif /* pac_output_h */
