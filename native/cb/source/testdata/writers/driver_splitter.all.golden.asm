; Splitter -- a virtual device driver, reconstructed from the image by CodeBreaker.
;
; One segment per object of the original. Nothing here is an address: every place the
; loader patches is written as the label it refers to. Labels are L<object>_<offset in the
; original>; names the image itself carries (entry points, the descriptor block) are used
; where there are any. Instructions written as bytes are ones the assembler would encode
; differently from the original; their mnemonic is in the comment.

.386p
.model flat

; object 1: 1540 bytes, preload
S1 segment dword public use32 'O1CODE'
L1_00000:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0Ch]
        push   eax
        int  20h
        dd   00040000Dh    ; IFSMgr_GetHeap
        pop    ecx
        test   eax, eax
        je     short L1_00015
L1_00012:
        cld
        leave
        ret
L1_00015:
        mov    ecx, dword ptr [L1_005EC]
        cmp    word ptr [ecx], 0
        jne    short L1_00012
        int  20h
        dd   000400011h    ; IFSMgr_FillHeapSpare
        push   dword ptr [ebp + 0Ch]
        int  20h
        dd   00040000Dh    ; IFSMgr_GetHeap
        pop    ecx
        cld
        leave
        ret
L1_00034:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        int  20h
        dd   00040000Eh    ; IFSMgr_RetHeap
        pop    eax
        cld
        xor    eax, eax
        leave
        ret
L1_00047:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 0Ch]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 010h]
        repe cmpsb
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave
        ret
L1_00066:
        int  20h
        dd   000150004h    ; DOSMGR_Get_IndosPtr
        mov    dword ptr [L1_005EC], eax
        int  20h
        dd   000140000h    ; VNETBIOS_Get_Version
        jb     short L1_00098
        cmp    eax, 030Ah
        jb     short L1_00098
        mov    eax, 5
        mov    ecx, 02Ch
        int  20h
        dd   00001009Bh    ; List_Create
        jb     short L1_00098
        mov    dword ptr [L1_005F0], esi
L1_00098:
        ret
Splitter_control:
        cmp    eax, 1
        je     short L1_00066
        cmp    eax, 01Bh
        je     short L1_00066
        clc
        ret
L1_000A5:
        push   esi
        push   edi
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   0000100A3h    ; List_Get_First
        je     short L1_000CE
L1_000B5:
        mov    cl, byte ptr [eax]
        cmp    byte ptr [ebx + 030h], cl
        jne    short L1_000C6
        mov    cl, byte ptr [eax + 1]
        cmp    byte ptr [ebx + 3], cl
        jne    short L1_000C6
        jmp    short L1_000CE
L1_000C6:
        int  20h
        dd   0000100A4h    ; List_Get_Next
        jne    short L1_000B5
L1_000CE:
        pop    edi
        pop    esi
        ret
L1_000D1:
        push   esi
        push   edi
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   0000100A3h    ; List_Get_First
        je     short L1_0010E
L1_000E1:
        mov    cl, byte ptr [eax]
        cmp    byte ptr [ebx + 030h], cl
        jne    short L1_00106
        push   eax
        push   ebx
        push   010h
        lea    ecx, [ebx + 01Ah]
        push   ecx
        lea    ecx, [eax + 2]
        push   ecx
        call   near ptr L1_00047
        add    esp, 0Ch
        mov    ecx, eax
        pop    ebx
        pop    eax
        or     ecx, ecx
        jne    short L1_00106
        jmp    short L1_0010E
L1_00106:
        int  20h
        dd   0000100A4h    ; List_Get_Next
        jne    short L1_000E1
L1_0010E:
        pop    edi
        pop    esi
        ret
L1_00111:
        pushfd
        cli
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   0000100A1h    ; List_Remove
        popfd
        push   esi
        mov    esi, dword ptr [eax + 024h]
        int  20h
        dd   00001009Ch    ; List_Destroy
        mov    esi, dword ptr [eax + 01Ch]
        int  20h
        dd   00001009Ch    ; List_Destroy
        pop    esi
        int  20h
        dd   0000100A2h    ; List_Deallocate
        ret
Splitter_Service_0000:
        mov    eax, 1
        clc
        ret
Splitter_Service_0001:
        push   esi
        push   edi
        call   near ptr L1_000D1
        or     eax, eax
        jne    near ptr L1_001E8
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   00001009Dh    ; List_Allocate
        jb     short L1_001CC
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [eax + 1], cl
        mov    cl, byte ptr [ebx + 030h]
        mov    byte ptr [eax], cl
        sub    ecx, ecx
        mov    dword ptr [eax + 028h], ecx
        mov    dword ptr [eax + 020h], ecx
        mov    dword ptr [eax + 018h], ecx
        mov    dword ptr [eax + 014h], 1
        cmp    byte ptr [ebx + 1], 0Dh
        jne    short L1_00186
        or     dword ptr [eax + 014h], 2
L1_00186:
        cld
        lea    edi, [eax + 2]
        lea    esi, [ebx + 01Ah]
        mov    ecx, 010h
        rep movsb
        push   eax
        mov    eax, 5
        mov    ecx, 4
        int  20h
        dd   00001009Bh    ; List_Create
        pop    eax
        jb     short L1_001D8
        mov    dword ptr [eax + 024h], esi
        push   eax
        mov    eax, 5
        int  20h
        dd   00001009Bh    ; List_Create
        pop    eax
        jb     short L1_001CF
        mov    dword ptr [eax + 01Ch], esi
        mov    esi, dword ptr [L1_005F0]
        pushfd
        cli
        int  20h
        dd   00001009Fh    ; List_Attach_Tail
        popfd
L1_001CC:
        pop    edi
        pop    esi
        ret
L1_001CF:
        mov    esi, dword ptr [eax + 024h]
        int  20h
        dd   00001009Ch    ; List_Destroy
L1_001D8:
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   0000100A2h    ; List_Deallocate
        xor    eax, eax
        jmp    short L1_001CC
L1_001E8:
        or     dword ptr [eax + 014h], 1
        jmp    short L1_001CC
Splitter_Service_0002:
        push   esi
        push   edi
        call   near ptr L1_000D1
        or     eax, eax
        je     short L1_00247
        mov    edi, eax
        btr    dword ptr [edi + 014h], 0
        jae    short L1_00237
        mov    esi, dword ptr [edi + 024h]
L1_00205:
        pushfd
        cli
        int  20h
        dd   0000100A5h    ; List_Remove_First
        popfd
        or     eax, eax
        je     short L1_00237
        dec    dword ptr [edi + 028h]
        pushad
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 017h
        mov    byte ptr [ebx + 031h], 017h
        cmp    dword ptr [ebx + 02Ch], 0
        je     short L1_0022E
        mov    eax, 017h
        call   dword ptr [ebx + 02Ch]
L1_0022E:
        popad
        int  20h
        dd   0000100A2h    ; List_Deallocate
        jmp    short L1_00205
L1_00237:
        mov    eax, edi
        test   dword ptr [eax + 014h], 2
        jne    short L1_0024D
        call   near ptr L1_00111
L1_00247:
        int  20h
        dd   000140002h    ; VNETBIOS_Submit
L1_0024D:
        xor    eax, eax
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 031h], al
        pop    edi
        pop    esi
        ret
Splitter_Service_0003:
        push   esi
        mov    dword ptr [L1_005F4], eax
        or     eax, eax
        je     short L1_0027C
        mov    eax, 0140002h
        mov    esi, offset Splitter_Service_0004
        int  20h
        dd   000010090h    ; Hook_Device_Service
        jb     short L1_00295
        mov    dword ptr [L1_005FC], esi
        jmp    short L1_00295
L1_0027C:
        cmp    dword ptr [L1_005FC], 0
        je     short L1_00295
        mov    eax, 0140002h
        mov    esi, offset Splitter_Service_0004
        int  20h
        dd   00001011Ch    ; Unhook_Device_Service
L1_00295:
        pop    esi
        ret
        db   0EBh, 006h, 0FFh, 025h
        dd   offset L1_00600
Splitter_Service_0004:
        push   ecx
        push   edx
        push   esi
        push   edi
        push   eax
        cld
        sub    eax, eax
        mov    al, byte ptr [ebx]
        and    eax, 0FFFFFF7Fh
        cmp    eax, 036h
        je     short L1_002F1
        cmp    eax, 021h
        je     near ptr L1_00372
        cmp    eax, 035h
        je     near ptr L1_00466
        cmp    eax, 031h
        je     short L1_00310
L1_002CA:
        clc
        pop    eax
L1_002CC:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        cld
        jb     short L1_002E2
        cmp    dword ptr [L1_005FC], 0
        je     short L1_002E2
        jmp    dword ptr [L1_005FC]
L1_002E2:
        ret
L1_002E3:
        sub    eax, eax
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 031h], al
        add    esp, 4
        stc
        jmp    short L1_002CC
L1_002F1:
        mov    eax, dword ptr [L1_005F4]
        or     eax, eax
        je     short L1_002CA
        cmp    dword ptr [ebx + 02Ch], eax
        je     short L1_002CA
        push   ebx
        call   near ptr L1_000D1
        pop    ebx
        or     eax, eax
        je     short L1_002CA
        or     dword ptr [eax + 014h], 2
        jmp    short L1_002CA
L1_00310:
        push   ebx
        call   near ptr L1_000D1
        pop    ebx
        or     eax, eax
        je     short L1_002CA
        mov    edi, eax
        btr    dword ptr [edi + 014h], 1
        jae    short L1_00359
        mov    esi, dword ptr [edi + 01Ch]
L1_00327:
        pushfd
        cli
        int  20h
        dd   0000100A5h    ; List_Remove_First
        popfd
        or     eax, eax
        je     short L1_00359
        pushad
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 017h
        mov    byte ptr [ebx + 031h], 017h
        cmp    dword ptr [ebx + 02Ch], 0
        je     short L1_0034D
        mov    eax, 017h
        call   dword ptr [ebx + 02Ch]
L1_0034D:
        popad
        dec    dword ptr [edi + 020h]
        int  20h
        dd   0000100A2h    ; List_Deallocate
        jmp    short L1_00327
L1_00359:
        mov    eax, edi
        test   dword ptr [eax + 014h], 1
        jne    near ptr L1_002E3
        call   near ptr L1_00111
        jmp    near ptr L1_002CA
L1_00372:
        push   ebx
        call   near ptr L1_000A5
        pop    ebx
        or     eax, eax
        je     near ptr L1_002CA
        mov    edi, eax
        sub    edx, edx
        mov    eax, dword ptr [L1_005F4]
        or     eax, eax
        je     short L1_00396
        cmp    dword ptr [ebx + 02Ch], eax
        jne    short L1_00396
        inc    edx
        jmp    short L1_003B5
L1_00396:
        test   dword ptr [edi + 014h], 1
        jne    short L1_003B5
        cmp    dword ptr [edi + 018h], 0
        jne    near ptr L1_002CA
        mov    eax, edi
        call   near ptr L1_00111
        jmp    near ptr L1_002CA
L1_003B5:
        mov    esi, dword ptr [edi + edx*8 + 01Ch]
        int  20h
        dd   00001009Dh    ; List_Allocate
        jb     near ptr L1_002CA
        mov    dword ptr [eax], ebx
        pushfd
        cli
        int  20h
        dd   00001009Fh    ; List_Attach_Tail
        inc    dword ptr [edi + edx*8 + 020h]
        mov    ecx, dword ptr [edi + 018h]
        cmp    ecx, dword ptr [edi + edx*8 + 020h]
        jb     short L1_003F0
        popfd
        mov    byte ptr [ebx + 1], 0FFh
        mov    byte ptr [ebx + 031h], 0FFh
        sub    eax, eax
        add    esp, 4
        stc
        jmp    near ptr L1_002CC
L1_003F0:
        inc    dword ptr [edi + 018h]
        popfd
        mov    esi, eax
        push   edx
        push   ebx
        mov    ecx, 040h
        add    ecx, 05ADh
        push   ecx
        push   0
        call   near ptr L1_00000
        add    esp, 8
        pop    ebx
        pop    edx
        or     eax, eax
        jne    short L1_0042E
        mov    eax, esi
        mov    esi, dword ptr [edi + edx*8 + 01Ch]
        pushfd
        cli
        int  20h
        dd   0000100A1h    ; List_Remove
        popfd
        int  20h
        dd   0000100A2h    ; List_Deallocate
        jmp    near ptr L1_002CA
L1_0042E:
        mov    edi, offset L1_004E6
        mov    dword ptr [eax + 02Ch], edi
        mov    cl, byte ptr [ebx]
        mov    byte ptr [eax], cl
        or     byte ptr [eax], 080h
        mov    word ptr [eax + 8], 05ADh
        mov    edi, eax
        add    edi, 040h
        mov    dword ptr [eax + 4], edi
        mov    cl, byte ptr [ebx + 030h]
        mov    byte ptr [eax + 030h], cl
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [ebx + 1], 0FFh
        mov    byte ptr [ebx + 031h], 0FFh
        mov    ebx, eax
        jmp    near ptr L1_002CA
L1_00466:
        sub    edx, edx
        mov    eax, dword ptr [L1_005F4]
        or     eax, eax
        je     short L1_00477
        cmp    dword ptr [ebx + 02Ch], eax
        jne    short L1_00477
        inc    edx
L1_00477:
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   0000100A3h    ; List_Get_First
        je     near ptr L1_002CA
L1_00489:
        push   eax
        mov    esi, dword ptr [eax + edx*8 + 01Ch]
        pushfd
        cli
        int  20h
        dd   0000100A3h    ; List_Get_First
        je     short L1_004D1
L1_00498:
        cmp    dword ptr [eax], ebx
        jne    short L1_004C9
        int  20h
        dd   0000100A1h    ; List_Remove
        popfd
        pushad
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 0Bh
        mov    byte ptr [ebx + 031h], 0Bh
        cmp    dword ptr [ebx + 02Ch], 0
        je     short L1_004BC
        mov    eax, 0Bh
        call   dword ptr [ebx + 02Ch]
L1_004BC:
        popad
        int  20h
        dd   0000100A2h    ; List_Deallocate
        pop    eax
        jmp    near ptr L1_002E3
L1_004C9:
        int  20h
        dd   0000100A4h    ; List_Get_Next
        jne    short L1_00498
L1_004D1:
        popfd
        pop    eax
        mov    esi, dword ptr [L1_005F0]
        int  20h
        dd   0000100A4h    ; List_Get_Next
        jne    short L1_00489
        jmp    near ptr L1_002CA
L1_004E6:
        pushad
        cld
        push   ebx
        call   near ptr L1_000A5
        pop    ebx
        or     eax, eax
        je     near ptr L1_00579
        mov    edi, eax
        mov    edx, 1
L1_004FE:
        mov    esi, dword ptr [edi + edx*8 + 01Ch]
        int  20h
        dd   0000100A5h    ; List_Remove_First
        je     short L1_00576
        dec    dword ptr [edi + edx*8 + 020h]
        push   eax
        push   edx
        push   esi
        push   edi
        mov    eax, dword ptr [eax]
        sub    ecx, ecx
        mov    cx, word ptr [eax + 8]
        cmp    cx, word ptr [ebx + 8]
        jge    short L1_00532
        cmp    byte ptr [ebx + 1], 0
        jne    short L1_0052D
        mov    edx, 6
        jmp    short L1_0053D
L1_0052D:
        mov    dl, byte ptr [ebx + 1]
        jmp    short L1_0053D
L1_00532:
        mov    dl, byte ptr [ebx + 1]
        mov    cx, word ptr [ebx + 8]
        mov    word ptr [eax + 8], cx
L1_0053D:
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [ebx + 4]
        cld
        rep movsb
        mov    ecx, 010h
        lea    edi, [eax + 0Ah]
        lea    esi, [ebx + 0Ah]
        cld
        rep movsb
        mov    byte ptr [eax + 1], dl
        mov    byte ptr [eax + 031h], dl
        pushad
        mov    ebx, eax
        sub    eax, eax
        cmp    dword ptr [ebx + 02Ch], eax
        je     short L1_0056B
        mov    al, byte ptr [ebx + 1]
        call   dword ptr [ebx + 02Ch]
        cld
L1_0056B:
        popad
        pop    edi
        pop    esi
        pop    edx
        pop    eax
        int  20h
        dd   0000100A2h    ; List_Deallocate
L1_00576:
        dec    edx
        je     short L1_004FE
L1_00579:
        dec    dword ptr [edi + 018h]
        push   ebx
        call   near ptr L1_00034
        add    esp, 4
        popad
        ret
        db   000h
L1_00588:
        dd   offset Splitter_Service_0000
        dd   offset Splitter_Service_0001
        dd   offset Splitter_Service_0002
        dd   offset Splitter_Service_0003
        dd   offset Splitter_Service_0004
public Splitter_DDB
Splitter_DDB:
        db   000h, 000h, 000h, 000h, 000h, 004h, 098h, 004h, 003h, 00Ah, 000h, 000h
        db   'Splitter'
        db   000h, 000h, 000h, 080h
        dd   offset Splitter_control
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h
        dd   offset L1_00588
        db   005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   'verPP'
        db   000h, 000h, 000h
        db   '1vsR2vsR3vsR'
L1_005EC:
        db   000h, 000h, 000h, 000h
L1_005F0:
        db   000h, 000h, 000h, 000h
L1_005F4:
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
L1_005FC:
        db   000h, 000h, 000h, 000h
L1_00600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
S1 ends

end
