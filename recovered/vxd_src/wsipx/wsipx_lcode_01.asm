; wsipx_lcode_01.asm -- WSIPX object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 0a4h]
        push   edx
        xor    eax, eax
        mov    ebx, 0
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    ecx
        or     al, al
        jne    SHORT loc_1_0026
        jecxz  loc_1_0031
loc_1_0026:
        cmp    al, 0ffh
        jne    SHORT loc_1_002C
        sub    al, al
loc_1_002C:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
loc_1_0031:
        mov    word ptr [esi + 0a4h], dx
        jmp    SHORT loc_1_0026
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 0a4h]
        xor    eax, eax
        mov    ebx, 1
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_005D:
        push   ebx
        mov    ebx, 16h
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0ah]
        mov    dx, word ptr [eax + 0cch]
        jmp    SHORT loc_1_0089
sub_1_0073:
        push   ebx
        mov    ebx, 3
        jmp    SHORT loc_1_0089
sub_1_007B:
        push   ebx
        mov    ebx, 17h
        jmp    SHORT loc_1_0089
sub_1_0083:
        push   ebx
        mov    ebx, 4
loc_1_0089:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 0eh
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        cmp    ebx, 4
        je     SHORT loc_1_00A2
        xor    al, al
loc_1_00A2:
        movzx  eax, al
        pop    esi
        pop    edi
        pop    ebp
        pop    ebx
        ret
sub_1_00AA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0eh
        mov    ebx, 6
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_00C7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, 11h
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0eh
        xor    al, al
        mov    ah, 1
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        or     al, al
        jne    SHORT loc_1_00F4
        sub    esi, 0eh
        mov    esi, dword ptr [esi + 0ah]
        mov    word ptr [esi + 0cch], dx
loc_1_00F4:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_00F9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, 12h
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0eh
        xor    al, al
        mov    ah, 1
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_011A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    ebx, 13h
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0ah]
        movzx  edx, word ptr [edi + 0cch]
        add    esi, 0eh
        xor    eax, eax
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_0143:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        xor    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, 15h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        movzx  eax, al
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_0165:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        xor    eax, eax
        mov    ebx, 10h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        cmp    al, 0ffh
        je     SHORT loc_1_0185
        jmp    SHORT loc_1_019D
sub_1_017F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
loc_1_0185:
        sub    eax, eax
        sub    edx, edx
        db  0BEh, 09Ch, 026h, 000h, 000h
        add    esi, 2
        mov    ebx, 9
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
loc_1_019D:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_01A2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    eax, eax
        sub    edx, edx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 2
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        movzx  eax, al
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h
sub_1_01C8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        db  080h, 03Dh, 07Ch, 004h, 000h, 000h, 000h
        jne    SHORT loc_1_01E4
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_01EA
loc_1_01E4:
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
loc_1_01EA:
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_01F6
loc_1_01F1:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
loc_1_01F6:
        jmp    SHORT loc_1_01F1
sub_1_01F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        db  080h, 03Dh, 07Ch, 004h, 000h, 000h, 000h
        jne    SHORT loc_1_0214
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        jmp    SHORT loc_1_021A
loc_1_0214:
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
loc_1_021A:
        add    esp, 8
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_1_0222:
        db  080h, 03Dh, 07Ch, 004h, 000h, 000h, 000h
        je     SHORT loc_1_023B
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        db  0CDh, 020h, 011h, 000h, 040h, 000h
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
loc_1_023B:
        ret
sub_1_023C:
        sub    eax, eax
        db  080h, 03Dh, 07Ch, 004h, 000h, 000h, 000h
        je     SHORT loc_1_025A
        db  080h, 03Dh, 07Dh, 004h, 000h, 000h, 002h
        jne    SHORT loc_1_025A
        push   ebp
        mov    ebp, esp
        db  0CDh, 020h, 091h, 001h, 001h, 000h
        pop    ebp
loc_1_025A:
        ret
loc_1_025B:
        mov    word ptr [edx + 8ah], 0
        ret
sub_1_0265:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    eax, 1388h
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 05Bh, 002h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        or     esi, esi
        je     SHORT loc_1_02D5
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_02A1:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [ebx + 8ah], 0
        je     SHORT loc_1_02B9
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        jmp    SHORT loc_1_02A1
loc_1_02B9:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_02D5:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
loc_1_02DA:
        push   edx
        call   sub_1_2147
        add    esp, 4
        ret
sub_1_02E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        db  0BEh, 0DAh, 002h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
loc_1_02FD:
        xor    eax, eax
        mov    ebx, 1
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        ret
sub_1_030C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        db  0BEh, 0FDh, 002h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h
WSIPX_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0CFh, 0FCh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 050h, 0FDh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0BDh, 0FCh, 0FEh, 0FFh
        clc
        ret
        db  000h, 000h, 000h
sub_1_0348:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_0387
        or     ebx, ebx
        je     SHORT loc_1_0387
        movzx  ecx, ax
        and    ch, 0fh
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_038B
        add    eax, esi
loc_1_0387:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_038B:
        xor    eax, eax
        jmp    SHORT loc_1_0387
sub_1_038F:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_03CA
        or     ebx, ebx
        je     SHORT loc_1_03CA
        movzx  ecx, ax
        and    ch, 0fh
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_03D3
loc_1_03CA:
        mov    eax, 1
loc_1_03CF:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_03D3:
        xor    eax, eax
        jmp    SHORT loc_1_03CF
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_0420:
        db  0CDh, 020h, 003h, 080h, 03Eh, 000h
        db  0CDh, 020h, 004h, 080h, 03Eh, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 03Fh, 000h, 000h, 00Bh, 000h, 000h, 057h, 053h, 049h, 050h
        db  058h, 020h, 020h, 020h, 002h, 060h, 000h, 054h, 028h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
sub_1_0480:
        db  0A1h, 0C4h, 026h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_04C3
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_1_0494
        sub    eax, eax
        jmp    SHORT loc_1_04FF
loc_1_0494:
        db  0C7h, 005h, 040h, 026h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0FFh, 035h, 0C0h, 026h, 000h, 000h
        call   sub_1_01C8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_04BF
        db  08Bh, 00Dh, 0D4h, 026h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 0D4h, 026h, 000h, 000h
        jmp    SHORT loc_1_04CB
loc_1_04BF:
        sub    eax, eax
        jmp    SHORT loc_1_04FF
loc_1_04C3:
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0C4h, 026h, 000h, 000h
loc_1_04CB:
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 8], 1
        mov    word ptr [eax + 30h], 0
        db  08Bh, 015h, 050h, 026h, 000h, 000h
        inc    edx
        db  089h, 015h, 050h, 026h, 000h, 000h
        db  03Bh, 015h, 048h, 026h, 000h, 000h
        jle    SHORT loc_1_04FF
        db  089h, 015h, 048h, 026h, 000h, 000h
loc_1_04FF:
        ret
sub_1_0500:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        test   byte ptr [ebx + 8], 4
        je     SHORT loc_1_051A
        push   dword ptr [ebx + 32h]
        call   sub_1_0617
        add    esp, 4
        jmp    SHORT loc_1_0537
loc_1_051A:
        mov    si, word ptr [ebx + 30h]
        lea    edi, [ebx + 32h]
        or     si, si
        je     SHORT loc_1_0537
loc_1_0526:
        push   dword ptr [edi]
        call   sub_1_0617
        add    esp, 4
        add    edi, 6
        dec    si
        jne    SHORT loc_1_0526
loc_1_0537:
        mov    word ptr [ebx + 30h], 0
        mov    word ptr [ebx + 8], 0
        db  0A1h, 0C4h, 026h, 000h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 0C4h, 026h, 000h, 000h
        db  0A1h, 050h, 026h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_055F
        dec    eax
        db  0A3h, 050h, 026h, 000h, 000h
loc_1_055F:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0563:
        db  0A1h, 0C8h, 026h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_05A5
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_1_0577
        sub    eax, eax
        jmp    SHORT loc_1_05C8
loc_1_0577:
        db  0C7h, 005h, 040h, 026h, 000h, 000h, 001h, 000h, 000h, 000h
        push   100h
        call   sub_1_01C8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_05A1
        db  08Bh, 00Dh, 0B8h, 026h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 0B8h, 026h, 000h, 000h
        jmp    SHORT loc_1_05AD
loc_1_05A1:
        sub    eax, eax
        jmp    SHORT loc_1_05C8
loc_1_05A5:
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0C8h, 026h, 000h, 000h
loc_1_05AD:
        db  08Bh, 015h, 04Ch, 026h, 000h, 000h
        inc    edx
        db  089h, 015h, 04Ch, 026h, 000h, 000h
        db  03Bh, 015h, 044h, 026h, 000h, 000h
        jle    SHORT loc_1_05C8
        db  089h, 015h, 044h, 026h, 000h, 000h
loc_1_05C8:
        ret
sub_1_05C9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        db  0A1h, 0ACh, 026h, 000h, 000h
        lea    ebx, [esi + 32h]
        mov    edi, dword ptr [esp + 18h]
        cmp    eax, edi
        jge    SHORT loc_1_05E3
        mov    edi, eax
loc_1_05E3:
        sub    ebp, ebp
        cmp    edi, ebp
        jle    SHORT loc_1_0607
loc_1_05E9:
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [ebx], eax
        mov    word ptr [ebx + 4], 100h
        or     eax, eax
        je     SHORT loc_1_0607
        inc    ebp
        add    ebx, 6
        cmp    ebp, edi
        jl     SHORT loc_1_05E9
loc_1_0607:
        mov    word ptr [esi + 30h], bp
        mov    ax, bp
        shl    ax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0617:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 0C8h, 026h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 0C8h, 026h, 000h, 000h
        db  0A1h, 04Ch, 026h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0637
        dec    eax
        db  0A3h, 04Ch, 026h, 000h, 000h
loc_1_0637:
        ret
sub_1_0638:
        push   esi
        push   edi
        db  08Bh, 03Dh, 0B4h, 026h, 000h, 000h
        db  00Fh, 0AFh, 03Dh, 098h, 026h, 000h, 000h
        db  083h, 03Dh, 098h, 026h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
        je     SHORT loc_1_0673
        mov    ax, word ptr [esi + 86h]
        cmp    ax, 1eh
        je     SHORT loc_1_0673
        db  08Bh, 03Dh, 0B8h, 026h, 000h, 000h
        movsx  ecx, ax
        db  00Fh, 0AFh, 00Dh, 0ACh, 026h, 000h, 000h
        sub    edi, ecx
loc_1_0673:
        db  039h, 03Dh, 0B8h, 026h, 000h, 000h
        jle    SHORT loc_1_06B6
loc_1_067B:
        push   0
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_06B6
        push   eax
        call   sub_1_01F8
        add    esp, 4
        db  08Bh, 00Dh, 0B8h, 026h, 000h, 000h
        or     ecx, ecx
        db  0A1h, 04Ch, 026h, 000h, 000h
        je     SHORT loc_1_06A2
        dec    ecx
loc_1_06A2:
        or     eax, eax
        je     SHORT loc_1_06AC
        dec    eax
        db  0A3h, 04Ch, 026h, 000h, 000h
loc_1_06AC:
        db  089h, 00Dh, 0B8h, 026h, 000h, 000h
        cmp    ecx, edi
        jg     SHORT loc_1_067B
loc_1_06B6:
        db  0A1h, 098h, 026h, 000h, 000h
        lea    edi, [eax*4]
        or     eax, eax
        je     SHORT loc_1_06DE
        mov    ax, word ptr [esi + 86h]
        cmp    ax, 1eh
        je     SHORT loc_1_06DE
        db  08Bh, 03Dh, 0D4h, 026h, 000h, 000h
        movsx  eax, ax
        sub    edi, eax
loc_1_06DE:
        db  039h, 03Dh, 0D4h, 026h, 000h, 000h
        jle    SHORT loc_1_0721
loc_1_06E6:
        push   0
        call   sub_1_0480
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0721
        push   eax
        call   sub_1_01F8
        add    esp, 4
        db  08Bh, 00Dh, 0D4h, 026h, 000h, 000h
        or     ecx, ecx
        db  0A1h, 050h, 026h, 000h, 000h
        je     SHORT loc_1_070D
        dec    ecx
loc_1_070D:
        or     eax, eax
        je     SHORT loc_1_0717
        dec    eax
        db  0A3h, 050h, 026h, 000h, 000h
loc_1_0717:
        db  089h, 00Dh, 0D4h, 026h, 000h, 000h
        cmp    ecx, edi
        jg     SHORT loc_1_06E6
loc_1_0721:
        pop    edi
        pop    esi
        ret
sub_1_0724:
        db  083h, 03Dh, 040h, 026h, 000h, 000h, 000h
        je     SHORT loc_1_0745
        call   sub_1_023C
        or     eax, eax
        jne    SHORT loc_1_0745
        db  0C7h, 005h, 040h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_0222
loc_1_0745:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0750:
        push   esi
        push   edi
        sub    eax, eax
        mov    ecx, dword ptr [esp + 10h]
        cmp    ecx, 3e8h
        jne    NEAR PTR loc_1_08CC
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 2ch], 2
        jne    NEAR PTR loc_1_082B
        cmp    ecx, 4000h
        je     SHORT loc_1_07A0
        cmp    ecx, 4001h
        je     SHORT loc_1_07BB
        cmp    ecx, 4006h
        je     SHORT loc_1_07EA
        cmp    ecx, 4007h
        je     SHORT loc_1_0805
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_08F6
loc_1_07A0:
        mov    edi, dword ptr [esp + 18h]
        mov    cl, byte ptr [esi + 0a6h]
        mov    byte ptr [edi], cl
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    NEAR PTR loc_1_08F6
loc_1_07BB:
        test   byte ptr [esi + 0ceh], 1
        je     SHORT loc_1_07E0
        mov    edi, dword ptr [esp + 18h]
        movzx  ecx, byte ptr [esi + 0cfh]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    NEAR PTR loc_1_08F6
loc_1_07E0:
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_08F6
loc_1_07EA:
        mov    ecx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [ecx], 4
        db  08Bh, 00Dh, 0B0h, 026h, 000h, 000h
        mov    dword ptr [edi], ecx
        jmp    NEAR PTR loc_1_08F6
loc_1_0805:
        mov    ecx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [ecx], 0eh
        mov    dword ptr [edi], 0
        add    esi, 9ah
        add    edi, 4
        movsd
        movsd
        movsw
        jmp    NEAR PTR loc_1_08F6
loc_1_082B:
        cmp    ecx, 4002h
        je     SHORT loc_1_0855
        cmp    ecx, 4005h
        je     SHORT loc_1_0870
        cmp    ecx, 4006h
        je     SHORT loc_1_0891
        cmp    ecx, 4007h
        je     SHORT loc_1_08A9
        mov    eax, 273ah
        jmp    NEAR PTR loc_1_08F6
loc_1_0855:
        mov    edi, dword ptr [esp + 18h]
        mov    cl, byte ptr [esi + 0d0h]
        mov    byte ptr [edi], cl
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    NEAR PTR loc_1_08F6
loc_1_0870:
        mov    ecx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [ecx], 4
        mov    cl, byte ptr [esi + 0ceh]
        and    cl, 2
        cmp    cl, 1
        sbb    ecx, ecx
        inc    ecx
        mov    dword ptr [edi], ecx
        jmp    SHORT loc_1_08F6
loc_1_0891:
        mov    ecx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [ecx], 4
        db  08Bh, 00Dh, 0BCh, 026h, 000h, 000h
        mov    dword ptr [edi], ecx
        jmp    SHORT loc_1_08F6
loc_1_08A9:
        mov    ecx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [ecx], 0eh
        mov    dword ptr [edi], 0
        add    esi, 9ah
        add    edi, 4
        movsd
        movsd
        movsw
        jmp    SHORT loc_1_08F6
loc_1_08CC:
        cmp    ecx, 0ffffh
        jne    SHORT loc_1_08F1
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, 8
        je     SHORT loc_1_08F6
        cmp    ecx, 10h
        je     SHORT loc_1_08F6
        cmp    ecx, 100h
        je     SHORT loc_1_08F6
        mov    eax, 273ah
        jmp    SHORT loc_1_08F6
loc_1_08F1:
        mov    eax, 2726h
loc_1_08F6:
        pop    edi
        pop    esi
        ret
loc_1_08F9:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, 3e8h
        jne    NEAR PTR loc_1_09B9
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [ecx + 2ch], 2
        jne    SHORT loc_1_098D
        cmp    eax, 4000h
        je     SHORT loc_1_0946
        cmp    eax, 4001h
        je     SHORT loc_1_0957
        cmp    eax, 4003h
        je     SHORT loc_1_096F
        cmp    eax, 4004h
        je     SHORT loc_1_097B
        cmp    eax, 4005h
        je     SHORT loc_1_0984
        mov    esi, 273ah
        jmp    NEAR PTR loc_1_09FE
loc_1_0946:
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0a6h], al
        jmp    NEAR PTR loc_1_09FE
loc_1_0957:
        mov    eax, dword ptr [esp + 14h]
        or     byte ptr [ecx + 0ceh], 1
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0cfh], al
        jmp    NEAR PTR loc_1_09FE
loc_1_096F:
        and    byte ptr [ecx + 0ceh], 0feh
        jmp    NEAR PTR loc_1_09FE
loc_1_097B:
        or     byte ptr [ecx + 0ceh], 4
        jmp    SHORT loc_1_09FE
loc_1_0984:
        or     byte ptr [ecx + 0ceh], 2
        jmp    SHORT loc_1_09FE
loc_1_098D:
        cmp    eax, 4002h
        je     SHORT loc_1_09A2
        cmp    eax, 4005h
        je     SHORT loc_1_09B0
        mov    esi, 273ah
        jmp    SHORT loc_1_09FE
loc_1_09A2:
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 0d0h], al
        jmp    SHORT loc_1_09FE
loc_1_09B0:
        or     byte ptr [ecx + 0ceh], 2
        jmp    SHORT loc_1_09FE
loc_1_09B9:
        cmp    eax, 0ffffh
        jne    SHORT loc_1_09F9
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, 1001h
        jne    SHORT loc_1_09DA
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 34h], eax
        jmp    SHORT loc_1_09FE
loc_1_09DA:
        cmp    eax, 1002h
        jne    SHORT loc_1_09FE
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 38h], eax
        push   ecx
        call   sub_1_0A65
        add    esp, 4
        jmp    SHORT loc_1_09FE
loc_1_09F9:
        mov    esi, 2726h
loc_1_09FE:
        mov    eax, esi
        pop    esi
        ret
loc_1_0A02:
        push   esi
        push   edi
        cmp    dword ptr [esp + 10h], 4004667fh
        jne    SHORT loc_1_0A5A
        sub    edi, edi
        mov    ecx, dword ptr [esp + 0ch]
        sub    esi, esi
        mov    edx, dword ptr [ecx + 7ch]
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, -7ch
        je     SHORT loc_1_0A37
loc_1_0A22:
        mov    eax, dword ptr [edx + 32h]
        inc    edi
        movzx  eax, word ptr [eax + 2]
        add    esi, eax
        mov    edx, dword ptr [edx]
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, -7ch
        jne    SHORT loc_1_0A22
loc_1_0A37:
        test   byte ptr [ecx + 0ceh], 2
        jne    SHORT loc_1_0A50
        cmp    dword ptr [ecx + 2ch], 2
        jne    SHORT loc_1_0A4B
        imul   edi, edi, -1eh
        jmp    SHORT loc_1_0A4E
loc_1_0A4B:
        imul   edi, edi, -2ah
loc_1_0A4E:
        add    esi, edi
loc_1_0A50:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], esi
        sub    eax, eax
        jmp    SHORT loc_1_0A5F
loc_1_0A5A:
        mov    eax, 2726h
loc_1_0A5F:
        pop    edi
        pop    esi
        ret
loc_1_0A62:
        sub    eax, eax
        ret
sub_1_0A65:
        push   ebx
        push   esi
        push   edi
        sub    edx, edx
        mov    edi, dword ptr [esp + 10h]
        db  08Bh, 00Dh, 0CCh, 026h, 000h, 000h
        mov    eax, dword ptr [edi + 38h]
        add    eax, ecx
        div    ecx
        mov    ebx, eax
        cmp    ebx, 1f4h
        jle    SHORT loc_1_0A8A
        mov    ebx, 1f4h
loc_1_0A8A:
        mov    word ptr [edi + 86h], bx
        sub    esi, esi
        cmp    ebx, esi
        je     SHORT loc_1_0ABF
loc_1_0A97:
        push   1
        call   sub_1_0480
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0AB8
        call   sub_1_0724
        push   1
        call   sub_1_0480
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0ABC
loc_1_0AB8:
        mov    dword ptr [eax], esi
        mov    esi, eax
loc_1_0ABC:
        dec    ebx
        jne    SHORT loc_1_0A97
loc_1_0ABF:
        or     esi, esi
        je     SHORT loc_1_0AD4
loc_1_0AC3:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_0500
        add    esp, 4
        or     esi, esi
        jne    SHORT loc_1_0AC3
loc_1_0AD4:
        movsx  eax, word ptr [edi + 86h]
        lea    edi, [eax*2]
        or     edi, edi
        je     SHORT loc_1_0B0E
loc_1_0AE6:
        push   1
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0B07
        call   sub_1_0724
        push   1
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0B0B
loc_1_0B07:
        mov    dword ptr [eax], esi
        mov    esi, eax
loc_1_0B0B:
        dec    edi
        jne    SHORT loc_1_0AE6
loc_1_0B0E:
        or     esi, esi
        je     SHORT loc_1_0B23
loc_1_0B12:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_0617
        add    esp, 4
        or     esi, esi
        jne    SHORT loc_1_0B12
loc_1_0B23:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B30:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, ecx
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esp + 14h], eax
        mov    ebx, dword ptr [esp + 24h]
        or     ebx, ebx
        je     NEAR PTR loc_1_0D7E
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        je     NEAR PTR loc_1_0D7E
        cmp    dword ptr [esp + 38h], 0
        je     NEAR PTR loc_1_0D7E
        mov    eax, dword ptr [esi + 2ch]
        cmp    eax, 2
        mov    ebp, dword ptr [esp + 30h]
        jne    SHORT loc_1_0B91
        or     ebp, ebp
        je     SHORT loc_1_0B91
        cmp    dword ptr [esp + 34h], 0
        jne    SHORT loc_1_0B91
        jmp    NEAR PTR loc_1_0D7E
loc_1_0B91:
        mov    edx, dword ptr [esp + 38h]
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [edx], 0
        mov    edi, dword ptr [esi + 7ch]
        mov    edx, esi
        sub    edx, edi
        cmp    edx, -7ch
        jne    SHORT loc_1_0BE0
        mov    eax, dword ptr [esi + 3ch]
        test   al, 2
        je     SHORT loc_1_0BD3
        or     eax, 1
        mov    dword ptr [esi + 3ch], eax
        push   20h
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0ch
        mov    eax, 2746h
        jmp    NEAR PTR loc_1_0D83
loc_1_0BD3:
        mov    dword ptr [esp + 18h], 2733h
        jmp    NEAR PTR loc_1_0CF5
loc_1_0BE0:
        cmp    eax, 5
        jne    SHORT loc_1_0C1F
        cmp    byte ptr [esi + 89h], 0
        jne    SHORT loc_1_0C1F
        mov    eax, dword ptr [esi + 3ch]
        test   al, 2
        je     SHORT loc_1_0C12
        or     eax, 1
        mov    dword ptr [esi + 3ch], eax
        push   20h
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0ch
        mov    eax, 2746h
        jmp    NEAR PTR loc_1_0D83
loc_1_0C12:
        mov    dword ptr [esp + 18h], 2733h
        jmp    NEAR PTR loc_1_0CF5
loc_1_0C1F:
        cmp    eax, 2
        jne    SHORT loc_1_0C74
        and    cl, 0bh
        lea    eax, [esp + 11h]
        push   ecx
        push   eax
        push   dword ptr [esp + 3ch]
        push   ebp
        mov    ebp, dword ptr [esp + 38h]
        push   ebp
        push   ebx
        push   edi
        call   sub_1_116F
        mov    word ptr [esp + 2eh], ax
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_1_0CF5
        mov    eax, dword ptr [esi + 7ch]
        push   edi
        mov    eax, dword ptr [eax]
        lea    ecx, [esi + 7ch]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 7ch], eax
        dec    word ptr [esi + 84h]
        call   sub_1_0500
        add    esp, 4
        jmp    NEAR PTR loc_1_0CF5
loc_1_0C74:
        mov    ebp, dword ptr [esp + 28h]
loc_1_0C78:
        or     ebp, ebp
        je     SHORT loc_1_0CF5
        push   ebp
        push   ebx
        push   edi
        call   sub_1_1270
        add    word ptr [esp + 1eh], ax
        add    esp, 0ch
        movzx  ecx, ax
        add    ebx, ecx
        sub    ebp, ecx
        cmp    dword ptr [esi + 2ch], 5
        jne    SHORT loc_1_0CB0
        mov    ecx, dword ptr [edi + 32h]
        test   byte ptr [ecx + 1eh], 10h
        je     SHORT loc_1_0CB0
        sub    ebp, ebp
        dec    byte ptr [esi + 89h]
        db  066h, 0A1h, 0CCh, 026h, 000h, 000h
loc_1_0CB0:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_0CE9
        or     ebp, ebp
        jne    SHORT loc_1_0CC9
        push   eax
        push   edi
        call   sub_1_1127
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0CF5
loc_1_0CC9:
        mov    eax, dword ptr [esi + 7ch]
        mov    edx, eax
        mov    eax, dword ptr [eax]
        lea    ecx, [esi + 7ch]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 7ch], eax
        dec    word ptr [esi + 84h]
        push   edx
        call   sub_1_0500
        add    esp, 4
loc_1_0CE9:
        mov    edi, dword ptr [esi + 7ch]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -7ch
        jne    SHORT loc_1_0C78
loc_1_0CF5:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_0D08
        mov    edx, dword ptr [esp + 38h]
        mov    dword ptr [edx], 0
        jmp    SHORT loc_1_0D4A
loc_1_0D08:
        mov    edx, dword ptr [esp + 38h]
        movzx  eax, word ptr [esp + 12h]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi + 2ch], 5
        jne    SHORT loc_1_0D22
        movzx  eax, byte ptr [esi + 89h]
        jmp    SHORT loc_1_0D29
loc_1_0D22:
        movsx  eax, word ptr [esi + 84h]
loc_1_0D29:
        or     eax, eax
        je     SHORT loc_1_0D46
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_0D4A
        and    dword ptr [esi + 50h], 0fffffffeh
        push   1
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0ch
        jmp    SHORT loc_1_0D4A
loc_1_0D46:
        and    dword ptr [esi + 50h], 0fffffffeh
loc_1_0D4A:
        push   esi
        call   sub_1_1027
        add    esp, 4
        push   esi
        call   sub_1_1027
        add    esp, 4
        test   byte ptr [esi + 0ceh], 40h
        je     SHORT loc_1_0D73
        call   sub_1_0724
        push   esi
        call   sub_1_1027
        add    esp, 4
loc_1_0D73:
        call   sub_1_0724
        mov    eax, dword ptr [esp + 18h]
        jmp    SHORT loc_1_0D83
loc_1_0D7E:
        mov    eax, 271eh
loc_1_0D83:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_0D8B:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi - 4]
        sub    edi, 0eh
        mov    dword ptr [ebp - 8], ecx
        mov    esi, ecx
        mov    eax, dword ptr [edi + 32h]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp - 8]
        dec    byte ptr [ecx + 88h]
        mov    al, byte ptr [edi + 17h]
        cmp    al, 0edh
        jne    SHORT loc_1_0E1F
        mov    ax, word ptr [edi + 1ah]
        cmp    word ptr [ecx + 0cch], ax
        je     SHORT loc_1_0DE2
        push   eax
        push   ecx
        call   sub_1_1393
        add    esp, 8
        mov    esi, eax
loc_1_0DE2:
        mov    ax, word ptr [edi + 1ah]
        cmp    word ptr [esi + 0cch], ax
        jne    NEAR PTR loc_1_0FD8
        cmp    dword ptr [esi + 4ch], 6
        jne    SHORT loc_1_0E04
        mov    dword ptr [esi + 4ch], 7
        or     dword ptr [esi + 3ch], 1
loc_1_0E04:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_0E0E
        and    dword ptr [esi + 50h], 0fffffffeh
loc_1_0E0E:
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0FD8
loc_1_0E1F:
        or     al, al
        je     SHORT loc_1_0E4C
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 58h], 0
        je     SHORT loc_1_0E30
        and    dword ptr [ecx + 50h], 0fffffffeh
loc_1_0E30:
        push   1
        push   dword ptr [ebp - 8]
        push   edi
        call   sub_1_24F0
        add    esp, 0ch
        push   edi
        call   sub_1_0500
        add    esp, 4
        jmp    NEAR PTR loc_1_1020
loc_1_0E4C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebp - 2], ax
        push   eax
        mov    ax, word ptr [ebp - 2]
        xchg   ah, al
        mov    word ptr [ebp - 2], ax
        pop    eax
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 2], cx
        movzx  eax, word ptr [ebp - 2]
        add    eax, 0ffh
        cdq
        and    edx, 0ffh
        add    eax, edx
        sar    eax, 8
        mov    word ptr [ebp - 2], ax
        movzx  eax, ax
        db  08Bh, 00Dh, 0ACh, 026h, 000h, 000h
        cmp    ecx, eax
        jle    SHORT loc_1_0EC8
        imul   eax, eax, 6
        lea    ebx, [edi + eax + 32h]
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 30h], ax
        movzx  eax, word ptr [ebp - 2]
        cmp    eax, ecx
        jge    SHORT loc_1_0EC8
loc_1_0EAB:
        push   dword ptr [ebx]
        call   sub_1_0617
        add    esp, 4
        inc    word ptr [ebp - 2]
        add    ebx, 6
        movzx  ecx, word ptr [ebp - 2]
        db  03Bh, 00Dh, 0ACh, 026h, 000h, 000h
        jl     SHORT loc_1_0EAB
loc_1_0EC8:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 2ch], 2
        jne    SHORT loc_1_0EE0
        push   edi
        push   ecx
        call   sub_1_1402
        add    esp, 8
        jmp    NEAR PTR loc_1_0FE1
loc_1_0EE0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 22h]
        cmp    word ptr [ecx + 0cch], ax
        je     SHORT loc_1_0EFF
        push   eax
        push   ecx
        call   sub_1_1393
        add    esp, 8
        mov    esi, eax
loc_1_0EFF:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 22h]
        cmp    word ptr [esi + 0cch], ax
        jne    NEAR PTR loc_1_0FD8
        mov    eax, dword ptr [ebp - 0ch]
        cmp    byte ptr [eax + 1fh], 0feh
        jne    SHORT loc_1_0F73
        mov    eax, dword ptr [esi + 3ch]
        or     eax, 2
        mov    dword ptr [esi + 3ch], eax
        cmp    dword ptr [esi + 68h], -1
        je     SHORT loc_1_0F6B
        mov    ecx, dword ptr [esi + 7ch]
        sub    ecx, esi
        cmp    ecx, 7ch
        jne    SHORT loc_1_0F53
        or     eax, 1
        mov    dword ptr [esi + 3ch], eax
        mov    dword ptr [esi + 4ch], 7
        push   20h
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0ch
        mov    byte ptr [edi + 17h], 0edh
loc_1_0F53:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_0F5D
        and    dword ptr [esi + 50h], 0fffffffeh
loc_1_0F5D:
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0ch
        jmp    SHORT loc_1_0FD8
loc_1_0F6B:
        or     eax, 1
        mov    dword ptr [esi + 3ch], eax
        jmp    SHORT loc_1_0FD8
loc_1_0F73:
        mov    word ptr [edi + 1ah], 0
        mov    word ptr [edi + 1ch], 2ah
        lea    eax, [esi + 7ch]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 80h]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 80h], edi
        inc    word ptr [esi + 84h]
        cmp    dword ptr [esi + 2ch], 5
        jne    SHORT loc_1_0FBA
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [eax + 1eh], 10h
        je     SHORT loc_1_0FB3
        inc    byte ptr [esi + 89h]
        jmp    SHORT loc_1_0FBA
loc_1_0FB3:
        mov    dword ptr [ebp - 10h], 0
loc_1_0FBA:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_0FC4
        and    dword ptr [esi + 50h], 0fffffffeh
loc_1_0FC4:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_0FE1
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0ch
        jmp    SHORT loc_1_0FE1
loc_1_0FD8:
        push   edi
        call   sub_1_0500
        add    esp, 4
loc_1_0FE1:
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 86h]
        cmp    word ptr [ecx + 84h], ax
        jge    SHORT loc_1_1016
        push   esi
        call   sub_1_1027
        add    esp, 4
        test   byte ptr [esi + 0ceh], 40h
        je     SHORT loc_1_1020
        call   sub_1_0724
        push   esi
        call   sub_1_1027
        add    esp, 4
        jmp    SHORT loc_1_1020
loc_1_1016:
        mov    ecx, dword ptr [ebp - 8]
        or     byte ptr [ecx + 0ceh], 40h
loc_1_1020:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1027:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 88h], 3
        jb     SHORT loc_1_103B
        jmp    NEAR PTR loc_1_1122
loc_1_103B:
        mov    ax, word ptr [esi + 86h]
        cmp    word ptr [esi + 84h], ax
        jl     SHORT loc_1_1050
        jmp    NEAR PTR loc_1_1122
loc_1_1050:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_106A
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_1124
loc_1_106A:
        db  0FFh, 035h, 0ACh, 026h, 000h, 000h
        push   edi
        call   sub_1_05C9
        add    esp, 8
        movzx  ecx, ax
        db  03Bh, 00Dh, 0CCh, 026h, 000h, 000h
        jge    SHORT loc_1_109E
        or     byte ptr [esi + 0ceh], 40h
        push   edi
        call   sub_1_0500
        add    esp, 4
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_1124
loc_1_109E:
        mov    dword ptr [edi + 0ah], esi
        mov    ax, word ptr [esi + 0a4h]
        mov    word ptr [edi + 18h], ax
        db  0C7h, 047h, 012h, 08Bh, 00Dh, 000h, 000h
        lea    eax, [esi + 74h]
        inc    byte ptr [esi + 88h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 78h], edi
        cmp    dword ptr [esi + 2ch], 2
        push   edi
        jne    SHORT loc_1_1113
        call   sub_1_0083
        add    esp, 4
        or     al, al
        je     SHORT loc_1_111B
        mov    byte ptr [edi + 17h], 0ffh
        push   1
        push   esi
        push   edi
        call   sub_1_24F0
        add    esp, 0ch
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        dec    byte ptr [esi + 88h]
        or     byte ptr [esi + 0ceh], 40h
        push   edi
        call   sub_1_0500
        add    esp, 4
        mov    eax, 2742h
        jmp    SHORT loc_1_1124
loc_1_1113:
        call   sub_1_007B
        add    esp, 4
loc_1_111B:
        and    byte ptr [esi + 0ceh], 0bfh
loc_1_1122:
        sub    eax, eax
loc_1_1124:
        pop    edi
        pop    esi
        ret
sub_1_1127:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [esp + 0ch]
        add    cx, word ptr [eax + 1ch]
        mov    dx, cx
        shr    dx, 8
        add    dx, word ptr [eax + 1ah]
        mov    word ptr [eax + 1ah], dx
        and    cx, 0ffh
        mov    word ptr [eax + 1ch], cx
        mov    esi, dword ptr [eax + 32h]
        movzx  edx, dx
        shl    edx, 8
        movzx  eax, cx
        add    edx, eax
        movzx  eax, word ptr [esi + 2]
        cmp    edx, eax
        mov    eax, 0
        jge    SHORT loc_1_116D
        mov    eax, 1
loc_1_116D:
        pop    esi
        ret
sub_1_116F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        add    eax, 32h
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], eax
        mov    esi, dword ptr [ecx + 32h]
        cmp    byte ptr [esp + 30h], 0
        mov    bx, word ptr [esi + 2]
        mov    ax, bx
        jne    SHORT loc_1_11E0
        mov    bp, word ptr [esp + 20h]
        cmp    bp, bx
        jb     SHORT loc_1_11A3
        mov    bp, bx
loc_1_11A3:
        cmp    bp, 0e2h
        jbe    SHORT loc_1_11AE
        mov    bp, 0e2h
loc_1_11AE:
        movzx  edx, bp
        add    esi, 1eh
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [ebx - 1eh]
        add    dword ptr [esp + 1ch], edx
        sub    word ptr [esp + 20h], bp
        sub    bx, bp
        mov    ecx, dword ptr [esp + 18h]
        add    ecx, 38h
        mov    dword ptr [esp + 10h], ecx
loc_1_11E0:
        cmp    word ptr [esp + 20h], 0
        je     SHORT loc_1_1239
loc_1_11E8:
        or     bx, bx
        je     SHORT loc_1_1239
        mov    dx, word ptr [esp + 20h]
        cmp    dx, bx
        jb     SHORT loc_1_11FA
        mov    dx, bx
loc_1_11FA:
        cmp    dx, 100h
        jbe    SHORT loc_1_1205
        mov    dx, 100h
loc_1_1205:
        movzx  ebp, dx
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [ecx]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    dword ptr [esp + 1ch], ebp
        sub    word ptr [esp + 20h], dx
        sub    bx, dx
        add    dword ptr [esp + 10h], 6
        cmp    word ptr [esp + 20h], 0
        jne    SHORT loc_1_11E8
loc_1_1239:
        mov    edi, dword ptr [esp + 24h]
        or     edi, edi
        je     SHORT loc_1_1268
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [ecx + 32h]
        mov    word ptr [edi], 6
        lea    esi, [edx + 12h]
        add    edi, 2
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [esp + 28h]
        mov    word ptr [ecx], 0eh
        mov    ecx, dword ptr [esp + 2ch]
        mov    dl, byte ptr [edx + 5]
        mov    byte ptr [ecx], dl
loc_1_1268:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_1270:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        sub    ax, ax
        mov    bx, word ptr [esi + 1ah]
        movzx  edi, bx
        mov    ecx, edi
        imul   ecx, ecx, 6
        lea    ecx, [esi + ecx + 32h]
        mov    dword ptr [esp + 14h], ecx
        mov    ecx, dword ptr [esi + 32h]
        mov    bp, word ptr [esi + 1ch]
        mov    dx, word ptr [ecx + 2]
        movzx  esi, bp
        shl    edi, 8
        add    edi, esi
        movzx  ecx, dx
        cmp    edi, ecx
        jl     SHORT loc_1_12B1
        jmp    NEAR PTR loc_1_138B
loc_1_12B1:
        mov    cx, bx
        shl    cx, 8
        add    cx, bp
        sub    dx, cx
        mov    word ptr [esp + 12h], bx
        or     bp, bp
        je     SHORT loc_1_131B
        mov    ax, word ptr [esp + 24h]
        cmp    ax, dx
        jb     SHORT loc_1_12D5
        mov    ax, dx
loc_1_12D5:
        mov    ecx, 100h
        sub    ecx, esi
        movzx  edi, ax
        cmp    ecx, edi
        jge    SHORT loc_1_12EA
        mov    ax, 100h
        sub    ax, bp
loc_1_12EA:
        movzx  ebx, ax
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 20h]
        add    esi, dword ptr [ecx]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        add    dword ptr [esp + 20h], ebx
        sub    word ptr [esp + 24h], ax
        sub    dx, ax
        add    dword ptr [esp + 14h], 6
        inc    word ptr [esp + 12h]
loc_1_131B:
        cmp    word ptr [esp + 24h], 0
        je     SHORT loc_1_138B
loc_1_1323:
        or     dx, dx
        je     SHORT loc_1_138B
        mov    esi, dword ptr [esp + 1ch]
        mov    cx, word ptr [esp + 12h]
        cmp    word ptr [esi + 30h], cx
        jbe    SHORT loc_1_138B
        mov    bx, word ptr [esp + 24h]
        cmp    bx, dx
        jb     SHORT loc_1_1344
        mov    bx, dx
loc_1_1344:
        cmp    bx, 100h
        jbe    SHORT loc_1_134F
        mov    bx, 100h
loc_1_134F:
        movzx  ebp, bx
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [ecx]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    dword ptr [esp + 20h], ebp
        add    ax, bx
        add    dword ptr [esp + 14h], 6
        sub    dx, bx
        sub    word ptr [esp + 24h], bx
        inc    word ptr [esp + 12h]
        cmp    word ptr [esp + 24h], 0
        jne    SHORT loc_1_1323
loc_1_138B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_1393:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0bch]
        sub    eax, 0bch
        cmp    eax, ecx
        je     SHORT loc_1_13C3
        mov    dx, word ptr [esp + 8]
loc_1_13AB:
        cmp    word ptr [eax + 0cch], dx
        je     SHORT loc_1_13C3
        mov    eax, dword ptr [eax + 0bch]
        sub    eax, 0bch
        cmp    eax, ecx
        jne    SHORT loc_1_13AB
loc_1_13C3:
        ret
sub_1_13C4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 14h]
        add    edx, 32h
        movzx  ebx, word ptr [eax + 30h]
        add    eax, 32h
        or     ebx, ebx
        je     SHORT loc_1_13FD
loc_1_13DE:
        movzx  ebp, word ptr [eax + 4]
        mov    edi, dword ptr [edx]
        mov    esi, dword ptr [eax]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, 6
        add    eax, 6
        dec    ebx
        jne    SHORT loc_1_13DE
loc_1_13FD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1402:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [esp + 14h], 1
        mov    ecx, dword ptr [edi + 32h]
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [ecx + 0ah], -1
        lea    eax, [ecx + 4]
        jne    SHORT loc_1_1439
        cmp    word ptr [eax + 0ah], 0ffffh
        jne    SHORT loc_1_1439
        mov    dword ptr [esp + 18h], 1
loc_1_1439:
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [esp + 13h], al
        mov    esi, edi
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 0ah], 0
        mov    ebx, dword ptr [esp + 20h]
loc_1_1453:
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_1_1460
        test   byte ptr [ebx + 40h], 1
        je     SHORT loc_1_14D2
loc_1_1460:
        test   byte ptr [ebx + 0ceh], 1
        je     SHORT loc_1_1475
        mov    al, byte ptr [esp + 13h]
        cmp    byte ptr [ebx + 0cfh], al
        jne    SHORT loc_1_14D2
loc_1_1475:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_14C7
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_14D2
        movzx  eax, word ptr [edi + 30h]
        push   eax
        push   ebp
        call   sub_1_05C9
        add    esp, 8
        movzx  ecx, word ptr [edi + 30h]
        shl    ecx, 8
        movzx  eax, ax
        cmp    ecx, eax
        jle    SHORT loc_1_14B3
        push   ebp
        call   sub_1_0500
        add    esp, 4
        jmp    SHORT loc_1_14D2
loc_1_14B3:
        push   edi
        push   ebp
        call   sub_1_13C4
        add    esp, 8
        mov    dword ptr [ebp], esi
        mov    esi, ebp
        mov    dword ptr [ebp + 0ah], ebx
        jmp    SHORT loc_1_14D2
loc_1_14C7:
        mov    dword ptr [edi + 0ah], ebx
        mov    dword ptr [esp + 14h], 0
loc_1_14D2:
        mov    ebx, dword ptr [ebx + 0bch]
        sub    ebx, 0bch
        cmp    dword ptr [esp + 20h], ebx
        jne    NEAR PTR loc_1_1453
        or     esi, esi
        je     SHORT loc_1_153A
        sub    edi, edi
loc_1_14EE:
        mov    edx, esi
        mov    esi, dword ptr [esi]
        mov    ecx, dword ptr [edx + 0ah]
        cmp    ecx, edi
        je     SHORT loc_1_152D
        lea    eax, [ecx + 7ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 80h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 80h], edx
        inc    word ptr [ecx + 84h]
        cmp    dword ptr [ecx + 58h], edi
        je     SHORT loc_1_151F
        and    dword ptr [ecx + 50h], 0fffffffeh
loc_1_151F:
        push   1
        push   ecx
        push   edx
        call   sub_1_24F0
        add    esp, 0ch
        jmp    SHORT loc_1_1536
loc_1_152D:
        push   edx
        call   sub_1_0500
        add    esp, 4
loc_1_1536:
        cmp    esi, edi
        jne    SHORT loc_1_14EE
loc_1_153A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1550:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 24h], 0
        cmp    dword ptr [esp + 38h], 0
        je     NEAR PTR loc_1_191C
        mov    ebx, dword ptr [esp + 34h]
        or     ebx, ebx
        je     NEAR PTR loc_1_191C
        cmp    dword ptr [esp + 4ch], 0
        je     NEAR PTR loc_1_191C
        mov    esi, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [esp + 4ch]
        or     esi, esi
        mov    dword ptr [ecx], 0
        jne    SHORT loc_1_159A
        sub    eax, eax
        jmp    NEAR PTR loc_1_1921
loc_1_159A:
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_1603
        db  039h, 035h, 0B0h, 026h, 000h, 000h
        jae    SHORT loc_1_15B2
        mov    eax, 2738h
        jmp    NEAR PTR loc_1_1921
loc_1_15B2:
        cmp    dword ptr [esp + 44h], 0
        je     SHORT loc_1_1603
        cmp    dword ptr [esp + 48h], 0eh
        jae    SHORT loc_1_15C5
        jmp    NEAR PTR loc_1_191C
loc_1_15C5:
        mov    ecx, dword ptr [esp + 44h]
        cmp    word ptr [ecx], 6
        je     SHORT loc_1_15D9
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_1921
loc_1_15D9:
        mov    ecx, dword ptr [esp + 44h]
        cmp    dword ptr [ecx + 6], -1
        jne    SHORT loc_1_1603
        cmp    word ptr [ecx + 0ah], 0ffffh
        jne    SHORT loc_1_1603
        test   byte ptr [ebx + 40h], 1
        jne    SHORT loc_1_15FB
        mov    eax, 271dh
        jmp    NEAR PTR loc_1_1921
loc_1_15FB:
        mov    dword ptr [esp + 24h], 1
loc_1_1603:
        test   byte ptr [ebx + 41h], 1
        je     SHORT loc_1_161D
        cmp    word ptr [ebx + 8ah], 0
        je     SHORT loc_1_1637
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_1921
loc_1_161D:
        lea    eax, [ebx + 8ah]
        cmp    word ptr [ebx + 8ah], 0ah
        jl     SHORT loc_1_1637
        mov    eax, 2733h
        jmp    NEAR PTR loc_1_1921
loc_1_1637:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_1903
        mov    eax, dword ptr [ebx + 2ch]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fffffff4h
        test   byte ptr [ebx + 41h], 1
        lea    ecx, [eax + 2ah]
        mov    dword ptr [esp + 14h], ecx
        je     NEAR PTR loc_1_16F3
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [ebp + 32h], eax
        or     eax, eax
        je     NEAR PTR loc_1_18F6
        push   esi
        mov    word ptr [ebp + 30h], 1
        push   dword ptr [esp + 3ch]
        call   sub_1_0348
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_18F6
        mov    dword ptr [ebx + 8ch], eax
        mov    dword ptr [ebx + 90h], esi
        mov    dword ptr [ebx + 94h], 0
        mov    word ptr [ebp + 30h], 2
        or     word ptr [ebp + 8], 4
        mov    cx, word ptr [esp + 14h]
        mov    word ptr [ebp + 36h], cx
        mov    ecx, esi
        mov    dword ptr [ebp + 38h], eax
        db  0A1h, 0CCh, 026h, 000h, 000h
        sub    eax, dword ptr [esp + 14h]
        cmp    ecx, eax
        jb     SHORT loc_1_16D8
        mov    ecx, eax
loc_1_16D8:
        mov    word ptr [ebp + 3ch], cx
        cmp    esi, eax
        mov    eax, 0
        setbe  al
        mov    dword ptr [esp + 18h], esi
        mov    dword ptr [esp + 1ch], eax
        jmp    NEAR PTR loc_1_17C5
loc_1_16F3:
        db  08Bh, 00Dh, 0CCh, 026h, 000h, 000h
        sub    ecx, dword ptr [esp + 14h]
        cmp    esi, ecx
        mov    eax, 0
        setbe  al
        mov    dword ptr [esp + 1ch], eax
        jb     SHORT loc_1_170F
        mov    esi, ecx
loc_1_170F:
        mov    dword ptr [esp + 10h], esi
        mov    esi, dword ptr [esp + 14h]
        add    esi, dword ptr [esp + 10h]
        lea    eax, [esi + 0ffh]
        shr    eax, 8
        push   eax
        push   ebp
        call   sub_1_05C9
        add    esp, 8
        movzx  eax, ax
        cmp    eax, esi
        jb     NEAR PTR loc_1_18F6
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 18h], eax
        mov    edx, 100h
        sub    edx, dword ptr [esp + 14h]
        cmp    edx, eax
        jb     SHORT loc_1_1750
        mov    edx, eax
loc_1_1750:
        lea    eax, [ebp + 32h]
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 38h]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [ebp + 32h]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        sub    dword ptr [esp + 10h], edx
        mov    ax, word ptr [esp + 14h]
        add    dword ptr [esp + 38h], edx
        add    ax, dx
        cmp    dword ptr [esp + 10h], 0
        mov    word ptr [ebp + 36h], ax
        lea    eax, [ebp + 38h]
        je     SHORT loc_1_17C5
loc_1_178A:
        mov    edx, dword ptr [esp + 10h]
        cmp    edx, 100h
        jb     SHORT loc_1_179B
        mov    edx, 100h
loc_1_179B:
        mov    edi, dword ptr [eax]
        mov    esi, dword ptr [esp + 38h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        sub    dword ptr [esp + 10h], edx
        mov    word ptr [eax + 4], dx
        add    dword ptr [esp + 38h], edx
        add    eax, 6
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_178A
loc_1_17C5:
        db  0C7h, 045h, 012h, 029h, 019h, 000h, 000h
        mov    dword ptr [ebp + 0ah], ebx
        inc    word ptr [ebx + 8ah]
        lea    eax, [ebx + 74h]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [ebx + 78h]
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebx + 78h], ebp
        and    dword ptr [ebx + 50h], 0fffffffdh
        cmp    dword ptr [ebx + 2ch], 2
        jne    NEAR PTR loc_1_18A2
        mov    ax, word ptr [ebx + 0a4h]
        cmp    dword ptr [esp + 44h], 0
        mov    word ptr [ebp + 18h], ax
        mov    eax, dword ptr [ebp + 32h]
        jne    SHORT loc_1_182D
        lea    edi, [eax + 6]
        lea    esi, [ebx + 0a9h]
        movsd
        movsd
        movsd
        mov    cl, byte ptr [ebx + 0a6h]
        lea    esi, [ebx + 0b6h]
        mov    byte ptr [eax + 5], cl
        lea    edi, [ebp + 2ah]
        movsd
        movsw
        jmp    SHORT loc_1_189A
loc_1_182D:
        lea    edx, [eax + 6]
        mov    ecx, dword ptr [esp + 44h]
        add    ecx, 2
        mov    dword ptr [esp + 20h], ecx
        mov    edi, edx
        mov    esi, ecx
        movsd
        movsd
        movsd
        test   byte ptr [ebx + 0ceh], 4
        je     SHORT loc_1_1854
        mov    ecx, dword ptr [esp + 44h]
        mov    cl, byte ptr [ecx + 0eh]
        jmp    SHORT loc_1_185A
loc_1_1854:
        mov    cl, byte ptr [ebx + 0a6h]
loc_1_185A:
        mov    byte ptr [eax + 5], cl
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_1871
        lea    edi, [ebp + 2ah]
        mov    eax, 0ffffffffh
        stosd
        stosw
        jmp    SHORT loc_1_189A
loc_1_1871:
        mov    ecx, dword ptr [esp + 20h]
        db  0A1h, 09Eh, 026h, 000h, 000h
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_1_188D
        mov    esi, dword ptr [esp + 44h]
        add    esi, 6
        lea    edi, [ebp + 2ah]
        movsd
        movsw
        jmp    SHORT loc_1_189A
loc_1_188D:
        lea    eax, [ebp + 2ah]
        push   eax
        push   edx
        call   sub_1_01A2
        add    esp, 8
loc_1_189A:
        push   ebp
        call   sub_1_0073
        jmp    SHORT loc_1_18D3
loc_1_18A2:
        mov    eax, dword ptr [ebp + 32h]
        mov    cx, word ptr [ebx + 0cch]
        mov    word ptr [eax + 20h], cx
        mov    byte ptr [eax + 1eh], 0
        mov    cl, byte ptr [ebx + 0d0h]
        mov    byte ptr [eax + 1fh], cl
        or     word ptr [ebp + 8], 2
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_18CD
        mov    byte ptr [eax + 1eh], 10h
loc_1_18CD:
        push   ebp
        call   sub_1_005D
loc_1_18D3:
        mov    ecx, dword ptr [esp + 50h]
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        mov    dword ptr [ecx], eax
        call   sub_1_0724
        test   byte ptr [ebx + 41h], 1
        je     SHORT loc_1_18F2
        mov    eax, 2733h
        jmp    SHORT loc_1_1921
loc_1_18F2:
        sub    eax, eax
        jmp    SHORT loc_1_1921
loc_1_18F6:
        or     ebp, ebp
        je     SHORT loc_1_1903
        push   ebp
        call   sub_1_0500
        add    esp, 4
loc_1_1903:
        call   sub_1_0724
        test   byte ptr [ebx + 41h], 1
        je     SHORT loc_1_1915
        mov    eax, 2747h
        jmp    SHORT loc_1_1921
loc_1_1915:
        mov    eax, 2733h
        jmp    SHORT loc_1_1921
loc_1_191C:
        mov    eax, 271eh
loc_1_1921:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_1929:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    dx, word ptr [esi - 6]
        sub    esi, 0eh
        test   dl, 8
        je     SHORT loc_1_1942
        jmp    NEAR PTR loc_1_1A18
loc_1_1942:
        mov    edi, dword ptr [esi + 0ah]
        mov    ebp, dword ptr [edi + 40h]
        and    ebp, 100h
        je     SHORT loc_1_19C1
        cmp    dword ptr [edi + 2ch], 2
        je     SHORT loc_1_19C1
        cmp    byte ptr [esi + 17h], 0
        jne    SHORT loc_1_19C1
        movzx  ebx, word ptr [esi + 3ch]
        add    ebx, dword ptr [edi + 94h]
        mov    dword ptr [edi + 94h], ebx
        mov    ecx, dword ptr [edi + 90h]
        cmp    ecx, ebx
        jbe    SHORT loc_1_19C1
        mov    eax, dword ptr [edi + 8ch]
        add    eax, ebx
        mov    dword ptr [esi + 38h], eax
        mov    eax, ecx
        sub    ax, bx
        mov    word ptr [esi + 3ch], ax
        movzx  edi, ax
        db  0A1h, 0BCh, 026h, 000h, 000h
        cmp    edi, eax
        jle    SHORT loc_1_199A
        mov    word ptr [esi + 3ch], ax
loc_1_199A:
        movzx  eax, word ptr [esi + 3ch]
        add    eax, ebx
        cmp    eax, ecx
        jb     SHORT loc_1_19AB
        mov    eax, dword ptr [esi + 32h]
        or     byte ptr [eax + 1eh], 10h
loc_1_19AB:
        mov    word ptr [esi + 30h], 2
        or     dx, 2
        mov    word ptr [esi + 8], dx
        push   esi
        call   sub_1_005D
        jmp    SHORT loc_1_1A1E
loc_1_19C1:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        dec    word ptr [edi + 8ah]
        or     ebp, ebp
        je     SHORT loc_1_1A0C
        push   dword ptr [edi + 90h]
        push   dword ptr [edi + 8ch]
        call   sub_1_038F
        add    esp, 8
        mov    dword ptr [edi + 8ch], 0
        mov    dword ptr [edi + 90h], 0
        mov    dword ptr [edi + 94h], 0
        and    dword ptr [edi + 50h], 0fffffffdh
loc_1_1A0C:
        push   2
        push   edi
        push   esi
        call   sub_1_24F0
        add    esp, 0ch
loc_1_1A18:
        push   esi
        call   sub_1_0500
loc_1_1A1E:
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1A30:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_1A43
        mov    eax, 273dh
        jmp    SHORT loc_1_1A7C
loc_1_1A43:
        call   sub_1_0724
        mov    esi, dword ptr [edi + 4ch]
        mov    dword ptr [edi + 4ch], 3
        movzx  eax, byte ptr [esp + 10h]
        mov    dword ptr [edi + 44h], eax
        mov    dword ptr [edi + 48h], 0
        mov    dword ptr [edi + 54h], 37h
        push   edi
        call   sub_1_1F38
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1A7A
        mov    dword ptr [edi + 4ch], esi
        jmp    SHORT loc_1_1A7C
loc_1_1A7A:
        sub    eax, eax
loc_1_1A7C:
        pop    edi
        pop    esi
        ret
loc_1_1A7F:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0724
        mov    ebx, dword ptr [esp + 24h]
        mov    eax, dword ptr [ebx + 4ch]
        cmp    eax, 5
        je     NEAR PTR loc_1_1C89
        cmp    eax, 6
        je     NEAR PTR loc_1_1C89
        cmp    dword ptr [esp + 2ch], 0eh
        jae    SHORT loc_1_1AB5
        mov    eax, 271eh
        jmp    NEAR PTR loc_1_1C8E
loc_1_1AB5:
        mov    ecx, dword ptr [esp + 28h]
        cmp    word ptr [ecx], 6
        je     SHORT loc_1_1AC9
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_1C8E
loc_1_1AC9:
        cmp    dword ptr [ecx + 0ch], -1
        lea    eax, [ecx + 6]
        jne    SHORT loc_1_1AE5
        movzx  eax, word ptr [eax + 0ah]
        cmp    eax, -1
        jne    SHORT loc_1_1AE5
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_1C8E
loc_1_1AE5:
        lea    eax, [ebx + 0a7h]
        mov    edi, eax
        mov    esi, ecx
        movsd
        movsd
        movsd
        movsw
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 24h], 0eh
        test   byte ptr [ebx + 0ceh], 4
        je     SHORT loc_1_1B17
        mov    al, byte ptr [ecx + 0eh]
        mov    byte ptr [ebx + 0b5h], al
        mov    dword ptr [ebx + 24h], 0fh
loc_1_1B17:
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_1B41
        lea    eax, [ebx + 0b6h]
        push   eax
        lea    eax, [ebx + 0a9h]
        push   eax
        call   sub_1_01A2
        add    esp, 8
        or     al, al
        je     SHORT loc_1_1B41
        mov    eax, 2751h
        jmp    NEAR PTR loc_1_1C8E
loc_1_1B41:
        cmp    dword ptr [ebx + 4ch], 1
        jne    SHORT loc_1_1B8B
        lea    edi, [esp + 10h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosw
        mov    word ptr [esp + 10h], 6
        lea    eax, [esp + 10h]
        push   0eh
        push   eax
        push   ebx
        call   sub_1_232C
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_1C8E
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_1BA3
        mov    dword ptr [ebx + 54h], 0
        push   0
        push   2
        push   ebx
        call   sub_1_0420
        add    esp, 0ch
loc_1_1B8B:
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_1BA3
        mov    dword ptr [ebx + 4ch], 6
        and    dword ptr [ebx + 50h], 0ffffffefh
        sub    eax, eax
        jmp    NEAR PTR loc_1_1C8E
loc_1_1BA3:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_1BBD
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_1C8E
loc_1_1BBD:
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [ebp + 32h], eax
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_1_1BE3
        push   ebp
        call   sub_1_0500
        add    esp, 4
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_1C8E
loc_1_1BE3:
        and    dword ptr [ebx + 54h], 0ffffffefh
        mov    word ptr [ebp + 30h], 1
        mov    word ptr [ebp + 36h], 2ah
        mov    ax, word ptr [ebx + 0a4h]
        lea    esi, [ebx + 0a9h]
        mov    word ptr [ebp + 18h], ax
        mov    dword ptr [ebp + 0ah], ebx
        db  0C7h, 045h, 012h, 0F0h, 01Eh, 000h, 000h
        lea    edi, [ecx + 6]
        movsd
        movsd
        movsd
        mov    byte ptr [ecx + 5], 5
        lea    eax, [ebx + 74h]
        mov    esi, dword ptr [ebx + 4ch]
        mov    dword ptr [ebx + 4ch], 5
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [ebx + 78h]
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebx + 78h], ebp
        or     word ptr [ebp + 8], 2
        and    dword ptr [ebx + 50h], 0ffffffefh
        push   ebp
        call   sub_1_00C7
        add    esp, 4
        or     al, al
        je     SHORT loc_1_1C67
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   ebp
        call   sub_1_0500
        add    esp, 4
        mov    dword ptr [ebx + 4ch], esi
        mov    eax, 2742h
        jmp    SHORT loc_1_1C8E
loc_1_1C67:
        push   ebx
        call   sub_1_1027
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1C82
        call   sub_1_0724
        push   ebx
        call   sub_1_1027
        add    esp, 4
loc_1_1C82:
        mov    eax, 2733h
        jmp    SHORT loc_1_1C8E
loc_1_1C89:
        mov    eax, 2748h
loc_1_1C8E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1C96:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
        and    dword ptr [esi + 50h], 0fffffff7h
        or     edi, edi
        jne    SHORT loc_1_1CB3
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_1D97
loc_1_1CB3:
        cmp    dword ptr [esi + 48h], 0
        jne    SHORT loc_1_1CBE
        jmp    NEAR PTR loc_1_1D92
loc_1_1CBE:
        call   sub_1_0724
        mov    dword ptr [edi], 0
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 0bch]
        sub    eax, 0bch
        cmp    esi, eax
        je     SHORT loc_1_1D0C
        mov    ecx, 4
loc_1_1CDF:
        cmp    dword ptr [eax + 4ch], ecx
        jne    SHORT loc_1_1CEC
        cmp    dword ptr [eax + 94h], esi
        je     SHORT loc_1_1CFD
loc_1_1CEC:
        mov    eax, dword ptr [eax + 0bch]
        sub    eax, 0bch
        cmp    esi, eax
        jne    SHORT loc_1_1CDF
        jmp    SHORT loc_1_1D0C
loc_1_1CFD:
        mov    ebx, eax
        test   byte ptr [eax + 3ch], 1
        je     SHORT loc_1_1D0C
        mov    dword ptr [eax + 4ch], 7
loc_1_1D0C:
        or     ebx, ebx
        je     SHORT loc_1_1D8B
        dec    dword ptr [esi + 48h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 68h], eax
        cmp    dword ptr [ebx + 4ch], 4
        jne    SHORT loc_1_1D2E
        mov    dword ptr [ebx + 4ch], 6
        mov    dword ptr [ebx + 54h], 8
loc_1_1D2E:
        test   byte ptr [ebx + 41h], 1
        jne    SHORT loc_1_1D60
        and    dword ptr [ebx + 50h], 0fffffffdh
        push   2
        push   ebx
        push   0
        call   sub_1_24F0
        add    esp, 0ch
        cmp    word ptr [ebx + 84h], 0
        je     SHORT loc_1_1D60
        and    dword ptr [ebx + 50h], 0fffffffeh
        push   1
        push   ebx
        push   0
        call   sub_1_24F0
        add    esp, 0ch
loc_1_1D60:
        mov    dword ptr [edi], ebx
        cmp    dword ptr [esi + 48h], 0
        je     SHORT loc_1_1D79
        and    dword ptr [esi + 50h], 0fffffff7h
        push   8
        push   esi
        push   0
        call   sub_1_24F0
        add    esp, 0ch
loc_1_1D79:
        push   esi
        call   sub_1_1F38
        add    esp, 4
        call   sub_1_0724
        sub    eax, eax
        jmp    SHORT loc_1_1D97
loc_1_1D8B:
        mov    dword ptr [esi + 48h], 0
loc_1_1D92:
        mov    eax, 2733h
loc_1_1D97:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1D9B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        sub    eax, 0eh
        mov    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [eax + 0ah]
        and    byte ptr [ebx + 0ceh], 7fh
        cmp    byte ptr [eax + 17h], 0
        je     SHORT loc_1_1DEC
        push   8
        push   ebx
        and    dword ptr [ebx + 50h], 0fffffff7h
        push   dword ptr [esp + 18h]
        call   sub_1_24F0
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        call   sub_1_0500
        jmp    NEAR PTR loc_1_1EE5
loc_1_1DEC:
        push   ebx
        call   sub_1_2069
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_1EC0
        mov    dword ptr [ebp + 54h], 10h
        mov    dword ptr [ebp + 68h], 0ffffffffh
        mov    dword ptr [ebp + 50h], 0
        mov    dword ptr [ebp + 3ch], 0
        lea    edi, [ebp + 0a9h]
        mov    dword ptr [ebp + 4ch], 4
        mov    dword ptr [ebp + 94h], ebx
        mov    esi, dword ptr [esp + 10h]
        add    esi, 1eh
        movsd
        movsd
        lea    ecx, [ebp + 0a7h]
        movsd
        mov    word ptr [ecx], 6
        mov    ax, word ptr [ebx + 0a4h]
        mov    word ptr [ebp + 0b3h], ax
        lea    esi, [ebx + 98h]
        mov    eax, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 1ah]
        mov    word ptr [ebp + 0cch], ax
        lea    eax, [ebp + 98h]
        mov    edi, eax
        movsd
        movsd
        movsd
        movsw
        mov    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp + 18h], ecx
        lea    ecx, [ebp + 0bch]
        mov    dword ptr [ebp + 24h], 0eh
        lea    eax, [ebx + 0bch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0c0h]
        mov    dword ptr [ebp + 0c0h], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebx + 0c0h], ecx
        inc    dword ptr [ebx + 48h]
        push   ebp
        call   sub_1_1027
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1EC0
        call   sub_1_0724
        push   ebp
        call   sub_1_1027
        add    esp, 4
loc_1_1EC0:
        push   8
        push   ebx
        and    dword ptr [ebx + 50h], 0fffffff7h
        push   dword ptr [esp + 18h]
        call   sub_1_24F0
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        call   sub_1_0500
        add    esp, 4
        push   ebx
        call   sub_1_1F38
loc_1_1EE5:
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_1EF0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        sub    esi, 0eh
        test   byte ptr [esi + 8], 8
        je     SHORT loc_1_1F00
        jmp    SHORT loc_1_1F2D
loc_1_1F00:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0ah]
        cmp    byte ptr [esi + 17h], 0
        jne    SHORT loc_1_1F21
        mov    dword ptr [eax + 4ch], 6
        mov    dword ptr [eax + 54h], 8
loc_1_1F21:
        push   10h
        push   eax
        push   esi
        call   sub_1_24F0
        add    esp, 0ch
loc_1_1F2D:
        push   esi
        call   sub_1_0500
        add    esp, 4
        pop    esi
        ret
sub_1_1F38:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        test   byte ptr [esi + 0ceh], 80h
        je     SHORT loc_1_1F49
        jmp    SHORT loc_1_1FB3
loc_1_1F49:
        mov    eax, dword ptr [esi + 44h]
        cmp    dword ptr [esi + 48h], eax
        jne    SHORT loc_1_1F58
        mov    eax, 2747h
        jmp    SHORT loc_1_1FB5
loc_1_1F58:
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_1F6F
        mov    eax, 2747h
        jmp    SHORT loc_1_1FB5
loc_1_1F6F:
        or     byte ptr [esi + 0ceh], 80h
        mov    word ptr [edi + 36h], 2ah
        mov    ax, word ptr [esi + 0a4h]
        mov    word ptr [edi + 18h], ax
        lea    eax, [esi + 74h]
        db  0C7h, 047h, 012h, 09Bh, 01Dh, 000h, 000h
        mov    dword ptr [edi + 0ah], esi
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 78h], edi
        push   esi
        call   sub_1_1027
        add    esp, 4
        push   edi
        call   sub_1_00F9
        add    esp, 4
loc_1_1FB3:
        sub    eax, eax
loc_1_1FB5:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1FC0:
        mov    edx, dword ptr [esp + 8]
        cmp    edx, 1
        je     SHORT loc_1_1FDA
        cmp    edx, 2
        je     SHORT loc_1_1FDA
        cmp    edx, 5
        je     SHORT loc_1_1FDA
        mov    eax, 273ch
        jmp    SHORT loc_1_204D
loc_1_1FDA:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     SHORT loc_1_1FF2
        cmp    ecx, 6
        je     SHORT loc_1_1FEE
        mov    eax, 273fh
        jmp    SHORT loc_1_204D
loc_1_1FEE:
        or     ecx, ecx
        jne    SHORT loc_1_2001
loc_1_1FF2:
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        jne    SHORT loc_1_2005
        mov    eax, 273bh
        jmp    SHORT loc_1_204D
loc_1_2001:
        mov    eax, dword ptr [esp + 0ch]
loc_1_2005:
        cmp    eax, 6
        jne    SHORT loc_1_200C
        jmp    SHORT loc_1_204B
loc_1_200C:
        or     eax, eax
        jne    SHORT loc_1_2017
        cmp    ecx, 6
        jne    SHORT loc_1_2017
        jmp    SHORT loc_1_204B
loc_1_2017:
        cmp    edx, 1
        je     SHORT loc_1_2038
        cmp    edx, 5
        je     SHORT loc_1_2038
        cmp    eax, 4e7h
        ja     SHORT loc_1_2031
        cmp    eax, 3e8h
        jb     SHORT loc_1_2031
        jmp    SHORT loc_1_204B
loc_1_2031:
        mov    eax, 273bh
        jmp    SHORT loc_1_204D
loc_1_2038:
        cmp    eax, 4e8h
        je     SHORT loc_1_204B
        cmp    eax, 4e9h
        mov    eax, 273bh
        jne    SHORT loc_1_204D
loc_1_204B:
        sub    eax, eax
loc_1_204D:
        ret
loc_1_204E:
        push   esi
        call   sub_1_0724
        push   0
        call   sub_1_2069
        add    esp, 4
        mov    esi, eax
        call   sub_1_0724
        mov    eax, esi
        pop    esi
        ret
sub_1_2069:
        push   esi
        push   edi
        push   0d1h
        call   sub_1_01C8
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2083
        sub    eax, eax
        jmp    NEAR PTR loc_1_2144
loc_1_2083:
        mov    word ptr [eax + 86h], 1eh
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        je     SHORT loc_1_209F
        mov    edi, eax
        mov    ecx, 1dh
        rep movsd
        jmp    SHORT loc_1_20AD
loc_1_209F:
        mov    dword ptr [eax + 34h], 800h
        mov    dword ptr [eax + 38h], 800h
loc_1_20AD:
        db  0C7h, 080h, 0C4h, 000h, 000h, 000h, 090h, 026h, 000h, 000h
        lea    edx, [eax + 0c4h]
        db  08Bh, 00Dh, 094h, 026h, 000h, 000h
        mov    dword ptr [eax + 0c8h], ecx
        db  08Bh, 00Dh, 094h, 026h, 000h, 000h
        mov    dword ptr [ecx], edx
        db  089h, 015h, 094h, 026h, 000h, 000h
        db  08Bh, 00Dh, 098h, 026h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 098h, 026h, 000h, 000h
        lea    ecx, [eax + 7ch]
        mov    dword ptr [eax + 80h], ecx
        mov    dword ptr [ecx], ecx
        mov    word ptr [eax + 84h], 0
        mov    byte ptr [eax + 89h], 0
        lea    ecx, [eax + 0bch]
        mov    byte ptr [eax + 88h], 0
        mov    word ptr [eax + 8ah], 0
        mov    word ptr [eax + 0cch], 0
        mov    dword ptr [eax + 0c0h], ecx
        mov    dword ptr [ecx], ecx
        lea    ecx, [eax + 74h]
        mov    dword ptr [eax + 78h], ecx
        mov    dword ptr [ecx], ecx
        mov    byte ptr [eax + 0ceh], 0
        mov    byte ptr [eax + 0d0h], 0
        lea    ecx, [eax + 8]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ecx], ecx
loc_1_2144:
        pop    edi
        pop    esi
        ret
sub_1_2147:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [esi + 4ch], 9
        mov    eax, dword ptr [esi + 7ch]
        lea    edi, [esi + 7ch]
        sub    eax, esi
        cmp    eax, 7ch
        je     SHORT loc_1_217D
loc_1_2162:
        mov    ecx, dword ptr [edi]
        push   ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        call   sub_1_0500
        add    esp, 4
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 7ch
        jne    SHORT loc_1_2162
loc_1_217D:
        cmp    word ptr [esi + 8ah], 0
        je     SHORT loc_1_2190
        push   esi
        call   sub_1_0265
        add    esp, 4
loc_1_2190:
        cmp    dword ptr [esi + 2ch], 2
        je     SHORT loc_1_220A
        cmp    word ptr [esi + 0cch], 0
        je     SHORT loc_1_220A
        test   byte ptr [esi + 3ch], 1
        jne    SHORT loc_1_220A
        push   1
        call   sub_1_0480
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_220A
        push   1
        call   sub_1_0563
        add    esp, 4
        mov    dword ptr [edi + 32h], eax
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_2201
        mov    word ptr [edi + 30h], 1
        push   ebx
        movzx  eax, word ptr [esi + 0cch]
        push   eax
        call   sub_1_0143
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_2201
        cmp    byte ptr [ebx], 3
        jne    SHORT loc_1_2201
        mov    word ptr [edi + 36h], 2ah
        mov    dword ptr [edi + 0ah], esi
        db  0C7h, 047h, 012h, 019h, 022h, 000h, 000h
        push   edi
        call   sub_1_011A
        jmp    SHORT loc_1_2210
loc_1_2201:
        push   edi
        call   sub_1_0500
        add    esp, 4
loc_1_220A:
        push   esi
        call   sub_1_2238
loc_1_2210:
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2219:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax - 4]
        sub    eax, 0eh
        push   eax
        call   sub_1_0500
        add    esp, 4
        push   esi
        call   sub_1_2238
        add    esp, 4
        pop    esi
        ret
sub_1_2238:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 74h]
        lea    edi, [esi + 74h]
        sub    eax, esi
        cmp    eax, 74h
        je     SHORT loc_1_2282
loc_1_224C:
        mov    ebx, dword ptr [edi]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    ax, word ptr [ebx + 8]
        test   al, 2
        je     SHORT loc_1_2267
        or     ax, 8
        mov    word ptr [ebx + 8], ax
        jmp    SHORT loc_1_2279
loc_1_2267:
        push   ebx
        call   sub_1_00AA
        add    esp, 4
        push   ebx
        call   sub_1_0500
        add    esp, 4
loc_1_2279:
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 74h
        jne    SHORT loc_1_224C
loc_1_2282:
        mov    edi, dword ptr [esi + 0bch]
        sub    edi, 0bch
        cmp    edi, esi
        je     SHORT loc_1_22BF
        mov    ebx, 4
loc_1_2297:
        mov    eax, edi
        mov    edi, dword ptr [edi + 0bch]
        sub    edi, 0bch
        cmp    dword ptr [eax + 4ch], ebx
        jne    SHORT loc_1_22BB
        cmp    dword ptr [eax + 94h], esi
        jne    SHORT loc_1_22BB
        push   eax
        call   sub_1_02E4
        add    esp, 4
loc_1_22BB:
        cmp    edi, esi
        jne    SHORT loc_1_2297
loc_1_22BF:
        mov    ecx, dword ptr [esi + 0c8h]
        lea    eax, [esi + 0c4h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 0bch]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0bch
        jne    SHORT loc_1_22F5
        movzx  eax, word ptr [esi + 0a4h]
        push   eax
        call   sub_1_030C
        add    esp, 4
        jmp    SHORT loc_1_2300
loc_1_22F5:
        mov    eax, dword ptr [esi + 0c0h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_1_2300:
        push   esi
        call   sub_1_0638
        add    esp, 4
        db  0A1h, 098h, 026h, 000h, 000h
        dec    eax
        db  0A3h, 098h, 026h, 000h, 000h
        jne    SHORT loc_1_231F
        push   esi
        call   sub_1_0638
        add    esp, 4
loc_1_231F:
        push   esi
        call   sub_1_01F8
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_232C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 0
        db  08Bh, 015h, 090h, 026h, 000h, 000h
        sub    edx, 0c4h
        mov    ebx, dword ptr [esp + 1ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_24E2
        cmp    dword ptr [esp + 20h], 0eh
        jb     NEAR PTR loc_1_24E2
        cmp    word ptr [ebx], 6
        je     SHORT loc_1_236B
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_24E7
loc_1_236B:
        mov    cx, word ptr [ebx + 0ch]
        or     cx, cx
        je     SHORT loc_1_23F3
        sub    eax, eax
        db  08Bh, 035h, 098h, 026h, 000h, 000h
        cmp    esi, eax
        mov    ebp, dword ptr [esp + 18h]
        jle    SHORT loc_1_23BB
        mov    edi, 1
loc_1_2389:
        db  081h, 0FAh, 0CCh, 025h, 000h, 000h
        je     SHORT loc_1_23BB
        cmp    ebp, edx
        je     SHORT loc_1_23A3
        cmp    dword ptr [edx + 4ch], edi
        je     SHORT loc_1_23A3
        cmp    word ptr [edx + 0a4h], cx
        je     SHORT loc_1_23B6
loc_1_23A3:
        inc    eax
        mov    edx, dword ptr [edx + 0c4h]
        sub    edx, 0c4h
        cmp    esi, eax
        jg     SHORT loc_1_2389
        jmp    SHORT loc_1_23BB
loc_1_23B6:
        mov    byte ptr [esp + 13h], 1
loc_1_23BB:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_23F7
        test   byte ptr [ebp + 40h], 20h
        jne    SHORT loc_1_23D2
        mov    eax, 2740h
        jmp    NEAR PTR loc_1_24E7
loc_1_23D2:
        lea    eax, [ebp + 0bch]
        mov    ecx, dword ptr [edx + 0bch]
        add    edx, 0bch
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp + 0c0h], edx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx], eax
        jmp    SHORT loc_1_23F7
loc_1_23F3:
        mov    ebp, dword ptr [esp + 18h]
loc_1_23F7:
        call   sub_1_017F
        lea    edx, [ebx + 6]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_240B
        cmp    word ptr [ebx + 0ah], 0
        je     SHORT loc_1_2448
loc_1_240B:
        db  0B8h, 0A2h, 026h, 000h, 000h
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    SHORT loc_1_241E
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
loc_1_241E:
        je     SHORT loc_1_2448
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_243E
        mov    ecx, dword ptr [ebp + 0c0h]
        mov    eax, dword ptr [ebp + 0bch]
        add    ebp, 0bch
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_243E:
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_24E7
loc_1_2448:
        lea    ecx, [ebp + 98h]
        db  0BEh, 09Ch, 026h, 000h, 000h
        mov    edi, ecx
        movsd
        movsd
        movsd
        movsw
        test   byte ptr [ebp + 0ceh], 4
        mov    ax, word ptr [ebx + 0ch]
        mov    word ptr [ebp + 0a4h], ax
        mov    dword ptr [ebp + 14h], ecx
        mov    dword ptr [ebp + 20h], 0eh
        je     SHORT loc_1_248A
        mov    al, byte ptr [ebx + 0eh]
        mov    byte ptr [ebp + 0a6h], al
        mov    dword ptr [ebp + 20h], 0fh
        jmp    SHORT loc_1_249D
loc_1_248A:
        mov    eax, dword ptr [ebp + 2ch]
        sub    eax, 2
        cmp    eax, 1
        sbb    al, al
        add    al, 5
        mov    byte ptr [ebp + 0a6h], al
loc_1_249D:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_24C1
        push   ebp
        call   0
        add    esp, 4
        or     al, al
        je     SHORT loc_1_24C1
        mov    word ptr [ebp + 0a4h], 0
        mov    eax, 2747h
        jmp    SHORT loc_1_24E7
loc_1_24C1:
        mov    dword ptr [ebp + 4ch], 2
        cmp    dword ptr [ebp + 2ch], 2
        jne    SHORT loc_1_24D9
        push   ebp
        call   sub_1_1027
        add    esp, 4
        jmp    SHORT loc_1_24E7
loc_1_24D9:
        call   sub_1_0724
        sub    eax, eax
        jmp    SHORT loc_1_24E7
loc_1_24E2:
        mov    eax, 271eh
loc_1_24E7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h
sub_1_24F0:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, eax
        je     SHORT loc_1_2510
        cmp    byte ptr [ecx + 17h], al
        je     SHORT loc_1_2510
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   ecx
        call   sub_1_2524
        add    esp, 0ch
loc_1_2510:
        push   eax
        movzx  eax, byte ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_1_0420
        add    esp, 0ch
        ret
sub_1_2524:
        mov    eax, dword ptr [esp + 4]
        movzx  eax, byte ptr [eax + 17h]
        or     eax, eax
        je     SHORT loc_1_253D
        cmp    eax, 0edh
        je     SHORT loc_1_2541
loc_1_2537:
        mov    eax, 2742h
loc_1_253C:
        ret
loc_1_253D:
        sub    eax, eax
        jmp    SHORT loc_1_253C
loc_1_2541:
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 2ch], 2
        je     SHORT loc_1_2537
        movzx  eax, byte ptr [esp + 0ch]
        cmp    eax, 1
        jl     SHORT loc_1_2537
        cmp    eax, 2
        jle    SHORT loc_1_2561
        cmp    eax, 10h
        je     SHORT loc_1_2571
        jmp    SHORT loc_1_2537
loc_1_2561:
        mov    word ptr [ecx + 0cch], 0
        mov    eax, 2746h
        jmp    SHORT loc_1_253C
loc_1_2571:
        mov    eax, 2741h
        jmp    SHORT loc_1_253C
sub_1_2578:
        push   esi
        db  0C7h, 005h, 094h, 026h, 000h, 000h, 090h, 026h, 000h, 000h
        db  0C7h, 005h, 090h, 026h, 000h, 000h, 090h, 026h, 000h, 000h
        db  0C7h, 005h, 098h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B8h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D4h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D0h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C8h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C4h, 026h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 09Ch, 026h, 000h, 000h, 006h, 000h
        db  066h, 0C7h, 005h, 0A8h, 026h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 0CCh, 026h, 000h, 000h
        cmp    esi, 200h
        jge    SHORT loc_1_25F8
        mov    esi, 200h
loc_1_25F8:
        db  089h, 035h, 0CCh, 026h, 000h, 000h
        mov    eax, esi
        cdq
        and    edx, 0ffh
        add    eax, edx
        sar    eax, 8
        inc    eax
        db  0A3h, 0ACh, 026h, 000h, 000h
        mov    ecx, eax
        imul   ecx, ecx, 6
        add    ecx, 32h
        db  089h, 00Dh, 0C0h, 026h, 000h, 000h
        shl    eax, 2
        db  0A3h, 0B4h, 026h, 000h, 000h
        lea    eax, [esi - 2ah]
        db  0A3h, 0BCh, 026h, 000h, 000h
        sub    esi, 1eh
        db  089h, 035h, 0B0h, 026h, 000h, 000h
        sub    eax, eax
        pop    esi
        ret
loc_1_263D:
        sub    eax, eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_2654:
        rcr    byte ptr [edi], 0
        db  000h, 04Eh, 020h
        add    byte ptr [eax], al
        inc    edi
        and    dword ptr [eax], eax
        db  000h, 02Ch, 023h
        add    byte ptr [eax], al
        xchg   esi, eax
        sbb    al, 0
        db  000h, 07Fh, 01Ah
        add    byte ptr [eax], al
        xor    byte ptr [edx], bl
        add    byte ptr [eax], al
        xor    byte ptr [ebx], cl
        add    byte ptr [eax], al
        push   eax
        db  015h, 000h, 000h, 03Dh, 026h
        add    byte ptr [eax], al
        push   eax
        pop    es
        add    byte ptr [eax], al
        stc
        or     byte ptr [eax], al
        db  000h, 002h
        or     al, byte ptr [eax]
        db  000h, 062h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_269C:
        add    byte ptr [eax], al
loc_1_269E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_26D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
