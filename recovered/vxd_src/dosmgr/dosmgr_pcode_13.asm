; dosmgr_pcode_13.asm -- DOSMGR object 13 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0EFh, 003h, 000h, 000h, 03Bh, 004h, 000h, 000h, 0DAh, 003h, 000h, 000h, 09Eh, 000h, 000h, 000h
        db  018h, 001h, 000h, 000h, 018h, 001h, 000h, 000h, 090h, 000h, 000h, 000h, 03Bh, 004h, 000h, 000h
        db  0DAh, 003h, 000h, 000h, 09Eh, 000h, 000h, 000h, 018h, 001h, 000h, 000h, 01Fh, 001h, 000h, 000h
        db  0E8h, 003h, 000h, 000h, 0E1h, 003h, 000h, 000h, 02Dh, 004h, 000h, 000h, 0E8h, 003h, 000h, 000h
loc_13_0040:
        db  0E8h, 003h, 000h, 000h, 02Dh
        add    al, 0
        db  000h, 0E1h
        add    eax, dword ptr [eax]
        db  000h, 0E8h
        add    eax, dword ptr [eax]
        db  000h, 01Fh
        add    dword ptr [eax], eax
        db  000h, 018h
        add    dword ptr [eax], eax
        db  000h, 09Eh, 000h, 000h, 000h, 0DAh
        add    eax, dword ptr [eax]
        db  000h, 03Bh
        add    al, 0
        db  000h, 090h, 000h, 000h, 000h, 018h
        add    dword ptr [eax], eax
        db  000h, 018h
        add    dword ptr [eax], eax
        db  000h, 09Eh, 000h, 000h, 000h, 0DAh
        add    eax, dword ptr [eax]
        db  000h, 03Bh
        add    al, 0
        db  000h, 0EFh
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 41h], al
        inc    edx
        inc    esi
        inc    edi
        dec    eax
        pushad
        popad
        bound  esp, qword ptr [esi + 67h]
        push   716f6e6dh
loc_13_0090:
        db  005h, 0CDh, 000h, 000h, 000h
        db  003h, 01Bh
        add    al, 0
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_13_009E:
        add    dword ptr [ecx], ebx
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
loc_13_00A5:
        push   eax
        push   ecx
        push   edi
        mov    al, byte ptr [ebp + 18h]
        mov    ecx, 10h
        db  0BFh, 080h, 000h, 000h, 000h
        repne scasb
        db  08Bh, 014h, 08Dh, 040h, 000h, 000h, 000h
        pop    edi
        pop    ecx
        pop    eax
        db  00Fh, 085h, 097h, 0FFh, 0F2h, 0FFh
        db  0CDh, 020h, 006h, 080h, 006h, 000h
loc_13_00CD:
        push   20h
        pop    ecx
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        jmp    NEAR PTR sub_13_00EC
        db  000h, 000h, 000h
sub_13_00EC:
        db  080h, 03Dh, 064h, 001h, 000h, 000h, 000h
        je     SHORT loc_13_013D
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_13_013D
        pushad
        mov    ebx, dword ptr [ebp + 10h]
        test   bl, bl
        je     SHORT loc_13_013E
        xchg   edi, eax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        movzx  eax, word ptr [edi + 14h]
        movzx  edx, word ptr [edi + 16h]
        movzx  ecx, word ptr [edi + 7]
        db  0BEh, 044h, 001h, 000h, 000h
        cmp    byte ptr [esi], bl
        je     SHORT loc_13_0131
        db  0BFh, 054h, 001h, 000h, 000h
        cld
        movsd
        movsd
        movsd
        movsd
        sub    esi, 10h
loc_13_0131:
        mov    byte ptr [esi], bl
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0ch], edx
loc_13_013C:
        popad
loc_13_013D:
        ret
loc_13_013E:
        db  088h, 01Dh, 044h, 001h, 000h, 000h
        db  088h, 01Dh, 054h, 001h, 000h, 000h
        jmp    SHORT loc_13_013C
_PTEXT ends
        end
