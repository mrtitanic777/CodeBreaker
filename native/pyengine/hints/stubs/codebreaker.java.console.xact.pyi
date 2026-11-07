from typing import (
    Dict,
    List,
    Set,
    Union,
)


def _cue_name(event: str) -> str: ...


def _ogg_keys(rel_path: str) -> Set[str]: ...


def match_sources(java_dir: str, resources_dir: str) -> Dict[str, <class 'Union'>]: ...


def needed_cues(java_dir: str) -> Dict[str, List[str]]: ...


def scan_ogg_sources(resources_dir: str) -> Dict[str, List[str]]: ...


def write_ffmpeg_script(
    match: Dict[str, <class 'Union'>],
    out_dir: str,
    resources_dir: str,
    wav_subdir: str = ...,
    bat_name: str = ...
) -> str: ...
