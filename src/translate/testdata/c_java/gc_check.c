/* The C runtime's collector on its own: garbage is freed, what is reachable stays intact, and a pointer only on the stack keeps its block. */
#include "jrt.h"

static jarray* kept;
static int failures;
static void check(int ok, const char* what) { if (!ok) { ++failures; printf("FAILED: %s\n", what); } }

static void garbage(int n) {
	for (int i = 0; i < n; ++i) {
		jarray* a = jrt_new_array(sizeof(jobject*), 1000);
		jarray* b = jrt_new_array(sizeof(jobject*), 1000);
		((jobject**)a->data)[0] = (jobject*)b;      /* a ring: nothing counts references here */
		((jobject**)b->data)[0] = (jobject*)a;
	}
}

static void program(jarray* args) {
	(void)args;
	kept = jrt_new_array(sizeof(jobject*), 100);        /* reachable from a static */
	for (int i = 0; i < 100; ++i) {
		jarray* e = jrt_new_array(sizeof(int32_t), 10);
		((int32_t*)e->data)[3] = i;
		((jobject**)kept->data)[i] = (jobject*)e;
	}
	jarray* volatile local = jrt_new_array(sizeof(int32_t), 4);   /* reachable only from the stack */
	((int32_t*)local->data)[2] = 77;
	size_t before = jrt_gc_collect();
	garbage(20000);                                     /* about 320 MB in all */
	size_t after = jrt_gc_collect();
	check(after < before + 64 * 1024 * 1024, "garbage is freed");
	check(((int32_t*)local->data)[2] == 77, "a block held by the stack stays");
	for (int i = 0; i < 100; ++i) check(((int32_t*)((jarray*)((jobject**)kept->data)[i])->data)[3] == i, "a block held by a static stays");
	printf("before %zu, after %zu, failures %d\n", before, after, failures);
	fflush(stdout);
	if (failures) exit(1);
}

int main(int argc, char** argv) { return jrt_run_main(program, argc, argv); }
