#ifndef pac_embedded_h
#define pac_embedded_h

#include <string>
#include <vector>

#include "pac_common.h"

class EmbeddedCodeSegment {
public:
    explicit EmbeddedCodeSegment(const std::string& s);
    explicit EmbeddedCodeSegment(PacPrimitive* primitive);
    ~EmbeddedCodeSegment();

    std::string ToCode(Env* env);

private:
    std::string s_;
    PacPrimitive* primitive_;
};

typedef std::vector<EmbeddedCodeSegment*> EmbeddedCodeSegmentList;

class EmbeddedCode : public Object {
public:
    EmbeddedCode();
    ~EmbeddedCode();

    // Append a character
    void Append(int atom);
    void Append(const char* str);

    // Append a PAC primitive
    void Append(PacPrimitive* primitive);

    void GenCode(Output* out, Env* env);

private:
    std::string current_segment_;
    EmbeddedCodeSegmentList* segments_;
};

#endif // pac_embedded_h
