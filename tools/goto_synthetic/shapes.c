/* Functions written to have the flow shapes the structurer turns into gotos
   (docs/STRUCTURING_PLAN.md), for tools/goto_synthetic.py: built, decompiled, rebuilt and run
   beside this build. Each takes plain numbers or a buffer and returns a number, so what it does
   can be compared; none calls anything. */

#define EXPORT __declspec(dllexport)

/* out of a loop, past code: the normal exit runs code the early exit skips */
EXPORT int __stdcall find_or_count(int *a, int n, int key)
{
    int i, extra = 0;
    for (i = 0; i < n; i++) {
        if (a[i] == key)
            goto found;
        extra += a[i] & 3;
    }
    extra = extra * 7 + n;
    a[0] = extra;
found:
    return i * 100 + extra;
}

/* out of two nested loops at once */
EXPORT int __stdcall grid_search(int *a, int w, int h, int key)
{
    int x, y, steps = 0;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            steps++;
            if (a[y * w + x] == key)
                goto done;
        }
        steps += 2;
    }
    steps = -steps;
done:
    return steps;
}

/* a loop with a second entry: the first pass starts in the middle */
EXPORT int __stdcall middle_entry(int n, int seed)
{
    int acc = seed, i = 0;
    if (seed & 1)
        goto inside;
    while (i < n) {
        acc = acc * 3 + 1;
inside:
        acc ^= i;
        i++;
    }
    return acc;
}

/* a loop made only of a goto, with a retry from two places */
EXPORT int __stdcall retry(int n, int limit)
{
    int tries = 0, v = n;
again:
    tries++;
    v = (v * 5 + 3) & 0xff;
    if (v < limit && tries < 20)
        goto again;
    if ((v & 1) && tries < 20) {
        v += 7;
        goto again;
    }
    return v * 32 + tries;
}

/* a jump from one branch into the tail of the other */
EXPORT int __stdcall crossing(int a, int b)
{
    int r;
    if (a > b) {
        r = a - b;
        if (r > 10)
            goto shared;
        r *= 2;
    } else {
        r = b - a;
        r += 3;
shared:
        r = r * r + a;
    }
    return r;
}

/* out of nested blocks to code that goes on (no return right after the label) */
EXPORT int __stdcall nested_out(int a, int b, int c)
{
    int r = 0;
    if (a) {
        r = 1;
        if (b) {
            r = 2;
            if (c > 5)
                goto merge;
            r += c;
        }
        r *= 3;
    }
    r += 100;
merge:
    r = r * 2 + a;
    if (r & 1)
        r -= b;
    return r;
}

/* a clean-up label reached from error paths: the gotos a programmer writes */
EXPORT int __stdcall cleanup(int *buf, int n)
{
    int status = 0, i;
    buf[0] = 1;
    if (n <= 0) { status = -1; goto out; }
    for (i = 1; i < n; i++) {
        if (buf[i] < 0) { status = -2; goto out; }
        buf[i] += buf[i - 1];
    }
    if (buf[n - 1] > 1000) { status = -3; goto out; }
    status = buf[n - 1];
out:
    buf[0] = 0;
    return status;
}

/* a state machine: gotos between states */
EXPORT int __stdcall states(int *in, int n)
{
    int i = 0, count = 0;
s0:
    if (i >= n) return count;
    if (in[i++] > 0) goto s1;
    goto s0;
s1:
    count++;
    if (i >= n) return count;
    if (in[i++] < 0) goto s0;
    goto s1;
}
