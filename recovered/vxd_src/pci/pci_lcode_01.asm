; pci_lcode_01.asm -- PCI object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 043h, 000h, 004h, 000h, 000h, 000h, 050h, 043h, 049h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 080h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
PCI_Control:
        db  083h, 0F8h, 01Bh, 075h, 009h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0F8h, 001h, 0C3h, 083h, 0F8h
        db  022h, 075h, 009h, 052h, 053h, 0E8h, 000h, 000h, 000h, 000h, 0F9h, 0C3h, 0F8h, 0C3h, 0CCh, 0CCh
_LTEXT ends
        end
