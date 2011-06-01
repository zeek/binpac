#include "pac_dbg.h"
#include "pac_output.h"

extern bool FLAGS_add_debug_messages;

void AddDebugMsg(Output *out_cc, const string &msg) {
  if (!FLAGS_add_debug_messages) return;
  out_cc->println("fprintf(stderr, \"%s\\n\");", msg.c_str());
}
