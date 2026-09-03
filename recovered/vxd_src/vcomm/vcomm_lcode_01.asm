; vcomm_lcode_01.asm -- VCOMM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VCOMM_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 22h
        db  00Fh, 084h, 017h, 004h, 0FFh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 08Ch, 000h, 0FFh, 0FFh
        clc
        ret
        db  000h, 000h, 000h
loc_1_0020:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        bts    word ptr [edx + 39h], 0
        jae    SHORT loc_1_0037
        or     dword ptr [edx + 28h], eax
        jmp    SHORT loc_1_0048
loc_1_0037:
        mov    dword ptr [edx + 28h], eax
        mov    ebx, dword ptr [edx + 2ch]
        db  0BEh, 04Bh, 000h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_1_0048:
        pop    esi
        leave
        ret
loc_1_004B:
        mov    eax, edx
        call   sub_1_01D6
        jne    SHORT loc_1_00A8
        and    byte ptr [edx + 39h], 0feh
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 20h]
        mov    ebx, dword ptr [edx + 28h]
        mov    ecx, dword ptr [edx + 14h]
        mov    dword ptr [ebp + 14h], edx
        movzx  edx, cx
        shr    ecx, 10h
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], bx
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_00A8:
        ret
        db  000h, 000h, 000h
_VCOMM_ClearCommError:
        mov    cl, 6
        jmp    SHORT sub_1_00F0
_VCOMM_GetModemStatus:
        mov    cl, 12h
        jmp    SHORT sub_1_00F0
_VCOMM_TransmitCommChar:
        mov    cl, 3
        jmp    SHORT sub_1_00F0
_VCOMM_WriteComm:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    cl, 0dh
        jmp    SHORT sub_1_00F0
_VCOMM_ReadComm:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    cl, 0eh
        jmp    SHORT sub_1_00F0
_VCOMM_GetCommEventMask:
        mov    cl, 0ch
        jmp    SHORT sub_1_00F0
_VCOMM_EscapeCommFunction:
        mov    cl, 9
        jmp    SHORT sub_1_00F0
_VCOMM_GetCommQueueStatus:
        mov    cl, 5
        jmp    SHORT sub_1_00F0
_VCOMM_PurgeComm:
        mov    cl, 0ah
        jmp    SHORT sub_1_00F0
_VCOMM_GetLastError:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0ch]
        ret
_VCOMM_SetCommState:
        mov    cl, 0
        jmp    SHORT sub_1_00F0
sub_1_00F0:
        mov    eax, dword ptr [esp + 4]
        call   sub_1_01D6
        jne    SHORT loc_1_0114
        movzx  ecx, cl
        cmp    dword ptr [eax + 8], ecx
        jbe    SHORT loc_1_010D
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [edx + ecx*4]
        jecxz  loc_1_010D
        jmp    ecx
loc_1_010D:
        mov    dword ptr [eax + 0ch], 0fffffffah
loc_1_0114:
        xor    eax, eax
        ret
        db  0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0C3h
sub_1_0124:
        push   esi
        push   edi
        push   ebx
        xor    esi, esi
        mov    edi, dword ptr [esp + 14h]
        xchg   dword ptr [edi + 30h], esi
        test   esi, esi
        je     NEAR PTR loc_1_01B9
        xor    esi, esi
        xchg   dword ptr [edi + 20h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_1_0155
        xor    esi, esi
        xchg   dword ptr [edi + 1ch], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_0155:
        mov    ebx, dword ptr [edi + 28h]
        call   sub_1_01C5
        jne    SHORT loc_1_0190
        mov    edx, dword ptr [esp + 10h]
        mov    eax, 400000h
        mov    ecx, 100h
        db  0BEh, 0AAh, 00Ah, 000h, 000h
        cmp    dword ptr [esp + 18h], 1
        je     SHORT loc_1_017E
        db  0BEh, 00Dh, 00Ah, 000h, 000h
loc_1_017E:
        test   byte ptr [edx + 39h], 10h
        je     SHORT loc_1_0188
        call   esi
        jmp    SHORT loc_1_01B9
loc_1_0188:
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_01B9
loc_1_0190:
        mov    edx, dword ptr [esp + 10h]
        mov    esi, dword ptr [edx + 80h]
        cmp    dword ptr [esp + 18h], 1
        je     SHORT loc_1_01A7
        mov    esi, dword ptr [edx + 84h]
loc_1_01A7:
        test   esi, esi
        je     SHORT loc_1_01B9
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_1_01B9
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_1_01B9:
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_1_01BD:
        cmp    dword ptr [edi + 64h], 534d5446h
        ret
sub_1_01C5:
        cmp    dword ptr [ebx + 0ch], 42434854h
        jne    SHORT loc_1_01D1
        test   byte ptr [ebx], 10h
loc_1_01D1:
        ret
sub_1_01D2:
        mov    dword ptr [ecx + 4], eax
        ret
sub_1_01D6:
        cmp    dword ptr [eax + 64h], 534d5446h
loc_1_01DD:
        ret
loc_1_01DE:
        test   esp, esp
        ret
        db  000h, 000h, 000h
loc_1_01E4:
        push   edx
        mov    eax, dword ptr [edx + 10h]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        pop    edx
        mov    ecx, dword ptr [edx + 2ch]
        mov    eax, dword ptr [edx + 0ch]
        call   sub_1_01D2
        ret
sub_1_01FB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        btr    word ptr [edx + 39h], 0
        jae    SHORT loc_1_0237
        cmp    dword ptr [edx + 64h], 534d5446h
        je     SHORT loc_1_021F
        mov    eax, dword ptr [edx + 28h]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        jmp    SHORT loc_1_0237
loc_1_021F:
        mov    ebx, dword ptr [edx + 2ch]
        mov    eax, 400000h
        mov    ecx, 100h
        db  0BEh, 03Bh, 002h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_0237:
        pop    ebx
        pop    esi
        leave
        ret
loc_1_023B:
        mov    esi, edx
        mov    eax, dword ptr [edx + 28h]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        push   dword ptr [esi + 24h]
        push   dword ptr [esi + 10h]
        push   esi
        db  0CDh, 020h, 011h, 000h, 02Bh, 000h
        add    esp, 0ch
        push   0
        push   0
        push   esi
        db  0CDh, 020h, 014h, 000h, 02Bh, 000h
        add    esp, 0ch
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 094h, 000h, 000h, 000h, 063h
        db  001h, 000h, 000h, 04Eh, 005h, 000h, 000h, 0ECh, 000h, 000h, 000h, 080h, 006h, 000h, 000h, 087h
        db  006h, 000h, 000h, 0B4h, 000h, 000h, 000h, 08Eh, 006h, 000h, 000h, 0DCh, 000h, 000h, 000h, 0ACh
        db  000h, 000h, 000h, 0B0h, 000h, 000h, 000h, 049h, 007h, 000h, 000h, 0D8h, 000h, 000h, 000h, 0E0h
        db  000h, 000h, 000h, 065h, 007h, 000h, 000h, 0D4h, 000h, 000h, 000h, 0B8h, 000h, 000h, 000h, 0C6h
        db  000h, 000h, 000h, 050h, 007h, 000h, 000h, 0E4h, 000h, 000h, 000h, 096h, 001h, 000h, 000h, 057h
        db  007h, 000h, 000h, 05Eh, 007h, 000h, 000h, 041h, 000h, 000h, 000h, 06Ch, 007h, 000h, 000h, 0AAh
        db  007h, 000h, 000h, 0ACh, 008h, 000h, 000h, 0ADh, 00Ch, 000h, 000h, 068h, 015h, 000h, 000h, 048h
        db  002h, 000h, 000h, 0A1h, 000h, 000h, 000h, 06Eh, 015h, 000h, 000h, 08Ch, 015h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 02Bh, 000h, 001h, 000h, 000h, 000h, 056h, 043h, 04Fh, 04Dh
        db  04Dh, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 0ACh, 001h, 000h, 000h
        db  0ACh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 002h, 000h, 000h, 023h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 0BDh, 001h, 000h, 000h, 0DDh, 001h, 000h, 000h, 0DEh, 001h, 000h, 000h
loc_1_0354:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    ah, 0ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        enter  0ch, 0
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        fmul   qword ptr [eax + eax]
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        db  000h, 0F0h
        or     al, 0
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 004h, 00Dh, 000h, 000h, 002h, 000h
        add    byte ptr [eax], al
        sbb    byte ptr [20000h], cl
        add    byte ptr [eax], al
        sub    al, 0dh
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        or     eax, 20000h
        add    byte ptr [eax], al
        push   esp
        or     eax, 20000h
        add    byte ptr [eax], al
        push   200000dh
        add    byte ptr [eax], al
        db  000h, 07Ch, 00Dh, 000h
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 090h, 00Dh, 000h, 000h, 002h
        add    byte ptr [eax], al
        db  000h, 0A4h, 00Dh, 000h, 000h, 002h, 000h
        add    byte ptr [eax], al
        mov    eax, 400000dh
        add    byte ptr [eax], al
        db  000h, 0CCh
        or     eax, 20000h
        add    byte ptr [eax], al
        db  0E0h, 00Dh
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        db  000h, 0F4h
        or     eax, 60000h
        add    byte ptr [eax], al
        or     byte ptr [esi], cl
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        sbb    al, 0eh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    esp
        push   cs
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        pop    eax
        push   cs
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        insb
        push   cs
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [esi], 0
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        db  000h, 094h, 00Eh, 000h, 000h, 002h, 000h
        add    byte ptr [eax], al
        test   al, 0eh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        mov    esp, 800000eh
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
