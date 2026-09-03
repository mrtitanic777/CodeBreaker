; lptenum_lcode_01.asm -- LPTENUM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

LPTENUM_Control:
        cmp    eax, 1bh
        jne    SHORT loc_1_000E
        db  0E8h, 0C6h, 003h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_000E:
        cmp    eax, 1ch
        jne    SHORT loc_1_001C
        db  0E8h, 0BEh, 003h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_001C:
        cmp    eax, 22h
        jne    SHORT loc_1_002A
        push   edx
        push   ebx
        db  0E8h, 0B4h, 003h, 0FFh, 0FFh
        stc
        ret
loc_1_002A:
        clc
        ret
        db  000h, 000h, 000h, 000h
sub_1_0030:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movzx  edi, word ptr [esp + 14h]
        lea    esi, [edi + 1]
        add    edi, 2
        push   edi
        call   sub_1_0440
        add    esp, 4
        and    al, 40h
        mov    bl, al
        or     al, 0ch
        mov    byte ptr [esp + 0ch], al
        push   dword ptr [esp + 0ch]
        push   edi
        call   sub_1_04DE
        add    esp, 8
        cmp    dword ptr [esp + 18h], 0
        push   23h
        je     SHORT loc_1_00A6
        push   10h
        push   50h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_00B7
loc_1_007B:
        or     bl, 8
        push   ebx
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   dword ptr [esp + 0ch]
        push   edi
        call   sub_1_04DE
        add    esp, 8
loc_1_009F:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_00A6:
        push   0
        push   50h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_007B
loc_1_00B7:
        mov    al, bl
        or     al, 0eh
        push   eax
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   23h
        push   40h
        push   40h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_007B
        push   dword ptr [esp + 0ch]
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   23h
        push   80h
        push   80h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        jmp    SHORT loc_1_009F
sub_1_00FC:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  edi, word ptr [esp + 1ch]
        lea    ebp, [edi + 1]
        lea    esi, [edi + 2]
        push   esi
        call   sub_1_0440
        add    esp, 4
        and    al, 40h
        mov    bl, al
        or     al, 0ch
        mov    byte ptr [esp + 14h], al
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   dword ptr [esp + 20h]
        push   edi
        call   sub_1_04DE
        add    esp, 8
        mov    al, bl
        or     al, 6
        push   eax
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   23h
        push   38h
        push   78h
        push   ebp
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0173
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        mov    eax, 100000h
        jmp    NEAR PTR loc_1_0233
loc_1_0173:
        mov    al, bl
        or     al, 7
        mov    byte ptr [esp + 10h], al
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        or     bl, 4
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   ebx
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   ebx
        push   esi
        call   sub_1_04DE
        movzx  eax, byte ptr [esp + 28h]
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_01CF
        cmp    eax, 4
        je     SHORT loc_1_0209
        cmp    eax, 10h
        je     SHORT loc_1_01E9
        cmp    eax, 14h
        je     SHORT loc_1_01E9
loc_1_01C4:
        cmp    byte ptr [esp + 20h], 0
        jne    SHORT loc_1_0223
        push   0
        jmp    SHORT loc_1_0225
loc_1_01CF:
        push   23h
        push   40h
        push   50h
        push   ebp
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_01E9
        mov    eax, 10000h
        jmp    SHORT loc_1_0233
loc_1_01E9:
        push   23h
        push   0d8h
        push   0f8h
        push   ebp
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0209
        mov    eax, 10000h
        jmp    SHORT loc_1_0233
loc_1_0209:
        push   23h
        push   50h
        push   50h
        push   ebp
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_01C4
        mov    eax, 10000h
        jmp    SHORT loc_1_0233
loc_1_0223:
        push   1
loc_1_0225:
        push   dword ptr [esp + 20h]
        call   sub_1_0030
        add    esp, 8
        sub    eax, eax
loc_1_0233:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_023B:
        push   ebp
        mov    ebp, esp
        push   eax
        mov    al, byte ptr [ebp + 8]
        rcl    al, 1
        jb     SHORT loc_1_024D
        bts    ax, 7
        jmp    SHORT loc_1_0252
loc_1_024D:
        btr    ax, 7
loc_1_0252:
        shr    al, 4
        mov    ah, byte ptr [ebp + 0ch]
        rcl    ah, 1
        jb     SHORT loc_1_0263
        bts    ax, 0fh
        jmp    SHORT loc_1_0268
loc_1_0263:
        btr    ax, 0fh
loc_1_0268:
        and    ah, 0f0h
        or     al, ah
        mov    byte ptr [ebp + 0ch], al
        pop    eax
        mov    al, byte ptr [ebp + 0ch]
        pop    ebp
        ret
sub_1_0276:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  ebp, word ptr [esp + 1ch]
        sub    edi, edi
        lea    esi, [ebp + 1]
        add    ebp, 2
        push   ebp
        call   sub_1_0440
        mov    byte ptr [esp + 15h], al
        add    esp, 4
        test   al, 2
        je     SHORT loc_1_02CC
        push   esi
        call   sub_1_03D0
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_1_02B5
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        jmp    NEAR PTR loc_1_03B7
loc_1_02B5:
        and    byte ptr [esp + 11h], 0f0h
        mov    al, byte ptr [esp + 11h]
        or     al, 4
        push   eax
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        jmp    SHORT loc_1_02D1
loc_1_02CC:
        and    byte ptr [esp + 11h], 0f0h
loc_1_02D1:
        mov    al, byte ptr [esp + 11h]
        or     al, 6
        mov    byte ptr [esp + 14h], al
loc_1_02DB:
        push   dword ptr [esp + 14h]
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   23h
        push   0
        push   40h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_03AC
        push   esi
        call   sub_1_0440
        mov    byte ptr [esp + 16h], al
        mov    bl, byte ptr [esp + 15h]
        add    esp, 4
        or     bl, 4
        push   ebx
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   23h
        push   40h
        push   40h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_03AC
        push   dword ptr [esp + 14h]
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   23h
        push   0
        push   40h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_03AC
        push   esi
        call   sub_1_0440
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        push   ebx
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   dword ptr [esp + 13h]
        inc    edi
        push   dword ptr [esp + 16h]
        call   sub_1_023B
        mov    ecx, dword ptr [esp + 28h]
        add    esp, 8
        mov    byte ptr [ecx], al
        inc    dword ptr [esp + 20h]
        push   23h
        push   40h
        push   40h
        push   esi
        call   sub_1_03E8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_03AC
        push   esi
        call   sub_1_03D0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_03AC
        cmp    edi, dword ptr [esp + 24h]
        jb     NEAR PTR loc_1_02DB
loc_1_03AC:
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], edi
        mov    eax, 1
loc_1_03B7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
sub_1_03D0:
        push   dword ptr [esp + 4]
        call   sub_1_0440
        add    esp, 4
        test   al, 8
        mov    eax, 1
        jne    SHORT loc_1_03E7
        sub    eax, eax
loc_1_03E7:
        ret
sub_1_03E8:
        push   ebx
        push   esi
        push   edi
        db  068h, 0E0h, 005h, 000h, 000h
        db  0E8h, 00Bh, 0FCh, 0FEh, 0FFh
        mov    esi, dword ptr [esp + 14h]
        mov    bl, byte ptr [esp + 18h]
        mov    edi, dword ptr [esp + 20h]
        add    esp, 4
loc_1_0404:
        db  068h, 0E4h, 005h, 000h, 000h
        db  0E8h, 0F2h, 0FBh, 0FEh, 0FFh
        add    esp, 4
        push   esi
        call   sub_1_0440
        add    esp, 4
        and    al, bl
        cmp    al, byte ptr [esp + 18h]
        je     SHORT loc_1_0437
        db  0A1h, 0E4h, 005h, 000h, 000h
        db  02Bh, 005h, 0E0h, 005h, 000h, 000h
        cmp    eax, edi
        jl     SHORT loc_1_0404
        sub    eax, eax
loc_1_0433:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0437:
        mov    eax, 1
        jmp    SHORT loc_1_0433
        db  000h, 000h
sub_1_0440:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_0461:
        in     al, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        loop   loc_1_0461
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     ax, dx
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
loc_1_0490:
        in     ax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_0490
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     eax, dx
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 2
loc_1_04C4:
        in     eax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        shr    eax, 10h
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_04C4
        pop    ebx
        pop    esi
        leave
        ret
sub_1_04DE:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_0502:
        mov    al, byte ptr [ebx + esi]
        out    dx, al
        inc    ebx
        loop   loc_1_0502
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        out    dx, ax
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
loc_1_0535:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        out    dx, ax
        loop   loc_1_0535
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        out    dx, eax
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 2
loc_1_056C:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        shl    eax, 10h
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        ror    eax, 10h
        out    dx, eax
        loop   loc_1_056C
        pop    ebx
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 04Ch, 050h, 054h, 045h
        db  04Eh, 055h, 04Dh, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
