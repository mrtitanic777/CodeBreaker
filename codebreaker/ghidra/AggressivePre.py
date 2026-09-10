# preScript: enable the heavier analyzers that raise decompile accuracy.
for name, val in [
    ("Decompiler Parameter ID", "true"),      # re-derive real param counts/types
    ("Aggressive Instruction Finder", "true"), # find code in gaps
    ("Decompiler Switch Analysis", "true"),
    ("Call Convention ID", "true"),
]:
    try:
        setAnalysisOption(currentProgram, name, val)
        print("PRE set %s=%s" % (name, val))
    except Exception as e:
        print("PRE opt %s ERR %s" % (name, e))
