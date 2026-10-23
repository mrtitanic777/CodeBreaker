#!/usr/bin/env python3

from . import patch_compile
from . import se_patcher
from . import patch_checker
def main() -> None:
	print("Compiling...")
	patch_compile.main()
	# print("Patching...")
	# se_patcher.main()
	print("Checking...")
	patch_checker.main()

	print("Done!")

if __name__ == "__main__":
	main()