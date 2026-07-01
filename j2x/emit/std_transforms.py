# -*- coding: utf-8 -*-
"""std_transforms.py -- C++ standard-revision text transforms for emitted code.

Applied as a final pass over rendered .h/.cpp text. Each transform is gated on
the target CppStandard year so C++03 output is untouched.

Transforms:
  C++11+  NULL -> nullptr  (outside string/char literals)
  C++11+  verbose iterator types in for-loops and declarations -> auto
  C++11+  static const int X = N  ->  constexpr int X = N  (in enum structs)

Each transform annotates the FIRST occurrence in a file with an inline comment
identifying which C++ standard introduced it, so the developer knows at a glance.
"""

import re

_NULL_WORD = re.compile(r'\bNULL\b')
_STR_LITERAL = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')

_ITER_FOR = re.compile(
    r'for\s*\(\s*'
    r'((?:const\s+)?(?:std::)?(?:map|set|vector|list|deque)<[^>]*(?:<[^>]*>)*>\s*::\s*'
    r'(?:const_)?iterator)\s+'
    r'(\w+)')

_ITER_DECL = re.compile(
    r'^(\s*)((?:const\s+)?(?:std::)?(?:map|set|vector|list|deque)<[^>]*(?:<[^>]*>)*>\s*::\s*'
    r'(?:const_)?iterator)\s+(\w+\s*=)',
    re.MULTILINE)

_STATIC_CONST_INT = re.compile(
    r'\bstatic\s+const\s+int\b')


def _sub_outside_literals(rx, repl, s):
    """Apply re.sub only to parts of s outside string/char literals."""
    out, last = [], 0
    for m in _STR_LITERAL.finditer(s):
        before = s[last:m.start()]
        out.append(rx.sub(repl, before))
        out.append(m.group(0))
        last = m.end()
    out.append(rx.sub(repl, s[last:]))
    return ''.join(out)


def _annotate_first(text, pattern, annotation):
    """Find the first line containing `pattern` and append `annotation` as an
    inline comment, but only if that line doesn't already have one."""
    lines = text.split('\n')
    for i, line in enumerate(lines):
        if pattern in line and '//' not in line.split(pattern, 1)[-1]:
            lines[i] = line.rstrip() + '  ' + annotation
            break
    return '\n'.join(lines)


def apply_std_transforms(text, cpp_std):
    """Apply C++ standard-specific text transforms to emitted code."""
    if cpp_std is None or cpp_std.year < 2011:
        return text

    had_null = bool(_NULL_WORD.search(text))
    text = _sub_outside_literals(_NULL_WORD, 'nullptr', text)
    if had_null:
        text = _annotate_first(text, 'nullptr', '// [C++11] nullptr replaces NULL')

    had_iter = bool(_ITER_FOR.search(text)) or bool(_ITER_DECL.search(text))
    text = _ITER_FOR.sub(lambda m: 'for(auto ' + m.group(2), text)
    text = _ITER_DECL.sub(lambda m: m.group(1) + 'auto ' + m.group(3), text)
    if had_iter:
        text = _annotate_first(text, 'auto ', '// [C++11] auto type deduction')

    had_const = bool(_STATIC_CONST_INT.search(text))
    text = _sub_outside_literals(_STATIC_CONST_INT, 'static constexpr int', text)
    if had_const:
        text = _annotate_first(text, 'constexpr', '// [C++11] constexpr replaces const')

    return text
