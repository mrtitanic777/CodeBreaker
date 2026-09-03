; socketsv_lcode_01.asm -- SOCKETSV object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 053h, 04Fh, 043h, 04Bh
        db  045h, 054h, 053h, 056h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  055h, 08Bh, 0ECh, 083h, 0ECh, 004h, 08Ah, 045h, 00Ch, 002h, 045h, 010h, 08Bh, 055h, 008h, 09Ch
        db  0FAh, 0EEh, 042h, 0ECh, 09Dh, 088h, 045h, 0FFh, 08Ah, 045h, 0FFh, 08Bh, 0E5h, 05Dh, 0C3h
sub_1_00A7:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 0ch]
        add    al, byte ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        pushfd
        cli
        out    dx, al
        inc    edx
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
        popfd
        mov    al, byte ptr [ebp + 14h]
        pop    ebp
        ret
sub_1_00C1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 0B5h, 0FFh, 0FFh, 0FFh
        movzx  si, al
        add    esp, 0ch
        mov    al, byte ptr [ebp + 10h]
        inc    al
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 09Dh, 0FFh, 0FFh, 0FFh
        movzx  ax, al
        shl    ax, 8
        add    esp, 0ch
        or     si, ax
        mov    ax, si
        pop    esi
        pop    ebp
        ret
sub_1_00FF:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ebp + 14h]
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
        mov    ax, si
        add    esp, 10h
        shr    ax, 8
        push   eax
        mov    al, byte ptr [ebp + 10h]
        inc    al
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_00A7
        mov    ax, si
        add    esp, 10h
        pop    esi
        pop    ebp
        ret
sub_1_013B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 4], 5353534dh
        je     SHORT loc_1_015C
        mov    byte ptr [edi + 1dh], 1
        jmp    NEAR PTR loc_1_0211
loc_1_015C:
        xor    ebx, ebx
        mov    word ptr [edi + 18h], bx
        cmp    byte ptr [esi + 38h], bl
        jbe    NEAR PTR loc_1_020D
loc_1_016B:
        movzx  ecx, bl
        movzx  edx, bl
        push   4
        mov    al, byte ptr [ecx + esi + 28h]
        push   eax
        push   dword ptr [esi + edx*4 + 18h]
        db  0E8h, 007h, 0FFh, 0FFh, 0FFh
        movzx  ecx, bl
        movzx  edx, bl
        add    esp, 0ch
        and    al, 0fh
        or     byte ptr [ecx + esi + 2ch], al
        push   5
        mov    cl, byte ptr [edx + esi + 28h]
        movzx  edx, bl
        push   ecx
        push   dword ptr [esi + edx*4 + 18h]
        db  0E8h, 0E5h, 0FEh, 0FFh, 0FFh
        movzx  ecx, bl
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        mov    dl, byte ptr [ecx + esi + 2ch]
        and    dl, al
        je     SHORT loc_1_0202
        mov    al, 1
        mov    cl, bl
        shl    al, cl
        movzx  ax, al
        or     word ptr [edi + 18h], ax
        test   dl, 3
        je     SHORT loc_1_0202
        movzx  ecx, bl
        mov    al, byte ptr [ebp - 1]
        and    al, 0fch
        push   eax
        push   5
        mov    al, byte ptr [ecx + esi + 28h]
        movzx  ecx, bl
        push   eax
        push   dword ptr [esi + ecx*4 + 18h]
        call   sub_1_00A7
        movzx  ecx, bl
        add    esp, 10h
        mov    al, byte ptr [ecx + esi + 28h]
        movzx  ecx, bl
        push   dword ptr [ebp - 1]
        push   5
        push   eax
        push   dword ptr [esi + ecx*4 + 18h]
        call   sub_1_00A7
        add    esp, 10h
loc_1_0202:
        inc    bl
        cmp    byte ptr [esi + 38h], bl
        ja     NEAR PTR loc_1_016B
loc_1_020D:
        mov    byte ptr [edi + 1dh], 0
loc_1_0211:
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_021D
        and    byte ptr [edi + 20h], 0feh
        jmp    SHORT loc_1_0221
loc_1_021D:
        or     byte ptr [edi + 20h], 1
loc_1_0221:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
SOCKETSV_Control:
        cmp    eax, 22h
        jne    SHORT loc_1_0236
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0234:
        stc
        ret
loc_1_0236:
        cmp    eax, 1bh
        jne    SHORT loc_1_0244
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0240:
        cmp    eax, 1
        ret
loc_1_0244:
        clc
        ret
loc_1_0246:
        pushfd
        pushad
        mov    ebp, esp
        cmp    ah, 9eh
        jne    SHORT loc_1_025A
        push   ebp
        push   esi
        call   sub_1_013B
        pop    ecx
        pop    ecx
        jmp    SHORT loc_1_02B5
loc_1_025A:
        cmp    dword ptr [esi + 4], 5353534dh
        je     SHORT loc_1_026D
        mov    byte ptr [ebp + 1dh], 1
loc_1_0267:
        or     dword ptr [ebp + 20h], 1
        jmp    SHORT loc_1_02B5
loc_1_026D:
        bts    dword ptr [esi + 8], 1fh
        jae    SHORT loc_1_027A
        mov    byte ptr [ebp + 1dh], 18h
        jmp    SHORT loc_1_0267
loc_1_027A:
        sub    ah, 83h
        jae    SHORT loc_1_0289
loc_1_027F:
        mov    byte ptr [ebp + 1dh], 15h
        or     dword ptr [ebp + 20h], 1
        jmp    SHORT loc_1_02AE
loc_1_0289:
        cmp    ah, 0eh
        jbe    SHORT loc_1_029E
        cmp    ah, 1ah
        jne    SHORT loc_1_027F
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_029A:
        pop    ecx
        pop    ecx
        jmp    SHORT loc_1_02AE
loc_1_029E:
        movzx  eax, ah
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        push   ebp
        push   esi
        call   eax
        pop    ecx
        pop    ecx
loc_1_02AE:
        and    dword ptr [esi + 8], 7fffffffh
loc_1_02B5:
        popad
        popfd
        ret
_LTEXT ends
        end
