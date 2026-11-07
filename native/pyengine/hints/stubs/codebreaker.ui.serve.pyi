from codebreaker.ui.service import Client
from typing import List


def _bind(first: int, secret: str, idle: float) -> Service: ...


def _say(c: Client) -> str: ...


def _write(path: str, text: str) -> None: ...


def code_stamp() -> List[int]: ...


def fixed_port() -> int: ...


def idle_minutes() -> float: ...


def log(text: str) -> None: ...


def main(argv: List[<class 'Union'>[Any, str]]) -> int: ...


class Server:
    def server_bind(self) -> None: ...


class Service:
    def __init__(self, port: int, secret: str, idle: float) -> None: ...
