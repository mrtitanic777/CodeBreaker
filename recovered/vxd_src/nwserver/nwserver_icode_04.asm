; nwserver_icode_04.asm -- NWSERVER object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0F9h, 0C3h
NWSERVER_SYS_CRITICAL_INIT:
        clc
        ret
NWSERVER_DEVICE_INIT:
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 084h, 0B1h, 000h, 000h
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 088h, 0B1h, 000h, 000h
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 08Ch, 0B1h, 000h, 000h
        db  00Fh, 0B7h, 005h, 012h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 090h, 0B1h, 000h, 000h
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 094h, 0B1h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 00Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     SHORT loc_4_0097
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
loc_4_0097:
        or     eax, eax
        je     NEAR PTR loc_4_033B
        db  0A3h, 068h, 0B1h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 028h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 02Ah, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 02Ch, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 060h, 043h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 02Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 030h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 032h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 06Ch, 0B1h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 034h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 036h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 038h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 04Ch, 0B8h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 03Ah, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 03Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 03Eh, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 070h, 0B1h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 040h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 042h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 044h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 074h, 0B1h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 022h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 024h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 026h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 080h, 0B1h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 010h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 014h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 078h, 0B1h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        mov    dword ptr [ebp - 0ch], eax
        db  00Fh, 0B7h, 01Dh, 016h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 018h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 01Ah, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 07Ch, 0B1h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 0ch]
        db  00Fh, 0B7h, 01Dh, 01Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 020h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        push   eax
        push   edx
        push   0
        push   ecx
        push   ebx
        push   102h
        push   0
        push   esp
        db  0FFh, 035h, 068h, 0B1h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
loc_4_033B:
        clc
        ret
NWSERVER_INIT_COMPLETE:
        db  0E8h, 0AEh, 06Eh, 0FCh, 0FFh
        test   eax, eax
        stc
        je     SHORT loc_4_0348
        clc
loc_4_0348:
        ret
sub_4_0349:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_4_0354:
        xor    eax, eax
        db  0CDh, 020h, 000h, 000h, 094h, 004h
        jae    SHORT loc_4_0360
        xor    eax, eax
loc_4_0360:
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h
_ITEXT ends
        end
