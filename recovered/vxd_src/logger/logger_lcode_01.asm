; logger_lcode_01.asm -- LOGGER object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0008:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0010:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
        db  04Ah, 007h, 000h, 000h, 008h, 003h, 000h, 000h, 091h, 004h, 000h, 000h, 07Ch, 000h, 000h, 000h
        db  0E1h, 004h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 09Dh, 004h, 001h, 000h, 000h, 000h, 04Ch, 04Fh, 047h, 047h
        db  045h, 052h, 020h, 020h, 000h, 000h, 000h, 080h, 048h, 007h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_0080:
        pop    ds
        add    byte ptr [eax], al
        db  000h
loc_1_0084:
        sbb    al, 0
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
