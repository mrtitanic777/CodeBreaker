from codebreaker.java.ir import (
    Field,
    JClass,
    JType,
    Method,
)
from codebreaker.java.types import TypeResolver
from typing import (
    Any,
    Dict,
    Iterator,
    List,
    Set,
)


def _body_text(method: Method) -> str: ...


def _is_alloc_rhs(rhs: str) -> bool: ...


def _method_texts(cls: JClass) -> Iterator[Any]: ...


def _param_retained(name: str, text: str) -> bool: ...


def _reassigned_params(method: Method) -> Set[str]: ...


def _returns_container(rt: JType) -> bool: ...


def _returns_scalar_pointer(method: Method, resolver: TypeResolver) -> bool: ...


def aggregate_array_body(field: Field) -> <class 'Union'>[str, None]: ...


def nullable_value_params(method: Method, resolver: TypeResolver) -> Set[str]: ...


def owned_containers(cls: JClass, resolver: TypeResolver) -> Dict[str, str]: ...


def owned_pointers(cls: JClass, resolver: TypeResolver) -> Dict[str, bool]: ...


def owned_return_container_methods(
    classes: List[JClass],
    resolver: TypeResolver
) -> frozenset: ...


def owned_scalar_return_methods(
    classes: List[JClass],
    resolver: TypeResolver
) -> Dict[str, str]: ...


def owns_heap(cls: JClass, resolver: TypeResolver) -> bool: ...


def params_str(method: Method, resolver: TypeResolver) -> str: ...


def retained_param_positions(classes: List[JClass]) -> Dict[str, Set[int]]: ...


def return_cpp_type(method: Method, resolver: TypeResolver) -> str: ...


def returns_owned_container(method: Method, resolver: TypeResolver) -> bool: ...


def returns_owned_scalar(method: Method, resolver: TypeResolver) -> bool: ...


def returns_pointer(method: Method, resolver: TypeResolver) -> bool: ...
