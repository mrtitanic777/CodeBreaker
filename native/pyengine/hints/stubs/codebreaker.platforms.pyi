from capstone import Cs
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Type,
    Union,
)


def all() -> List[Platform]: ...


def decoder(bits: int = ..., arch: str = ..., detail: bool = ...) -> Cs: ...


def for_machine(machine: int) -> <class 'Union'>[codebreaker.platforms.Platform, None]: ...


def identify(path: str) -> <class 'Union'>[codebreaker.platforms.Platform, None]: ...


def lane_of(path: str) -> <class 'Union'>[str, None]: ...


def lanes() -> Dict[str, str]: ...


def machine(
    name: str,
    title: str,
    family: str,
    container: str,
    cpu: Cpu,
    recognise: Callable,
    reader: Tuple[str, str],
    runner: Tuple[str, str],
    assembler: Tuple[str, str],
    writer: Tuple[str, str],
    results: <class 'Union'>[Tuple[str, str, str, str, str, str, str], Tuple[str, str, str, str], Tuple[str, str, str, str, str], Tuple[str, str, str, str, str, str, str, str, str, str, str, str, str, str, str]],
    toolchain: str = ...
) -> Platform: ...


def named(name: str) -> Platform: ...


def of(kind: str) -> Platform: ...


def pe_machine(path: str) -> int: ...


def runners() -> Dict[str, Tuple[str, str]]: ...


def step_modules() -> Dict[str, str]: ...


class Compiler:
    def options(self) -> List[str]: ...


class Cpu:
    @staticmethod
    def __new__(cls: Type[Cpu], language: str = ..., **fields) -> Cpu: ...
    def disassembler(self, detail: bool = ...) -> Cs: ...
    def given(self) -> Dict[str, <class 'Union'>[<class 'Union'>, str]]: ...


class Platform:
    def __init__(
        self,
        name: str,
        title: str,
        family: str,
        cpu: Cpu,
        source: str,
        lane: <class 'Union'>[str, None],
        recognise: Callable,
        runner: None = ...,
        machine: <class 'Union'>[int, None] = ...,
        open: None = ...,
        container: str = ...,
        loader: <class 'Union'>[Loader, None] = ...,
        convention: <class 'Union'>[Convention, None] = ...,
        emulator: <class 'Union'>[Emulator, None] = ...,
        compiler: <class 'Union'>[Compiler, None] = ...,
        packager: <class 'Union'>[Packager, None] = ...,
        steps: <class 'Union'>[str, None] = ...
    ) -> None: ...
    def readable(self, path: str) -> str: ...
    def recognises(self, path: str) -> bool: ...
    @property
    def runner(self) -> <class 'Union'>[Tuple[str, str], None]: ...
    def slots(self) -> Dict[str, <class 'Union'>]: ...


class Ref:
    def __call__(self, *args, **kw) -> Any: ...
    def __init__(self, module: str, name: str, *args, **kw) -> None: ...
    def pair(self) -> Tuple[str, str]: ...
    def target(self) -> <class 'Union'>[Type[codebreaker.formats.ne.NEFile], Callable, List[str]]: ...


class Slot:
    def __bool__(self) -> bool: ...
    def __init__(self, **fields) -> None: ...
    def given(self) -> Dict[str, <class 'Union'>[<class 'Union'>, codebreaker.platforms.Ref, str]]: ...
