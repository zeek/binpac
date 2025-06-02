#ifndef pac_cstr_h
#define pac_cstr_h

#include "pac_common.h"

class ConstString : public Object {
public:
    ConstString(const std::string& s);

    // The string in its escaped form, with surrounding '"'s
    const std::string& str() const { return str_; }
    const char* c_str() const { return str_.c_str(); }

    // The unescaped string, without surrounding '"'s
    const std::string& unescaped() const { return unescaped_; }

private:
    std::string str_;
    std::string unescaped_;
};

#endif // pac_cstr_h
