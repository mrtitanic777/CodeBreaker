; pci_icode_04.asm -- PCI object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_4_0000:
        push   eax
        inc    ebx
        dec    ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        xor    eax, eax
        push   esi
        db  0A3h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        push   edi
        push   ebp
        push   eax
        push   28000h
        push   0c0000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_002D:
        add    esp, 0ch
        mov    esi, eax
        lea    edi, [eax + 20000h]
        cmp    eax, edi
        ja     NEAR PTR loc_4_00C2
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        imul   ecx, ecx, 0eh
        db  08Dh, 091h, 000h, 000h, 000h, 000h
loc_4_004F:
        cmp    word ptr [esi], 0aa55h
        jne    SHORT loc_4_00B8
        movzx  ebx, byte ptr [esi + 2]
        shl    ebx, 9
        test   ebx, ebx
        jne    SHORT loc_4_0066
        mov    ebx, 800h
loc_4_0066:
        movzx  ecx, word ptr [esi + 18h]
        lea    ebp, [ecx + 3]
        cmp    ebp, ebx
        jbe    SHORT loc_4_0075
        add    esi, ebx
        jmp    SHORT loc_4_00BE
loc_4_0075:
        add    ecx, esi
        db  08Bh, 02Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx], ebp
        jne    SHORT loc_4_00A9
        mov    ecx, dword ptr [ecx + 4]
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx], ecx
        mov    ecx, esi
        sub    ecx, eax
        add    edx, 0eh
        add    ecx, 0c0000h
        mov    dword ptr [edx - 0ah], ecx
        mov    dword ptr [edx - 6], ebx
        mov    byte ptr [edx - 2], 0
        mov    dword ptr [edx], 0ffffffffh
loc_4_00A9:
        lea    esi, [ebx + esi + 7ffh]
        and    esi, 0fffff800h
        jmp    SHORT loc_4_00BE
loc_4_00B8:
        add    esi, 800h
loc_4_00BE:
        cmp    esi, edi
        jbe    SHORT loc_4_004F
loc_4_00C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_4_00C7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        mov    eax, 0
        jb     SHORT loc_4_00EE
        call   8
        mov    eax, 1
loc_4_00EE:
        mov    esp, ebp
        pop    ebp
        ret
_ITEXT ends
        end
