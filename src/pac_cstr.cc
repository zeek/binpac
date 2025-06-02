#include "pac_cstr.h"

#include "pac_dbg.h"
#include "pac_exception.h"

namespace {

class EscapeException {
public:
    explicit EscapeException(const std::string& s) { msg_ = s; }

    const std::string& msg() const { return msg_; }

private:
    std::string msg_;
};

// Copied from util.cc of Zeek
int expand_escape(const char*& s) {
    switch ( *(s++) ) {
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'a': return '\a';
        case 'v': return '\v';

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7': { // \<octal>{1,3}
            --s;    // put back the first octal digit
            const char* start = s;

            // Don't increment inside loop control
            // because if isdigit() is a macro it might
            // expand into multiple increments ...

            // Here we define a maximum length for escape sequence
            // to allow easy handling of string like: "^H0" as
            // "\0100".

            for ( int len = 0; len < 3 && isascii(*s) && isdigit(*s); ++s, ++len )
                ;

            int result;
            if ( sscanf(start, "%3o", &result) != 1 )
                throw EscapeException(strfmt("bad octal escape: \"%s", start));

            return result;
        }

        case 'x': { /* \x<hex> */
            const char* start = s;

            // Look at most 2 characters, so that "\x0ddir" -> "^Mdir".
            for ( int len = 0; len < 2 && isascii(*s) && isxdigit(*s); ++s, ++len )
                ;

            int result;
            if ( sscanf(start, "%2x", &result) != 1 )
                throw EscapeException(strfmt("bad hexadecimal escape: \"%s", start));

            return result;
        }

        default: return s[-1];
    }
}

} // namespace

ConstString::ConstString(const std::string& s) : str_(s) {
    // Copied from scan.l of Zeek
    try {
        const char* text = str_.c_str();
        int len = strlen(text) + 1;
        int i = 0;

        char* new_s = new char[len];

        // Skip leading quote.
        for ( ++text; *text; ++text ) {
            if ( *text == '\\' ) {
                ++text; // skip '\'
                new_s[i++] = expand_escape(text);
                --text; // point to end of sequence
            }
            else {
                new_s[i++] = *text;
            }
        }
        ASSERT(i < len);

        // Get rid of trailing quote.
        ASSERT(new_s[i - 1] == '"');
        new_s[i - 1] = '\0';

        unescaped_ = new_s;
        delete[] new_s;
    } catch ( EscapeException const& e ) {
        // Throw again with the object
        throw Exception(this, e.msg().c_str());
    }
}
