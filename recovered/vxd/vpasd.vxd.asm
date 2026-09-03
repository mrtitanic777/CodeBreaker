; vpasd -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 4258 instructions recovered by recursive descent

Declare_Virtual_Device vpasd, 4, 0, vpasd_Control, 03203h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x13f8  R-X
        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  03203h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'vpasd   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  vpasd_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  vpasd_PM_API             ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0BCh, 0BDh, 0BEh, 0BFh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
loc_1_00FB:
        add    eax, 0
        add    byte ptr [eax], al
        db  000h
loc_1_0103:
        and    byte ptr [edx], al
        add    byte ptr [eax], al
loc_1_0107:
        pop    es
        add    byte ptr [eax], al
        db  000h
loc_1_010B:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_010F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0113:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x200], al
        add    byte ptr [0], al
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
        add    ah, cl
        int3   
loc_1_0144:
        add    byte ptr [eax], al            ; -> loc_1_0CD0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0C99
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0CD5
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D19
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D41
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D85
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0DA7
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_1_0DEB
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E16
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E3A
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E52
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E73
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E94
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0EC0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0EE8
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0F03
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0F12
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0F1C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
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
        adc    al, 0
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 0x20
        add    byte ptr [eax + ebp], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + esi], ch
        xor    al, 0
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        mov    eax, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        ret    0xc200
        db  0C2h, 000h, 000h, 000h, 000h, 0C2h, 000h, 0C2h, 0C2h, 000h, 000h, 000h, 000h, 0C2h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C2h, 000h, 000h, 000h, 020h, 020h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h
        db  0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 0C2h, 000h, 000h, 000h, 000h, 0C2h, 0C2h, 0C2h
        db  0C2h, 000h, 000h, 000h, 000h, 0C2h, 0C2h, 0C2h, 0C2h, 002h, 000h, 000h, 000h, 0C2h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C2h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
vpasd_Control:
        cmp    eax, 0
        je     loc_1_03EA
        cmp    eax, 7
        je     sub_1_0456
        cmp    eax, 0xe
        je     loc_1_078C
        cmp    eax, 0xd
        je     loc_1_0780
        cmp    eax, 6
        je     loc_1_03EC
        cmp    eax, 0xc
        je     loc_1_03FB
        cmp    eax, 0xb
        je     loc_4_0318
loc_1_03C3:
        cmp    eax, 0x1b
        je     loc_8_0000
loc_1_03CC:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_03D5:
        cmp    eax, 0x22
        je     loc_4_077C
loc_1_03DE:
        cmp    eax, 9
        je     loc_1_03FD
        cmp    eax, 3
        je     sub_1_03FE
        clc    
        ret    
loc_1_03EA:
        clc    
        ret    
loc_1_03EC:
        push   esi
        lea    esi, [0]                      ; -> obj1:0x0122
        call   sub_1_074B
        pop    esi
        clc    
        ret    
loc_1_03FB:
        clc    
        ret    
loc_1_03FD:
        ret    
sub_1_03FE:
        push   esi
        push   eax
        push   edx
        mov    byte ptr [0], 0xbf            ; -> obj1:0x0118
        test   dword ptr [0], 0x40           ; -> obj1:0x00FF
        jne    loc_1_044D
        test   dword ptr [0], 0x20           ; -> obj1:0x00FF
        jne    loc_1_044D
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    dx, 0xb8b
        in     al, dx
        test   al, 2
        je     loc_1_0449
        mov    dx, 0xf788
        in     al, dx
        test   al, 2
        je     loc_1_0449
        mov    dx, 0xf789
        in     al, dx
        test   al, 0xf
        je     loc_1_0449
        mov    byte ptr [esi + 0x2e], 0xff
        jmp    loc_1_044D
loc_1_0449:
        mov    byte ptr [esi + 0x2e], 0
loc_1_044D:
        call   sub_1_0456
        pop    edx
        pop    eax
        pop    esi
        ret    
sub_1_0456:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   byte ptr [esi + 0x44], 1
        jne    loc_1_046A
        call   sub_1_0548
loc_1_046A:
        pop    esi
        clc    
        ret    
        db  0F8h, 0C3h, 090h
sub_1_0470:
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0136
        or     eax, eax
        je     loc_1_0480
        cmp    ebx, eax
        je     loc_1_048A
        jmp    loc_1_048D
loc_1_0480:
        call   sub_1_0617
        call   sub_1_065E
loc_1_048A:
        pop    eax
        clc    
        ret    
loc_1_048D:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   dword ptr [esi + 0x46], 2
        jne    loc_1_04C1
        or     dword ptr [esi + 0x46], 2
        pushal 
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        popal  
loc_1_04C1:
        pop    esi
        pop    eax
        stc    
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_04C8:
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x012E
        or     eax, eax
        je     loc_1_04D8
        cmp    ebx, eax
        je     loc_1_050D
        jmp    loc_1_0510
loc_1_04D8:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0132
        je     loc_1_04E5
        call   sub_5_006E
loc_1_04E5:
        xchg   dword ptr [0], ebx            ; -> obj1:0x012E
        mov    dword ptr [0], ebx            ; -> obj1:0x0132
        or     ebx, ebx
        je     loc_1_0502
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_1_0502
        call   sub_5_0021
loc_1_0502:
        mov    ebx, dword ptr [0]            ; -> obj1:0x012E
        call   sub_5_0000
loc_1_050D:
        pop    eax
        clc    
        ret    
loc_1_0510:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   dword ptr [esi + 0x46], 1
        jne    loc_1_0544
        or     dword ptr [esi + 0x46], 1
        pushal 
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
        popal  
loc_1_0544:
        pop    esi
        pop    eax
        stc    
        ret    
sub_1_0548:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        mov    dword ptr [esi + 0x10], 0     ; -> loc_1_0CD4
        mov    dword ptr [esi + 0x14], 0     ; -> loc_1_0CD4
        mov    byte ptr [esi + 0x18], 0x7f
        mov    byte ptr [esi + 0x19], 0xaa
        mov    byte ptr [esi + 0x1a], 0xff
        mov    byte ptr [esi + 0x1b], 0
        mov    byte ptr [esi + 0x1c], 0xaa
        mov    byte ptr [esi + 0x1d], 0
        mov    byte ptr [esi + 0x1e], 0
        mov    byte ptr [esi + 0x1f], 0
        mov    byte ptr [esi + 0x20], 0xff
        mov    byte ptr [esi + 0x21], 0xff
        mov    word ptr [esi + 0x22], 0
        mov    byte ptr [esi + 0x24], 0x36
        mov    byte ptr [esi + 0x25], 0x74
        mov    word ptr [esi + 0x26], 0x6c
        mov    word ptr [esi + 0x28], 0x800
        mov    byte ptr [esi + 0x2a], 0x24
        mov    byte ptr [esi + 0x2b], 9
        test   dword ptr [0], 0x100          ; -> obj1:0x00FF
        je     loc_1_05B7
        mov    byte ptr [esi + 0x2b], 0
loc_1_05B7:
        mov    byte ptr [esi + 0x2c], 0x29
        mov    dx, 0xb8b
        in     al, dx
        and    al, 0xf3
        mov    byte ptr [esi + 0x2d], al
        mov    byte ptr [esi + 0x44], 5
        mov    byte ptr [esi + 0x45], 0
        test   dword ptr [0], 0x40           ; -> obj1:0x00FF
        jne    loc_1_0616
        test   dword ptr [0], 0x20           ; -> obj1:0x00FF
        jne    loc_1_05FD
        push   esi
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x2e]
        pop    ebx
        pop    esi
        mov    byte ptr [esi + 0x2e], al
loc_1_05FD:
        mov    byte ptr [esi + 0x21], 0
        mov    byte ptr [esi + 0x1b], 0
        push   esi
        push   edi
        cld    
        lea    edi, [esi + 0x38]
        lea    esi, [0]                      ; -> obj1:0x0122
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
loc_1_0616:
        ret    
sub_1_0617:
        push   esi
        push   edi
        push   eax
        push   ebx
        push   edx
        mov    ebx, dword ptr [0]            ; -> obj1:0x0136
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_1_0658
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x2a]
        and    al, 0xbf
        mov    edx, 0xb8a
        out    dx, al
        mov    dx, 0xb89
        out    dx, al
        mov    edx, 0xb8b
        in     al, dx
        mov    byte ptr [esi + 0x2d], al
        and    al, 0xe7
        out    dx, al
        push   edi
        lea    edi, [esi + 0x38]
        call   sub_1_0716
        pop    edi
loc_1_0658:
        pop    edx
        pop    ebx
        pop    eax
        pop    edi
        pop    esi
        ret    
sub_1_065E:
        push   esi
        push   edi
        push   edx
        push   ebx
        push   eax
        mov    dword ptr [0], ebx            ; -> obj1:0x0136
        or     ebx, ebx
        jne    loc_1_0673
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
loc_1_0673:
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   byte ptr [esi + 0x44], 1
        jne    loc_1_0686
        call   sub_1_0456
loc_1_0686:
        test   dword ptr [0], 0x20           ; -> obj1:0x00FF
        jne    loc_1_069A
        mov    al, byte ptr [esi + 0x2e]
        call   sub_1_138C
loc_1_069A:
        push   esi
        lea    esi, [esi + 0x38]
        call   sub_1_074B
        pop    esi
        mov    ax, word ptr [esi + 0x26]
        call   sub_1_108C
        mov    ax, word ptr [esi + 0x28]
        call   sub_1_10A5
        mov    al, byte ptr [esi + 0x2b]
        mov    edx, 0xf8a
        out    dx, al
        mov    al, 0xff
        xchg   byte ptr [esi + 0x2c], al
        call   sub_1_10E6
        mov    edx, 0xb8b
        mov    al, byte ptr [esi + 0x2d]
        out    dx, al
        mov    al, byte ptr [esi + 0x2a]
        mov    edx, 0xb8a
        out    dx, al
        test   dword ptr [0], 0x10           ; -> obj1:0x00FF
        je     loc_1_0710
        mov    edx, dword ptr [0]            ; -> loc_1_0103
        add    dl, 0xe
        in     al, dx
        test   byte ptr [esi + 0x44], 0x30
        je     loc_1_0710
        test   byte ptr [esi + 0x44], 4
        jne    loc_1_0710
        and    byte ptr [esi + 0x44], 0xcf
        or     byte ptr [esi + 0x44], 8
        mov    eax, dword ptr [0]            ; -> obj1:0x00F7
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0710:
        pop    eax
        pop    ebx
        pop    edx
        pop    edi
        pop    esi
        ret    
sub_1_0716:
        test   dword ptr [0], 0x10           ; -> obj1:0x00FF
        je     loc_1_074A
        cld    
        mov    edx, 0x8388
        in     al, dx
        stosb  byte ptr es:[edi], al
        inc    edx
        in     al, dx
        stosb  byte ptr es:[edi], al
        inc    edx
        in     al, dx
        stosb  byte ptr es:[edi], al
        inc    edx
        in     al, dx
        stosb  byte ptr es:[edi], al
        mov    edx, 0xf388
        in     al, dx
        stosb  byte ptr es:[edi], al
        inc    edx
        in     al, dx
        stosb  byte ptr es:[edi], al
        inc    edx
        in     al, dx
        stosb  byte ptr es:[edi], al
        inc    edx
        in     al, dx
        stosb  byte ptr es:[edi], al
        mov    edx, 0xbf8a
        in     al, dx
        stosb  byte ptr es:[edi], al
loc_1_074A:
        ret    
sub_1_074B:
        test   dword ptr [0], 0x10           ; -> obj1:0x00FF
        je     loc_1_077F
        cld    
        mov    edx, 0x8388
        lodsb  al, byte ptr [esi]
        out    dx, al
        inc    edx
        lodsb  al, byte ptr [esi]
        out    dx, al
        inc    edx
        lodsb  al, byte ptr [esi]
        out    dx, al
        inc    edx
        lodsb  al, byte ptr [esi]
        out    dx, al
        mov    edx, 0xf388
        lodsb  al, byte ptr [esi]
        out    dx, al
        inc    edx
        lodsb  al, byte ptr [esi]
        out    dx, al
        inc    edx
        lodsb  al, byte ptr [esi]
        out    dx, al
        inc    edx
        lodsb  al, byte ptr [esi]
        out    dx, al
        mov    edx, 0xbf8a
        lodsb  al, byte ptr [esi]
        out    dx, al
loc_1_077F:
        ret    
loc_1_0780:
        push   ecx
        mov    cx, 0
        call   sub_1_0798
        pop    ecx
        ret    
loc_1_078C:
        push   ecx
        mov    cx, 1
        call   sub_1_0798
        pop    ecx
        ret    
sub_1_0798:
        push   eax
        push   ebx
        mov    ax, 0xbc0f
        mov    bx, 5
        push   0x2f
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
        pop    eax
        ret    
loc_1_07B0:
        push   esi
        push   edx
        push   ebx
        push   eax
        mov    edx, 0xb89
        in     al, dx
        test   al, 0x1f
        je     loc_1_080B
        mov    ebx, dword ptr [0]            ; -> obj1:0x0136
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jae    loc_1_07D4
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        jmp    loc_1_080B
loc_1_07D4:
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   byte ptr [esi + 0x44], 0x30
        je     loc_1_07FF
        test   byte ptr [esi + 0x44], 0x20
        je     loc_1_07FF
        and    byte ptr [esi + 0x44], 0xdf
        and    byte ptr [esi + 0x2a], 0xbf
        mov    al, byte ptr [esi + 0x2a]
        mov    edx, 0xb8a
        out    dx, al
        mov    edx, 0xb89
        out    dx, al
loc_1_07FF:
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    ebx
        pop    edx
        pop    esi
        clc    
        ret    
loc_1_080B:
        pop    eax
        pop    ebx
        pop    edx
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        pop    esi
        clc    
        ret    
loc_1_0817:
        ret    
loc_1_0818:
        push   ebx
        push   esi
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0849
        mov    ebx, dword ptr [0]            ; -> obj1:0x0136
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        test   byte ptr [esi + 0x44], 8
        jne    loc_1_0842
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_1_0842:
        and    byte ptr [esi + 0x44], 0xf7
        pop    esi
        pop    ebx
        ret    
loc_1_0849:
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        and    byte ptr [esi + 0x44], 0xf7
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        pop    esi
        pop    ebx
        ret    
loc_1_085E:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        and    byte ptr [esi + 0x44], 0xfb
        jecxz  loc_1_0871
        or     byte ptr [esi + 0x44], 4
loc_1_0871:
        pop    esi
        ret    
loc_1_0873:
        ret    
loc_1_0874:
        cmp    ecx, 4
        jbe    loc_1_087F
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_087F:
        je     loc_1_0884
        mov    al, 0xff
        ret    
loc_1_0884:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0894
        call   sub_1_0470
        jae    loc_1_0894
        ret    
loc_1_0894:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   al, 1
        je     loc_1_08FC
        call   sub_1_0DEB
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        mov    dword ptr [esi + 0x10], 0     ; -> loc_1_0CD4
        mov    dword ptr [esi + 0x14], 0     ; -> loc_1_0CD4
        mov    byte ptr [esi + 0x19], 0xaa
        or     byte ptr [esi + 0x1a], 0x80
        and    byte ptr [esi + 0x18], 0x7f
        mov    byte ptr [esi + 0x1b], 0
        mov    byte ptr [esi + 0x20], 0xff
        mov    byte ptr [esi + 0x21], 0xff
        mov    al, 9
        mov    byte ptr [esi + 0x2b], al
        and    byte ptr [esi + 0x44], 0xc5
        mov    byte ptr [esi + 0x2c], 0xff
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_08FC
        mov    al, byte ptr [esi + 0x2b]
        mov    dx, 0xf8a
        out    dx, al
        mov    dx, 0xb89
        out    dx, al
        sub    eax, eax
        call   sub_1_10E6
loc_1_08FC:
        pop    esi
        ret    
loc_1_08FE:
        cmp    ecx, 4
        jbe    loc_1_0909
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0909:
        je     loc_1_092E
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x18]
        test   byte ptr [esi + 0x2b], 0x80
        je     loc_1_0923
        test   byte ptr [esi + 0x2a], 0x40
        jne    loc_1_0925
loc_1_0923:
        pop    esi
        ret    
loc_1_0925:
        xor    byte ptr [esi + 0x1f], 0x80
        xor    al, byte ptr [esi + 0x1f]
        pop    esi
        ret    
loc_1_092E:
        test   dword ptr [0], 0x40           ; -> obj1:0x00FF
        je     loc_1_0941
        mov    byte ptr [0], 1               ; -> loc_1_0144
loc_1_0941:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        or     byte ptr [esi + 0x18], 0x80
        test   byte ptr [esi + 0x44], 0x20
        je     loc_1_095D
        cmp    al, 0xd0
        je     loc_1_095D
        call   sub_1_0DEB
loc_1_095D:
        call   dword ptr [esi + 0xc]
        pop    esi
        ret    
loc_1_0962:
        cmp    ecx, 4
        jbe    loc_1_096D
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_096D:
        je     loc_1_098A
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x19]
        and    byte ptr [esi + 0x1a], 0x7f
        cmp    byte ptr [esi + 0x1b], 0
        je     loc_1_0988
        call   dword ptr [esi + 0x10]
loc_1_0988:
        pop    esi
        ret    
loc_1_098A:
        ret    
loc_1_098B:
        cmp    ecx, 4
        jbe    loc_1_0996
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0996:
        je     loc_1_09B7
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        cmp    byte ptr [esi + 0x1d], 0
        je     loc_1_09B0
        dec    byte ptr [esi + 0x1d]
        jne    loc_1_09B0
        or     byte ptr [esi + 0x1a], 0x80
loc_1_09B0:
        call   dword ptr [esi + 0x14]
        mov    al, byte ptr [esi + 0x1a]
        pop    esi
loc_1_09B7:
        ret    
loc_1_09B8:
        cmp    ecx, 4
        jbe    loc_1_09C3
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_09C3:
        je     loc_1_09C7
        mov    al, 0xff
loc_1_09C7:
        ret    
loc_1_09C8:
        cmp    ecx, 4
        jbe    loc_1_09D3
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_09D3:
        je     loc_1_09D7
        mov    al, 0xff
loc_1_09D7:
        ret    
loc_1_09D8:
        cmp    ecx, 4
        jbe    loc_1_09E3
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_09E3:
        je     loc_1_09F3
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x2a]
        pop    esi
        ret    
loc_1_09F3:
        test   dword ptr [0], 0x40           ; -> obj1:0x00FF
        je     loc_1_0A16
        cmp    byte ptr [0], 1               ; -> loc_1_0144
        jne    loc_1_0A16
        mov    al, 0xa
        call   sub_1_10E6
        mov    byte ptr [0], 0               ; -> loc_1_0144
loc_1_0A16:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0A26
        call   sub_1_0470
        jae    loc_1_0A26
        ret    
loc_1_0A26:
        test   dword ptr [0], 0x10           ; -> obj1:0x00FF
        je     loc_1_0A47
        cmp    byte ptr [0], 1               ; -> loc_1_0113
        jne    loc_1_0A47
        call   sub_1_1302
        mov    byte ptr [0], 0               ; -> loc_1_0113
loc_1_0A47:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    byte ptr [esi + 0x2a], al
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0A62
        call   sub_1_0470
        jb     loc_1_0A63
loc_1_0A62:
        out    dx, al
loc_1_0A63:
        pop    esi
        ret    
loc_1_0A65:
        cmp    ecx, 4
        jbe    loc_1_0A70
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0A70:
        je     loc_1_0A80
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x2d]
        pop    esi
        ret    
loc_1_0A80:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0A90
        call   sub_1_0470
        jae    loc_1_0A90
        ret    
loc_1_0A90:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        and    byte ptr [esi + 0x44], 0xef
        test   al, 0x1f
        je     loc_1_0AAF
        mov    ah, al
        xor    ah, 0x1f
        test   ah, 0x1f
        je     loc_1_0AAF
        or     byte ptr [esi + 0x44], 0x10
loc_1_0AAF:
        and    byte ptr [esi + 0x2d], 0xe0
        and    al, 0x1f
        or     byte ptr [esi + 0x2d], al
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0AC1
        out    dx, al
loc_1_0AC1:
        pop    esi
        ret    
loc_1_0AC3:
        cmp    ecx, 4
        jbe    loc_1_0ACE
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0ACE:
        je     loc_1_0ADE
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x2b]
        pop    esi
        ret    
loc_1_0ADE:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0AF6
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0AF6
        call   sub_1_0470
        jae    loc_1_0AF6
        ret    
loc_1_0AF6:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   dword ptr [0], 0x100          ; -> obj1:0x00FF
        je     loc_1_0B0D
        and    al, 0xf0
loc_1_0B0D:
        mov    byte ptr [esi + 0x2b], al
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0B19
        out    dx, al
loc_1_0B19:
        pop    esi
        ret    
loc_1_0B1B:
        cmp    ecx, 4
        jbe    loc_1_0B26
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0B26:
        je     loc_1_0B2A
        in     al, dx
        ret    
loc_1_0B2A:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   byte ptr [esi + 0x45], 1
        jne    loc_1_0B41
        btc    word ptr [esi + 0x24], 7
        jb     loc_1_0B4A
loc_1_0B41:
        mov    byte ptr [esi + 0x26], al
        mov    byte ptr [esi + 0x27], 0
        jmp    loc_1_0B4D
loc_1_0B4A:
        mov    byte ptr [esi + 0x27], al
loc_1_0B4D:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0B56
        out    dx, al
loc_1_0B56:
        pop    esi
        ret    
loc_1_0B58:
        cmp    ecx, 4
        jbe    loc_1_0B63
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0B63:
        je     loc_1_0B67
        in     al, dx
        ret    
loc_1_0B67:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        test   byte ptr [esi + 0x45], 1
        jne    loc_1_0B7E
        btc    word ptr [esi + 0x25], 7
        jb     loc_1_0B87
loc_1_0B7E:
        mov    byte ptr [esi + 0x28], al
        mov    byte ptr [esi + 0x29], 0
        jmp    loc_1_0B8A
loc_1_0B87:
        mov    byte ptr [esi + 0x29], al
loc_1_0B8A:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0B93
        out    dx, al
loc_1_0B93:
        pop    esi
        ret    
loc_1_0B95:
        cmp    ecx, 4
        jbe    loc_1_0BA0
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0BA0:
        je     loc_1_0BA4
        in     al, dx
        ret    
loc_1_0BA4:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0BB6
        out    dx, al
loc_1_0BB6:
        mov    ah, al
        and    ah, 0xc0
        je     loc_1_0BD7
        cmp    ah, 0x40
        je     loc_1_0BC4
        pop    esi
        ret    
loc_1_0BC4:
        mov    byte ptr [esi + 0x25], al
        not    al
        or     byte ptr [esi + 0x45], 2
        test   al, 0x30
        jne    loc_1_0BD5
        xor    byte ptr [esi + 0x45], 2
loc_1_0BD5:
        pop    esi
        ret    
loc_1_0BD7:
        mov    byte ptr [esi + 0x24], al
        not    al
        or     byte ptr [esi + 0x45], 1
        test   al, 0x30
        jne    loc_1_0BE8
        xor    byte ptr [esi + 0x45], 1
loc_1_0BE8:
        pop    esi
        ret    
loc_1_0BEA:
        cmp    ecx, 4
        jbe    loc_1_0BF5
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0BF5:
        je     loc_1_0BFA
        in     al, dx
loc_1_0BF8:
        clc    
        ret    
loc_1_0BFA:
        jmp    loc_1_0BF8
loc_1_0BFC:
        cmp    ecx, 4
        jbe    loc_1_0C07
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0C07:
        je     loc_1_0C17
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x38]
        pop    esi
        ret    
loc_1_0C17:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0C27
        call   sub_1_0470
        jae    loc_1_0C27
        ret    
loc_1_0C27:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    byte ptr [esi + 0x38], al
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0C3C
        out    dx, al
loc_1_0C3C:
        pop    esi
        ret    
loc_1_0C3E:
        cmp    ecx, 4
        jbe    loc_1_0C49
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0C49:
        je     loc_1_0C59
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    al, byte ptr [esi + 0x40]
        pop    esi
        ret    
loc_1_0C59:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0C69
        call   sub_1_0470
        jae    loc_1_0C69
        ret    
loc_1_0C69:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    byte ptr [esi + 0x40], al
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0C7E
        out    dx, al
loc_1_0C7E:
        pop    esi
        ret    
loc_1_0C80:
        xchg   ebx, edx
        lea    ebx, [0]                      ; -> obj1:0x018D
        xlatb  
        movzx  eax, al
        lea    ebx, [0]                      ; -> obj1:0x0145
        xchg   ebx, edx
        call   word ptr [eax + edx]
        ret    
loc_1_0C99:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0CA9
        call   sub_1_0470
        jae    loc_1_0CA9
        ret    
loc_1_0CA9:
        mov    al, 0x10
        call   sub_1_0F53
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0CBC
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0CBC:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0CC9
        mov    dx, 0xf88
        out    dx, al
loc_1_0CC9:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
loc_1_0CD0:
        and    byte ptr [esi + 0x18], 0x7f
loc_1_0CD4:
        ret    
loc_1_0CD5:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0CF1
        and    byte ptr [esi + 0x18], 0x7f
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0CF0
        call   sub_1_0470
        jae    loc_1_0CF0
        ret    
loc_1_0CF0:
        ret    
loc_1_0CF1:
        mov    byte ptr [esi + 0x28], al
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0D00
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0D00:
        mov    byte ptr [esi + 0x29], al
        call   sub_1_0FC3
        call   sub_1_1007
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0D19:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0D29
        call   sub_1_0470
        jae    loc_1_0D29
        ret    
loc_1_0D29:
        mov    al, 0
        call   sub_1_0F53
        mov    dx, 0xf88
        in     al, dx
        mov    byte ptr [esi + 0x19], al
        or     byte ptr [esi + 0x1a], 0x80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0D41:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0D5D
        and    byte ptr [esi + 0x18], 0x7f
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0D5C
        call   sub_1_0470
        jae    loc_1_0D5C
        ret    
loc_1_0D5C:
        ret    
loc_1_0D5D:
        mov    byte ptr [esi + 0x28], al
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0D6C
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0D6C:
        mov    byte ptr [esi + 0x29], al
        call   sub_1_0FE5
        call   sub_1_1007
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0D85:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0D91
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0D91:
        call   sub_1_0F3B
        call   sub_1_10BE
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0DA7:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0DC3
        and    byte ptr [esi + 0x18], 0x7f
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0DC2
        call   sub_1_0470
        jae    loc_1_0DC2
        ret    
loc_1_0DC2:
        ret    
loc_1_0DC3:
        mov    byte ptr [esi + 0x28], al
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0DD2
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0DD2:
        mov    byte ptr [esi + 0x29], al
        call   sub_1_0FE5
        call   sub_1_1007
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
sub_1_0DEB:
        push   eax
        push   edx
        and    byte ptr [esi + 0x2a], 0xbf
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0E0B
        mov    dx, 0xf8a
        mov    al, byte ptr [esi + 0x2b]
        and    al, 0x3f
        out    dx, al
        mov    al, byte ptr [esi + 0x2a]
        mov    dx, 0xb8a
        out    dx, al
loc_1_0E0B:
        and    byte ptr [esi + 0x44], 0xdf
        and    byte ptr [esi + 0x18], 0x7f
        pop    edx
        pop    eax
        ret    
loc_1_0E16:
        or     byte ptr [esi + 0x2b], 0x10
        or     byte ptr [esi + 0x44], 2
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0E35
        mov    al, byte ptr [esi + 0x2b]
        mov    dx, 0xf8a
        out    dx, al
        mov    al, 0x29
        call   sub_1_10E6
loc_1_0E35:
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0E3A:
        and    byte ptr [esi + 0x44], 0xfd
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0E4D
        sub    eax, eax
        call   sub_1_10E6
loc_1_0E4D:
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0E52:
        or     byte ptr [esi + 0x2a], 0x40
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0E6E
        mov    dx, 0xf8a
        mov    al, byte ptr [esi + 0x2b]
        out    dx, al
        mov    al, byte ptr [esi + 0x2a]
        mov    dx, 0xb8a
        out    dx, al
loc_1_0E6E:
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0E73:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0E7F
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0E7F:
        not    al
        mov    byte ptr [esi + 0x19], al
        or     byte ptr [esi + 0x1a], 0x80
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0E94:
        mov    byte ptr [esi + 0x19], 1
        or     byte ptr [esi + 0x1a], 0x80
        mov    byte ptr [esi + 0x1b], 1
        mov    dword ptr [esi + 0x10], 0     ; -> loc_1_0EAC
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0EAC:
        mov    byte ptr [esi + 0x19], 5
        or     byte ptr [esi + 0x1a], 0x80
        mov    byte ptr [esi + 0x1b], 0
        mov    dword ptr [esi + 0x10], 0     ; -> loc_1_0C80
        ret    
loc_1_0EC0:
        and    byte ptr [esi + 0x18], 0x7f
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0EDC
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_0EDB
        call   sub_1_0470
        jae    loc_1_0EDB
        ret    
loc_1_0EDB:
        ret    
loc_1_0EDC:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0EE8:
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0EF4
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0EF4:
        mov    byte ptr [esi + 0x1c], al
        mov    dword ptr [esi + 0xc], 0      ; -> loc_1_0C80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0F03:
        mov    al, byte ptr [esi + 0x1c]
        mov    byte ptr [esi + 0x19], al
        or     byte ptr [esi + 0x1a], 0x80
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0F12:
        call   sub_1_0F29
        and    byte ptr [esi + 0x18], 0x7f
        ret    
loc_1_0F1C:
        mov    byte ptr [esi + 0x1d], 0x32
        mov    byte ptr [esi + 0x19], 0
        and    byte ptr [esi + 0x18], 0x7f
        ret    
sub_1_0F29:
        push   ebx
        or     byte ptr [esi + 0x44], 8
        mov    eax, dword ptr [0]            ; -> obj1:0x00F7
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    ebx
        ret    
sub_1_0F3B:
        push   eax
        mov    ah, 0x24
        cmp    al, 0x59
        jb     loc_1_0F4A
        mov    ah, 0x39
        cmp    al, 0xa5
        jb     loc_1_0F4A
        mov    ah, 0x31
loc_1_0F4A:
        and    byte ptr [esi + 0x2a], 0xc0
        or     byte ptr [esi + 0x2a], ah
        pop    eax
        ret    
sub_1_0F53:
        mov    byte ptr [esi + 0x20], 0xff
        mov    ah, al
        xchg   byte ptr [esi + 0x21], al
        cmp    al, ah
        je     loc_1_0FC2
        mov    dx, 0xb8b
        in     al, dx
        or     al, 0xc
        mov    byte ptr [esi + 0x2d], al
        out    dx, al
        mov    al, 0xa5
        call   sub_1_0F3B
        call   sub_1_10BE
        or     al, byte ptr [esi + 0x21]
        or     al, 0x69
        mov    byte ptr [esi + 0x2b], al
        and    byte ptr [esi + 0x2a], 0x3f
        or     byte ptr [esi + 0x2a], 0x40
        mov    al, byte ptr [esi + 0x2a]
        and    byte ptr [esi + 0x44], 0xcf
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0FC2
        sub    eax, eax
        call   sub_1_10E6
        mov    dx, 0xb8b
        mov    al, byte ptr [esi + 0x2d]
        out    dx, al
        mov    ax, word ptr [esi + 0x26]
        mov    byte ptr [esi + 0x24], 0x36
        call   sub_1_108C
        mov    al, byte ptr [esi + 0x2b]
        mov    dx, 0xf8a
        out    dx, al
        mov    al, byte ptr [esi + 0x2a]
        mov    dx, 0xb8a
        out    dx, al
loc_1_0FC2:
        ret    
sub_1_0FC3:
        mov    byte ptr [esi + 0x21], 0xff
        cmp    byte ptr [esi + 0x20], 0x10
        mov    byte ptr [esi + 0x20], 0x10
        je     loc_1_0FE4
        or     byte ptr [esi + 0x44], 2
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0FE4
        mov    al, 0x29
        call   sub_1_10E6
loc_1_0FE4:
        ret    
sub_1_0FE5:
        mov    byte ptr [esi + 0x21], 0xff
        cmp    byte ptr [esi + 0x20], 0
        mov    byte ptr [esi + 0x20], 0
        je     loc_1_1006
        and    byte ptr [esi + 0x44], 0xfd
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_0FE4
        sub    eax, eax
        call   sub_1_10E6
loc_1_1006:
        ret    
sub_1_1007:
        cmp    byte ptr [esi + 0x20], 0xff
        je     loc_1_1025
        mov    ax, word ptr [0]              ; -> loc_1_00FB
        add    ax, 2
        cmp    ax, word ptr [esi + 0x28]
        jge    loc_1_1025
        sub    ax, 2
        sub    word ptr [esi + 0x28], ax
loc_1_1025:
        mov    al, byte ptr [esi + 0x2d]
        and    al, 0xf3
        or     al, 8
        mov    byte ptr [esi + 0x2d], al
        mov    al, byte ptr [esi + 0x20]
        or     al, 0xe9
        mov    byte ptr [esi + 0x2b], al
        or     byte ptr [esi + 0x44], 0x20
        or     byte ptr [esi + 0x2a], 0xc0
        mov    al, byte ptr [esi + 0x2a]
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_108B
        mov    eax, dword ptr [0]            ; -> obj1:0x0136
        mov    ax, word ptr [esi + 0x26]
        mov    byte ptr [esi + 0x24], 0x36
        call   sub_1_108C
        mov    ax, word ptr [esi + 0x28]
        inc    ax
        cmp    ax, 2
        adc    ax, 0
        mov    byte ptr [esi + 0x25], 0x74
        call   sub_1_10A5
        mov    dx, 0xb8b
        mov    al, byte ptr [esi + 0x2d]
        out    dx, al
        mov    al, byte ptr [esi + 0x2b]
        mov    dx, 0xf8a
        out    dx, al
        mov    al, byte ptr [esi + 0x2a]
        mov    dx, 0xb8a
        out    dx, al
loc_1_108B:
        ret    
sub_1_108C:
        push   eax
        mov    al, byte ptr [esi + 0x24]
        and    al, 0x7f
        mov    dx, 0x138b
        out    dx, al
        pop    eax
        mov    dx, 0x1388
        out    dx, al
        jmp    loc_1_109F
loc_1_109F:
        xchg   al, ah
        out    dx, al
        xchg   al, ah
        ret    
sub_1_10A5:
        push   eax
        mov    al, byte ptr [esi + 0x25]
        and    al, 0x7f
        mov    dx, 0x138b
        out    dx, al
        pop    eax
        mov    dx, 0x1389
        out    dx, al
        xchg   al, ah
        jmp    loc_1_10BA
loc_1_10BA:
        out    dx, al
        xchg   al, ah
        ret    
sub_1_10BE:
        push   ebx
        mov    bx, 0x100
        sub    ah, ah
        sub    bx, ax
        mov    dx, 0xf
        mov    ax, 0x4240
        div    bx
        mov    dx, 0x12
        mov    bx, 0x34dc
        xchg   bx, ax
        div    bx
        mov    word ptr [esi + 0x26], ax
        pop    ebx
        ret    
sub_1_10E6:
        cmp    al, byte ptr [esi + 0x2c]
        mov    byte ptr [esi + 0x2c], al
        je     loc_1_10FE
        push   eax
        mov    ah, 0xd
        call   sub_1_10FF
        pop    eax
        mov    ah, 6
        call   sub_1_10FF
loc_1_10FE:
        ret    
sub_1_10FF:
        test   dword ptr [0], 0x30           ; -> obj1:0x00FF
        je     loc_1_110C
        ret    
loc_1_110C:
        push   eax
        or     ah, 0x80
        or     ah, 0x40
        call   sub_1_1124
        pop    eax
        mov    ah, al
        or     ah, 0x40
        call   sub_1_1124
        ret    
sub_1_1124:
        pushfd 
        cli    
        mov    al, byte ptr [0]              ; -> obj1:0x0118
        or     al, 0x3f
        mov    ecx, 8
        mov    dx, 0xb88
        out    dx, al
loc_1_1137:
        xor    al, 2
        jmp    loc_1_113B
loc_1_113B:
        out    dx, al
        ror    al, 1
        ror    ah, 1
        adc    al, al
        out    dx, al
        xor    al, 2
        jmp    loc_1_1147
loc_1_1147:
        out    dx, al
        loop   loc_1_1137
        xor    al, 4
        out    dx, al
        jmp    loc_1_114F
loc_1_114F:
        xor    al, 4
        out    dx, al
        mov    byte ptr [0], al              ; -> obj1:0x0118
        popfd  
        ret    
        db  0CCh, 0CCh, 0CCh
loc_1_115C:
        push   esi
        push   ebx
        push   edx
        push   eax
        mov    edx, 0xb8b
        in     al, dx
        mov    ah, 0x1f
        and    ah, al
        mov    edx, 0xb89
        in     al, dx
        and    ah, al
        je     loc_1_11A8
        cmp    word ptr [0], 0               ; -> obj1:0x00ED
        jne    loc_1_11A8
        mov    ebx, dword ptr [0]            ; -> obj1:0x0136
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jae    loc_1_1194
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        jmp    loc_1_11A8
loc_1_1194:
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    edx
        pop    ebx
        pop    esi
        clc    
        ret    
loc_1_11A8:
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        pop    edx
        pop    ebx
        pop    esi
        clc    
        ret    
loc_1_11B4:
        ret    
loc_1_11B5:
        push   esi
        push   ebx
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_11D9
        test   byte ptr [esi + 0x44], 8
        jne    loc_1_11D9
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_1_11D9:
        and    byte ptr [esi + 0x44], 0xc7
        pop    ebx
        pop    esi
        ret    
loc_1_11E0:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        jecxz  loc_1_11F1
        or     byte ptr [esi + 0x44], 4
        pop    esi
        ret    
loc_1_11F1:
        and    byte ptr [esi + 0x44], 0xfb
        test   byte ptr [esi + 0x44], 8
        je     loc_1_1208
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00F7
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    eax
loc_1_1208:
        pop    esi
        ret    
loc_1_120A:
        ret    
loc_1_120B:
        cmp    ecx, 4
        jbe    loc_1_1216
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1216:
        je     loc_1_121A
        in     al, dx
        ret    
loc_1_121A:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_122A
        call   sub_1_0470
        jae    loc_1_122A
        ret    
loc_1_122A:
        call   sub_1_129A
        call   sub_1_136A
        out    dx, al
        mov    byte ptr [0], 1               ; -> loc_1_0113
        ret    
loc_1_123D:
        cmp    ecx, 4
        jbe    loc_1_1248
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1248:
        je     loc_1_124C
        in     al, dx
        ret    
loc_1_124C:
        out    dx, al
        mov    byte ptr [0], 1               ; -> loc_1_0113
        ret    
loc_1_1255:
        cmp    ecx, 4
        jbe    loc_1_1260
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1260:
        je     loc_1_1264
        in     al, dx
        ret    
loc_1_1264:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_1_1274
        call   sub_1_0470
        jae    loc_1_1274
        ret    
loc_1_1274:
        call   sub_1_129A
        out    dx, al
        mov    byte ptr [0], 1               ; -> loc_1_0113
        ret    
loc_1_1282:
        cmp    ecx, 4
        jbe    loc_1_128D
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_128D:
        je     loc_1_1291
        in     al, dx
        ret    
loc_1_1291:
        out    dx, al
        mov    byte ptr [0], 1               ; -> loc_1_0113
        ret    
sub_1_129A:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    byte ptr [esi + 0x2e], 0xff
        mov    edx, 6
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    edx, 0xa
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    edx, 0xc
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    edx, 0xe
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_12FE
        mov    ax, 0xff
        call   sub_1_138C
loc_1_12FE:
        pop    edx
        pop    eax
        pop    esi
        ret    
sub_1_1302:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    byte ptr [esi + 0x2e], 0
        mov    edx, 6
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    edx, 0xa
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    edx, 0xc
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    edx, 0xe
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_1_1366
        mov    ax, 0
        call   sub_1_138C
loc_1_1366:
        pop    edx
        pop    eax
        pop    esi
        ret    
sub_1_136A:
        push   eax
        push   edx
        mov    edx, dword ptr [0]            ; -> loc_1_0103
        add    dl, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        sub    eax, eax
        out    dx, al
loc_1_137F:
        in     al, dx
        dec    ah
        jne    loc_1_137F
        add    dx, 4
        in     al, dx
        pop    edx
        pop    eax
        ret    
sub_1_138C:
        push   ecx
        push   edx
        mov    byte ptr [0], al              ; -> obj1:0x0119
        test   dword ptr [0], 0x200          ; -> obj1:0x00FF
        je     loc_1_13F2
        cbw    
        and    ah, 2
        mov    dx, 0xf788
        in     al, dx
        and    al, 0xfd
        or     al, ah
        out    dx, al
        mov    dx, 0xf789
        in     al, dx
        and    al, 0xf0
        mov    ah, byte ptr [0]              ; -> loc_1_0103
        mov    cl, 4
        shr    ah, cl
        and    ah, byte ptr [0]              ; -> obj1:0x0119
        or     al, ah
        out    dx, al
        mov    dx, word ptr [0]              ; -> loc_1_0103
        add    dx, 6
        mov    al, 1
        out    dx, al
        jmp    loc_1_13D8
loc_1_13D8:
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        sub    ax, ax
        out    dx, al
        mov    ah, 2
        and    ah, byte ptr [0]              ; -> obj1:0x0119
        mov    dx, 0xb8b
        in     al, dx
        and    al, 0xfd
        or     al, ah
        out    dx, al
loc_1_13F2:
        pop    edx
        pop    ecx
        ret    
        db  0CCh, 0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xb7  R-X
        db  002h, 000h, 000h, 000h
        dd  obj2:0x0014              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        jg     loc_2_0010
loc_2_0010:
        mov    dword ptr [0x54cccc00], eax
        push   0x72502065
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + 0x75], al
        imul   ebp, dword ptr fs:[edi + 0x20], 0x63657053
        je     0x9a
        jne    0x97
        and    byte ptr [eax + 0x61], ch
        jb     0x93
        ja     0x92
        jb     0x98
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6c], ah
        jb     0xa0
        popal  
        jns    0x5f
        imul   ebp, dword ptr [esi + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xb6
        jb     0x71
        popal  
        jo     0xc4
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        and    byte ptr cs:[ecx + 0x75], al
        imul   ebp, dword ptr fs:[edi + 0x20], 0x70707573
        outsd  dx, dword ptr [esi]
        jb     0xdf
        and    byte ptr [edi + 0x69], dh
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x6f], ch
        je     loc_2_0095
        bound  esp, qword ptr [ebp + 0x20]
        popal  
        jbe    0xdc
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0xaa
        jo     0xfe
        outsd  dx, dword ptr [esi]
        jb     0xf1
        insd   dword ptr es:[edi], dx
        add    byte ptr cs:[ebp + 0x65], cl
loc_2_0095:
        imul   esp, dword ptr fs:[ecx + 0x56], 0x6f697369
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + 0x41], dl
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0x11c
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x11a
        jb     0xb7
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2c  R-X
loc_3_0000:
        mov    eax, 0x2f
        mov    esi, 0                        ; -> loc_5_05DC
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_4_03B8
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        mov    dword ptr [0], 0              ; -> obj6:0x0000
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x8ae  R-X
        db  060h, 0A1h
        dd  obj7:0x006C              ; fixup
        db  0C1h, 0E8h, 00Ah, 083h, 0F8h, 040h, 07Eh, 005h, 0B8h, 040h, 000h, 000h, 000h, 083h, 0C0h, 003h
        db  0C1h, 0E8h, 002h, 08Bh, 0D8h, 048h, 066h, 00Fh, 0BDh, 0C8h, 0FEh, 0C1h, 0B8h, 001h, 000h, 000h
        db  000h, 0D3h, 0E0h, 048h, 08Bh, 0CBh, 051h, 0BFh
        dd  obj7:0x0060              ; fixup
        db  06Ah, 00Eh, 057h, 068h, 0FFh, 00Fh, 000h, 000h, 06Ah, 000h, 050h, 06Ah, 000h, 06Ah, 001h, 051h
        db  0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 059h, 00Bh, 0C0h, 075h, 00Eh, 08Bh, 0C1h
        db  0D1h, 0E8h, 075h, 0C3h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 0EBh, 034h, 0A3h
        dd  obj1:0x00E0              ; fixup
        db  089h, 015h
        dd  obj7:0x0064              ; fixup
        db  0C1h, 0E1h, 00Ch, 06Ah, 000h, 06Ah, 000h, 068h, 092h, 000h, 000h, 000h, 051h, 052h, 0CDh, 020h
        db  07Ah, 000h, 001h, 000h, 083h, 0C4h, 014h, 06Ah, 000h, 050h, 052h, 0CDh, 020h, 076h, 000h, 001h
        db  000h, 083h, 0C4h, 00Ch, 0A3h
        dd  obj7:0x0068              ; fixup
        db  061h, 0C3h
sub_4_0094:
        push   edi
        lea    edi, [0]                      ; -> obj1:0x0122
        call   sub_1_0716
loc_4_00A0:
        pop    edi
        ret    
sub_4_00A2:
        push   edi
        mov    edi, 0                        ; -> obj7:0x0070
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        test   dword ptr [0], 0x10           ; -> obj1:0x00FF
        jne    loc_4_00CE
        mov    dword ptr [0], 5              ; -> obj1:0x011A
        mov    dword ptr [0], 0x23           ; -> obj1:0x011E
loc_4_00CE:
        pop    edi
        ret    
sub_4_00D0:
        push   esi
        sub    eax, eax
        mov    edx, 0xb8a
        mov    esi, 0                        ; -> loc_1_09D8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xb8b
        out    dx, al
        mov    esi, 0                        ; -> loc_1_0A65
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xf8a
        mov    esi, 0                        ; -> loc_1_0AC3
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x1388
        mov    esi, 0                        ; -> loc_1_0B1B
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x1389
        mov    esi, 0                        ; -> loc_1_0B58
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x138b
        mov    esi, 0                        ; -> loc_1_0B95
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xf388
        mov    esi, 0                        ; -> loc_1_0BEA
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xf389
        mov    esi, 0                        ; -> loc_1_0BEA
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xf38a
        mov    esi, 0                        ; -> loc_1_0BEA
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x8388
        mov    esi, 0                        ; -> loc_1_0BFC
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0x838b
        mov    esi, 0                        ; -> loc_1_0BEA
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xbf8a
        mov    esi, 0                        ; -> loc_1_0C3E
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        pop    esi
        ret    
sub_4_0196:
        push   esi
        test   dword ptr [0], 8              ; -> obj1:0x00FF
        je     loc_4_01C7
        mov    eax, dword ptr [0]            ; -> loc_1_010F
        or     eax, eax
        je     loc_4_01C7
        push   eax
        mov    edx, eax
        mov    esi, 0                        ; -> loc_1_09B8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        pop    edx
        inc    edx
        mov    esi, 0                        ; -> loc_1_09C8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
loc_4_01C7:
        pop    esi
        ret    
sub_4_01C9:
        push   esi
        test   dword ptr [0], 2              ; -> obj1:0x00FF
        je     loc_4_02E2
        test   dword ptr [0], 0x40           ; -> obj1:0x00FF
        jne    loc_4_026B
        test   dword ptr [0], 0x200          ; -> obj1:0x00FF
        je     loc_4_0257
        cmp    dword ptr [0], 0              ; -> loc_1_0103
        je     loc_4_0257
        mov    edx, 6
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_120B
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xa
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_123D
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xc
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_1255
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xe
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_1282
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
loc_4_0257:
        mov    edi, 0                        ; -> obj7:0x0040
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_4_02E2
        mov    dword ptr [0], eax            ; -> obj1:0x00F7
        jmp    loc_4_02D5
loc_4_026B:
        mov    edx, 6
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_0874
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xa
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_0962
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xc
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_08FE
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edx, 0xe
        add    edx, dword ptr [0]            ; -> loc_1_0103
        mov    esi, 0                        ; -> loc_1_098B
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        mov    edi, 0                        ; -> obj7:0x0000
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_4_02E2
        mov    dword ptr [0], eax            ; -> obj1:0x00F7
loc_4_02D5:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        or     dword ptr [0], 4              ; -> obj1:0x00FF
loc_4_02E2:
        pop    esi
        ret    
sub_4_02E4:
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_1_0107
        mov    word ptr [0], ax              ; -> obj7:0x0000
        mov    word ptr [0], ax              ; -> obj7:0x0020
        mov    word ptr [0], ax              ; -> obj7:0x0040
        pop    esi
        ret    
sub_4_02FE:
        mov    eax, dword ptr [0]            ; -> obj1:0x0136
        or     eax, dword ptr [0]            ; -> obj1:0x012E
        jne    loc_4_0312
        mov    eax, 0
        jmp    loc_4_0317
loc_4_0312:
        mov    eax, 0xffffffff
loc_4_0317:
        ret    
loc_4_0318:
        push   esi
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_03AD
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> obj1:0x00F3
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_4_033F
        clc    
        mov    ecx, 0x23
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_4_033F:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_4_0350
        clc    
        mov    ecx, dword ptr [esi + 8]
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_4_0350:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_4_0361
        push   ebx
        xor    ebx, ebx
        call   sub_1_065E
loc_4_0360:
        pop    ebx
loc_4_0361:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x012E
        jne    loc_4_0389
        call   sub_5_006E
loc_4_036E:
        call   sub_5_0021
loc_4_0373:
        mov    dword ptr [0], 0              ; -> obj1:0x012E
        mov    dword ptr [0], 0              ; -> obj1:0x0132
        jmp    loc_4_03AD
loc_4_0389:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0132
        jne    loc_4_039B
        mov    dword ptr [0], 0              ; -> obj1:0x0132
loc_4_039B:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x013A
        jne    loc_4_03AD
        mov    dword ptr [0], 0              ; -> obj1:0x013A
loc_4_03AD:
        pop    esi
        clc    
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  obj1:0x00D0              ; fixup
loc_4_03B8:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_03EE
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        jne    loc_4_03D3
        push   ebx
        xor    ebx, ebx
        call   sub_1_065E
loc_4_03D2:
        pop    ebx
loc_4_03D3:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x012E
        jne    loc_4_03EE
        call   sub_5_0021
loc_4_03E0:
        xor    ebx, ebx
        xchg   dword ptr [0], ebx            ; -> obj1:0x012E
        mov    dword ptr [0], ebx            ; -> obj1:0x0132
loc_4_03EE:
        popfd  
        popal  
        jmp    dword ptr [0]                 ; -> obj1:0x00D0
sub_4_03F6:
        mov    eax, dword ptr [0]            ; -> obj1:0x00F3
        mov    esi, ebx
        add    esi, eax
        call   sub_4_04A9
        call   sub_4_04D2
        call   sub_4_0414
        call   sub_4_0422
        ret    
sub_4_0414:
        push   edi
        mov    edi, 0                        ; -> obj7:0x0070
        VMMCall  VMM.Remove_Mult_IO_Handlers     ; cd2017010100
        pop    edi
        ret    
sub_4_0422:
        push   esi
        mov    edx, 0xb8a
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xb8b
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xf8a
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0x1388
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0x1389
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0x138b
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0x8388
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0x838b
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xf388
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xf389
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xf38a
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xbf8a
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        pop    esi
        ret    
sub_4_04A9:
        push   esi
        test   dword ptr [0], 8              ; -> obj1:0x00FF
        je     loc_4_04D0
        mov    eax, dword ptr [0]            ; -> loc_1_010F
        or     eax, eax
        je     loc_4_04D0
        push   eax
        mov    edx, eax
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        pop    edx
        inc    edx
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
loc_4_04D0:
        pop    esi
        ret    
sub_4_04D2:
        push   esi
        test   dword ptr [0], 2              ; -> obj1:0x00FF
        je     loc_4_0597
        test   dword ptr [0], 0x40           ; -> obj1:0x00FF
        jne    loc_4_054A
        test   dword ptr [0], 0x200          ; -> obj1:0x00FF
        je     loc_4_0548
        cmp    dword ptr [0], 0              ; -> loc_1_0103
        je     loc_4_0548
        mov    edx, 6
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xa
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xc
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xe
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
loc_4_0548:
        jmp    loc_4_0597
loc_4_054A:
        mov    edx, 6
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xa
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xc
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    edx, 0xe
        add    edx, dword ptr [0]            ; -> loc_1_0103
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        or     dword ptr [0], 0xfffffffb     ; -> obj1:0x00FF
        jmp    loc_4_0597
loc_4_0597:
        pop    esi
        ret    
        db  02Eh, 08Bh, 0C0h
loc_4_059C:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
sub_4_05A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj1:0x00D4
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [0], eax            ; -> obj1:0x00D8
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> obj1:0x00E4
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_0107
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0], eax            ; -> loc_1_010B
        movsx  eax, word ptr [ebp + 0x18]
        cmp    eax, -1
        jne    loc_4_05DC
        xor    eax, eax
loc_4_05DC:
        mov    dword ptr [0], eax            ; -> loc_1_0103
        mov    eax, dword ptr [ebp + 0x1c]
        shl    eax, 0xa
        mov    dword ptr [0], eax            ; -> obj7:0x006C
        mov    word ptr [0], 0               ; -> obj1:0x00ED
        call   sub_4_0790
        jb     loc_4_063F
        movzx  eax, word ptr [ebp + 0x14]
        mov    esi, 0                        ; -> loc_4_059C
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [0], eax            ; -> obj1:0x00DC
        jb     loc_4_063F
        mov    dword ptr [esp + 0x1c], 0xffffffff
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
        call   sub_1_03FE
loc_4_0626:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        je     loc_4_0638
        call   sub_1_0456
loc_4_0636:
        jmp    loc_4_0626
loc_4_0638:
        pop    ebx
loc_4_0639:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_063F:
        call   sub_4_064E
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_0639
sub_4_064E:
        pushal 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        call   sub_4_03F6
        mov    eax, dword ptr [0]            ; -> obj1:0x00DC
        or     eax, eax
        je     loc_4_0669
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0669:
        mov    eax, dword ptr [0]            ; -> obj7:0x0068
        or     eax, eax
        je     loc_4_068F
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0x00E0
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_068F:
        mov    eax, dword ptr [0]            ; -> obj1:0x00F7
        or     eax, eax
        je     loc_4_069E
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_069E:
        popal  
        ret    
sub_4_06A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_05A2
        add    esp, 0x1c
        or     eax, eax
        je     loc_4_0700
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [0], 0              ; -> obj1:0x012E
        mov    dword ptr [0], 0              ; -> obj1:0x0132
        mov    dword ptr [0], 0              ; -> obj1:0x0136
        mov    dword ptr [0], 0              ; -> obj1:0x013A
loc_4_0700:
        popal  
        add    esp, 4
        pop    ebp
        ret    
        db  0C7h, 044h, 024h, 01Ch, 024h, 000h, 000h, 000h, 0EBh, 0F0h
sub_4_0710:
        push   ebp
        mov    ebp, esp
        pushfd 
        pushal 
        cmp    dword ptr [0], 0              ; -> obj1:0x0136
        je     loc_4_0727
        push   ebx
        xor    ebx, ebx
        call   sub_1_065E
loc_4_0726:
        pop    ebx
loc_4_0727:
        cmp    dword ptr [0], 0              ; -> obj1:0x012E
        je     loc_4_074E
        call   sub_5_006E
loc_4_0735:
        mov    ebx, dword ptr [0]            ; -> obj1:0x012E
        call   sub_5_0021
loc_4_0740:
        xor    ebx, ebx
        xchg   dword ptr [0], ebx            ; -> obj1:0x012E
        mov    dword ptr [0], ebx            ; -> obj1:0x0132
loc_4_074E:
        call   sub_4_064E
        mov    dword ptr [esp + 0x1c], 0xffffffff
        popal  
        popfd  
        pop    ebp
        ret    
loc_4_075F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_03D0
loc_4_0776:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_077C:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_075F
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
sub_4_0790:
        mov    edx, dword ptr [0]            ; -> obj1:0x00E4
        xor    dx, 0x388
        xor    dx, 0xb8b
        in     al, dx
        cmp    al, 0xff
        je     loc_4_08AC
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        mov    byte ptr [0], al              ; -> obj1:0x00E8
        cmp    al, 7
        je     loc_4_07C4
        cmp    al, 5
        ja     loc_4_08AC
loc_4_07C4:
        in     al, dx
        mov    ah, al
        xor    al, 0xe0
        out    dx, al
        in     al, dx
        and    ax, 0xe0e0
        cmp    al, ah
        jne    loc_4_08AC
        mov    al, byte ptr [0]              ; -> obj1:0x00E8
        cmp    al, 0
        je     loc_4_07F9
        cmp    al, 1
        je     loc_4_0803
        cmp    al, 3
        je     loc_4_082E
        cmp    al, 4
        je     loc_4_0844
        cmp    al, 5
        je     loc_4_0854
        cmp    al, 7
        je     loc_4_0864
        jmp    loc_4_08AC
loc_4_07F9:
        mov    ebx, 1
        or     ebx, 2
        jmp    loc_4_0872
loc_4_0803:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 0x10
        or     ebx, 0x20
        mov    edx, dword ptr [0]            ; -> obj1:0x00E4
        xor    dx, 0x388
        xor    dx, 0xef8b
        in     al, dx
        test   al, 8
        jne    loc_4_0872
        or     ebx, 0x200
        jmp    loc_4_0872
loc_4_082E:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 0x10
        or     ebx, 0x20
        or     ebx, 0x100
        jmp    loc_4_0872
loc_4_0844:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 0x10
        or     ebx, 0x20
        jmp    loc_4_0872
loc_4_0854:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 0x10
        or     ebx, 0x20
        jmp    loc_4_0872
loc_4_0864:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 0x10
        or     ebx, 0x20
loc_4_0872:
        mov    dword ptr [0], ebx            ; -> obj1:0x00FF
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [0]            ; -> obj1:0x00F3
        mov    esi, ebx
        add    esi, eax
        call   sub_4_0094
        call   sub_4_02E4
        call   0
        call   sub_4_0196
        call   sub_4_01C9
        call   sub_4_00A2
        call   sub_4_00D0
        clc    
        ret    
loc_4_08AC:
        stc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x14bd  R-X
sub_5_0000:
        push   esi
        push   ecx
        push   edx
        mov    esi, 0                        ; -> obj7:0x0070
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_5_000E:
        movzx  edx, word ptr [esi]
        add    esi, 6
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        loop   loc_5_000E
        pop    edx
        pop    ecx
        pop    esi
        clc    
        ret    
sub_5_0021:
        push   esi
        push   ecx
        push   edx
        mov    esi, 0                        ; -> obj7:0x0070
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_5_002F:
        movzx  edx, word ptr [esi]
        add    esi, 6
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        loop   loc_5_002F
        pop    edx
        pop    ecx
        pop    esi
        clc    
        ret    
sub_5_0042:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_5_005F
        pop    eax
        inc    dl
        out    dx, al
        call   sub_5_005F
        pop    edx
        pop    ebx
        ret    
sub_5_005F:
        push   eax
        push   ecx
        inc    edx
        mov    ecx, 0x32
loc_5_0067:
        in     al, dx
        loop   loc_5_0067
        dec    edx
        pop    ecx
        pop    eax
        ret    
sub_5_006E:
        push   eax
        push   edx
        push   ebx
        push   ecx
        mov    edx, 0x388
        mov    ebx, 0x105
        mov    al, 1
        call   sub_5_0042
        mov    ebx, 4
        mov    al, 0x60
        call   sub_5_0042
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_5_0042
        mov    ebx, 8
        xor    al, al
        call   sub_5_0042
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_5_0042
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_00BF:
        xor    bh, bh
        call   sub_5_0042
        mov    bh, 1
        call   sub_5_0042
        inc    bl
        loop   loc_5_00BF
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_00DD:
        xor    bh, bh
        call   sub_5_0042
        mov    bh, 1
        call   sub_5_0042
        inc    bl
        loop   loc_5_00DD
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_5_0042
        xor    eax, eax
        mov    ebx, 0x105
        call   sub_5_0042
        call   sub_5_005F
        call   sub_5_005F
        call   sub_5_005F
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
loc_5_011B:
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x012E
        cmp    eax, ebx
        je     loc_5_012C
        call   sub_1_04C8
loc_5_012A:
        jb     loc_5_013F
loc_5_012C:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0138
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0138:
        je     loc_5_013D
        in     al, dx
        jmp    loc_5_013E
loc_5_013D:
        out    dx, al
loc_5_013E:
        ret    
loc_5_013F:
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
vpasd_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 3
        jae    loc_5_016E
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj7:0x00D6
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_016E
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_016E
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_016E:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_0174:
        mov    word ptr [ebp + 0x1c], 0x400
        ret    
loc_5_017B:
        mov    eax, dword ptr [0]            ; -> obj7:0x0060
        mov    dword ptr [ebp + 0x1c], eax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_018C:
        mov    eax, dword ptr [0]            ; -> obj1:0x00DC
        push   ecx
        mov    ecx, 0xffff
        or     eax, eax
        je     loc_5_01A1
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
loc_5_01A1:
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        ret    
loc_5_01A8:
        clc    
        ret    
loc_5_01AA:
        mov    eax, dword ptr [0]            ; -> obj7:0x0064
        mov    dword ptr [ebp + 0x1c], eax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_01BB:
        mov    eax, dword ptr [0]            ; -> obj7:0x0068
        mov    dword ptr [ebp + 0x1c], eax
        clc    
        ret    
loc_5_01C5:
        mov    eax, dword ptr [0]            ; -> obj7:0x006C
        mov    dword ptr [ebp + 0x1c], eax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_01D6:
        cmp    dword ptr [0], 0              ; -> obj1:0x0136
        je     loc_5_01E8
        mov    dword ptr [ebp + 0x1c], 1
        jmp    loc_5_0213
loc_5_01E8:
        mov    dword ptr [0], ebx            ; -> obj1:0x0136
        test   dword ptr [0], 1              ; -> obj1:0x0114
        jne    loc_5_020C
        mov    eax, dword ptr [0]            ; -> obj1:0x00DC
        or     dword ptr [0], 1              ; -> obj1:0x0114
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_020C:
        mov    dword ptr [ebp + 0x1c], 0
loc_5_0213:
        clc    
        ret    
loc_5_0215:
        cmp    dword ptr [0], ebx            ; -> obj1:0x0136
        jne    loc_5_0245
        mov    dword ptr [0], 0              ; -> obj1:0x0136
        test   dword ptr [0], 1              ; -> obj1:0x0114
        je     loc_5_0245
        mov    eax, dword ptr [0]            ; -> obj1:0x00DC
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
        and    dword ptr [0], 0xfffffffe     ; -> obj1:0x0114
loc_5_0245:
        clc    
        ret    
loc_5_0247:
        movzx  edx, word ptr [ebp + 0x1c]
        mov    word ptr [ebp + 0x1c], 0
        or     edx, edx
        je     loc_5_0266
        call   sub_1_04C8
loc_5_025A:
        jb     loc_5_025E
        clc    
        ret    
loc_5_025E:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_026C
loc_5_0266:
        mov    word ptr [ebp + 0x1c], 1
loc_5_026C:
        stc    
        ret    
loc_5_026E:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_02A3
        mov    ecx, dword ptr [0]            ; -> obj1:0x012E
        cmp    ecx, ebx
        je     loc_5_0288
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_02A9
loc_5_0288:
        call   sub_5_0021
        xor    ebx, ebx
        xchg   dword ptr [0], ebx            ; -> obj1:0x012E
        mov    dword ptr [0], ebx            ; -> obj1:0x0132
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_02A3:
        mov    word ptr [ebp + 0x1c], 1
loc_5_02A9:
        stc    
        ret    
loc_5_02AB:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_02F8
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, dword ptr [0]            ; -> obj1:0x00D4
        jne    loc_5_02F8
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_02F8
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_02F8
        cmp    ecx, 0x1c
        ja     loc_5_02F8
        mov    word ptr [edi + 4], 1
        mov    word ptr [edi + 6], 0x388
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_02F8:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_0301:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        push   dword ptr [ebp + 0x3c]
        push   dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x3c]
        mov    eax, dword ptr [esi]
        mov    eax, 0                        ; -> obj1:0x013E
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_08BA
        add    esp, 0x10
        or     eax, eax
        je     loc_5_0378
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_0378:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_5_0386:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        mov    eax, 0                        ; -> obj1:0x013E
        lea    eax, [0]                      ; -> obj1:0x013E
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_0A4A
        add    esp, 8
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_03B3:
        mov    eax, dword ptr [0]            ; -> obj1:0x00E4
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [0]            ; -> loc_1_0107
        mov    word ptr [ebp + 0x18], ax
        mov    eax, dword ptr [0]            ; -> loc_1_010B
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
sub_5_03D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x20c
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_040E
        cmp    eax, 1
        je     loc_5_0415
        cmp    eax, 2
        je     loc_5_0571
        cmp    eax, 3
        je     loc_5_05AC
        cmp    eax, 4
        je     loc_5_0571
        mov    eax, 1
        jmp    loc_5_05D2
loc_5_040E:
        xor    eax, eax
        jmp    loc_5_05D2
loc_5_0415:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_5_0425
        mov    eax, 1
        jmp    loc_5_05D2
loc_5_0425:
        push   0
        lea    eax, [ebp - 0x20c]
        push   0x100
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        push   0
        push   esi
        call   sub_5_13DC
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_05D2
        push   0
        lea    eax, [ebp - 0x10c]
        push   esi
        push   eax
        call   sub_5_13D4
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_05D2
        cmp    word ptr [ebp - 0xb0], 2
        jae    loc_5_0477
        mov    eax, 0x13
        jmp    loc_5_05D2
loc_5_0477:
        cmp    word ptr [ebp - 0x84], 4
        je     loc_5_048B
        mov    eax, 0x13
        jmp    loc_5_05D2
loc_5_048B:
        cmp    word ptr [ebp - 0x5e], 1
        mov    cx, word ptr [ebp - 0xac]
        jb     loc_5_056A
        cmp    word ptr [ebp - 0x4e], 1
        jb     loc_5_056A
        movzx  di, byte ptr [ebp - 0x5c]
        movzx  ax, byte ptr [ebp - 0x4c]
        movzx  edx, word ptr [ebp - 0xae]
        mov    word ptr [ebp - 2], ax
        cmp    edx, -1
        jne    loc_5_04CC
        mov    eax, 0x13
        jmp    loc_5_05D2
loc_5_04CC:
        cmp    word ptr [ebp - 0xb0], 8
        jbe    loc_5_04E6
        cmp    word ptr [ebp - 0x76], 0x10
        jne    loc_5_04E6
        mov    bx, word ptr [ebp - 0x9e]
        jmp    loc_5_04EA
loc_5_04E6:
        mov    bx, 0xffff
loc_5_04EA:
        push   0
        push   0x20
        push   ebx
        push   dword ptr [ebp - 2]
        push   edi
        push   ecx
        push   esi
        call   sub_4_06A0
loc_5_04FA:
        add    esp, 0x1c
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_5_0556
        cmp    bx, 0xffff
        je     loc_5_0556
        cmp    di, 9
        mov    eax, 2
        je     loc_5_0519
        movzx  eax, di
loc_5_0519:
        movzx  ecx, word ptr [ebp - 2]
        movzx  edx, bx
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x34]
        push   0                             ; -> obj7:0x00F8
        push   ecx
        call   sub_5_13CC
        add    esp, 0x14
        lea    ecx, [ebp - 0x34]
        mov    dword ptr [ebp - 0x10], 0     ; -> obj7:0x00F0
        mov    dword ptr [ebp - 0x14], ecx
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_0556:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_5_0565
        push   esi
        call   sub_5_0DA8
        add    esp, 4
loc_5_0565:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_5_05D2
loc_5_056A:
        mov    eax, 0x13
        jmp    loc_5_05D2
loc_5_0571:
        cmp    dword ptr [0], 0              ; -> loc_1_0103
        je     loc_5_0593
        mov    dword ptr [ebp - 0xc], 0      ; -> obj7:0x00F0
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0004h              ; cd2004004a04
        add    esp, 4
loc_5_0593:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_5_0E57
        add    esp, 4
        push   esi
        call   sub_4_0710
loc_5_05A5:
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_05D2
loc_5_05AC:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_05BF
        cmp    eax, 1
        je     loc_5_05BF
        mov    eax, 1
        jmp    loc_5_05D2
loc_5_05BF:
        call   sub_4_02FE
loc_5_05C4:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_5_05D2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh, 0CCh
loc_5_05DC:
        cmp    byte ptr [ebp + 0x1d], 0xbc
        jne    loc_5_063B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_063B
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0136
        je     loc_5_05F9
        call   sub_1_0470
loc_5_05F7:
        jb     loc_5_0638
loc_5_05F9:
        cmp    byte ptr [ebp + 0x1c], 0
        jne    loc_5_061B
        cmp    word ptr [ebp + 0x10], 0x3f3f
        jne    loc_5_063B
        mov    word ptr [ebp + 0x10], 0x6d00
        mov    word ptr [ebp + 0x18], 0x76
        mov    word ptr [ebp + 0x14], 0x2020
        jmp    loc_5_0638
loc_5_061B:
        push   ecx
        movzx  ecx, byte ptr [ebp + 0x1c]
        cmp    ecx, 0x15
        jae    loc_5_063A
        call   sub_7_0108
        jb     loc_5_063A
        mov    word ptr [ebp + 0x1c], 0x4d56
        pop    ecx
loc_5_0638:
        clc    
        ret    
loc_5_063A:
        pop    ecx
loc_5_063B:
        stc    
        ret    
loc_5_063D:
        stc    
        ret    
loc_5_063F:
        mov    bx, 0x3034
        add    bx, 0x3030
        mov    word ptr [ebp + 0x10], bx
        mov    word ptr [ebp + 0x18], 0x3030
        xor    bx, bx
        mov    bl, byte ptr [0]              ; -> obj1:0x00E8
        mov    word ptr [ebp + 0x14], bx
        clc    
        ret    
loc_5_0661:
        push   ecx
        push   esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00F3
        add    esi, ebx
        cmp    dword ptr [esi], 0
        jne    loc_5_0680
        clc    
        mov    ecx, 0x23
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_5_06CC
        mov    dword ptr [esi], edi
loc_5_0680:
        mov    eax, dword ptr [esi]
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        mov    ax, word ptr [esi]
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x18], 0x23
        mov    edi, dword ptr [esi]
        shr    edi, 0xc
        and    edi, 0xffff0
        movzx  eax, word ptr [esi]
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        push   edi
        xor    al, al
        mov    ecx, 0x23
        rep stosb byte ptr es:[edi], al
        pop    edi
        mov    al, 0x80
        mov    byte ptr [edi + 0xc], al
        mov    al, byte ptr [esi + 0x2b]
        mov    byte ptr [edi + 0x12], al
        mov    al, byte ptr [esi + 0x2a]
        mov    byte ptr [edi + 0xe], al
        mov    al, byte ptr [esi + 0x2d]
        mov    byte ptr [edi + 0xf], al
        clc    
loc_5_06CC:
        pop    esi
        pop    ecx
        ret    
loc_5_06CF:
        pushal 
        mov    eax, dword ptr [0]            ; -> obj6:0x0004
        or     eax, eax
        jne    loc_5_06EF
        mov    esi, 0                        ; -> loc_6_0008
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_5_0780
        mov    dword ptr [0], eax            ; -> obj6:0x0004
loc_5_06EF:
        mov    esi, dword ptr [0]            ; -> obj1:0x00F3
        add    esi, ebx
        cmp    dword ptr [esi + 4], 0
        jne    loc_5_0761
        clc    
        mov    ecx, 0xac
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_5_0780
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], ecx
        shr    edi, 0xc
        and    edi, 0xffff0
        movzx  eax, word ptr [esi + 4]
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [0]            ; -> obj6:0x0004
        stosd  dword ptr es:[edi], eax
        mov    ecx, 0xe
        mov    eax, dword ptr [esi + 4]
        add    eax, 0x3c
loc_5_0736:
        stosd  dword ptr es:[edi], eax
        add    eax, 8
        loop   loc_5_0736
        mov    ecx, 0xe
        mov    ax, word ptr [esi + 4]
        shl    eax, 0x10
        mov    ax, 0x2eff
loc_5_074C:
        push   eax
        mov    eax, 0xe
        sub    eax, ecx
        shl    eax, 8
        or     eax, 0x2e000068
        stosd  dword ptr es:[edi], eax
        pop    eax
        stosd  dword ptr es:[edi], eax
        loop   loc_5_074C
loc_5_0761:
        mov    eax, dword ptr [esi + 4]
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        mov    ax, word ptr [esi + 4]
        add    ax, 4
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x18], 0xe
        clc    
        jmp    loc_5_0793
loc_5_0780:
        mov    word ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x18], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
loc_5_0793:
        popal  
        ret    
loc_5_0795:
        xor    ax, ax
        mov    al, byte ptr [0]              ; -> loc_1_010B
        mov    word ptr [ebp + 0x10], ax
        mov    al, byte ptr [0]              ; -> loc_1_0107
        mov    word ptr [ebp + 0x18], ax
        mov    word ptr [ebp + 0x14], 0x65
        clc    
        ret    
loc_5_07B2:
        stc    
        ret    
loc_5_07B4:
        stc    
        ret    
loc_5_07B6:
        stc    
        ret    
loc_5_07B8:
        stc    
        ret    
loc_5_07BA:
        stc    
        ret    
loc_5_07BC:
        stc    
        ret    
loc_5_07BE:
        stc    
        ret    
loc_5_07C0:
        stc    
        ret    
loc_5_07C2:
        stc    
        ret    
loc_5_07C4:
        stc    
        ret    
loc_5_07C6:
        stc    
        ret    
loc_5_07C8:
        ret    
sub_5_07C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_08B3
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_0ADE
        add    esp, 4
        call   sub_5_0ACC
        call   sub_5_0B1B
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_0AFE
        add    esp, 8
        call   sub_5_0AD8
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_0AD2
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_0AEE
        add    esp, 4
        mov    eax, esi
loc_5_08B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_08BA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_08E0
        push   0x15
        push   4
        call   sub_5_1428
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_08DE
        xor    eax, eax
        jmp    loc_5_0A45
loc_5_08DE:
        mov    dword ptr [esi], ebp
loc_5_08E0:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_08FD
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_08FD:
        push   ebp
        call   sub_5_1418
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_09F1
loc_5_090D:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_0914:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_0934
        or     al, al
        je     loc_5_0930
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_0934
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_0914
loc_5_0930:
        xor    eax, eax
        jmp    loc_5_0939
loc_5_0934:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0939:
        test   eax, eax
        je     loc_5_094A
        push   ebx
        push   ebp
        call   sub_5_1408
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_090D
loc_5_094A:
        test   ebx, ebx
        je     loc_5_09F1
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_095F
        xor    eax, eax
        jmp    loc_5_0A45
loc_5_095F:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_07C9
        add    esp, 0x14
        test   eax, eax
        je     loc_5_098C
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_0A4A
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_0A45
loc_5_098C:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_07C9
        add    esp, 0x14
        test   eax, eax
        je     loc_5_09C3
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_0A4A
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_0A45
loc_5_09C3:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_07C9
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_07C9
        add    esp, 0x14
        jmp    loc_5_0A43
loc_5_09F1:
        push   ebp
        call   sub_5_13F4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_0A01
        xor    eax, eax
        jmp    loc_5_0A45
loc_5_0A01:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 8], eax
        mov    edx, dword ptr [ecx + 4]
        sub    eax, eax
        mov    ecx, 0xffffffff
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], 0
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        push   ebx
        and    ecx, 3
        push   ebp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        call   sub_5_13E4
loc_5_0A43:
        mov    eax, ebx
loc_5_0A45:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_0A4A:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_0A96
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_0A74
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_07C9
        add    esp, 0x14
        mov    dword ptr [edi], 0
loc_5_0A74:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_0A96
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_07C9
        mov    dword ptr [edi + 4], 0
        add    esp, 0x14
loc_5_0A96:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_0AC7
        push   edi
        push   eax
        call   sub_5_145C
        push   edi
        push   dword ptr [esi]
        call   sub_5_144C
        push   dword ptr [esi]
        call   sub_5_1418
        test   eax, eax
        jne    loc_5_0AC7
        push   dword ptr [esi]
        call   sub_5_143C
        mov    dword ptr [esi], 0
loc_5_0AC7:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh
sub_5_0ACC:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_0AD2:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_0AD8:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_0ADE:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_0AEE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_0AFE:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_0B0D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_0B1B:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
loc_5_0B26:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_0B5B
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_0B69
loc_5_0B5B:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0B69:
        pop    edi
        pop    esi
        ret    0xc
loc_5_0B6E:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_0B7C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_0B8A:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        lea    esi, [edi + 0x104]
        mov    dword ptr [edi], 0xb
        call   sub_5_10AC
        mov    ebx, eax
        push   0xff
        push   0                             ; -> loc_7_01BC
        push   esi
        call   sub_5_1470
        add    esp, 0xc
        lea    eax, [edi + 4]
        push   0xff
        push   ebx
        push   eax
        call   sub_5_1470
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [edi + 0x210], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0xb3
        mov    dword ptr [edi + 0x204], edx
        mov    dword ptr [edi + 0x20c], ecx
        mov    word ptr [edi + 0x208], 0x3203
        cmp    byte ptr [esi], cl
        je     loc_5_0C01
loc_5_0BF6:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_5_0BF6
loc_5_0C01:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_0C19:
        push   esi
        push   edi
        mov    esi, 0x8878000a
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 0xc], 0
        jne    loc_5_0C3E
        push   dword ptr [edi + 4]
        call   sub_5_117D
        add    esp, 4
        test   eax, eax
        jne    loc_5_0C3E
        xor    esi, esi
        inc    dword ptr [edi + 0xc]
loc_5_0C3E:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    4
loc_5_0C45:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], 7
        cmp    byte ptr [0], 0               ; -> obj7:0x020A
        jne    loc_5_0C5E
        mov    dword ptr [eax], 0xf
loc_5_0C5E:
        mov    dword ptr [eax + 0xc], 1
        xor    ecx, ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x30], ecx
        mov    dword ptr [eax + 0x34], ecx
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x3c], ecx
        mov    dword ptr [eax + 0x40], ecx
        mov    dword ptr [eax + 0x44], ecx
        mov    dword ptr [eax + 0x48], ecx
        xor    eax, eax
        ret    8
loc_5_0C9F:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xf0
        mov    dword ptr [esi], ebx
        je     loc_5_0CC1
        mov    eax, 0x8878001e
        jmp    loc_5_0D67
loc_5_0CC1:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_0D46
        push   0x201
        push   0x31
        call   sub_5_1480
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0D3F
        mov    dword ptr [ebx], 0            ; -> loc_7_01D8
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_11C6
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_0D4B
        cmp    eax, 0xffff
        jae    loc_5_0D4B
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_0D4B
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_0D4B
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_0D4B
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_0EBF
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 0xc
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [ebx + 0x24], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 0x28], edx
        jmp    loc_5_0D4B
loc_5_0D3F:
        mov    edi, 0x8007000e
        jmp    loc_5_0D4B
loc_5_0D46:
        mov    edi, 0x8878000a
loc_5_0D4B:
        test   edi, edi
        je     loc_5_0D60
        test   ebx, ebx
        je     loc_5_0D65
        push   0
        push   ebx
        call   sub_5_1478
        add    esp, 8
        jmp    loc_5_0D65
loc_5_0D60:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_0D65:
        mov    eax, edi
loc_5_0D67:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_0D6E:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_0D80:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_0DA5
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_0DA5
        push   dword ptr [ecx + 4]
        call   sub_5_1173
        add    esp, 4
        xor    eax, eax
loc_5_0DA5:
        ret    4
sub_5_0DA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   0x201
        mov    dword ptr [ebp - 4], ebx
        push   0x14
        call   sub_5_1480
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_5_0E27
        mov    edi, dword ptr [ebp + 8]
        push   1
        push   0                             ; -> obj7:0x01C8
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi], 0            ; -> loc_7_0198
        call   sub_5_149C
        test   eax, eax
        jne    loc_5_0E27
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [ebp - 0xc], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VxDCall  Dev_357E.Service_0001h          ; cd2001007e35
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_5_0E27
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_13BC
        add    esp, 8
loc_5_0E27:
        test   ebx, ebx
        jne    loc_5_0E50
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0E41
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1488
loc_5_0E41:
        test   esi, esi
        je     loc_5_0E50
        push   0
        push   esi
        call   sub_5_1478
        add    esp, 8
loc_5_0E50:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0E57:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_13B2
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_0EB9
        push   0
        push   edi
        call   sub_5_13BC
        add    esp, 8
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VxDCall  Dev_357E.Service_0002h          ; cd2002007e35
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1488
        push   0
        push   esi
        call   sub_5_1478
        add    esp, 8
loc_5_0EB9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0EBF:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_0ED1
        mov    dword ptr [edx], eax
loc_5_0ED1:
        mov    eax, dword ptr [esp + 0xc]
        and    dword ptr [edx], 0xfffc
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 0x1c], edx
        inc    dword ptr [esi + 0x20]
        mov    dword ptr [esi + 8], 0
        pop    esi
        ret    
loc_5_0EF1:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_0F16
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_0F16
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_1478
        add    esp, 8
loc_5_0F16:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_0F1C:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_0F51
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_0F5F
loc_5_0F51:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0F5F:
        pop    edi
        pop    esi
        ret    0xc
loc_5_0F64:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_0F72:
        mov    eax, 0x80004001
        ret    0x20
loc_5_0F7A:
        mov    eax, 0x80004001
        ret    0x14
loc_5_0F82:
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [edx + 0x24], ecx
        movzx  eax, word ptr [eax + 2]
        mov    dword ptr [edx + 0x28], eax
        mov    eax, 0x80004001
        ret    8
loc_5_0FA0:
        mov    eax, 0x80004001
        ret    8
loc_5_0FA8:
        mov    eax, 0x80004001
        ret    8
loc_5_0FB0:
        mov    eax, 0x80004001
        ret    8
loc_5_0FB8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_0FEA
        push   dword ptr [esi + 0x18]
        call   sub_5_10BA
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x30], 3
        jbe    loc_5_0FD7
        add    eax, eax
loc_5_0FD7:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_0FFE
loc_5_0FEA:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_0FFE:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_1004:
        push   ebx
        mov    eax, 0x80004005
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        cmp    dword ptr [edi + 8], 0
        jne    loc_5_1070
        movzx  esi, word ptr [edi + 0x2e]
        mov    ebx, dword ptr [edi + 0x18]
        push   ebx
        call   sub_5_10D5
        add    esp, 4
        push   dword ptr [edi + 0x1c]
        push   dword ptr [edi + 0xc]
        push   ebx
        call   sub_5_10F9
        add    esp, 0xc
        push   ebx
        call   sub_5_111A
        add    esp, 4
        push   ebx
        call   sub_5_10E3
        mov    cx, word ptr [edi + 0x1c]
        add    esp, 4
        cmp    byte ptr [edi + 0x30], 4
        jbe    loc_5_1054
        shr    cx, 1
loc_5_1054:
        push   dword ptr [edi + 0x28]
        movzx  ecx, cx
        push   dword ptr [edi + 0x24]
        push   ecx
        push   esi
        call   sub_5_120C
        add    esp, 0x10
        xor    eax, eax
        mov    dword ptr [edi + 8], 1
loc_5_1070:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
loc_5_1076:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     loc_5_10A7
        movzx  eax, word ptr [esi + 0x2e]
        push   eax
        call   sub_5_1314
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_10D5
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_10A7:
        pop    esi
        ret    4
        db  0CCh
sub_5_10AC:
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_10BA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        movzx  eax, cx
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
sub_5_10D5:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_10E3:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_10F9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        xor    bx, bx
        mov    bh, 1
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        pop    ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_5_111A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    dx, dx
        or     dl, 8
        or     dl, 0x10
        or     dl, 0x40
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        pop    edx
        pop    ebx
        pop    ebp
        ret    
sub_5_113E:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        mov    edx, dword ptr [ebp + 8]
        xor    dx, 0x388
        mov    ax, word ptr [ebp + 0xc]
        xor    dx, ax
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        pop    edx
        pop    eax
        pop    ebp
        ret    
sub_5_115A:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        xor    dx, 0x388
        mov    ax, word ptr [ebp + 0xc]
        xor    dx, ax
        in     al, dx
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_1173:
        mov    word ptr [0], 0               ; -> obj1:0x00ED
        ret    
sub_5_117D:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [0]            ; -> obj1:0x0136
        or     eax, eax
        je     loc_5_119D
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_119D
        jmp    loc_5_11BF
loc_5_119D:
        mov    word ptr [0], 1               ; -> obj1:0x00ED
        mov    edx, 0
        sub    eax, eax
        test   dword ptr [0], 0x200          ; -> obj1:0x00FF
        je     loc_5_11BA
        inc    eax
loc_5_11BA:
        mov    byte ptr [0], al              ; -> obj7:0x020A
loc_5_11BF:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_11C6:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [0]            ; -> obj1:0x00E4
        mov    word ptr [esi + 0x2e], ax
        mov    eax, dword ptr [0]            ; -> loc_1_010B
        mov    byte ptr [esi + 0x30], al
        mov    eax, dword ptr [0]            ; -> obj7:0x0060
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj7:0x0064
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [0]            ; -> obj7:0x006C
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00DC
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x00D8
        mov    word ptr [esi + 0x2c], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_5_120C:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        pushfd 
        cli    
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        cmp    byte ptr [ebp + 0x10], 8
        jne    loc_5_1221
        mov    al, 0x80
loc_5_1221:
        push   eax
        push   0xf88
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, 0x74
        push   eax
        push   0x138b
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   0x1389
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        xchg   al, ah
        push   eax
        push   0x1389
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        push   0xb89
        push   edx
        call   sub_5_115A
        add    esp, 8
        push   eax
        push   0xb89
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        push   0xb8b
        push   edx
        call   sub_5_115A
        add    esp, 8
        or     al, 8
        push   eax
        push   0xb8b
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    ecx, 1
        jne    loc_5_12AF
        mov    al, 0x20
loc_5_12AF:
        or     al, 0x10
        or     al, 0xc0
        push   ax
        push   0xf8a
        push   edx
        call   sub_5_115A
        add    esp, 8
        mov    cx, ax
        pop    ax
        mov    ah, 0xf
        and    ah, cl
        or     al, ah
        xor    al, 0x40
        push   eax
        push   0xf8a
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        xor    al, 0x40
        push   eax
        push   0xf8a
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        mov    al, byte ptr [0]              ; -> loc_7_0208
        or     al, 0xc0
        or     al, 0x20
        push   eax
        push   0xb8a
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        mov    byte ptr [0], al              ; -> loc_7_0208
        popfd  
        clc    
        pop    edx
        pop    ecx
        pop    eax
        pop    ebp
        ret    
sub_5_1314:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        pushfd 
        cli    
        xor    eax, eax
        push   0xf8a
        push   edx
        call   sub_5_115A
        add    esp, 8
        and    al, 0x7f
        and    al, 0xdf
        push   eax
        push   0xf8a
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        mov    al, byte ptr [0]              ; -> loc_7_0208
        and    al, 0x3f
        or     al, 0x20
        mov    byte ptr [0], al              ; -> loc_7_0208
        push   eax
        push   0xb8a
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        mov    bl, 8
        and    bl, 0xc
        not    bl
        or     bl, byte ptr [0]              ; -> obj7:0x0209
        push   0xb8b
        push   edx
        call   sub_5_115A
        add    esp, 8
        and    al, bl
        push   eax
        push   0xb8b
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        push   0xf8a
        push   edx
        call   sub_5_115A
        add    esp, 8
        and    al, 0xbf
        and    al, 0xef
        push   eax
        push   0xf8a
        push   edx
        call   sub_5_113E
        add    esp, 0xc
        popfd  
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_5_13B2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [0]            ; -> obj1:0x00E9
        pop    ebp
        ret    
sub_5_13BC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> obj1:0x00E9
        pop    ebp
        ret    
        db  0CCh, 0CCh, 0CCh
sub_5_13CC:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_13D4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_13DC:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_13E4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_13F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_1408:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1418:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1428:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_5_143C:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_144C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_145C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_1470:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_1478:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_1480:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_1488:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        VxDCall  APM.Service_0002h               ; cd2002002700
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_149C:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_14BA
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_14BA:
        ret    0x10
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x2c  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_0008:
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        cmp    dword ptr [0], 0              ; -> obj6:0x0000
        je     loc_6_0025
        cmp    eax, 0xe
        jae    loc_6_0025
        call   sub_7_015C
loc_6_0025:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 7  vsize=0x20b  RW-
        db  007h, 000h, 000h, 000h
        dd  loc_1_07B0               ; fixup
        dd  loc_1_0817               ; fixup
        dd  loc_1_0818               ; fixup
        dd  loc_1_085E               ; fixup
        dd  loc_1_0873               ; fixup
        db  0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h
        dd  loc_1_115C               ; fixup
        dd  loc_1_11B4               ; fixup
        dd  loc_1_11B5               ; fixup
        dd  loc_1_11E0               ; fixup
        dd  loc_1_120A               ; fixup
        db  0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h
        dd  loc_1_115C               ; fixup
        dd  loc_1_11B4               ; fixup
        dd  loc_1_11B5               ; fixup
        dd  loc_1_11E0               ; fixup
        dd  loc_1_120A               ; fixup
        db  0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 088h, 003h
        dd  loc_5_011B               ; fixup
        db  089h, 003h
        dd  loc_5_011B               ; fixup
        db  08Ah, 003h
        dd  loc_5_011B               ; fixup
        db  08Bh, 003h
        dd  loc_5_011B               ; fixup
        dd  loc_5_0174               ; fixup
        dd  loc_5_017B               ; fixup
        dd  loc_5_01BB               ; fixup
        dd  loc_5_01C5               ; fixup
        dd  loc_5_01A8               ; fixup
        dd  loc_5_01A8               ; fixup
        dd  loc_5_01AA               ; fixup
        dd  loc_5_01D6               ; fixup
        dd  loc_5_0215               ; fixup
        dd  loc_5_01A8               ; fixup
        dd  loc_5_01A8               ; fixup
        dd  loc_5_03B3               ; fixup
        dd  loc_5_018C               ; fixup
        dd  loc_5_0174               ; fixup
        dd  loc_5_02AB               ; fixup
        dd  loc_5_0247               ; fixup
        dd  loc_5_026E               ; fixup
        dd  loc_5_0301               ; fixup
        dd  loc_5_0386               ; fixup
        db  00Dh, 000h, 000h, 000h
        dd  obj7:0x008A              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj7:0x00BE              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj7:0x00CE              ; fixup
        db  000h, 000h, 042h, 04Ch, 041h, 053h, 054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h
        db  049h, 025h, 064h, 020h, 044h, 025h, 064h, 000h, 000h, 000h
sub_7_0108:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_7_015C:
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
loc_7_0198:
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
        add    byte ptr [eax], al
loc_7_01BC:
        push   esi
        push   eax
        inc    ecx
        push   ebx
        inc    esp
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [ebx + esi*2 + 0x6f], ah
        jne    0x23b
        jbe    0x249
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_01D8:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_0208:
        and    dword ptr [eax + eax], eax
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x44  R-X
loc_8_0000:
        push   0
        push   0x4a
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        jne    loc_8_0013
        stc    
        ret    
loc_8_0013:
        mov    dword ptr [0], eax            ; -> obj1:0x00F3
        mov    eax, 0x2f
        mov    esi, 0                        ; -> loc_5_05DC
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_4_03B8
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0], 0xffffffff     ; -> obj6:0x0000
        clc    
        ret    
VxD__LTEXT ENDS
