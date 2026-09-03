; shell_pcode_06.asm -- SHELL object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_6_0000:
        db  00Dh, 000h, 000h, 000h, 028h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [9b021f00h], bh
        add    al, byte ptr [ebx + eax - 75h]
        add    edx, edx
        add    esp, dword ptr [esp + eax]
        db  072h, 004h
        lds    eax, ptr [ebx + edx]
        add    eax, 5710542h
        db  079h, 005h
        push   eax
        insb
        popad
        db  073h, 065h
        and    byte ptr [edi + 61h], dh
        imul   esi, dword ptr [eax + 77h], 656c6968h
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [eax + 72h], dh
        db  065h, 070h, 061h
        db  072h, 065h
        jae    SHORT loc_6_006B
        db  074h, 06Fh
        and    byte ptr [edx + 75h], dh
        outsb
        and    byte ptr [eax + ebp*2 + 69h], dh
        jae    SHORT loc_6_0077
        db  070h, 072h
        outsd
        db  067h, 072h, 061h
        insd
        db  02Eh, 02Eh, 02Eh, 00Dh, 00Ah, 024h, 000h
SHELL_PM_API:
        or     cl, byte ptr [0cdcdcdc9h]
loc_6_006B:
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
loc_6_0077:
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        int    0cdh
        db  0CDh
_PTEXT ends
        end
