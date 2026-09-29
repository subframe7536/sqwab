#define SQLITE_CORE
#include "sqlite-vec.h"

/* Distinct from sqlite-vec's own extra-init symbol in the static archive. */
int sqwab_sqlite3_wasm_extra_init(const char *unused) {
  (void)unused;
  return sqlite3_auto_extension((void (*)(void))sqlite3_vec_init);
}
