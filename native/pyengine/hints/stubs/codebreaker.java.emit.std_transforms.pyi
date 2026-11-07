from re import Pattern


def _annotate_first(text: str, pattern: str, annotation: str) -> str: ...


def _sub_outside_literals(rx: Pattern, repl: str, s: str) -> str: ...


def apply_std_transforms(text: str, cpp_std: <class 'Union'>[None, codebreaker.java.transpile.CppStandard]) -> str: ...


def sanitize_reserved_members(text: str) -> str: ...

from re import Pattern


def _annotate_first(text: str, pattern: str, annotation: str) -> str: ...


def _sub_outside_literals(rx: Pattern, repl: str, s: str) -> str: ...


def apply_std_transforms(text: str, cpp_std: <class 'Union'>[codebreaker.java.transpile.CppStandard, None]) -> str: ...


def sanitize_reserved_members(text: str) -> str: ...
