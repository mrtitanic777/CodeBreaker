"""show.py -- what the knowledge base holds, and where each piece came from.

    python -m codebreaker knowledge                          every platform's kinds
    python -m codebreaker knowledge <platform>               ... of one platform
    python -m codebreaker knowledge <platform> <kind>        its keys (the first 200)
    python -m codebreaker knowledge <platform> <kind> <key>  one piece: its value, source, origin
    python -m codebreaker knowledge --load                   load every authored file first
"""
import json
import time

from codebreaker import knowledge
from codebreaker.knowledge import sources, store


def main(argv):
    kb = store.open()
    if "--load" in argv:
        argv = [a for a in argv if a != "--load"]
        for p, k in sources.AUTHORED:
            sources.authored(p, k, kb)
    if len(argv) <= 1:
        want = knowledge.platform(argv[0]) if argv else None
        print("%s" % kb.path)
        for p, k, n, size, origins, source in kb.kinds(want):
            print("  %-8s %-24s %7d  %8.1f KB  %-28s %s" % (p, k, n, size / 1024.0,
                                                           ", ".join("%s %d" % o for o in sorted(origins.items())),
                                                           (source or "")[:72] + ("..." if len(source or "") > 72 else "")))
        total, distinct, shared, copies = kb.shared_bodies()
        print("  (bodies) %d recorded, %d distinct, %d in two or more modules" % (total, distinct, shared))
        return 0
    p, kind = knowledge.platform(argv[0]), argv[1]
    if len(argv) == 2:
        t = knowledge.table(p, kind)
        for key in list(t)[:200]:
            print(key)
        print("(%d)" % len(t))
        return 0
    key = argv[2]
    value = knowledge.table(p, kind).get(key, kb.get(p, kind, key))
    about = kb.about(p, kind, key)
    if about is None:
        print("nothing known: %s %s %s" % (p, kind, key))
        return 1
    print(json.dumps(value, indent=1, default=repr)[:20000])
    source, origin, made, digest = about
    print("source: %s\norigin: %s\nmade:   %s\nhash:   %s" % (source, origin,
                                                            time.strftime("%Y-%m-%d %I:%M %p", time.localtime(made)),
                                                            digest))
    return 0
