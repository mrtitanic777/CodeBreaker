; logger_mcode_02.asm -- LOGGER object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h
loc_2_0004:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
        sub    al, byte ptr [eax]
loc_2_0010:
        and    eax, 2d643230h
        and    eax, 2d643230h
        and    eax, 20643230h
        and    eax, 3a643230h
        and    eax, 3a643230h
        and    eax, 322e3230h
        and    eax, 322e3230h
        db  064h, 020h, 02Dh, 020h, 000h
_LMSGTABLE ends
        end
