from typing import Optional


def _corpus_sets(
    classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None]
) -> <class 'Union'>[Tuple[Set[Any], Set[Any]], Tuple[Set[str], Set[str]], Tuple[Set[str], Set[Any]]]: ...


def detect_edition(classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None]) -> <class 'Union'>[str, None]: ...


def edition_for_version(version_id: <class 'Union'>[str, None]) -> <class 'Union'>[str, None]: ...


def profile_name(edition: <class 'Union'>[str, None]) -> <class 'Union'>[str, None]: ...


def resolve(
    explicit: <class 'Union'>[str, None] = ...,
    mc_version: <class 'Union'>[str, None] = ...,
    classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None] = ...
) -> <class 'Union'>[str, None]: ...

from typing import Optional


def _corpus_sets(
    classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None]
) -> <class 'Union'>[Tuple[Set[str], Set[Any]], Tuple[Set[str], Set[str]], Tuple[Set[Any], Set[Any]]]: ...


def detect_edition(classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None]) -> <class 'Union'>[str, None]: ...


def edition_for_version(version_id: <class 'Union'>[str, None]) -> <class 'Union'>[str, None]: ...


def profile_name(edition: <class 'Union'>[str, None]) -> <class 'Union'>[str, None]: ...


def resolve(
    explicit: <class 'Union'>[str, None] = ...,
    mc_version: <class 'Union'>[str, None] = ...,
    classes: <class 'Union'>[List[codebreaker.java.ir.JClass], None] = ...
) -> <class 'Union'>[str, None]: ...
