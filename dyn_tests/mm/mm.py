
class Point:
    __match_args__ = ('x', 'y')
    def __init__(self, x, y):
        self.x, self.y = x, y

def what(v):
    match v:
        case 0 | 1 as small:
            return 'small %d' % small
        case None:
            return 'none'
        case [first, *rest] if rest:
            return ('seq', first, rest)
        case {'kind': 'pt', **others}:
            return ('map', others)
        case Point(0, y=yy):
            return ('on y', yy)
        case Point(x, y):
            return ('pt', x, y)
        case str() | bytes():
            return 'text'
        case _:
            return 'other'

def loop(xs):
    out = []
    for x in xs:
        match x:
            case 'stop':
                break
            case _:
                out.append(x)
    return out

mode = 'a'
match mode:
    case 'a':
        chosen = 1
    case other:
        chosen = other
