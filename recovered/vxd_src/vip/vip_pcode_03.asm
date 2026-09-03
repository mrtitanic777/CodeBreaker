; vip_pcode_03.asm -- VIP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_3_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  066h, 000h
loc_3_0010:
        xchg   dword ptr [eax], eax
        mov    dword ptr [eax], eax
loc_3_0014:
        push   esp
        push   79732065h
        jae    SHORT loc_3_0090
        insd
        and    byte ptr [eax + 61h], ch
        jae    SHORT loc_3_0043
        db  064h, 065h, 074h, 065h
        arpl   word ptr [ebp + 64h], si
        and    byte ptr [ecx + 20h], ah
        arpl   word ptr [edi + 6eh], bp
        insb
        imul   esp, dword ptr [ebx + 74h], 726f6620h
        and    byte ptr [ecx + 50h], cl
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
loc_3_0043:
        db  073h, 073h
        and    byte ptr [69772073h], ah
        db  074h, 068h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 79h], dh
        db  073h, 074h
        insd
        and    byte ptr [eax + 61h], ch
        db  076h, 069h
        outsb
        and    byte ptr [bx + si + 61h], ch
        db  072h, 064h
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [202e7332h], ah
        and    eax, 54007333h
        push   6e692065h
        db  074h, 065h
        db  072h, 066h
        popad
        arpl   word ptr [ebp + 20h], sp
        push   62207361h
        outsb
loc_3_0090:
        and    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ebp + 64h]
        add    byte ptr cs:[eax], ah
        db  018h
_PTEXT ends
        end
