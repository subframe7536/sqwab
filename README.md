# sqwab
This is a helper project for custom builds of the [wa-sqlite](https://github.com/rhashimoto/wa-sqlite) or the [official SQLite WASM library](https://sqlite.org/wasm/doc/trunk/index.md). It allows overriding Makefile settings and adding C code without creating your own build environment.

To use it:

* Fork or clone this repo.
* Add or edit files under [overlay.wa-sqlite](https://github.com/rhashimoto/sqwab/tree/master/overlay.wa-sqlite) or [overlay.sqlite/ext/wasm](https://github.com/rhashimoto/sqwab/tree/master/overlay.sqlite/ext/wasm) (in your fork). This will typically be:
  * Adding a modified [ext/wasm/GNUMakefile](https://sqlite.org/src/file?name=ext/wasm/GNUmakefile&ci=trunk), e.g. to change build options.
  * Using the [ext/wasm/sqlite3_wasm_extra_init.c](https://sqlite.org/src/info?name=0e362f3fc04eab6628cbe4f1e35f4ab4a200881f6b5f753b27fb45eabeddd9d2&ln=216-237) extension point to add C code.
* Once your changes are on GitHub, [manually run the workflow](https://docs.github.com/en/actions/using-workflows/manually-running-a-workflow) for wa-sqlite or SQLite. You will be prompted for some options:
  * wa-sqlite
    * SQLite source tree tag - Enter a SQLite source tree tag, e.g. "version-3.53.4"
    * Emscripten SDK version - This is the [Emscripten toolchain]([url](https://emscripten.org/docs/tools_reference/emsdk.html)https://emscripten.org/docs/tools_reference/emsdk.html) version. By default, this is something close to current when I created the workflow.
    * make arguments - Any arguments will be passed to make, e.g. to include full text search enter WASQLITE_EXTRA_DEFINES="-DSQLITE_ENABLE_FTS5".
  * SQLite
    * Source tarball URL - Enter a source code tarball link (see the [SQLite repo home](https://sqlite.org/src/doc/trunk/README.md) under "Obtaining the SQLite Source Code"). By default this is the latest release.
    * Emscripten SDK version - This is the [Emscripten toolchain]([url](https://emscripten.org/docs/tools_reference/emsdk.html)https://emscripten.org/docs/tools_reference/emsdk.html) version. By default, this is something close to current when I created the workflow.
* A successful run of the workflow will produce a new [GitHub release](https://docs.github.com/en/repositories/releasing-projects-on-github) on your fork with an attached file containing the built artifacts.

## sqlite-vec in wa-sqlite

The workflow clones upstream wa-sqlite `master` on each run and records its commit in the release notes. The wa-sqlite builds include [sqlite-vec](https://github.com/asg017/sqlite-vec) as a statically linked extension. Its source is the `third_party/sqlite-vec` git submodule; the committed submodule revision pins the version. The wa-sqlite overlay's `sqlite3_wasm_extra_init.c` registers sqlite-vec during SQLite initialization, and its `GNUmakefile` adds the archive to the upstream build. Every opened database can use sqlite-vec without `.load` or a JS initialization call. Dynamic extension loading remains disabled.

```sql
SELECT vec_version();

CREATE VIRTUAL TABLE items USING vec0(
  embedding float[2]
);
```

To upgrade sqlite-vec, explicitly update and commit the submodule revision. The separate official SQLite WASM build does not include this extension.

If you want to change the workflow itself, they are under .github/workflows/.

If the build workflow fails with an error like `HTTP 403: Resource not accessible by integration`, you may need to change your [GitHub Actions permissions](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/enabling-features-for-your-repository/managing-github-actions-settings-for-a-repository#managing-github-actions-permissions-for-your-repository). Make sure that "Workflow permissions" is set to "Read and write permissions".
