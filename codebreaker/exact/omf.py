"""omf.py -- now codebreaker/formats/omf.py (one OMF-86 reader for exact, invert and the builders);
this name is kept for exact's imports and is the same module."""
import sys

from codebreaker.formats import omf as _omf

sys.modules[__name__] = _omf
