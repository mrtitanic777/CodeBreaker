; voltrack_icode_03.asm -- VOLTRACK object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        mov    ecx, 1
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        db  0A3h, 0F0h, 005h, 000h, 000h
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 52h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ebx, word ptr [ebp + 38h]
        shl    ebx, 4
        db  089h, 01Dh, 0F4h, 005h, 000h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        db  0C7h, 005h, 090h, 000h, 000h, 000h, 080h, 003h, 000h, 000h
        db  0C7h, 005h, 094h, 000h, 000h, 000h, 0BCh, 005h, 000h, 000h
        db  068h, 084h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        ret
loc_3_0084:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0090:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0094:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        outsd
        insb
        db  075h, 06Dh
        and    byte ptr gs:[edx + esi*2 + 61h], dl
        arpl   word ptr [ebx + 69h], bp
        outsb
        and    byte ptr [bx + si], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
