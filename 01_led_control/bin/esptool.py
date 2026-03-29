#!/usr/bin/env python3
import sys
import subprocess
import os

# Compatibility shim for esptool to support 'merge-bin' syntax
args = sys.argv[1:]
# Replace merge-bin with merge_bin
args = [arg if arg != 'merge-bin' else 'merge_bin' for arg in args]

# Use the current Python interpreter to run the real esptool module
subprocess.run([sys.executable, '-m', 'esptool'] + args)
