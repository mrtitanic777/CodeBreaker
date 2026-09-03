; vshare_lcode_01.asm -- VSHARE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0FFh, 035h, 074h, 003h, 000h, 000h
        db  0FFh, 035h, 07Ch, 003h, 000h, 000h
        db  089h, 01Dh, 074h, 003h, 000h, 000h
        db  089h, 02Dh, 07Ch, 003h, 000h, 000h
        call   edx
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  08Fh, 005h, 07Ch, 003h, 000h, 000h
        db  08Fh, 005h, 074h, 003h, 000h, 000h
        ret
sub_1_002D:
        push   ebp
        mov    ebp, esp
        db  0FFh, 035h, 074h, 003h, 000h, 000h
        db  0FFh, 035h, 07Ch, 003h, 000h, 000h
        db  089h, 01Dh, 074h, 003h, 000h, 000h
        db  089h, 02Dh, 07Ch, 003h, 000h, 000h
        call   dword ptr [ebp + 8]
        db  08Fh, 005h, 07Ch, 003h, 000h, 000h
        db  08Fh, 005h, 074h, 003h, 000h, 000h
        leave
        ret    4
loc_1_005B:
        db  0FFh, 035h, 074h, 003h, 000h, 000h
        db  0FFh, 035h, 07Ch, 003h, 000h, 000h
        db  089h, 01Dh, 074h, 003h, 000h, 000h
        db  089h, 02Dh, 07Ch, 003h, 000h, 000h
        push   edx
        call   sub_1_13C0
        db  08Fh, 005h, 07Ch, 003h, 000h, 000h
        db  08Fh, 005h, 074h, 003h, 000h, 000h
        ret
sub_1_0086:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 05Bh, 000h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        mov    eax, esi
        pop    ebx
        pop    esi
        leave
        ret    8
sub_1_00A4:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        leave
        ret
sub_1_00B6:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        leave
        ret
sub_1_00C8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        xor    ebx, ebx
        xor    eax, eax
loc_1_00D6:
        lodsb
        add    ebx, eax
        inc    edx
        and    al, al
        jne    SHORT loc_1_00D6
        mov    eax, edx
        shl    eax, 10h
        mov    ax, bx
        pop    ebx
        pop    esi
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        xor    ecx, ecx
        dec    ecx
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, edi
        xor    al, al
        repne scasb
        not    ecx
        mov    edi, edx
        repe cmpsb
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        xor    ecx, ecx
        dec    ecx
        xor    al, al
        mov    edi, esi
        repne scasb
        not    ecx
        mov    edi, edx
        rep movsb
        mov    eax, edx
        pop    edi
        pop    esi
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        xor    al, al
        xor    ecx, ecx
        dec    ecx
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, ecx
        pop    edi
        leave
        ret    4
sub_1_0153:
        push   ebp
        mov    ebp, esp
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        xor    al, al
        mov    ecx, dword ptr [ebp + 0ch]
        rep stosb
        pop    edi
        leave
        ret    8
loc_1_0167:
        push   esi
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        inc    ebp
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        db  074h, 065h
        jb     SHORT loc_1_01E3
        popad
        insb
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        db  072h, 02Eh
        db  000h
sub_1_017F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0BEh, 067h, 001h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        leave
        ret    8
sub_1_019D:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 015h, 07Ch, 003h, 000h, 000h
        mov    byte ptr [edx + 1dh], 12h
        mov    byte ptr [edx + 1ch], al
        db  0A1h, 080h, 003h, 000h, 000h
        shr    eax, 4
        mov    word ptr [edx + 3ch], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
loc_1_01E3:
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        leave
        ret    4
sub_1_01F5:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
        db  0A1h, 0B5h, 003h, 000h, 000h
        add    eax, dword ptr [ebx + 4]
        mov    eax, dword ptr [eax]
        movzx  edx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, edx
        add    eax, dword ptr [ebx + 4]
        mov    edi, eax
        mov    edx, dword ptr [ebp + 8]
loc_1_021D:
        cmp    dx, word ptr [edi + 4]
        jb     SHORT loc_1_0243
        sub    dx, word ptr [edi + 4]
        mov    edi, dword ptr [edi]
        cmp    di, -1
        je     SHORT loc_1_0259
        mov    eax, edi
        and    eax, 0ffffh
        shr    edi, 10h
        shl    edi, 4
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        jmp    SHORT loc_1_021D
loc_1_0243:
        db  0A1h, 0D4h, 003h, 000h, 000h
        push   edx
        mul    edx
        pop    edx
        add    edi, eax
        add    edi, 6
        mov    eax, edi
loc_1_0253:
        pop    ebx
        pop    edi
        leave
        ret    4
loc_1_0259:
        xor    eax, eax
        jmp    SHORT loc_1_0253
sub_1_025D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
        sub    eax, dword ptr [ebx + 4]
        db  08Bh, 00Dh, 020h, 018h, 000h, 000h
        db  08Bh, 03Dh, 024h, 018h, 000h, 000h
        repne scasd
        xor    ecx, ecx
        sub    edi, 4
        cmp    eax, dword ptr [edi]
        mov    eax, ecx
        jne    SHORT loc_1_0289
        inc    eax
loc_1_0289:
        pop    edi
        pop    ecx
        pop    ebx
        leave
        ret    4
VSHARE_PM_API:
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_1_029D
        mov    word ptr [ebp + 1ch], 110h
loc_1_029D:
        ret
loc_1_029E:
        cmp    byte ptr [ebp + 1dh], 10h
        stc
        jne    SHORT loc_1_02A6
        clc
loc_1_02A6:
        ret
VSHARE_VM_NOT_EXECUTEABLE:
        test   edx, 3
        je     SHORT loc_1_02B9
        db  068h, 080h, 016h, 000h, 000h
        call   sub_1_002D
loc_1_02B9:
        clc
        ret
VSHARE_Control:
        cmp    eax, 0ch
        jae    SHORT loc_1_02C7
        db  0FFh, 024h, 085h, 0C9h, 002h, 000h, 000h
loc_1_02C7:
        clc
        ret
        db  017h, 003h, 000h, 000h, 0A4h, 001h, 000h, 000h, 0C7h, 002h, 000h, 000h, 0C7h, 002h, 000h, 000h
        db  0C7h, 002h, 000h, 000h, 0F9h, 002h, 000h, 000h, 0C7h, 002h, 000h, 000h, 0C7h, 002h, 000h, 000h
        db  0C7h, 002h, 000h, 000h, 0C7h, 002h, 000h, 000h, 0C7h, 002h, 000h, 000h, 0A7h, 002h, 000h, 000h
VSHARE_SYSTEM_EXIT:
        push   esi
        push   edi
        db  0A1h, 0A1h, 003h, 000h, 000h
        mov    edi, eax
        db  0BEh, 030h, 004h, 000h, 000h
        mov    ecx, 0fh
        cld
        rep movsd
        db  0A1h, 099h, 003h, 000h, 000h
        db  08Ah, 00Dh, 070h, 004h, 000h, 000h
        mov    word ptr [eax], cx
        db  08Bh, 015h, 06Ch, 004h, 000h, 000h
        or     edx, edx
        je     SHORT loc_1_0339
        db  0A1h, 0BDh, 003h, 000h, 000h
        mov    dword ptr [eax + 1ah], edx
        db  0A1h, 0A5h, 003h, 000h, 000h
        mov    word ptr [eax], 0
loc_1_0339:
        db  068h, 0B0h, 014h, 000h, 000h
        call   sub_1_002D
        pop    edi
        pop    esi
        clc
        ret
VSHARE_Service_00:
        mov    eax, 110h
        clc
        ret
sub_1_034E:
        db  0CDh, 020h, 052h, 080h, 040h, 000h
sub_1_0354:
        db  0CDh, 020h, 056h, 080h, 040h, 000h
sub_1_035A:
        db  0CDh, 020h, 057h, 080h, 040h, 000h
sub_1_0360:
        db  0CDh, 020h, 053h, 080h, 040h, 000h
sub_1_0366:
        db  0CDh, 020h, 055h, 080h, 040h, 000h
sub_1_036C:
        db  0CDh, 020h, 054h, 080h, 040h, 000h
        add    byte ptr [eax], al
loc_1_0374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_037C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0384:
        add    dword ptr [eax + eax], esp
        add    byte ptr [eax], al
loc_1_0389:
        into
        add    eax, 3300000h
        add    byte ptr [eax], al
loc_1_0391:
        and    eax, dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_0395:
        test   byte ptr [3030000h], 0
        add    byte ptr [ebp + eax + 900000h], ch
        add    byte ptr [eax], al
loc_1_03A5:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [esi - 51fffffdh], bh
        add    eax, 33c0000h
        add    byte ptr [eax], al
loc_1_03B5:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        db  000h
loc_1_03BD:
        add    byte ptr es:[eax], al
        add    byte ptr [esi + 3e000005h], bl
        add    eax, dword ptr [eax]
        add    byte ptr [edx], bh
        add    eax, dword ptr [eax]
        add    byte ptr [edx + 5], dh
        add    byte ptr [eax], al
loc_1_03D4:
        cmp    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_03D8:
        pop    es
        add    byte ptr [eax], al
        db  047h, 003h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 083h, 004h, 001h, 000h, 000h, 000h, 056h, 053h, 048h, 041h
        db  052h, 045h, 020h, 020h, 000h, 080h, 000h, 058h, 0BBh, 002h, 000h, 000h, 090h, 002h, 000h, 000h
        db  090h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0DCh, 003h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0480:
        db  08Bh, 00Dh, 07Ch, 003h, 000h, 000h
        mov    ax, word ptr [esp + 4]
        or     word ptr [ecx + 2ch], 1
        mov    word ptr [ecx + 1ch], ax
        db  0A1h, 074h, 003h, 000h, 000h
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 091h, 003h, 000h, 000h
        mov    byte ptr [ecx + eax], 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_04B0:
        push   1
        call   sub_1_0480
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_04C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 07Ch, 003h, 000h, 000h
        xor    ecx, ecx
        and    word ptr [eax + 2ch], 0fffeh
        db  0A1h, 074h, 003h, 000h, 000h
        db  08Bh, 03Dh, 080h, 003h, 000h, 000h
        mov    esi, dword ptr [eax + 4]
        db  0A1h, 0CDh, 003h, 000h, 000h
        mov    cx, word ptr [esi + eax]
        add    ecx, esi
        add    edi, ecx
        db  0A1h, 0C1h, 003h, 000h, 000h
        mov    eax, dword ptr [esi + eax]
        push   eax
        call   sub_1_1580
        lea    ebp, [esi + eax]
        push   1
        push   edi
        call   sub_1_0F50
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_05C2
        db  08Ah, 00Dh, 018h, 018h, 000h, 000h
        or     cl, cl
        je     SHORT loc_1_058D
        db  0A1h, 074h, 003h, 000h, 000h
        mov    edx, dword ptr [eax + 4]
        db  0A1h, 0C1h, 003h, 000h, 000h
        mov    eax, dword ptr [edx + eax]
        mov    dword ptr [esp + 10h], eax
        db  066h, 0A1h, 0D8h, 003h, 000h, 000h
        cmp    word ptr [esp + 10h], ax
        jne    SHORT loc_1_0586
        db  0A1h, 080h, 003h, 000h, 000h
        xor    edx, edx
        mov    dx, word ptr [esp + 12h]
        shr    eax, 4
        cmp    eax, edx
        jne    SHORT loc_1_0586
        cmp    cl, 1
        jne    SHORT loc_1_0570
        db  0BFh, 028h, 018h, 000h, 000h
        mov    esi, ebp
        mov    ecx, 0eh
        rep movsd
        movsb
        db  0C6h, 005h, 018h, 018h, 000h, 000h, 002h
        jmp    SHORT loc_1_058D
loc_1_0570:
        cmp    cl, 3
        jne    SHORT loc_1_0586
        db  0BEh, 028h, 018h, 000h, 000h
        mov    edi, ebp
        mov    ecx, 0eh
        rep movsd
        movsb
        jmp    SHORT loc_1_058D
loc_1_0586:
        db  0C6h, 005h, 018h, 018h, 000h, 000h, 000h
loc_1_058D:
        push   ebp
        push   ebx
        call   sub_1_1060
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_05C2
        mov    eax, dword ptr [ebx + 0ch]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_05C2
        lea    esi, [ebp + 0bh]
        lea    edi, [ebx + 10h]
        mov    ecx, 5
        rep movsd
        movsb
        db  080h, 03Dh, 084h, 003h, 000h, 000h, 001h
        jne    SHORT loc_1_05C2
        mov    ax, word ptr [ebp + 35h]
        mov    word ptr [ebx + 20h], ax
loc_1_05C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_05D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0621
        mov    ax, word ptr [esi]
        or     ax, ax
        je     SHORT loc_1_05F0
        cmp    ax, 0ffffh
        jne    SHORT loc_1_0621
loc_1_05F0:
        mov    esi, dword ptr [esi + 2bh]
        push   esi
        call   sub_1_1270
        add    esp, 4
        mov    eax, dword ptr [esi + 0ch]
        push   esi
        mov    dword ptr [eax + 2bh], 0
        mov    eax, dword ptr [esi + 0ch]
        mov    word ptr [eax + 33h], 0
        call   sub_1_12C0
        add    esp, 4
        push   esi
        call   sub_1_00B6
        add    esp, 4
loc_1_0621:
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0630:
        call   sub_1_15D0
        push   eax
        call   sub_1_05D0
        db  0A1h, 07Ch, 003h, 000h, 000h
        add    esp, 4
        and    word ptr [eax + 2ch], 0fffeh
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0650:
        db  0A1h, 074h, 003h, 000h, 000h
        push   0
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 0C5h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        push   eax
        call   sub_1_1310
        db  0A1h, 07Ch, 003h, 000h, 000h
        add    esp, 8
        and    word ptr [eax + 2ch], 0fffeh
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0680:
        db  0A1h, 074h, 003h, 000h, 000h
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 0B1h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        push   eax
        db  0A1h, 0C5h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        push   eax
        call   sub_1_1310
        db  0A1h, 07Ch, 003h, 000h, 000h
        add    esp, 8
        and    word ptr [eax + 2ch], 0fffeh
        ret
loc_1_06B0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_1600
        mov    cx, word ptr [eax + 0ch]
        mov    word ptr [esp + 12h], cx
        mov    ax, word ptr [eax + 6]
        mov    word ptr [esp + 10h], ax
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_1580
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        mov    edi, dword ptr [ecx + 4]
        add    edi, eax
        mov    eax, ecx
loc_1_06E5:
        push   eax
        push   edi
        call   sub_1_17D0
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        db  08Bh, 01Dh, 0C1h, 003h, 000h, 000h
        db  0A1h, 074h, 003h, 000h, 000h
        je     SHORT loc_1_073C
        mov    ecx, dword ptr [esi + 0ch]
        cmp    word ptr [ecx], 1
        jne    SHORT loc_1_071D
        push   esi
        call   sub_1_0DC0
        add    esp, 4
        db  08Bh, 01Dh, 0C1h, 003h, 000h, 000h
        db  0A1h, 074h, 003h, 000h, 000h
loc_1_071D:
        mov    ebp, dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, ebp
        push   eax
        call   sub_1_15A0
        mov    dword ptr [ebx + ebp], eax
        push   1
        call   sub_1_019D
        db  0A1h, 074h, 003h, 000h, 000h
        jmp    SHORT loc_1_06E5
loc_1_073C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0750:
        db  068h, 060h, 007h, 000h, 000h
        call   sub_1_0830
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_0760:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0A1h, 074h, 003h, 000h, 000h
        push   esi
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 0B1h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cx, word ptr [esi + 14h]
        shl    eax, 10h
        add    eax, ecx
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        add    eax, 8
        push   eax
        call   sub_1_0360
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_1_07B1
        push   eax
        call   sub_1_0480
        add    esp, 4
        pop    esi
        ret
loc_1_07B1:
        push   esi
        call   sub_1_08A0
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07C0:
        db  068h, 0D0h, 007h, 000h, 000h
        call   sub_1_0830
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_07D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0A1h, 074h, 003h, 000h, 000h
        push   esi
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 0B1h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cx, word ptr [esi + 14h]
        shl    eax, 10h
        add    eax, ecx
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        add    eax, 8
        push   eax
        call   sub_1_036C
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_1_0821
        push   eax
        call   sub_1_0480
        add    esp, 4
        pop    esi
        ret
loc_1_0821:
        push   esi
        call   sub_1_08A0
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0830:
        push   esi
        push   edi
        db  080h, 03Dh, 084h, 003h, 000h, 000h, 001h
        db  08Bh, 015h, 07Ch, 003h, 000h, 000h
        jne    SHORT loc_1_0861
        cmp    word ptr [edx + 18h], 1
        je     SHORT loc_1_0855
        push   21h
        call   sub_1_0480
        add    esp, 4
        pop    edi
        pop    esi
        ret
loc_1_0855:
        call   sub_1_1640
        mov    edi, dword ptr [eax]
        mov    esi, dword ptr [eax + 4]
        jmp    SHORT loc_1_0885
loc_1_0861:
        xor    eax, eax
        mov    ax, word ptr [edx + 18h]
        shl    eax, 10h
        xor    ecx, ecx
        mov    cx, word ptr [edx + 14h]
        lea    edi, [eax + ecx]
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        shl    eax, 10h
        xor    ecx, ecx
        mov    cx, word ptr [edx + 1ch]
        lea    esi, [eax + ecx]
loc_1_0885:
        call   sub_1_15D0
        push   esi
        push   edi
        mov    eax, dword ptr [eax + 2bh]
        push   eax
        call   dword ptr [esp + 18h]
        add    esp, 0ch
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_08A0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        or     esi, esi
        je     SHORT loc_1_08C2
        nop
loc_1_08B0:
        or     byte ptr [esi + 1bh], 10h
        push   4
        push   esi
        call   sub_1_1380
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_08B0
loc_1_08C2:
        pop    esi
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08D0:
        push   ebx
        push   esi
        push   edi
        call   sub_1_15D0
        db  08Bh, 00Dh, 07Ch, 003h, 000h, 000h
        mov    esi, dword ptr [eax + 2bh]
        and    word ptr [ecx + 2ch], 0fffeh
        push   eax
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_095A
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        je     SHORT loc_1_095A
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    di, word ptr [eax + 18h]
        or     di, di
        je     SHORT loc_1_095A
        db  0A1h, 074h, 003h, 000h, 000h
        push   esi
        mov    edx, dword ptr [eax + 4]
        db  0A1h, 0B1h, 003h, 000h, 000h
        xor    ebx, ebx
        mov    bx, word ptr [edx + eax]
        xor    eax, eax
        mov    ax, word ptr [esi + 14h]
        shl    ebx, 10h
        add    ebx, eax
        push   ebx
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        db  0A1h, 089h, 003h, 000h, 000h
        mov    eax, dword ptr [edx + eax]
        push   eax
        push   ecx
        call   sub_1_0354
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_095A
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    word ptr [eax + 1ch], 21h
        or     word ptr [eax + 2ch], 1
loc_1_095A:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_0960:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_1600
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        xor    esi, esi
        db  08Bh, 01Dh, 07Ch, 003h, 000h, 000h
        mov    si, word ptr [eax + 4]
        mov    ax, word ptr [ebx + 34h]
        mov    word ptr [ebx + 38h], ax
        db  08Bh, 015h, 0A9h, 003h, 000h, 000h
        db  066h, 02Bh, 015h, 080h, 003h, 000h, 000h
        mov    word ptr [ebx], dx
        db  08Bh, 015h, 000h, 018h, 000h, 000h
        db  081h, 0FAh, 000h, 018h, 000h, 000h
        je     SHORT loc_1_09CA
loc_1_09A4:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        jne    SHORT loc_1_09C0
        mov    ebp, dword ptr [edx + 0ch]
        or     ebp, ebp
        je     SHORT loc_1_09C0
loc_1_09B2:
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_1_09E0
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_1_09B2
loc_1_09C0:
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 000h, 018h, 000h, 000h
        jne    SHORT loc_1_09A4
loc_1_09CA:
        push   12h
        call   sub_1_0480
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_09E0:
        lea    edi, [edx + 29h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    dword ptr [esp + 10h], ecx
        mov    esi, edi
        db  0A1h, 074h, 003h, 000h, 000h
        db  08Bh, 03Dh, 0A9h, 003h, 000h, 000h
        shr    ecx, 2
        add    edi, dword ptr [eax + 4]
        rep movsd
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 3
        rep movsb
        push   ebp
        mov    eax, dword ptr [edx + 8]
        push   eax
        call   sub_1_035A
        add    esp, 8
        mov    word ptr [ebx + 18h], ax
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    cx, word ptr [ebp + 14h]
        and    word ptr [eax + 2ch], 0fffeh
        mov    word ptr [eax + 10h], cx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_0A40:
        push   esi
        call   sub_1_15D0
        mov    esi, eax
        call   sub_1_1600
        mov    cl, byte ptr [esi + 4]
        mov    byte ptr [eax + 1eh], cl
        mov    cx, word ptr [esi + 0bh]
        mov    word ptr [eax + 1ah], cx
        mov    word ptr [eax + 1ch], 0
        mov    ecx, dword ptr [esi + 2bh]
        or     ecx, ecx
        je     SHORT loc_1_0A73
        mov    ecx, dword ptr [ecx + 4]
        mov    cx, word ptr [ecx + 25h]
        mov    word ptr [eax + 1ch], cx
loc_1_0A73:
        db  0A1h, 07Ch, 003h, 000h, 000h
        pop    esi
        mov    byte ptr [eax + 10h], 0c0h
        ret
        db  08Bh, 0FFh
loc_1_0A80:
        push   esi
        push   edi
        call   sub_1_15D0
        mov    edi, eax
        call   sub_1_1600
        mov    esi, eax
        push   edi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0AC9
        cmp    word ptr [edi], 0
        je     SHORT loc_1_0AC9
        mov    eax, dword ptr [edi + 2bh]
        mov    eax, dword ptr [eax + 4]
        mov    cx, word ptr [esi + 1ch]
        cmp    word ptr [eax + 25h], cx
        jne    SHORT loc_1_0AC9
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    cx, word ptr [esi + 1ah]
        and    word ptr [eax + 2ch], 0fffeh
        mov    word ptr [eax + 10h], cx
        pop    edi
        pop    esi
        ret
loc_1_0AC9:
        db  0A1h, 07Ch, 003h, 000h, 000h
        pop    edi
        or     word ptr [eax + 2ch], 1
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 015h, 07Ch, 003h, 000h, 000h
        or     word ptr [edx + 2ch], 1
        db  0A1h, 074h, 003h, 000h, 000h
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 09Dh, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [edx + 1ch], ax
        call   sub_1_1600
        mov    edi, eax
        push   edi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_0BDC
        mov    ax, word ptr [edi + 2]
        and    al, 0f0h
        cmp    al, 70h
        jne    NEAR PTR loc_1_0BDC
        sub    esi, esi
        add    eax, 0
loc_1_0B30:
        push   esi
        call   sub_1_01F5
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_0BDC
        cmp    ebx, edi
        je     SHORT loc_1_0B83
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0B83
        cmp    word ptr [ebx], 0
        je     SHORT loc_1_0B83
        mov    ax, word ptr [edi + 2]
        cmp    word ptr [ebx + 2], ax
        jne    SHORT loc_1_0B83
        mov    eax, dword ptr [ebx + 2bh]
        mov    ecx, dword ptr [edi + 2bh]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ecx + 4], eax
        jne    SHORT loc_1_0B83
        mov    ax, word ptr [edi + 2fh]
        cmp    word ptr [ebx + 2fh], ax
        jne    SHORT loc_1_0B83
        mov    ax, word ptr [edi + 31h]
        cmp    word ptr [ebx + 31h], ax
        je     SHORT loc_1_0B86
loc_1_0B83:
        inc    esi
        jmp    SHORT loc_1_0B30
loc_1_0B86:
        db  0A1h, 07Ch, 003h, 000h, 000h
        push   ebx
        and    word ptr [eax + 2ch], 0fffeh
        mov    word ptr [ebx], 0
        call   sub_1_05D0
        add    esp, 4
        inc    word ptr [edi]
        sub    edi, edi
loc_1_0BA4:
        push   edi
        call   sub_1_1740
        add    esp, 4
        or     eax, eax
        db  0A1h, 074h, 003h, 000h, 000h
        db  08Bh, 00Dh, 0ADh, 003h, 000h, 000h
        je     SHORT loc_1_0BDC
        cmp    edi, esi
        jne    SHORT loc_1_0BD9
        mov    ebx, dword ptr [eax + 4]
        mov    eax, dword ptr [ecx + ebx]
        push   eax
        call   sub_1_1580
        mov    byte ptr [ebx + eax], 0ffh
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    word ptr [eax + 1ch], si
loc_1_0BD9:
        inc    edi
        jmp    SHORT loc_1_0BA4
loc_1_0BDC:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0BE0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 07Ch, 003h, 000h, 000h
        or     word ptr [eax + 2ch], 1
        db  0A1h, 074h, 003h, 000h, 000h
        db  0C6h, 005h, 018h, 018h, 000h, 000h, 001h
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 0CDh, 003h, 000h, 000h
        mov    cx, word ptr [eax + ecx]
        and    ecx, 0ffffh
        add    ecx, eax
        db  0A1h, 080h, 003h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [esp + 10h], eax
loc_1_0C20:
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   eax
        call   sub_1_0F50
        add    esp, 8
        or     eax, eax
        db  08Bh, 03Dh, 0C5h, 003h, 000h, 000h
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
        je     SHORT loc_1_0C92
        mov    edx, dword ptr [eax + 0ch]
        or     edx, edx
        je     SHORT loc_1_0C84
loc_1_0C46:
        mov    si, word ptr [edx + 18h]
        and    si, 0f0h
        mov    ebp, dword ptr [ebx + 4]
        db  08Bh, 00Dh, 0B1h, 003h, 000h, 000h
        mov    cx, word ptr [ebp + ecx]
        cmp    word ptr [edx + 16h], cx
        jne    SHORT loc_1_0C92
        mov    cx, word ptr [edx + 14h]
        cmp    word ptr [ebp + edi], cx
        jne    SHORT loc_1_0C92
        cmp    dword ptr [edx + 8], ebx
        jne    SHORT loc_1_0C92
        cmp    si, 70h
        je     SHORT loc_1_0C7E
        or     si, si
        jne    SHORT loc_1_0C92
loc_1_0C7E:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_0C46
loc_1_0C84:
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_0DC0
        add    esp, 4
        jmp    SHORT loc_1_0C20
loc_1_0C92:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0CA0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_15D0
        mov    ebx, eax
        test   word ptr [ebx + 5], 8080h
        jne    NEAR PTR loc_1_0DAC
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_0DAC
        mov    eax, dword ptr [ebx + 2bh]
        db  08Bh, 02Dh, 07Ch, 003h, 000h, 000h
        mov    edx, dword ptr [eax + 4]
        mov    cx, word ptr [ebp + 1ch]
        mov    word ptr [esp + 10h], cx
        and    ecx, 0ffffh
        or     ecx, ecx
        jl     NEAR PTR loc_1_0DAC
        cmp    ecx, 2
        jle    SHORT loc_1_0D00
        cmp    ecx, 3
        je     SHORT loc_1_0D50
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0D00:
        lea    esi, [ebx + 0bh]
        lea    edi, [edx + 10h]
        mov    ecx, 5
        rep movsd
        movsb
        db  080h, 03Dh, 084h, 003h, 000h, 000h, 001h
        jne    SHORT loc_1_0D1F
        mov    ax, word ptr [ebx + 35h]
        mov    word ptr [edx + 20h], ax
loc_1_0D1F:
        mov    esi, dword ptr [edx + 0ch]
        or     esi, esi
        je     NEAR PTR loc_1_0DAC
loc_1_0D2A:
        cmp    dword ptr [ebx + 2bh], esi
        je     SHORT loc_1_0D40
        mov    ax, word ptr [ebp + 1ch]
        push   eax
        push   esi
        call   sub_1_1380
        db  08Bh, 02Dh, 07Ch, 003h, 000h, 000h
loc_1_0D40:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_0D2A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_0D50:
        mov    ebx, dword ptr [edx + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_0DAC
loc_1_0D57:
        cmp    eax, ebx
        jne    SHORT loc_1_0D70
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_0D57
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D70:
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        cmp    dword ptr [ebx + 8], ecx
        jne    SHORT loc_1_0DA0
        mov    esi, dword ptr [ebx + 0ch]
        lea    edi, [edx + 10h]
        add    esi, 0bh
        mov    ecx, 5
        rep movsd
        movsb
        db  080h, 03Dh, 084h, 003h, 000h, 000h, 001h
        jne    SHORT loc_1_0DA0
        mov    ecx, dword ptr [ebx + 0ch]
        mov    cx, word ptr [ecx + 35h]
        mov    word ptr [edx + 20h], cx
loc_1_0DA0:
        mov    cx, word ptr [esp + 10h]
        push   ecx
        push   eax
        call   sub_1_1380
loc_1_0DAC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    di, di
        mov    esi, dword ptr [esp + 18h]
        mov    edi, edi
loc_1_0DD0:
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        call   sub_1_01F5
        or     eax, eax
        je     SHORT loc_1_0DF0
        cmp    dword ptr [esi + 0ch], eax
        je     SHORT loc_1_0DF9
        inc    di
        jmp    SHORT loc_1_0DD0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0DF0:
        cmp    dword ptr [esi + 0ch], eax
        jne    NEAR PTR loc_1_0E7F
loc_1_0DF9:
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
        mov    word ptr [esp + 10h], 0
        mov    ecx, dword ptr [ebx + 4]
        db  0A1h, 085h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [esp + 12h], ax
loc_1_0E17:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_1580
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
        mov    ebp, dword ptr [ebx + 4]
        add    ebp, eax
        cmp    byte ptr [ebp], 4dh
        jne    SHORT loc_1_0E69
        mov    ax, word ptr [ebp + 1]
        xor    ecx, ecx
        mov    cx, word ptr [esp + 12h]
        mov    edx, eax
        and    edx, 0ffffh
        sub    ecx, edx
        cmp    ecx, -1
        jne    SHORT loc_1_0E5C
        push   edi
        push   eax
        call   sub_1_0EB0
        add    esp, 8
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
loc_1_0E5C:
        mov    ax, word ptr [ebp + 3]
        inc    ax
        add    word ptr [esp + 12h], ax
        jmp    SHORT loc_1_0E17
loc_1_0E69:
        push   edi
        mov    ecx, dword ptr [ebx + 4]
        db  0A1h, 08Dh, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        push   eax
        call   sub_1_0EB0
        add    esp, 8
loc_1_0E7F:
        db  08Bh, 01Dh, 074h, 003h, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, dword ptr [ebx + 4]
        sub    eax, edi
        push   eax
        call   sub_1_15A0
        db  08Bh, 00Dh, 0C1h, 003h, 000h, 000h
        mov    dword ptr [edi + ecx], eax
        push   1
        call   sub_1_019D
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_0EB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 10h], 0
        mov    ax, word ptr [esp + 1ch]
        mov    word ptr [esp + 12h], ax
        mov    si, word ptr [esp + 20h]
        db  08Bh, 03Dh, 074h, 003h, 000h, 000h
loc_1_0ED3:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_1580
        mov    ebx, dword ptr [edi + 4]
        add    ebx, eax
        db  08Bh, 03Dh, 074h, 003h, 000h, 000h
        cmp    word ptr [ebx], 20cdh
        jne    SHORT loc_1_0F3A
        mov    bp, word ptr [ebx + 32h]
        mov    eax, dword ptr [ebx + 34h]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [edi + 4]
        add    ecx, eax
        mov    ax, bp
        dec    bp
        or     ax, ax
        db  08Bh, 03Dh, 074h, 003h, 000h, 000h
        je     SHORT loc_1_0F2E
        mov    eax, esi
        and    eax, 0ffffh
loc_1_0F18:
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        cmp    edx, eax
        jne    SHORT loc_1_0F23
        mov    byte ptr [ecx], 0ffh
loc_1_0F23:
        inc    ecx
        mov    dx, bp
        dec    bp
        or     dx, dx
        jne    SHORT loc_1_0F18
loc_1_0F2E:
        mov    eax, dword ptr [ebx + 38h]
        mov    dword ptr [esp + 10h], eax
        cmp    eax, -1
        jne    SHORT loc_1_0ED3
loc_1_0F3A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F50:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        push   edi
        call   sub_1_00C8
        mov    dword ptr [esp + 10h], eax
        db  08Bh, 01Dh, 000h, 018h, 000h, 000h
        db  081h, 0FBh, 000h, 018h, 000h, 000h
        je     SHORT loc_1_0FB6
loc_1_0F73:
        mov    ax, word ptr [esp + 10h]
        cmp    word ptr [ebx + 27h], ax
        jne    SHORT loc_1_0FAC
        mov    eax, edi
        lea    ecx, [ebx + 29h]
loc_1_0F83:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_0FA3
        or     dl, dl
        je     SHORT loc_1_0F9F
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_0FA3
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_0F83
loc_1_0F9F:
        sub    eax, eax
        jmp    SHORT loc_1_0FA8
loc_1_0FA3:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0FA8:
        or     eax, eax
        je     SHORT loc_1_0FC7
loc_1_0FAC:
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 000h, 018h, 000h, 000h
        jne    SHORT loc_1_0F73
loc_1_0FB6:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_0FD1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0FC7:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0FD1:
        xor    eax, eax
        mov    ax, word ptr [esp + 12h]
        add    eax, 2bh
        push   eax
        call   sub_1_00A4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_1044
        push   2bh
        push   ebp
        call   sub_1_0153
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ebp + 27h], ax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp + 29h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  066h, 0A1h, 008h, 018h, 000h, 000h
        mov    word ptr [ebp + 25h], ax
        inc    ax
        db  066h, 0A3h, 008h, 018h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], ebx
        mov    dword ptr [ebx], ebp
        mov    dword ptr [eax + 4], ebp
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_1044:
        push   24h
        call   sub_1_0480
        add    esp, 4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1060:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bp, 1
        mov    edi, dword ptr [esp + 18h]
        push   edi
        call   sub_1_16F0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_108A
        push   24h
        call   sub_1_0480
        add    esp, 4
        jmp    NEAR PTR loc_1_1146
loc_1_108A:
        push   1ch
        push   ebx
        call   sub_1_0153
        mov    esi, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 4], esi
        db  0A1h, 074h, 003h, 000h, 000h
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0ch], edi
        mov    ecx, dword ptr [eax + 4]
        db  0A1h, 0C5h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [edi + 2fh], ax
        mov    word ptr [ebx + 14h], ax
        db  0A1h, 0B1h, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [edi + 31h], ax
        mov    word ptr [ebx + 16h], ax
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ebx + 18h], ax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebx + 1ah], al
        mov    ax, word ptr [edi + 5]
        test   al, 80h
        je     SHORT loc_1_10EC
        test   al, 7
        je     SHORT loc_1_10EC
        db  080h, 03Dh, 014h, 018h, 000h, 000h, 000h
        je     SHORT loc_1_1111
loc_1_10EC:
        push   ebx
        call   sub_1_11A0
        add    esp, 4
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_1_1111
        push   ebp
        call   sub_1_0480
        add    esp, 4
        push   ebx
        call   sub_1_00B6
        add    esp, 4
        jmp    SHORT loc_1_1146
loc_1_1111:
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_1_1128
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1128
        push   ebx
        call   sub_1_00B6
        add    esp, 4
        jmp    SHORT loc_1_1146
loc_1_1128:
        mov    dword ptr [edi + 2bh], ebx
        cmp    dword ptr [esi + 8], 1
        sbb    ecx, ecx
        and    ecx, 2000h
        add    ecx, 4853h
        mov    word ptr [edi + 33h], cx
        mov    dword ptr [esi + 0ch], ebx
        mov    dword ptr [ebx], eax
loc_1_1146:
        mov    eax, ebp
        and    eax, 0ffffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1160:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        jne    SHORT loc_1_1174
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        sub    eax, eax
        ret
loc_1_1174:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [edx + 18h]
        mov    word ptr [ecx + 0ch], ax
        xor    eax, eax
        mov    ax, word ptr [edx + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11A0:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        sub    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 8], ecx
        cmp    ecx, eax
        je     NEAR PTR loc_1_1269
        mov    eax, ecx
        mov    dword ptr [esp + 0ch], eax
        mov    al, byte ptr [esi + 1ah]
        and    eax, 0ffff00ffh
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esp + 10h], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 14h]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 8]
        db  068h, 060h, 011h, 000h, 000h
        push   eax
        push   1
        call   sub_1_034E
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1269
        db  080h, 03Dh, 00Ch, 018h, 000h, 000h, 000h
        je     SHORT loc_1_1267
        db  080h, 03Dh, 010h, 018h, 000h, 000h, 000h
        je     SHORT loc_1_1233
        db  08Bh, 00Dh, 078h, 003h, 000h, 000h
        cmp    dword ptr [esi + 8], ecx
        jne    SHORT loc_1_124F
        mov    ecx, dword ptr [esi + 0ch]
        add    ecx, 28h
        cmp    byte ptr [ecx], 49h
        jne    SHORT loc_1_124F
        inc    ecx
        cmp    byte ptr [ecx], 4eh
        jne    SHORT loc_1_124F
        cmp    byte ptr [ecx + 1], 49h
        jne    SHORT loc_1_124F
loc_1_1233:
        db  080h, 03Dh, 084h, 003h, 000h, 000h, 001h
        jne    SHORT loc_1_124F
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        mov    edx, dword ptr [ecx + 4]
        db  08Bh, 00Dh, 095h, 003h, 000h, 000h
        or     byte ptr [edx + ecx], 2
loc_1_124F:
        db  08Ah, 00Dh, 018h, 018h, 000h, 000h
        cmp    cl, 2
        jne    SHORT loc_1_1269
        inc    cl
        db  088h, 00Dh, 018h, 018h, 000h, 000h
        pop    esi
        add    esp, 10h
        ret
loc_1_1267:
        sub    eax, eax
loc_1_1269:
        pop    esi
        add    esp, 10h
        ret
        db  08Bh, 0FFh
sub_1_1270:
        push   esi
        db  0A1h, 074h, 003h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 4]
        xor    ecx, ecx
        db  0A1h, 0B1h, 003h, 000h, 000h
        mov    ax, word ptr [esi + eax]
        and    eax, 0ffffh
        mov    cx, word ptr [edx + 14h]
        shl    eax, 10h
        add    eax, ecx
        db  08Bh, 00Dh, 0C9h, 003h, 000h, 000h
        cmp    word ptr [esi + ecx], 5d04h
        je     SHORT loc_1_12A6
        sub    eax, eax
loc_1_12A6:
        push   edx
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 8
        push   eax
        call   sub_1_0366
        add    esp, 0ch
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12C0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esi + 0ch]
        cmp    edx, eax
        jne    SHORT loc_1_12F0
        or     ecx, ecx
        jne    SHORT loc_1_12EA
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        push   esi
        call   sub_1_00B6
        add    esp, 4
        pop    esi
        ret
loc_1_12EA:
        mov    dword ptr [esi + 0ch], ecx
        pop    esi
        ret
        db  090h
loc_1_12F0:
        cmp    dword ptr [edx], eax
        je     SHORT loc_1_12FA
loc_1_12F4:
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx], eax
        jne    SHORT loc_1_12F4
loc_1_12FA:
        mov    eax, dword ptr [edx]
        pop    esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1310:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 000h, 018h, 000h, 000h
        db  03Dh, 000h, 018h, 000h, 000h
        je     SHORT loc_1_1370
        mov    si, word ptr [esp + 14h]
        mov    di, word ptr [esp + 18h]
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
loc_1_1330:
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_1_1366
loc_1_1339:
        mov    ebp, dword ptr [eax]
        cmp    word ptr [eax + 14h], si
        jne    SHORT loc_1_1360
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_1360
        or     di, di
        je     SHORT loc_1_1351
        cmp    word ptr [eax + 16h], di
        jne    SHORT loc_1_1360
loc_1_1351:
        push   eax
        call   sub_1_0DC0
        add    esp, 4
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
loc_1_1360:
        mov    eax, ebp
        or     ebp, ebp
        jne    SHORT loc_1_1339
loc_1_1366:
        mov    eax, ebx
        db  081h, 0FBh, 000h, 018h, 000h, 000h
        jne    SHORT loc_1_1330
loc_1_1370:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1380:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, 1
        mov    cl, byte ptr [esp + 0ch]
        shl    al, cl
        or     byte ptr [esi + 1bh], al
        mov    eax, dword ptr [esi + 8]
        db  039h, 005h, 074h, 003h, 000h, 000h
        jne    SHORT loc_1_13A5
        push   esi
        call   sub_1_13C0
        pop    esi
        ret    8
loc_1_13A5:
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_1_13B5
        push   eax
        push   esi
        call   sub_1_0086
        mov    dword ptr [esi + 10h], eax
loc_1_13B5:
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13C0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0ch]
        mov    dl, byte ptr [esi + 1bh]
        test   dl, 1
        je     SHORT loc_1_13DB
        mov    eax, dword ptr [ecx + 12h]
        mov    dword ptr [edi + 0dh], eax
loc_1_13DB:
        test   dl, 4
        je     SHORT loc_1_1410
        mov    eax, dword ptr [ecx + 16h]
        mov    dword ptr [edi + 11h], eax
        db  0A0h, 084h, 003h, 000h, 000h
        cmp    al, 1
        mov    bx, word ptr [ecx + 10h]
        mov    word ptr [edi + 0bh], bx
        jne    SHORT loc_1_1403
        mov    word ptr [edi + 35h], bx
        mov    word ptr [edi + 19h], 0
        jmp    SHORT loc_1_1415
loc_1_1403:
        mov    word ptr [edi + 1bh], bx
        mov    word ptr [edi + 19h], 0
        jmp    SHORT loc_1_1415
        db  090h
loc_1_1410:
        db  0A0h, 084h, 003h, 000h, 000h
loc_1_1415:
        test   dl, 2
        je     SHORT loc_1_1455
        mov    ebx, dword ptr [ecx + 16h]
        mov    dword ptr [edi + 11h], ebx
        mov    bx, word ptr [ecx + 10h]
        mov    word ptr [edi + 0bh], bx
        cmp    al, 1
        jne    SHORT loc_1_1440
        cmp    word ptr [edi + 35h], 0
        jne    SHORT loc_1_1455
        mov    word ptr [edi + 19h], 0
        mov    word ptr [edi + 35h], bx
        jmp    SHORT loc_1_1455
        db  090h
loc_1_1440:
        cmp    word ptr [edi + 1bh], 0
        jne    SHORT loc_1_1455
        mov    word ptr [edi + 19h], 0
        mov    bx, word ptr [edi + 0bh]
        mov    word ptr [edi + 1bh], bx
loc_1_1455:
        test   dl, 8
        je     SHORT loc_1_1482
        mov    ebx, dword ptr [ecx + 12h]
        mov    dword ptr [edi + 0dh], ebx
        mov    ebx, dword ptr [ecx + 16h]
        mov    dword ptr [edi + 11h], ebx
        cmp    al, 1
        mov    ax, word ptr [ecx + 10h]
        mov    word ptr [edi + 0bh], ax
        jne    SHORT loc_1_1478
        mov    word ptr [edi + 35h], ax
        jmp    SHORT loc_1_147C
loc_1_1478:
        mov    word ptr [edi + 1bh], ax
loc_1_147C:
        mov    word ptr [edi + 19h], 0
loc_1_1482:
        test   dl, 10h
        je     SHORT loc_1_1499
        cmp    dword ptr [ecx + 8], 0
        mov    word ptr [edi + 33h], 6853h
        je     SHORT loc_1_1499
        mov    word ptr [edi + 33h], 4853h
loc_1_1499:
        mov    byte ptr [esi + 1bh], 0
        mov    dword ptr [esi + 10h], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 074h, 003h, 000h, 000h
        mov    esi, dword ptr [eax + 4]
        db  0A1h, 0BDh, 003h, 000h, 000h
        mov    eax, dword ptr [esi + eax + 4]
        push   eax
        call   sub_1_1580
        lea    edi, [esi + eax]
        mov    edi, edi
loc_1_14D0:
        mov    si, word ptr [edi + 4]
        lea    ebx, [edi + 6]
        mov    ax, si
        dec    si
        or     ax, ax
        je     SHORT loc_1_1508
        sub    ebp, ebp
loc_1_14E3:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        cmp    eax, ebp
        db  0A1h, 0D4h, 003h, 000h, 000h
        je     SHORT loc_1_14FC
        mov    word ptr [ebx + 33h], bp
        mov    dword ptr [ebx + 2bh], ebp
loc_1_14FC:
        add    ebx, eax
        mov    ax, si
        dec    si
        cmp    ax, bp
        jne    SHORT loc_1_14E3
loc_1_1508:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1580
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        mov    edi, dword ptr [ecx + 4]
        add    edi, eax
        cmp    word ptr [edi], 0ffffh
        jne    SHORT loc_1_14D0
        mov    esi, dword ptr [ecx + 4]
        db  0A1h, 0BDh, 003h, 000h, 000h
        mov    eax, dword ptr [esi + eax + 1ah]
        push   eax
        call   sub_1_1580
        add    eax, esi
        lea    esi, [eax + 6]
        mov    di, word ptr [eax + 4]
        mov    ax, di
        dec    di
        or     ax, ax
        je     SHORT loc_1_156E
        sub    ebx, ebx
loc_1_1549:
        push   esi
        call   sub_1_17B0
        add    esp, 4
        cmp    eax, ebx
        db  0A1h, 0D4h, 003h, 000h, 000h
        je     SHORT loc_1_1562
        mov    word ptr [esi + 33h], bx
        mov    dword ptr [esi + 2bh], ebx
loc_1_1562:
        add    esi, eax
        mov    ax, di
        dec    di
        cmp    ax, bx
        jne    SHORT loc_1_1549
loc_1_156E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1580:
        xor    eax, eax
        mov    ax, word ptr [esp + 6]
        shl    eax, 4
        mov    ecx, dword ptr [esp + 4]
        and    ecx, 0ffffh
        add    eax, ecx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15A0:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp], eax
        shr    eax, 4
        and    word ptr [esp], 0fh
        mov    word ptr [esp + 2], ax
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15D0:
        sub    esp, 4
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    cx, word ptr [eax + 38h]
        mov    word ptr [esp + 2], cx
        mov    ax, word ptr [eax]
        mov    word ptr [esp], ax
        mov    eax, dword ptr [esp]
        push   eax
        call   sub_1_1580
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret
sub_1_1600:
        sub    esp, 4
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    cx, word ptr [eax + 3ch]
        mov    word ptr [esp + 2], cx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esp], ax
        mov    eax, dword ptr [esp]
        push   eax
        call   sub_1_1580
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1640:
        sub    esp, 4
        db  0A1h, 07Ch, 003h, 000h, 000h
        mov    cx, word ptr [eax + 3ch]
        mov    word ptr [esp + 2], cx
        mov    ax, word ptr [eax + 14h]
        mov    word ptr [esp], ax
        mov    eax, dword ptr [esp]
        push   eax
        call   sub_1_1580
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1680:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 000h, 018h, 000h, 000h
        db  081h, 0FEh, 000h, 018h, 000h, 000h
        je     SHORT loc_1_16E7
        mov    edi, dword ptr [esp + 10h]
loc_1_1699:
        mov    ebp, dword ptr [esi + 0ch]
        mov    esi, dword ptr [esi]
        or     ebp, ebp
        je     SHORT loc_1_16DF
        mov    ebx, 0ffh
loc_1_16A7:
        mov    ebp, dword ptr [ebp]
        db  0A1h, 074h, 003h, 000h, 000h
        cmp    dword ptr [edi + 8], eax
        jne    SHORT loc_1_16D9
        push   edi
        call   sub_1_12C0
        add    esp, 4
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        call   sub_1_025D
        or     eax, eax
        je     SHORT loc_1_16D0
        mov    byte ptr [edi + 1bh], bl
        jmp    SHORT loc_1_16D9
        db  090h
loc_1_16D0:
        push   edi
        call   sub_1_00B6
        add    esp, 4
loc_1_16D9:
        mov    edi, ebp
        or     ebp, ebp
        jne    SHORT loc_1_16A7
loc_1_16DF:
        db  081h, 0FEh, 000h, 018h, 000h, 000h
        jne    SHORT loc_1_1699
loc_1_16E7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_16F0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1729
        mov    eax, dword ptr [esi + 2bh]
        cmp    byte ptr [eax + 1bh], 0ffh
        jne    SHORT loc_1_1720
        mov    word ptr [esi + 33h], 0
        mov    dword ptr [esi + 2bh], 0
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1720:
        push   1
        call   sub_1_017F
        pop    esi
        ret
loc_1_1729:
        push   1ch
        call   sub_1_00A4
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1740:
        sub    esp, 4
        push   esi
        db  08Bh, 035h, 074h, 003h, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        db  0A1h, 08Dh, 003h, 000h, 000h
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [esp + 6], ax
        mov    word ptr [esp + 4], 0
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_1580
        add    eax, dword ptr [esi + 4]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 32h]
        mov    edx, dword ptr [esp + 0ch]
        cmp    ecx, edx
        jle    SHORT loc_1_17A1
        mov    eax, dword ptr [eax + 34h]
        mov    dword ptr [esp + 4], eax
        add    word ptr [esp + 4], dx
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_1580
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        pop    esi
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret
loc_1_17A1:
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_17B0:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 33h]
        cmp    ax, 6853h
        je     SHORT loc_1_17C7
        cmp    ax, 4853h
        je     SHORT loc_1_17C7
        sub    eax, eax
        ret
loc_1_17C7:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h
sub_1_17D0:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        call   sub_1_0F50
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_17FB
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_1_17FB
        mov    ecx, dword ptr [esp + 8]
        mov    edi, edi
loc_1_17F0:
        cmp    dword ptr [eax + 8], ecx
        je     SHORT loc_1_17FD
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_17F0
loc_1_17FB:
        sub    eax, eax
loc_1_17FD:
        ret
        db  08Bh, 0FFh, 000h, 018h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_180C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1810:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1814:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1818:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1824:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1828:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
