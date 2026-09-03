; mssndsys_pcode_03.asm -- MSSNDSYS object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 00Dh, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 00Dh, 000h
        xor    esi, esi
        db  087h, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_0060
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        push   ebx
loc_3_0045:
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_3_0054
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_3_0054:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_3_0045
        pop    ebx
loc_3_0060:
        clc
        ret
sub_3_0062:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0072:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_3_00DC
        mov    ebx, dword ptr [edi + 24h]
        or     ebx, ebx
        je     SHORT loc_3_00DC
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_00DC
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_008D:
        jb     SHORT loc_3_00DC
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0094:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_3_00C0
        mov    edi, esi
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        mov    word ptr [edi + 30h], dx
        mov    dword ptr [edi + 28h], ecx
        mov    dword ptr [edi + 2ch], esi
        mov    esi, edi
        test   word ptr [esi + 30h], 1
        jne    SHORT loc_3_00C0
        db  0CDh, 020h, 007h, 000h, 004h, 000h
loc_3_00C0:
        pop    edi
        movzx  edx, word ptr [edi + 42h]
        mov    ah, 9
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00CE:
        inc    ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00D5:
        pop    edx
        pop    ecx
        or     word ptr [edi + 4], 10h
loc_3_00DC:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
sub_3_00E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00F4:
        add    esp, 8
        or     edi, edi
        je     NEAR PTR loc_3_01D1
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, byte ptr [edi + 8]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, byte ptr [edi + 9]
        mov    dword ptr [ebp - 0ch], eax
        movzx  eax, byte ptr [edi + 0ah]
        mov    dword ptr [ebp - 10h], eax
        test   word ptr [edi + 4], 100h
        jne    SHORT loc_3_0154
        test   word ptr [edi + 4], 200h
        jne    SHORT loc_3_0149
        test   word ptr [edi + 52h], 4
        je     SHORT loc_3_0154
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0144:
        add    esp, 10h
        jmp    SHORT loc_3_0154
loc_3_0149:
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0151:
        add    esp, 4
loc_3_0154:
        mov    ebx, dword ptr [edi + 24h]
        or     ebx, ebx
        je     SHORT loc_3_01D1
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_01D1
        test   word ptr [edi + 4], 10h
        je     SHORT loc_3_01D1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0170:
        jb     SHORT loc_3_01D1
        or     byte ptr [esi + 11h], 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_017B:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_3_01A4
        mov    edi, esi
        mov    dx, word ptr [edi + 30h]
        mov    ecx, dword ptr [edi + 28h]
        mov    esi, dword ptr [edi + 2ch]
        db  0CDh, 020h, 005h, 000h, 004h, 000h
        mov    esi, edi
        test   dl, 1
        jne    SHORT loc_3_01A4
        db  0CDh, 020h, 008h, 000h, 004h, 000h
loc_3_01A4:
        pop    edi
        movzx  edx, word ptr [edi + 42h]
        mov    ecx, 9
        mov    ah, cl
        and    byte ptr [ecx + esi + 8], 0f7h
        mov    al, byte ptr [ecx + esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01BE:
        inc    ecx
        inc    ah
        mov    al, byte ptr [ecx + esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01CA:
        pop    edx
        pop    ecx
        and    word ptr [edi + 4], 0ffefh
loc_3_01D1:
        pop    esi
        pop    edi
        pop    ebx
        add    esp, 10h
        pop    ebp
        ret
_PTEXT ends
        end
