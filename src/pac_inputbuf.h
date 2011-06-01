#ifndef pac_inputbuf_h
#define pac_inputbuf_h

#include "pac_datadep.h"
#include "pac_dataptr.h"

class Expr;

// InputBuffer represents the input data specified by a &withinput
// attribute.
class InputBuffer : public Object, public DataDepElement {
 public:
  InputBuffer(Expr *expr);

  bool RequiresAnalyzerContext() const;
  DataPtr GenDataBeginEnd(Output *out_cc, Env *env);

 protected:
  virtual bool TraverseDataDependency(DataDepVisitor *visitor, Env *env);

 private:
  Expr *expr_;
};

#endif // pac_inputbuf_h
