#ifndef pac_exception_h
#define pac_exception_h

#include <string>

#include "pac_common.h"

class Exception {
public:
    Exception(const Object* o, std::string msg = "");

    const char* msg() const { return msg_.c_str(); }
    void append(std::string_view s) { msg_ += s; }

private:
    std::string msg_;
};

class ExceptionIDNotFound : public Exception {
public:
    ExceptionIDNotFound(const ID* id);
    const ID* id() const { return id_; }

private:
    const ID* id_;
};

class ExceptionIDRedefinition : public Exception {
public:
    ExceptionIDRedefinition(const ID* id);
    const ID* id() const { return id_; }

private:
    const ID* id_;
};

class ExceptionIDNotEvaluated : public Exception {
public:
    ExceptionIDNotEvaluated(const ID* id);
    const ID* id() const { return id_; }

private:
    const ID* id_;
};

class ExceptionCyclicDependence : public Exception {
public:
    ExceptionCyclicDependence(const ID* id);
    const ID* id() const { return id_; }

private:
    const ID* id_;
};

class ExceptionPaddingError : public Exception {
public:
    ExceptionPaddingError(const Object* o, std::string msg);
};

class ExceptionIDNotField : public Exception {
public:
    ExceptionIDNotField(const ID* id);
    const ID* id() const { return id_; }

private:
    const ID* id_;
};

class ExceptionMemberNotFound : public Exception {
public:
    ExceptionMemberNotFound(const ID* type_id, const ID* member_id);

private:
    const ID *type_id_, *member_id_;
};

class ExceptionNonConstExpr : public Exception {
public:
    ExceptionNonConstExpr(const Expr* expr);

private:
    const Expr* expr;
};

class ExceptionInvalidCaseSizeExpr : public Exception {
public:
    ExceptionInvalidCaseSizeExpr(const Expr* expr);

private:
    const Expr* expr;
};

class ExceptionInvalidCaseLimitExpr : public Exception {
public:
    ExceptionInvalidCaseLimitExpr(const Expr* expr);

private:
    const Expr* expr;
};

#endif /* pac_exception_h */
