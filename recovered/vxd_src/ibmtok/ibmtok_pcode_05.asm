; ibmtok_pcode_05.asm -- IBMTOK object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 58h
        mov    word ptr [ebp - 14h], 7
        mov    word ptr [ebp - 12h], 7
        db  0C7h, 045h, 0F0h, 098h, 055h, 000h, 000h
        mov    word ptr [ebp - 20h], 0
        mov    word ptr [ebp - 1eh], 2
        mov    word ptr [ebp - 1ch], 0e000h
        mov    word ptr [ebp - 1ah], 0e001h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 006h, 005h, 0FBh, 0FFh
        db  0A1h, 0C8h, 056h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0DFh, 005h, 0FBh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_5_0069
        mov    eax, 0c000009ah
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0069:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 58h], 3
        mov    byte ptr [ebp - 57h], 0
        db  0C7h, 045h, 0AEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B2h, 060h, 00Eh, 000h, 000h
        db  0C7h, 045h, 0C2h, 0A0h, 010h, 000h, 000h
        db  0C7h, 045h, 0B6h, 0C0h, 02Ah, 000h, 000h
        db  0C7h, 045h, 0BAh, 010h, 02Ch, 000h, 000h
        db  0C7h, 045h, 0BEh, 0B0h, 01Ah, 000h, 000h
        db  0C7h, 045h, 0CAh, 050h, 024h, 000h, 000h
        db  0C7h, 045h, 0C6h, 0C0h, 019h, 000h, 000h
        db  0C7h, 045h, 0CEh, 060h, 01Fh, 000h, 000h
        db  0C7h, 045h, 0D2h, 080h, 023h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 2ah], eax
        lea    eax, [ebp - 58h]
        mov    dword ptr [ebp - 26h], ecx
        push   36h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 06Dh, 004h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_0105
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 056h, 004h, 0FBh, 0FFh
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0105:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 032h
_PTEXT ends
        end
