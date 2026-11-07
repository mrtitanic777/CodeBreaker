from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _apply_layout(cfg: Dict[str, <class 'Union'>]) -> Dict[str, str]: ...


def _choices_md(
    cfg: Dict[str, <class 'Union'>],
    sdk_info: <class 'Union'>[codebreaker.java.console.sdk.SdkInfo, None] = ...,
    unverified: <class 'Union'>[List[<class 'Union'>[Any, str]], None] = ...
) -> str: ...


def _controller_header(cfg: Dict[str, <class 'Union'>]) -> str: ...


def _decisions() -> List[Dict[str, <class 'Union'>]]: ...


def _get_path(cfg: Dict[str, <class 'Union'>], path: str) -> str: ...


def _kernel_files(cfg: Dict[str, <class 'Union'>]) -> Tuple[str, str]: ...


def default_config() -> Dict[str, <class 'Union'>]: ...


def emit(
    out_dir: str,
    cfg: Dict[str, <class 'Union'>],
    sdk_info: <class 'Union'>[codebreaker.java.console.sdk.SdkInfo, None] = ...,
    unverified: <class 'Union'>[List[<class 'Union'>[Any, str]], None] = ...
) -> List[str]: ...


def gate_sdk(
    cfg: Dict[str, <class 'Union'>],
    platform: str = ...,
    sdk_override: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[Tuple[None, List[Any]], Tuple[codebreaker.java.console.sdk.SdkInfo, List[Any]], Tuple[codebreaker.java.console.sdk.SdkInfo, List[str]]]: ...


def load_config(path: str) -> Dict[str, <class 'Union'>]: ...


def required_symbols(cfg: Dict[str, <class 'Union'>]) -> List[<class 'Union'>[Any, str]]: ...


def run(
    out_dir: str,
    config_path: None = ...,
    updates: <class 'Union'>[Dict[str, str], None] = ...,
    list_only: bool = ...,
    platform: str = ...,
    sdk_override: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[Tuple[Dict[str, <class 'Union'>], List[str], codebreaker.java.console.sdk.SdkInfo, List[Any]], Tuple[Dict[str, <class 'Union'>], List[str], codebreaker.java.console.sdk.SdkInfo, List[str]], Tuple[Dict[str, <class 'Union'>], List[str], None, List[Any]]]: ...


def save_config(path: str, cfg: Dict[str, <class 'Union'>]) -> None: ...

from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _apply_layout(cfg: Dict[str, <class 'Union'>]) -> Dict[str, str]: ...


def _choices_md(
    cfg: Dict[str, <class 'Union'>],
    sdk_info: <class 'Union'>[codebreaker.java.console.sdk.SdkInfo, None] = ...,
    unverified: <class 'Union'>[List[<class 'Union'>[Any, str]], None] = ...
) -> str: ...


def _controller_header(cfg: Dict[str, <class 'Union'>]) -> str: ...


def _decisions() -> List[Dict[str, <class 'Union'>]]: ...


def _get_path(cfg: Dict[str, <class 'Union'>], path: str) -> str: ...


def _kernel_files(cfg: Dict[str, <class 'Union'>]) -> Tuple[str, str]: ...


def default_config() -> Dict[str, <class 'Union'>]: ...


def emit(
    out_dir: str,
    cfg: Dict[str, <class 'Union'>],
    sdk_info: <class 'Union'>[codebreaker.java.console.sdk.SdkInfo, None] = ...,
    unverified: <class 'Union'>[List[<class 'Union'>[Any, str]], None] = ...
) -> List[str]: ...


def gate_sdk(
    cfg: Dict[str, <class 'Union'>],
    platform: str = ...,
    sdk_override: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[Tuple[codebreaker.java.console.sdk.SdkInfo, List[Any]], Tuple[codebreaker.java.console.sdk.SdkInfo, List[str]], Tuple[None, List[Any]]]: ...


def load_config(path: str) -> Dict[str, <class 'Union'>]: ...


def required_symbols(cfg: Dict[str, <class 'Union'>]) -> List[<class 'Union'>[Any, str]]: ...


def run(
    out_dir: str,
    config_path: None = ...,
    updates: <class 'Union'>[Dict[str, str], None] = ...,
    list_only: bool = ...,
    platform: str = ...,
    sdk_override: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[Tuple[Dict[str, <class 'Union'>], List[str], codebreaker.java.console.sdk.SdkInfo, List[Any]], Tuple[Dict[str, <class 'Union'>], List[str], codebreaker.java.console.sdk.SdkInfo, List[str]], Tuple[Dict[str, <class 'Union'>], List[str], None, List[Any]]]: ...


def save_config(path: str, cfg: Dict[str, <class 'Union'>]) -> None: ...
