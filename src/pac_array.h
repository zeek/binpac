#ifndef pac_array_h
#define pac_array_h

#include "pac_common.h"
#include "pac_type.h"

// Fixed-length array and variable length sequence with an ending pattern

class ArrayType : public Type {
public:
    ArrayType(Type* arg_elemtype, Expr* arg_length = nullptr);
    ~ArrayType() override;

    bool DefineValueVar() const override;
    std::string DataTypeStr() const override;
    std::string DefaultValue() const override { return "0"; }
    Type* ElementDataType() const override;

    std::string EvalElement(const std::string& array, const std::string& index) const override;

    void ProcessAttr(Attr* a) override;

    void Prepare(Env* env, int flags) override;

    void GenPubDecls(Output* out, Env* env) override;
    void GenPrivDecls(Output* out, Env* env) override;

    void GenInitCode(Output* out, Env* env) override;
    void GenCleanUpCode(Output* out, Env* env) override;

    int StaticSize(Env* env) const override;

    void SetBoundaryChecked() override;
    void GenUntilInputCheck(Output* out_cc, Env* env);

    bool IsPointerType() const override { return true; }

protected:
    void init();

    void DoGenParseCode(Output* out, Env* env, const DataPtr& data, int flags) override;
    void GenDynamicSize(Output* out, Env* env, const DataPtr& data) override;
    void GenArrayLength(Output* out_cc, Env* env, const DataPtr& data);
    std::string GenArrayInit(Output* out_cc, Env* env, bool known_array_length);
    void GenElementAssignment(Output* out_cc, Env* env, std::string const& array_str, bool use_vector);
    void GenUntilCheck(Output* out_cc, Env* env, Expr* until_condition, bool delete_elem);

    bool ByteOrderSensitive() const override { return elemtype_->RequiresByteOrder(); }
    bool RequiresAnalyzerContext() override;

    Type* DoClone() const override;

    void DoMarkIncrementalInput() override;

    const ID* arraylength_var() const;
    const ID* elem_it_var() const;
    const ID* elem_var() const;
    const ID* elem_dataptr_var() const;
    const ID* elem_input_var() const;

protected:
    bool DoTraverse(DataDepVisitor* visitor) override;

private:
    Type* elemtype_;
    Expr* length_;

    std::string vector_str_;
    std::string datatype_str_;
    std::string end_of_array_loop_label_;

    Field* arraylength_var_field_;
    Field* elem_it_var_field_;
    Field* elem_var_field_;
    Field* elem_dataptr_var_field_;
    Field* elem_input_var_field_;

    // This does not come from &until, but is internally generated
    Expr* elem_dataptr_until_expr_;

    Expr* attr_generic_until_expr_;
    Expr* attr_until_element_expr_;
    Expr* attr_until_input_expr_;
};

#endif // pac_array_h
