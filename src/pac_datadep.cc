#include "pac_datadep.h"
#include "pac_expr.h"
#include "pac_id.h"
#include "pac_type.h"

DataDepElement::DataDepElement(DDE_Type type) 
    : dde_type_(type), in_traversal_(false) { 
}

bool DataDepElement::Traverse(DataDepVisitor *visitor, Env *env) {
  // Avoid infinite loop
  if ( in_traversal_ )
    return true;
  if ( ! visitor->PreProcess(this) )
    return false;

  in_traversal_ = true;
  bool cont = TraverseDataDependency(visitor, env);
  in_traversal_ = false;

  if ( ! cont )
    return false;
  if ( ! visitor->PostProcess(this) )
    return false;
  return true;
}

Expr *DataDepElement::expr() { 
  return static_cast<Expr *>(this); 
}

Type *DataDepElement::type() { 
  return static_cast<Type *>(this); 
}

bool RequiresAnalyzerContext::PreProcess(DataDepElement *element) {
  switch ( element->dde_type() ) {
    case DataDepElement::EXPR:
      ProcessExpr(element->expr());
      break;

    case DataDepElement::TYPE:
      if (element->type()->buffer_input()) {
        requires_analyzer_context_ = true;
      }
      break;

    default:
      break;
  }

  // Continue traversal until we know the answer is 'yes'
  return ! requires_analyzer_context_;
}

bool RequiresAnalyzerContext::PostProcess(DataDepElement *element) {
  return ! requires_analyzer_context_;
}

void RequiresAnalyzerContext::ProcessExpr(Expr *expr) {
  if (expr->expr_type() == Expr::EXPR_ID &&
      (*expr->id() == *analyzer_context_id ||
       *expr->id() == *context_macro_id)) {
    requires_analyzer_context_ = true;
  }
}

bool RequiresAnalyzerContext::Compute(DataDepElement *element,
                                      Env *env) {
  RequiresAnalyzerContext visitor;
  element->Traverse(&visitor, env);
  return visitor.requires_analyzer_context_;
}
