"""ctree.py -- the decompiler's C as a syntax tree, for the passes of c_text.py that rewrite it.

A pass that finds what to change with a regular expression over the text also finds it inside a
string or a comment, takes `a.b._1_2_` for a piece of `b`, and cannot tell a declaration from an
expression that looks like one. Here the text is parsed (tree-sitter's C grammar, which goes on
past what it cannot parse -- a calling convention it does not know, a type never declared -- and
keeps every byte's place), a pass finds the nodes it means, and replaces exactly their text:
everything else stays as it was, byte for byte.

    src = ctree.Source(text)
    for node in src.find("field_expression"):
        ...
        src.replace(node, "new text")
    text = src.text()                   # the edits applied (they must not overlap)

Nodes are tree-sitter nodes: `.type`, `.children`, `.named_children`, `.child_by_field_name`,
`.start_byte`, `.end_byte`, `.parent`. `src.at(node)` is a node's text as written (with its
`__far` -- see Source.__init__)."""

import re

_PARSER = []

# The decompiler's own type names (decompile/c_text.py HEADER) and C's. The grammar does not know
# which names are types, so `(uint4)*p` reads as `(uint4) * p`, a product, `(uint2)&x` as a bitwise
# and, and `(uint2)(uint1)*p` as a call: misread() tells such a node from the cast it is.
_TYPE = (r"(?:u?int\d+|xunknown\d+|undefined\d*|unk(?:byte|int|uint)\d+|float\d+|code(?:_callee_pops)?|bool|char|"
         r"short|int|long|unsigned|signed|double|float|void)")
_QUALIFIERS = re.compile(rb"\b(?:__(?:far|near|huge|export|loadds|saveregs|interrupt|cdecl\w*|stdcall\w*|pascal\w*|"
                         rb"thiscall|fastcall)|_Packed)\b")
_LAST_LABEL = re.compile(rb"(?m)^(\s*(?!case\b|default\b)[A-Za-z_]\w*\s*):(?=\s*\})")
_CASTS = re.compile(r"(?:\(\s*%s(?:\s+%s)*(?:\s*(?:__far|__near))?(?:\s*\*+)?(?:\s*\[\d*\])?\s*\)\s*)+\Z" % (_TYPE, _TYPE))



def _parser():
    if not _PARSER:
        import tree_sitter
        import tree_sitter_c
        _PARSER.append(tree_sitter.Parser(tree_sitter.Language(tree_sitter_c.language())))
    return _PARSER[0]


class Source(object):
    def __init__(self, text):
        self.original = text
        self.data = text.encode("utf-8")
        # (parsed with the 16-bit qualifiers and the calling conventions blanked, byte for byte:
        # the grammar has no word for `__far`, and took `(char __far *)` for a cast to a type
        # named __far; every place in the tree is still a place in the text as written)
        # A label at the end of a block (`L:` then `}`) is not C -- a label needs a statement -- and
        # the grammar's recovery took what came after it for something else; read as `L;`.
        masked = _QUALIFIERS.sub(lambda m: b" " * len(m.group(0)), self.data)
        masked = _LAST_LABEL.sub(lambda m: m.group(1) + b";", masked)
        self.tree = _parser().parse(masked)
        self.root = self.tree.root_node
        self.edits = []

    # -- reading ----------------------------------------------------------------------------------
    def at(self, node):
        return self.data[node.start_byte:node.end_byte].decode("utf-8")

    def find(self, *types, under=None):
        """Every node of these types, in the order they appear (outermost first)."""
        out, todo = [], [under or self.root]
        while todo:
            n = todo.pop()
            if n.type in types:
                out.append(n)
            todo.extend(reversed(n.children))
        return out

    def body(self):
        """The function's compound statement (its `{ ... }`), or None."""
        for n in self.find("function_definition"):
            return n.child_by_field_name("body")
        return None

    def parameter_list(self):
        """The function's own `( ... )` (of the function being defined, not of a pointer it returns)."""
        for n in self.find("function_definition"):
            dcl = n.child_by_field_name("declarator")
            while dcl is not None and dcl.type != "function_declarator":
                dcl = dcl.child_by_field_name("declarator")
            return dcl.child_by_field_name("parameters") if dcl is not None else None
        return None

    def parameters(self):
        """Each parameter as written (stripped), or None when there is no parameter list."""
        plist = self.parameter_list()
        if plist is None:
            return None
        inner = self.data[plist.start_byte + 1:plist.end_byte - 1].decode("utf-8")
        return [x.strip() for x in inner.split(",")] if "(" not in inner else \
            [self.at(c) for c in plist.named_children]

    def leading(self, node, kind):
        """The outermost node of `kind` that starts where `node` starts (`node` itself, or the
        first thing in it: the cast in `(T *)x + 2`), else None."""
        n = node
        while n is not None and n.start_byte == node.start_byte:
            if n.type == kind:
                return n
            n = n.children[0] if n.children else None
        return None

    def is_call(self, node):
        """A call_expression that is a call: not a cast the grammar read as one (`(uint4)(x)`,
        `(uint2)(uint1)x`), nor a declaration with a type name in front (`uint2 (*p)`)."""
        f = self.at(node.child_by_field_name("function"))
        return not (_CASTS.match(f) or re.fullmatch(_TYPE, f.strip()))

    def misread(self, node):
        """For a binary `*`, `&`, `-` or `+` whose left side is nothing but casts -- `(uint4)`,
        `(uint2)(uint1)`, `(xunknown1 [4])` -- the operator of the unary expression the grammar
        took for an operation on two values; otherwise None."""
        if node.type != "binary_expression":
            return None
        op = node.child_by_field_name("operator").type
        if op in ("*", "&", "-", "+") and _CASTS.match(self.at(node.child_by_field_name("left"))):
            return op
        return None

    def unary(self, node):
        """(operator, where it starts) when `node` is the operand of a unary `*`, `&`, `-` or `+`
        -- also one the grammar read as a binary operation after a cast (see misread) -- else None."""
        up = node.parent
        if up is None:
            return None
        if up.type in ("pointer_expression", "unary_expression") and up.child_by_field_name("argument") == node:
            op = up.child_by_field_name("operator")
            return self.at(op), op.start_byte
        if self.misread(up) and up.child_by_field_name("right") == node:
            op = up.child_by_field_name("operator")
            return self.at(op), op.start_byte
        return None

    def line(self, node):
        """The text of the line `node` starts on (without its line end)."""
        s, e = self.line_span(node)
        return self.data[s:e].decode("utf-8").rstrip("\r\n")

    def line_span(self, node):
        """(start, end) of the line `node` starts on, its line end included."""
        s = self.data.rfind(b"\n", 0, node.start_byte) + 1
        e = self.data.find(b"\n", node.start_byte)
        return s, (len(self.data) if e < 0 else e + 1)

    def ancestors(self, node, *types):
        """The nodes `node` is inside, nearest first (only those of `types`, if given)."""
        out, up = [], node.parent
        while up is not None:
            if not types or up.type in types:
                out.append(up)
            up = up.parent
        return out

    def raw_arguments(self, call):
        """A call's arguments as written, each with the spaces and line breaks before it."""
        args = call.child_by_field_name("arguments")
        delims = [c for c in args.children if c.type in ("(", ",", ")")]
        out = []
        for a, b in zip(delims, delims[1:]):
            piece = self.data[a.end_byte:b.start_byte].decode("utf-8")
            if b.type == ")" and not piece.strip():
                continue                                     # (no argument after the last comma, or none at all)
            out.append(piece)
        return out

    def declared_name(self, declarator):
        """The name a declarator declares (through pointers, arrays, an initialiser)."""
        n = declarator
        while n is not None and n.type in ("pointer_declarator", "array_declarator", "init_declarator",
                                           "parenthesized_declarator"):
            n = n.child_by_field_name("declarator") or (n.named_children[0] if n.named_children else None)
        return self.at(n) if n is not None and n.type == "identifier" else None

    def render(self, node, override):
        """`node`'s text with each node inside it for which override(n) gives text replaced by that
        (override(node) itself is not asked)."""
        out, pos = [], node.start_byte
        for c in node.children:
            new = override(c)
            if new is None:
                if c.child_count:
                    out.append(self.data[pos:c.start_byte].decode("utf-8"))
                    out.append(self.render(c, override))
                    pos = c.end_byte
                continue
            out.append(self.data[pos:c.start_byte].decode("utf-8"))
            out.append(new)
            pos = c.end_byte
        out.append(self.data[pos:node.end_byte].decode("utf-8"))
        return "".join(out)

    @staticmethod
    def in_text(node):
        """Inside a string, a character constant or a comment: not code."""
        while node is not None:
            if node.type in ("string_literal", "char_literal", "comment", "system_lib_string"):
                return True
            node = node.parent
        return False

    # -- writing ----------------------------------------------------------------------------------
    def replace(self, node_or_span, text):
        s, e = (node_or_span.start_byte, node_or_span.end_byte) if hasattr(node_or_span, "start_byte") else node_or_span
        self.edits.append((s, e, text.encode("utf-8")))

    def insert(self, at, text):
        self.edits.append((at, at, text.encode("utf-8")))

    def text(self):
        if not self.edits:
            return self.original
        out, pos = [], 0
        for s, e, t in sorted(self.edits, key=lambda x: (x[0], x[1])):
            if s < pos:
                raise ValueError("overlapping edits at byte %d" % s)
            out.append(self.data[pos:s])
            out.append(t)
            pos = e
        out.append(self.data[pos:])
        return b"".join(out).decode("utf-8")


def rewrite(text, fn):
    """`fn(src)` records edits on the parsed text; the text with them applied."""
    src = Source(text)
    fn(src)
    return src.text()
