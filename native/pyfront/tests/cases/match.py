match command.split():
    case [action]:
        pass
    case [action, obj]:
        pass
    case Point(x=0, y=0):
        print("Origin")
    case Point(1, 2, x=3):
        pass
    case [Point(x=0, y=0), *rest]:
        pass
    case {"x": x, "y": y, **rest}:
        pass
    case {1: a, None: b, True: c, x.y: d}:
        pass
    case 0 | 1 | 2:
        pass
    case -1 | 1.5 | -2j | 3 + 4j | 3 - 4j | -3 + 4j:
        pass
    case "a" "b" | b"c":
        pass
    case None | True | False:
        pass
    case Color.RED | a.b.c:
        pass
    case (1, 2) as t:
        pass
    case [1, 2, *_]:
        pass
    case (x,):
        pass
    case ():
        pass
    case []:
        pass
    case (x):
        pass
    case x if x > 0:
        pass
    case _:
        pass
#----
match x, y:
    case a, b:
        pass
    case *a, b:
        pass
    case a, *_:
        pass
#----
match (x):
    case 1: pass
#----
match *a, b:
    case _: pass
#----
match x:
    case {}:
        pass
    case {**rest}:
        pass
    case C():
        pass
    case C(a, b=c,):
        pass
    case [a, (b | c) as d]:
        pass
#----
match = {'a': 1}
match['a']
match(x)
match.x = 1
print(match)
#----
case = 1
case.x = case
#----
def match(x): return x
match(1)
