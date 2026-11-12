LIMIT = 100
NAMES = ['a', 'b']


class Point:
    ORIGIN = 0

    def __init__(self, x: int, y: int = 0):
        self.x = x
        self.y = y

    def total(self):
        return self.x + self.y


class Labeled(Point):
    def __init__(self, x: int, label: str):
        Point.__init__(self, x, 1)
        self.label = label

    def describe(self):
        return self.label


def make(n):
    p = Point(n, 2)
    return p.total()


def caller():
    return make(5) + make(300)


def clamp(v: int):
    if v < 0:
        return 0
    if v > 255:
        return 255
    return v


def count_up(n: int):
    i = 0
    total = 0
    while i < n:
        total += i
        i += 1
    return total


def sum_range():
    s = 0
    for i in range(10):
        s += i
    return s


def grow():
    k = 0
    while True:
        k += 1
    return k


def read(data: bytes, p: int):
    return data[p] + data[p + 1]


def opt(x):
    if x is None:
        return 0
    return x


def uses_opt():
    return opt(None) + opt(3)


def build():
    rec = {}
    rec['a'] = 1
    rec['b'] = 'x'
    return rec


def collect():
    out = [0]
    for i in range(4):
        out[0] = i
    return out


def head(data: bytes):
    first = data[0:4]
    pair = (1, 'a')
    return pair[1] + str_of(first)


def str_of(data: bytes):
    try:
        return 'bytes'
    except ValueError:
        return 'none'


def bad():
    return missing_name + len('x')


def mixed(flag: bool):
    if flag:
        return 1
    return 'one'


def unpack():
    a, b = 1, 'two'
    return a


def fmt(n: int):
    return f'{n:08x} {n!r}'


def labeled():
    item = Labeled(3, 'x')
    return item.describe()
