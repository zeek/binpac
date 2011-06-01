#include "pac_exception.h"
#include "pac_expr.h"
#include "pac_exttype.h"
#include "pac_field.h"
#include "pac_id.h"
#include "pac_type.h"
#include "pac_utils.h"

#define DEFINE_ID(id_var, id_name) const ID *id_var = 0;
#include "pac_builtin_id.def"
#undef DEFINE_ID

int ID::anonymous_id_seq = 0;

ID *ID::NewAnonymousID(const string &prefix)
	{
	ID *id = new ID(fmt("%s%03d", prefix.c_str(), ++anonymous_id_seq));
	id->anonymous_id_ = true;
	return id;
	}

IDRecord::IDRecord(Env *arg_env, const ID* arg_id, IDType arg_id_type)
	: env(arg_env), id(arg_id), id_type(arg_id_type)
	{
	eval = 0;
	evaluated = in_evaluation = false;
	setfunc = "";  // except for STATE_VAR
	switch (id_type)
		{
                case ID_NOT_FOUND:
                  ASSERT(id_type != ID_NOT_FOUND);
                  break;
		case MEMBER_VAR:
			rvalue = strfmt("%s()", id->Name());
			lvalue = strfmt("%s_", id->Name());
			break;
		case PRIV_MEMBER_VAR:
			rvalue = strfmt("%s_", id->Name());
			lvalue = strfmt("%s_", id->Name());
			break;
		case UNION_VAR:
			rvalue = strfmt("%s()", id->Name());
			lvalue = strfmt("%s_", id->Name());
			break;
		case CONST:
		case GLOBAL_VAR:
			rvalue = strfmt("%s", id->Name());
			lvalue = strfmt("%s", id->Name());
			break;
		case TEMP_VAR:
			rvalue = strfmt("t_%s", id->Name());
			lvalue = strfmt("t_%s", id->Name());
			break;
		case STATE_VAR:
			rvalue = strfmt("%s()", id->Name());
			lvalue = strfmt("%s_", id->Name());
			break;
		case MACRO:
			rvalue = "@MACRO@";
			lvalue = "@MACRO@";
			break;
		case FUNC_ID:
			rvalue = strfmt("%s", id->Name());
			lvalue = "@FUNC_ID@";
			break;
		case FUNC_PARAM:
			rvalue = strfmt("%s", id->Name());
			lvalue = "@FUNC_PARAM@";
			break;
		}
	field = 0;
	constant_set = false;
	}

IDRecord::~IDRecord()
	{
	}

void IDRecord::SetConstant(int c)			
	{ 
	ASSERT(id_type == CONST);
	constant_set = true;
	constant = c; 
	}

bool IDRecord::GetConstant(int *pc) const
	{ 
	if ( constant_set ) 
		*pc = constant;
	return constant_set;
	}

void IDRecord::SetMacro(Expr *e)
	{
	ASSERT(id_type == MACRO);
	macro = e;
	}

Expr *IDRecord::GetMacro() const
	{
	ASSERT(id_type == MACRO);
	return macro;
	}

void IDRecord::SetEvaluated(bool v)
	{
	if ( v )
		ASSERT(! evaluated);
	evaluated = v;
	}

void IDRecord::Evaluate(Output* out, Env* env)	
	{ 
	if ( evaluated )
		return;

	if ( ! out )
		throw ExceptionIDNotEvaluated(id);

	if ( ! eval )
		throw Exception(id, "no evaluation method");
		
	if ( in_evaluation )
		throw ExceptionCyclicDependence(id);

	in_evaluation = true;
	eval->GenEval(out, env); 
	in_evaluation = false;

	evaluated = true;
	}

const char* IDRecord::RValue() const
	{
	if ( id_type == MACRO )
		return macro->EvalExpr(0, env);

	if ( id_type == TEMP_VAR && ! evaluated )
		throw ExceptionIDNotEvaluated(id);

	return rvalue.c_str();
	}

const char* IDRecord::LValue() const
	{
	ASSERT(id_type != MACRO && id_type != FUNC_ID);
	return lvalue.c_str();
	}

Env::Env(Env* parent_env, Object* context_object)
	: parent(parent_env), context_object_(context_object)
	{
	allow_undefined_id_ = false;
	in_branch_ = false;
	}

Env::~Env()
	{
	for ( id_map_t::iterator it = id_map.begin(); it != id_map.end(); ++it )
		{
		delete it->second;
		it->second = 0;
		}
	}

void Env::AddID(const ID* id, IDType id_type, Type *data_type)
	{
	DEBUG_MSG("To add ID `%s'...\n", id->Name());
	id_map_t::iterator it = id_map.find(id);
	if ( it != id_map.end() )
		{
		DEBUG_MSG("Duplicate definition: `%s'\n", it->first->Name());
		throw ExceptionIDRedefinition(id);
		}
	id_map[id] = new IDRecord(this, id, id_type);
	// TODO: figure out when data_type must be non-NULL
	// ASSERT(data_type);
	SetDataType(id, data_type);
	}

void Env::AddConstID(const ID* id, const int c, Type *type)
	{
	if ( ! type )
		type = extern_type_int;
	AddID(id, CONST, type);
	SetConstant(id, c);
	SetEvaluated(id);  // a constant is always evaluated
	}

void Env::AddMacro(const ID *id, Expr *macro)
	{
	AddID(id, MACRO, macro->DataType(this));
	SetMacro(id, macro);
	SetEvaluated(id);
	}

ID *Env::AddTempID(Type *type)
	{
	ID *id = ID::NewAnonymousID("t_var_");
	AddID(id, TEMP_VAR, type);
	return id;
	}

IDRecord* Env::LookUp(const ID* id, bool recursive, bool raise_exception) const
	{
	ASSERT(id);

	id_map_t::const_iterator it = id_map.find(id);
	if ( it != id_map.end() )
		return it->second;

	if ( recursive && parent )
		return parent->LookUp(id, recursive, raise_exception);

	if ( raise_exception )
		throw ExceptionIDNotFound(id);
	else
		return 0;
	}

IDType Env::GetIDType(const ID* id) const
	{
          IDRecord *r = LookUp(id, RECURSIVE, !RAISE_EXCEPTION);
          if (!r) {
            return ID_NOT_FOUND;
          } else {
            return r->GetType();
          }
	}

const char* Env::RValue(const ID* id) const
	{
	IDRecord *r = LookUp(id, RECURSIVE, !RAISE_EXCEPTION);
	if ( r )
		return r->RValue();
	else
		{
		if ( allow_undefined_id() )
			return id->Name();
		else
			throw ExceptionIDNotFound(id);
		}
	}

const char* Env::LValue(const ID* id) const
	{
	return LookUp(id, RECURSIVE, RAISE_EXCEPTION)->LValue();
	}

void Env::SetEvalMethod(const ID* id, Evaluatable* eval)
	{
	LookUp(id, RECURSIVE, RAISE_EXCEPTION)->SetEvalMethod(eval);
	}

void Env::Evaluate(Output* out, const ID* id)
	{
	IDRecord *r = LookUp(id, RECURSIVE, !allow_undefined_id());
	if ( r )
		r->Evaluate(out, this);
	}

bool Env::Evaluated(const ID* id) const
	{
	IDRecord *r = LookUp(id, RECURSIVE, !allow_undefined_id());
	if ( r )
		return r->Evaluated();
	else
		// Assume undefined variables are already evaluated
		return true;  
	}

void Env::SetEvaluated(const ID* id, bool v)
	{
	if ( in_branch() )
		{
		Field *f = GetField(id);
		if (f && f->tof() == LET_FIELD)
			{
			throw Exception(
				context_object_,
				fmt("INTERNAL ERROR: "
				"evaluating let field '%s' in a branch! "
				"To work around this problem, "
				"add '&requires(%s)' to the case type. "
				"Sorry for the inconvenience.\n",
				id->Name(),
				id->Name()));
			ASSERT(0);
			}
		}

	IDRecord *r = LookUp(id, !RECURSIVE, !RAISE_EXCEPTION);
	if ( r )
		r->SetEvaluated(v);
	else if ( parent )
		parent->SetEvaluated(id, v);
	else
		throw ExceptionIDNotFound(id);
	}

void Env::SetField(const ID* id, Field* field)
	{
	LookUp(id, !RECURSIVE, RAISE_EXCEPTION)->SetField(field);
	}

Field* Env::GetField(const ID* id) const
	{
	IDRecord *r = LookUp(id, RECURSIVE, !RAISE_EXCEPTION);
        if (r)
          return r->GetField();
        else
          return 0;
	}

void Env::SetDataType(const ID* id, Type* type)
	{
	LookUp(id, RECURSIVE, RAISE_EXCEPTION)->SetDataType(type);
	}

Type* Env::GetDataType(const ID* id) const
	{
	IDRecord *r = LookUp(id, RECURSIVE, !RAISE_EXCEPTION);
	if ( r )
		return r->GetDataType();
	else
		return 0;  
	}

string Env::DataTypeStr(const ID *id) const
	{
	Type *type = GetDataType(id);
	if ( ! type )
		throw Exception(id, "data type not defined");
	return type->DataTypeStr();
	}

void Env::SetConstant(const ID* id, int constant)
	{
	LookUp(id, !RECURSIVE, RAISE_EXCEPTION)->SetConstant(constant);
	}

bool Env::GetConstant(const ID* id, int* pc) const
	{
	ASSERT(pc);
	// lookup without raising exception
	IDRecord* r = LookUp(id, RECURSIVE, !RAISE_EXCEPTION);
	if ( r )
		return r->GetConstant(pc);
	else
		return false;
	}

void Env::SetMacro(const ID* id, Expr *macro)
	{
	LookUp(id, RECURSIVE, RAISE_EXCEPTION)->SetMacro(macro);
	}

Expr* Env::GetMacro(const ID* id) const
	{
	return LookUp(id, RECURSIVE, RAISE_EXCEPTION)->GetMacro();
	}

void init_builtin_identifiers()
	{
#define DEFINE_ID(id_var, id_name) id_var = new ID(id_name);
#include "pac_builtin_id.def"
#undef DEFINE_ID

	current_decl_id = null_decl_id;
	}

Env* global_env()
	{
	static Env *the_global_env = 0;

	if ( ! the_global_env )
		{
		the_global_env = new Env(0, 0);

		// These two are defined in binpac.h, so we do not need to
		// generate code for them.
		the_global_env->AddConstID(bigendian_id, 0);
		the_global_env->AddConstID(littleendian_id, 1);
		the_global_env->AddConstID(unspecified_byteorder_id, -1);
		the_global_env->AddConstID(const_false_id, 0);
		the_global_env->AddConstID(const_true_id, 1);
		// A hack for ID "this"
		the_global_env->AddConstID(this_id, 0);
		the_global_env->AddConstID(null_id, 0, extern_type_nullptr);

                the_global_env->AddID(const_bytestring_cstr_id, FUNC_ID, extern_type_const_bytestring);
                the_global_env->SetEvaluated(const_bytestring_cstr_id);
		}

	return the_global_env;
	}

string set_function(const ID *id)
	{
	return strfmt("set_%s", id->Name());
	}
