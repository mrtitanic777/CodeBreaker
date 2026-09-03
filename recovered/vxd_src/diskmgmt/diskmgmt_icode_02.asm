; diskmgmt_icode_02.asm -- DISKMGMT object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_2_0000:
        db  068h, 014h, 000h, 000h, 000h
        db  0E8h, 04Eh, 000h, 0FEh, 0FFh
        add    esp, 4
        mov    eax, 1
        ret
        db  000h
loc_2_0014:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr ds:[eax], al
        db  000h, 0A0h, 003h, 000h, 000h, 044h
        dec    ecx
        push   ebx
        dec    ebx
        dec    ebp
        inc    edi
        dec    ebp
        push   esp
        and    byte ptr [esi + 53h], dl
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
        dec    ecx
        push   ebx
        dec    ebx
        dec    ebp
        inc    edi
        dec    ebp
        push   esp
        and    byte ptr [esi + 69h], dl
        db  072h, 074h
        db  075h, 061h
        insb
        db  020h, 044h, 065h
_ITEXT ends
        end
