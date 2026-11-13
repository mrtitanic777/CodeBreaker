.586p
include vmm.inc
include dosmgr.inc
include ifsmgr.inc
include vnetbios.inc

IFNDEF Splitter_Device_ID
Splitter_Device_ID EQU 00498h
ENDIF
Create_Splitter_Service_Table EQU 1
Begin_Service_Table Splitter
        Splitter_Service Splitter_Service_000, LOCAL
        Splitter_Service Splitter_Service_001, LOCAL
        Splitter_Service Splitter_Service_002, LOCAL
        Splitter_Service Splitter_Service_003, LOCAL
        Splitter_Service Splitter_Service_004, LOCAL
End_Service_Table Splitter

Declare_Virtual_Device Splitter, 3, 10, Splitter_Control, 00498h, 080000000h

VxD_LOCKED_CODE_SEG
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        VxDcall IFSMgr_GetHeap
        pop    ecx
        test   eax, eax
        je     loc_1_0015
loc_1_0012:
        cld
        leave
        ret
loc_1_0015:
        db  08Bh, 00Dh, 0ECh, 005h, 000h, 000h
        db  066h, 083h, 039h, 000h
        jne    loc_1_0012
        VxDcall IFSMgr_FillHeapSpare
        push   dword ptr [ebp + 0ch]
        VxDcall IFSMgr_GetHeap
        pop    ecx
        cld
        leave
        ret
BeginProc sub_1_0034
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDcall IFSMgr_RetHeap
        pop    eax
        cld
        xor    eax, eax
        leave
        ret
EndProc sub_1_0034
BeginProc sub_1_0047
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp
