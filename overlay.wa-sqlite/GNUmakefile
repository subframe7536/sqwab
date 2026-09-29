SQLITE_VEC_DIR := $(abspath ../third_party/sqlite-vec)
SQLITE_VEC_ARCHIVE := $(SQLITE_VEC_DIR)/dist/.wasm/libsqlite_vec.wasm.a

# Make expands target prerequisites while reading the upstream Makefile.
override CFILES_EXTRA += sqlite3_wasm_extra_init.c

include Makefile

# Keep command-line CFLAGS_EXTRA/EMFLAGS_EXTRA while adding the static extension.
override CFLAGS_EXTRA += -I$(SQLITE_VEC_DIR) -DSQLITE_EXTRA_INIT=sqwab_sqlite3_wasm_extra_init
# The upstream archive precedes the object files in wa-sqlite's link command.
# Force extraction of sqlite3_vec_init without pulling its own init object.
override EMFLAGS_EXTRA += -Wl,--undefined=sqlite3_vec_init $(SQLITE_VEC_ARCHIVE)

$(SQLITE_VEC_ARCHIVE): deps/$(SQLITE_VERSION)/sqlite3.h
	$(MAKE) -C $(SQLITE_VEC_DIR) sqlite-vec.h
	CPATH="$(abspath deps/$(SQLITE_VERSION))$${CPATH:+:$$CPATH}" \
	  $(MAKE) -C $(SQLITE_VEC_DIR) dist/.wasm/libsqlite_vec.wasm.a

dist/wa-sqlite.mjs dist/wa-sqlite-async.mjs dist/wa-sqlite-jspi.mjs: $(SQLITE_VEC_ARCHIVE)
