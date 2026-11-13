"""_paths.py -- where the data the Java transpiler ships with is found: the platform layer
(platform/canonical, reference, companions, assets) and the API maps (api_maps/), all inside the
checkout's data/java folder. Machine locations (the XDK, Java, the decompiler jar, overlays) are settings, not here.

    from codebreaker.java._paths import data_path
    data_path('platform', 'canonical')
"""
import os

_HERE = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), 'data', 'java')


def data_path(*parts):
    """Absolute path to a data file or folder shipped with the checkout (data/java)."""
    return os.path.join(_HERE, *parts)
