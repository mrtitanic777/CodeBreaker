@echo off
REM Convenience wrapper: run the clean-room comment stripper from the repo root.
REM   cleanroom C:\path\to\code            (preview only)
REM   cleanroom C:\path\to\code --apply    (rewrite files)
python "%~dp0tools\cleanroom_stripper.py" %*
