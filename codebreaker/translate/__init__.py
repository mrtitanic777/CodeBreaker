"""codebreaker.translate -- the universal translator: front ends (a language -> the IR), passes on the
IR, and back ends (the IR -> a language). See docs/TRANSLATOR_DESIGN.md.

Phase 1 is under way: the Java front end (javac's typed tree), the typed IR for method bodies, and a
C++03 back end beside the transpiler's text path in codebreaker/java.
"""
