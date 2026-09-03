; ifsmgr_icode_08.asm -- IFSMGR object 8 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_8_0000:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        movsb
        add    dword ptr [eax], eax
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        db  000h, 087h, 005h, 000h, 000h, 062h
        sub    dword ptr [ecx], eax
        db  000h
_ITEXT ends
        end
