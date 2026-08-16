# -*- coding: utf-8 -*-
"""Top-level entry point for PyInstaller (avoids relative-import issue in __main__.py)."""
import sys
from j2ctool.cli import main

if __name__ == '__main__':
    sys.exit(main())
