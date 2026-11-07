from typing import (
    Dict,
    List,
)


def _listdir(d: str) -> List[str]: ...


def find_base(project_dir: str) -> str: ...


def find_jar(base: str) -> str: ...


def find_java_src(project_dir: str, base: str) -> str: ...


def find_lang(base: str) -> str: ...


def find_resources(base: str) -> str: ...


def resolve(project_dir: str) -> Dict[str, str]: ...
