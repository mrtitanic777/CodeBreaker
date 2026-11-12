# -*- coding: utf-8 -*-
"""What a stub/reference header PROVIDES (platform_coverage._provided_symbols): function names, `const T NAME`
constants, `#define NAME` constants and `NAME =` enumerators."""

from codebreaker.java.projects.minecraft import platform_coverage


def test_define_constants_are_provided():
    # `\b#define` could only match after a word character, so a header's #define lines were never counted
    syms = platform_coverage._provided_symbols('#define GL_FOO_BAR 1\n#  define MAX_LEN 4\n#define AB 1\n'
                                               '#define Mixed_Case 2\n')
    assert syms == {'GL_FOO_BAR', 'MAX_LEN'}


def test_functions_constants_and_enumerators():
    syms = platform_coverage._provided_symbols(
        'const int GL_BAZ_QUX = 3;\nenum { ABC_DEF = 2, GHI_JKL=4 };\nvoid f(int x); int g (int y);\n'
        'if (x) while(y) for(;;) return(1); switch(a) sizeof(b);\n')
    assert syms == {'GL_BAZ_QUX', 'ABC_DEF', 'GHI_JKL', 'f', 'g'}


def test_used_holder_symbols_and_type_counts():
    used = platform_coverage._used_holder_symbols(
        'GL11::glBegin(1); GL11 :: glEnd(); GL11::glBegin(2);\n  MyGL11::glNope(); GL11::\nglLate();\n', 'GL11')
    assert used == {'glBegin': 2, 'glEnd': 1, 'glLate': 1}
    assert platform_coverage._type_usage_count(
        'new Socket(1); Socket* s; List<Socket> l; MySocket m; Socketry r; a::Socket q;', 'Socket') == 3
