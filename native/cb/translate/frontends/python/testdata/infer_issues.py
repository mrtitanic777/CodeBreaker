def f_global():
    global X
    return 1


def f_nested():
    def inner():
        return 1
    return 1


def f_with(path):
    with path:
        pass
    return 0


def f_lambda():
    g = lambda x: x
    return 1


def f_gen():
    yield 1


def f_star(*args):
    return 1


def f_raise_from():
    raise ValueError('x') from None


def f_try_finally():
    try:
        return 1
    finally:
        pass


def f_del(x: int):
    del x
    return 0


def f_walrus(x: int):
    if (y := x):
        return y
    return 0


def f_cmp_mixed(a: int, b: str):
    return a < b


def f_is(a: int, b: int):
    return a is b


def f_attr(x: int):
    return x.bit_length


def f_in_bytes(a: str, b: bytes):
    return a in b


def f_percent(n: int):
    return '%d %s' % (n,)


def f_unpack_len():
    a, b = (1, 2, 3)
    return a


def f_tuple_index():
    pair = (1, 'a')
    return pair[5]


def f_slice_step(data: bytes):
    return data[::2]


def f_bytes_percent():
    return b'%d' % 3


def f_div(n: int):
    return n / 3


def f_pow(n: int, m: int):
    return n ** m


def f_complex():
    return 3j


def f_ellipsis():
    return ...


def f_assert(x: int, y: int = 0):
    assert x > 0
    return x


def f_tuple_for():
    for v in (1, 'a'):
        pass
    return 0


def f_extern():
    import numpy
    return numpy


def f_call_kw(n: int):
    return f_assert(x=n, z=2)


def f_call_many(n: int):
    return f_assert(n, 1, 2)


def f_missing_arg():
    return f_assert()


def f_set_iter():
    for v in {1, 2}:
        pass
    return 0


def f_str_of_list(items: list):
    return f'{items}'


def f_ifexp(flag: bool):
    return 1 if flag else 'a'


def f_not_callable(n: int):
    return n()


def f_subscript_int(n: int):
    return n[0]
