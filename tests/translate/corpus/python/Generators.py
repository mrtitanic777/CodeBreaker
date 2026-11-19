"""Generators: `yield` in loops, a generator that reads another, an early return, items of several kinds of value."""


def count_to(n):
    for i in range(n):
        yield i


def evens(limit):
    for x in count_to(limit):
        if x % 2 == 0:
            yield x


def fib(n):
    a = 0
    b = 1
    for _ in range(n):
        yield a
        a, b = b, (a + b) % 1000


def words():
    yield "alpha"
    yield "beta"
    yield "gamma"


def until_negative(values):
    for v in values:
        if v < 0:
            return
        yield v * 10


def run_count():
    out = []
    for x in count_to(5):
        out.append(x)
    return out


def run_sum_evens():
    total = 0
    for x in evens(10):
        total = (total + x) % 1000
    return total


def run_fib():
    out = []
    for x in fib(12):
        out.append(x)
    return out


def run_words():
    out = []
    for w in words():
        out.append(w.upper())
    return out


def run_early_return():
    out = []
    for v in until_negative([1, 2, 3, -1, 5, 6]):
        out.append(v)
    return out


def run_nested_use():
    pairs = []
    for a in count_to(3):
        for b in evens(5):
            pairs.append(a * 10 + b)
    return pairs
