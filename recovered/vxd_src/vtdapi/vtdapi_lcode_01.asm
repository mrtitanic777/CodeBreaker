; vtdapi_lcode_01.asm -- VTDAPI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  08Bh, 035h, 0D8h, 002h, 000h, 000h
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0031
loc_1_0010:
        mov    edx, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    ebx, dword ptr [edx + 1ch]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_002D
        mov    cx, word ptr [ebp + 18h]
        cmp    cx, word ptr [edx + 22h]
        je     SHORT loc_1_0033
loc_1_002D:
        or     eax, eax
        jne    SHORT loc_1_0010
loc_1_0031:
        popfd
        ret
loc_1_0033:
        mov    ecx, dword ptr [edx + 24h]
        push   eax
        mov    eax, edx
        push   esi
        db  068h, 045h, 000h, 000h, 000h
        pushfd
        db  0E9h, 0B7h, 001h, 0FFh, 0FFh
loc_1_0045:
        pop    esi
        pop    eax
        jmp    SHORT loc_1_002D
loc_1_0049:
        pushfd
        cli
        xor    eax, eax
        mov    dword ptr [edx + 14h], eax
        test   dword ptr [edx + 2ch], 100h
        jne    SHORT loc_1_00B8
        test   dword ptr [edx + 2ch], 1
        je     SHORT loc_1_0094
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [edx]
        mov    ebx, dword ptr [edx + 8]
        mov    edi, edx
        xor    edx, edx
        idiv   ebx
        mov    ecx, eax
        mov    eax, ebx
        sub    eax, edx
        mov    edx, edi
        db  0BEh, 049h, 000h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        mov    dword ptr [edx + 14h], esi
        mov    eax, ecx
        sub    eax, dword ptr [edx + 4]
        add    dword ptr [edx + 28h], eax
        mov    dword ptr [edx + 4], ecx
loc_1_0094:
        bts    dword ptr [edx + 2ch], 9
        jb     SHORT loc_1_00B8
        popfd
        db  0BEh, 0BAh, 000h, 000h, 000h
        mov    ebx, dword ptr [edx + 1ch]
        mov    eax, 1000h
        mov    ecx, 1
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [edx + 18h], esi
        ret
loc_1_00B8:
        popfd
        ret
loc_1_00BA:
        pushfd
        cli
        xor    eax, eax
        mov    dword ptr [edx + 18h], eax
        test   dword ptr [edx + 2ch], 100h
        jne    NEAR PTR loc_1_0228
        test   dword ptr [edx + 2ch], 10h
        je     NEAR PTR loc_1_015D
        test   dword ptr [edx + 2ch], 1
        jne    SHORT loc_1_00EB
        or     dword ptr [edx + 2ch], 100h
loc_1_00EB:
        test   dword ptr [edx + 2ch], 60h
        je     SHORT loc_1_0119
        mov    eax, dword ptr [edx + 20h]
        or     eax, eax
        je     SHORT loc_1_0119
        push   edx
        test   dword ptr [edx + 2ch], 40h
        je     SHORT loc_1_010D
        db  0CDh, 020h, 00Fh, 000h, 02Ah, 000h
        jmp    SHORT loc_1_0113
loc_1_010D:
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
loc_1_0113:
        pop    edx
        jmp    NEAR PTR loc_1_0228
loc_1_0119:
        mov    edi, dword ptr [edx + 30h]
        db  0CDh, 020h, 00Ch, 001h, 001h, 000h
        jb     SHORT loc_1_0151
loc_1_0124:
        push   edx
        push   dword ptr [edx + 30h]
        push   dword ptr [edx + 10h]
        push   dword ptr [edx + 20h]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        pop    edx
        je     SHORT loc_1_0151
        test   dword ptr [edx + 2ch], 100h
        jne    NEAR PTR loc_1_0228
        dec    dword ptr [edx + 28h]
        jne    SHORT loc_1_0124
        jmp    NEAR PTR loc_1_0228
loc_1_0151:
        mov    dword ptr [edx + 28h], 0
        jmp    NEAR PTR loc_1_0228
loc_1_015D:
        movzx  edi, word ptr [edx + 22h]
        lar    ax, di
        jne    NEAR PTR loc_1_0238
        test   eax, 800h
        je     NEAR PTR loc_1_0238
        popfd
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        and    dword ptr [ebp + 2ch], 0fffffaffh
        xor    eax, eax
        mov    dword ptr [ebp + 8], eax
loc_1_0198:
        mov    dword ptr [ebp + 1ch], edx
        test   dword ptr [edx + 2ch], 2
        jne    SHORT loc_1_01DB
        push   edx
        movzx  esi, word ptr [ebp + 34h]
        push   0
        push   esi
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        movzx  esi, word ptr [ebp + 30h]
        add    esi, eax
        mov    eax, dword ptr [edx + 24h]
        mov    word ptr [esi - 2], ax
        xor    eax, eax
        mov    word ptr [esi - 4], ax
        mov    ecx, dword ptr [edx + 10h]
        mov    dword ptr [esi - 8], ecx
        mov    dword ptr [esi - 0ch], eax
        mov    dword ptr [esi - 10h], eax
        sub    word ptr [ebp + 30h], 10h
loc_1_01DB:
        mov    eax, edx
        mov    cx, word ptr [eax + 22h]
        movzx  edx, word ptr [eax + 20h]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        mov    edx, eax
        test   dword ptr [edx + 2ch], 1
        jne    SHORT loc_1_01FD
        or     dword ptr [edx + 2ch], 100h
loc_1_01FD:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        test   dword ptr [edx + 2ch], 100h
        jne    SHORT loc_1_0211
        dec    dword ptr [edx + 28h]
        jne    SHORT loc_1_0198
loc_1_0211:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pushfd
        cli
loc_1_0228:
        and    dword ptr [edx + 2ch], 0fffffdffh
        test   dword ptr [edx + 2ch], 100h
        je     SHORT loc_1_023F
loc_1_0238:
        mov    eax, edx
        db  0E9h, 0CDh, 0FFh, 0FEh, 0FFh
loc_1_023F:
        popfd
        ret
VTDAPI_Control:
        cmp    eax, 23h
        db  00Fh, 084h, 093h, 000h, 0FFh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0ADh, 0FDh, 0FEh, 0FFh
        cmp    eax, 16h
        db  00Fh, 084h, 063h, 001h, 0FFh, 0FFh
        cmp    eax, 17h
        je     SHORT loc_1_0278
        db  0F7h, 005h, 067h, 002h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_0276
        cmp    eax, 14h
        db  00Fh, 084h, 08Ah, 0FDh, 0FEh, 0FFh
loc_1_0276:
        clc
        ret
loc_1_0278:
        mov    al, 0c3h
        db  0A2h, 049h, 000h, 000h, 000h
        db  0A2h, 0BAh, 000h, 000h, 000h
        clc
        ret
        db  000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 042h, 004h, 004h, 000h, 000h, 000h, 056h, 054h, 044h, 041h
        db  050h, 049h, 020h, 020h, 000h, 000h, 000h, 080h, 041h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
