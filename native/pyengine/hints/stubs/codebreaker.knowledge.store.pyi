from codebreaker.engine.native_engine import KnowledgeBase
from typing import Optional


def KnowledgeBase(path: str) -> KnowledgeBase: ...


def default_path() -> str: ...


def open(path: None = ...) -> KnowledgeBase: ...
