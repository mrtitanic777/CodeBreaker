; E30N3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 5843 instructions recovered by recursive descent

Declare_Virtual_Device E30N3, 3, 10, E30N3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x67c  R-X
loc_1_0000:
        mov    al, byte ptr [0x660]          ; -> obj1:0x0660
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x660]              ; -> obj1:0x0660
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_4_2CA0
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
E30N3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0038
        jmp    dword ptr [eax*4 + 0x3a]      ; -> obj1:0x003A
loc_1_0038:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0000
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0000
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_00B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_00E0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_0180
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_0120
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_0115
        cmp    al, 0xff
        jne    loc_1_0115
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_0115
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_0187
loc_1_0115:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_0187
        db  08Bh, 0FFh
loc_1_0120:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_0170
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0146
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_014D
loc_1_0146:
        mov    dword ptr [ebp - 0x10], 1
loc_1_014D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0160
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_018A
        db  08Dh, 064h, 024h, 000h
loc_1_0160:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_018A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0170:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_018A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0180:
        mov    dword ptr [ebp - 4], 0x20
loc_1_0187:
        mov    esi, dword ptr [ebp + 8]
loc_1_018A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_0281
loc_1_0195:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_01ED
        test   al, 2
        je     loc_1_01D9
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_1_01D9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_01ED
loc_1_01D9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_01E4
        test   bl, 4
        jne    loc_1_01ED
loc_1_01E4:
        test   bl, 0x20
        je     loc_1_0276
loc_1_01ED:
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_0462
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0276
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_022F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_022F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0240
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0246
        db  08Bh, 0FFh
loc_1_0240:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_0246:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0255
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
loc_1_0255:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_04D4
        mov    edi, ebx
        jmp    loc_1_0279
loc_1_0276:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0279:
        or     edi, edi
        jne    loc_1_0195
loc_1_0281:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_02A0
        pop    ebp
        ret    4
sub_1_02A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_031E
loc_1_02B0:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_0317
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_0317
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_02DC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_02DC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_02F0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_02F6
        db  005h, 000h, 000h, 000h, 000h
loc_1_02F0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_02F6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_04D4
        mov    edi, ebx
        jmp    loc_1_031A
loc_1_0317:
        mov    edi, dword ptr [edi + 0x10]
loc_1_031A:
        or     edi, edi
        jne    loc_1_02B0
loc_1_031E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0330:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_0345
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0345:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_0381
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_0372
        cmp    bl, 0xff
        jne    loc_1_0372
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_0372
        test   al, 8
        je     loc_1_036B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_036B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0372:
        test   al, 4
        jne    loc_1_037A
        test   al, 2
        je     loc_1_0399
loc_1_037A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0381:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_0399
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_0399
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0399:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h
loc_1_03B4:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h
loc_1_03F1:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_041A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_0420:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_0426:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_042C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_0432:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_0438:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_043E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_0444:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_044A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_0456:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_045C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0462:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0468:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_046E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_047A:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_0480:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
loc_1_0492:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
loc_1_0498:
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_04AA:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_04B0:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_04B6:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_04BC:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_04C2:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_04C8:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_04CE:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_04D4:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_04F8:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_051C:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0540:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_0546:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_054C:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0558:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
sub_1_059A:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_05A0:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_05B4:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
sub_1_05D4:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_05F4
        or     ecx, ecx
        je     loc_1_060A
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_0608
loc_1_05F4:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_0608
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_0608:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_060A:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'E30N3   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  E30N3_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0670:
        inc    ebp
        xor    esi, dword ptr [eax]
        dec    esi
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x678]        ; -> obj1:0x0678
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_04CE
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_004E
        mov    esi, 2
loc_2_004E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_04CE
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_0082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_04CE
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_00B1
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_00B1:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x18]
        add    ecx, 0x2c
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dl, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], dl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0140
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_04D4
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_04D4
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_04D4
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_0189
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0189:
        mov    eax, dword ptr [0x678]        ; -> obj1:0x0678
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_04CE
        or     eax, eax
        je     loc_2_01A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_01A9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_01B0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_01C3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_01B0
        jmp    loc_2_01D2
loc_2_01C3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_01D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_01E8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_01E8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_0240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_05E0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0269
        cmp    ebx, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    ebx, eax
loc_2_0280:
        or     ebx, ebx
        je     loc_2_028C
        cmp    ebx, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0468
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_04D4
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    ebx, 0xc001000e
loc_2_02EB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0300:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x20]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [esi + 0x24]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_05D4
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_05D4
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_0384
        sub    eax, eax
        nop    
loc_2_0360:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 0x1c]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 0x24], edi
        ja     loc_2_0360
loc_2_0384:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_03EF
loc_2_0391:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_03E4
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        call   sub_1_05D4
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        call   sub_1_05D4
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_03E7
loc_2_03E4:
        inc    dword ptr [ebp - 4]
loc_2_03E7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_0391
loc_2_03EF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_04DA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_0403:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_2_043A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_04C8
loc_2_043A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_0514
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        call   sub_1_05D4
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x24]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_05D4
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x14], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 0x19]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 0x24]
loc_2_04C8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_0565
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_0552
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_0552
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0559
loc_2_0514:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D4
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_05D4
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0552:
        mov    dword ptr [ebp - 8], 1
loc_2_0559:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_04F0
loc_2_0565:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_05D0
loc_2_056B:
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_05D2
        cmp    edi, 0x103
        je     loc_2_05D2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D4
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_05D4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_05D0:
        sub    edi, edi
loc_2_05D2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        mov    dword ptr [esi + 0xc], 0
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_2_0651
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0651:
        sub    eax, eax
loc_2_0653:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_0660:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x19]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xff
        mov    edi, dword ptr [edx + 0x24]
        cmp    edi, eax
        je     loc_2_0692
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_0687:
        test   dword ptr [esi], edx
        je     loc_2_068C
        inc    eax
loc_2_068C:
        add    esi, 4
        dec    edi
        jne    loc_2_0687
loc_2_0692:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_06A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x19]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [edi + 0x24], 0
        jbe    loc_2_071C
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        call   sub_1_05D4
        add    ebx, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [edi + 0x24], eax
        ja     loc_2_06E0
loc_2_071C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0736:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x18]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0760:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_0790
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0790:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_05D4
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_07C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     loc_2_0859
        dec    esi
        cmp    esi, edx
        jb     loc_2_0859
        nop    
loc_2_07E0:
        cmp    edx, ecx
        jb     loc_2_0859
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_0803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0803:
        jbe    loc_2_0810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0837
        db  08Bh, 0FFh
loc_2_0810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_0827
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0827:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_0837
        mov    dword ptr [ebp - 4], 0
loc_2_0837:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0869
        jle    loc_2_0848
        or     edx, edx
        je     loc_2_0859
        lea    esi, [edx - 1]
        jmp    loc_2_084B
loc_2_0848:
        lea    ecx, [edx + 1]
loc_2_084B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_07E0
loc_2_0859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0869:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x38c0  R-X
loc_4_0000:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0x1789]
        push   esi
        call   sub_4_1DA0
        test   al, al
        jne    loc_4_0026
        push   esi
        call   sub_4_1F50
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0026:
        and    ax, 0xff
        or     word ptr [edi + 0x48], ax
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0040:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x13], 0
        jne    loc_4_00C4
        mov    bl, byte ptr [esi + 0x48]
        test   bl, 0x1f
        mov    word ptr [esi + 0x48], 0
        je     loc_4_00C4
        inc    dword ptr [esi + 0x4a]
        mov    byte ptr [esi + 0x13], 1
        test   bl, 0x10
        je     loc_4_0080
        push   esi
        call   sub_4_02F0
        test   al, al
        je     loc_4_007D
        mov    byte ptr [ebp - 1], 1
loc_4_007D:
        and    bl, 0xea
loc_4_0080:
        test   bl, 1
        je     loc_4_0096
        push   esi
        call   sub_4_0340
        test   al, al
        je     loc_4_0093
        mov    byte ptr [ebp - 1], 1
loc_4_0093:
        and    bl, 0xfe
loc_4_0096:
        test   bl, 8
        je     loc_4_00A4
        push   esi
        and    bl, 0xf5
        call   sub_4_05B0
loc_4_00A4:
        test   bl, 2
        je     loc_4_00B2
        push   esi
        and    bl, 0xfd
        call   sub_4_0470
loc_4_00B2:
        test   bl, 4
        je     loc_4_00BD
        push   esi
        call   sub_4_02B0
loc_4_00BD:
        mov    byte ptr [esi + 0x13], 0
        dec    dword ptr [esi + 0x4a]
loc_4_00C4:
        cmp    byte ptr [esi + 0x14], 0
        jne    loc_4_00FB
        cmp    byte ptr [esi + 0x9c], 0
        jne    loc_4_00FB
        cmp    dword ptr [esi + 0x9e], 0
        je     loc_4_00FB
        cmp    byte ptr [esi + 0x98], 0
        je     loc_4_00FB
        push   1
        inc    dword ptr [esi + 0x4a]
        mov    byte ptr [esi + 0x14], 1
        push   esi
        call   sub_4_1830
        mov    byte ptr [esi + 0x14], 0
        dec    dword ptr [esi + 0x4a]
loc_4_00FB:
        cmp    dword ptr [esi + 0x7a], 0
        je     loc_4_010B
        push   esi
        call   sub_4_0680
        mov    byte ptr [ebp - 1], 1
loc_4_010B:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_013C
        cmp    byte ptr [esi + 0x92], 0
        je     loc_4_0120
        cmp    dword ptr [esi + 0x4a], 1
        je     loc_4_012A
loc_4_0120:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_013C
loc_4_012A:
        push   0
        lea    eax, [esi + 0x18]
        mov    byte ptr [esi + 0x15], 1
        push   eax
        inc    dword ptr [esi + 0x4a]
        call   sub_1_04AA
loc_4_013C:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_014B
        mov    eax, dword ptr [esi + 0x6a]
        push   eax
        call   sub_1_0290
loc_4_014B:
        add    esi, 0x1789
        push   esi
        call   sub_4_1F50
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
loc_4_0160:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
loc_4_016B:
        mov    cl, byte ptr [esi + 0x92]
        test   cl, cl
        je     loc_4_017B
        cmp    dword ptr [esi + 0x4a], 2
        je     loc_4_0185
loc_4_017B:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_01C7
loc_4_0185:
        test   cl, cl
        je     loc_4_018F
        cmp    dword ptr [esi + 0x4a], 2
        je     loc_4_0200
loc_4_018F:
        mov    ecx, dword ptr [esi + 0x5a]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_016B
        cmp    dword ptr [ecx + 0x14], ebx
        jne    loc_4_016B
        mov    edi, dword ptr [ecx + 0x10]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        push   ebx
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        push   0x45
        push   ecx
        mov    dword ptr [edx + 4], eax
        call   sub_1_04D4
loc_4_01BB:
        push   ebx
        dec    dword ptr [esi + 0x56]
        push   edi
        call   sub_1_0444
loc_4_01C5:
        jmp    loc_4_016B
loc_4_01C7:
        mov    byte ptr [esi + 0x15], 0
        dec    dword ptr [esi + 0x4a]
        cmp    byte ptr [esi + 0x9c], 0
        jne    loc_4_0228
        cmp    dword ptr [esi + 0x9e], 0
        je     loc_4_0228
        cmp    byte ptr [esi + 0x98], 0
        je     loc_4_0228
        mov    byte ptr [esi + 0x14], 1
        push   1
        push   esi
        call   sub_4_1830
        mov    byte ptr [esi + 0x14], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_4_0200:
        mov    byte ptr [esi + 0x15], 0
        push   esi
        dec    dword ptr [esi + 0x4a]
        call   sub_4_34E0
        mov    byte ptr [esi + 0x92], 0
        mov    esi, dword ptr [esi + 0x8e]
        push   0
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_0456
loc_4_0225:
        dec    dword ptr [esi + 0x14]
loc_4_0228:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  090h
loc_4_0230:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x3e8
        mov    esi, dword ptr [ebp + 0xc]
        lea    eax, [esi + 0x24]
        push   eax
        call   sub_1_04AA
loc_4_0245:
        cmp    byte ptr [esi + 0x9c], 0
        jne    loc_4_0270
        cmp    dword ptr [esi + 0x9e], 0
        je     loc_4_0270
        cmp    byte ptr [esi + 0x98], 0
        je     loc_4_0270
        mov    byte ptr [esi + 0x14], 1
        push   1
        push   esi
        call   sub_4_1830
        mov    byte ptr [esi + 0x14], 0
loc_4_0270:
        cmp    byte ptr [esi + 0x18c], 1
        jne    loc_4_028C
        push   esi
        inc    dword ptr [esi + 0x16a]
        call   sub_4_0470
        mov    byte ptr [esi + 0x18c], 0
loc_4_028C:
        mov    eax, dword ptr [esi + 0x182]
        test   eax, eax
        je     loc_4_02A5
        dec    eax
        mov    dword ptr [esi + 0x182], eax
        jne    loc_4_02A5
        push   esi
        call   sub_4_05B0
loc_4_02A5:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_02B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x1789
        push   esi
        inc    dword ptr [esi - 0x16bf]
        inc    dword ptr [esi - 0x1627]
        call   sub_4_1E00
        movsx  eax, ax
        cmp    eax, -1
        jne    loc_4_02DE
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_4_02DE:
        push   esi
        call   sub_4_1F00
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_02F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0x1789]
        inc    dword ptr [edi + 0x136]
        push   esi
        inc    dword ptr [edi + 0x166]
        call   sub_4_25C0
        push   edi
        call   sub_4_0340
        test   al, al
        je     loc_4_0322
        mov    ebx, 1
loc_4_0322:
        push   esi
        call   sub_4_2670
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0340:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], ebx
loc_4_0351:
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_1E00
        movsx  edi, ax
        cmp    edi, -1
        je     loc_4_0460
        mov    eax, edi
        add    dword ptr [esi + 0x15e], edi
        mov    dword ptr [ebp - 8], 1
        inc    dword ptr [esi + 0x156]
        cmp    edi, 0x5ea
        jbe    loc_4_038B
        mov    eax, 0x5ea
loc_4_038B:
        mov    ecx, dword ptr [esi + 0x17c9]
        cmp    edi, 0xe
        jae    loc_4_03AE
        cmp    edi, 6
        jb     loc_4_03CB
        lea    eax, [ecx + 0xe]
        push   0
        push   0
        push   0
        push   edi
        push   ecx
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 0x6a]
        push   ecx
        jmp    loc_4_03C6
loc_4_03AE:
        lea    edx, [edi - 0xe]
        sub    eax, 0xe
        push   edx
        push   eax
        lea    edx, [ecx + 0xe]
        mov    eax, dword ptr [esi + 0x6a]
        push   edx
        push   0xe
        push   ecx
        push   ecx
        add    ecx, 0xe
        push   ecx
        push   eax
loc_4_03C6:
        call   sub_1_00B0
loc_4_03CB:
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_1F00
        mov    al, byte ptr [esi + 0x17c8]
        test   al, 0x10
        je     loc_4_03ED
        inc    dword ptr [esi + 0xce]
        inc    dword ptr [esi + 0x152]
loc_4_03ED:
        test   al, 1
        je     loc_4_0431
        inc    dword ptr [esi + 0xc2]
        test   al, 0x20
        je     loc_4_0416
        lea    ecx, [esi + 0xf2]
        inc    dword ptr [esi + 0x112]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    loc_4_044D
        inc    dword ptr [ecx + 4]
        jmp    loc_4_044D
loc_4_0416:
        lea    ecx, [esi + 0xea]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    loc_4_0429
        inc    dword ptr [ecx + 4]
loc_4_0429:
        inc    dword ptr [esi + 0x10e]
        jmp    loc_4_044D
loc_4_0431:
        inc    dword ptr [esi + 0xca]
        test   al, 2
        je     loc_4_0443
        inc    dword ptr [esi + 0x11a]
        jmp    loc_4_044D
loc_4_0443:
        test   al, 4
        je     loc_4_044D
        inc    dword ptr [esi + 0x122]
loc_4_044D:
        inc    ebx
        cmp    ebx, 0xa
        jb     loc_4_0351
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0460:
        mov    al, byte ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_0470:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x197]
        mov    edi, dword ptr [ecx + 0x2c]
        add    ecx, 0x24
        inc    dword ptr [esi + 0xbe]
        mov    al, byte ptr [ecx + 0xe]
        cmp    eax, 1
        je     loc_4_04A1
        cmp    eax, 2
        je     loc_4_04C3
        cmp    eax, 3
        je     loc_4_04E8
        jmp    loc_4_050B
loc_4_04A1:
        lea    edx, [esi + 0xd2]
        xor    eax, eax
        xor    ebx, ebx
        inc    dword ptr [esi + 0x102]
        mov    ax, word ptr [ecx + 0xc]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        mov    bx, word ptr [ecx + 0xc]
        cmp    ebx, eax
        jbe    loc_4_050B
        jmp    loc_4_0508
loc_4_04C3:
        lea    eax, [esi + 0xda]
        xor    edx, edx
        xor    ebx, ebx
        inc    dword ptr [esi + 0x106]
        mov    dx, word ptr [ecx + 0xc]
        add    edx, dword ptr [eax]
        mov    dword ptr [eax], edx
        mov    bx, word ptr [ecx + 0xc]
        cmp    ebx, edx
        jbe    loc_4_050B
        inc    dword ptr [eax + 4]
        jmp    loc_4_050B
loc_4_04E8:
        lea    edx, [esi + 0xe2]
        xor    eax, eax
        xor    ebx, ebx
        inc    dword ptr [esi + 0x10a]
        mov    ax, word ptr [ecx + 0xc]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        mov    bx, word ptr [ecx + 0xc]
        cmp    ebx, eax
        jbe    loc_4_050B
loc_4_0508:
        inc    dword ptr [edx + 4]
loc_4_050B:
        mov    al, byte ptr [ecx + 0xf]
        inc    al
        mov    byte ptr [ecx + 0xf], al
        cmp    al, 3
        jne    loc_4_0538
        push   0
        mov    eax, dword ptr [esi + 0x197]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_044A
loc_4_0529:
        dec    dword ptr [edi + 0x14]
        dec    dword ptr [esi + 0x16e]
        dec    dword ptr [esi + 0x11e]
loc_4_0538:
        cmp    byte ptr [esi + 0x18a], 0
        je     loc_4_0599
        mov    eax, dword ptr [esi + 0x19b]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x195]
        lea    edx, [esi + 0x1789]
        push   edx
        mov    dword ptr [esi + 0x197], eax
        inc    dword ptr [esi + 0x18d]
        add    dword ptr [esi + 0x191], ecx
        call   sub_4_1FF0
        mov    dword ptr [esi + 0x186], 0
        mov    al, 1
        pop    edi
        mov    byte ptr [esi + 0x18b], 1
        mov    byte ptr [esi + 0x18a], 0
        mov    dword ptr [esi + 0x182], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_0599:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x18b], al
        mov    dword ptr [esi + 0x182], eax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_4_05B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x197]
        inc    dword ptr [esi + 0x16a]
        mov    edi, dword ptr [eax + 0x2c]
        add    eax, 0x24
        inc    dword ptr [esi + 0xc6]
        mov    cl, byte ptr [eax + 0xf]
        inc    cl
        mov    byte ptr [eax + 0xf], cl
        cmp    cl, 3
        jne    loc_4_0601
        push   0xc0000001
        mov    eax, dword ptr [esi + 0x197]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_044A
loc_4_05F2:
        dec    dword ptr [edi + 0x14]
        dec    dword ptr [esi + 0x16e]
        dec    dword ptr [esi + 0x11e]
loc_4_0601:
        cmp    byte ptr [esi + 0x18a], 0
        je     loc_4_0661
        mov    eax, dword ptr [esi + 0x19b]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x195]
        lea    edx, [esi + 0x1789]
        push   edx
        mov    dword ptr [esi + 0x197], eax
        inc    dword ptr [esi + 0x18d]
        add    dword ptr [esi + 0x191], ecx
        call   sub_4_1FF0
        mov    dword ptr [esi + 0x186], 0
        mov    al, 1
        pop    edi
        mov    byte ptr [esi + 0x18b], 1
        mov    byte ptr [esi + 0x18a], 0
        mov    dword ptr [esi + 0x182], 4
        pop    esi
        pop    ebp
        ret    4
loc_4_0661:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x18b], al
        mov    dword ptr [esi + 0x182], eax
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0680:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x7a]
        test   edi, edi
        je     loc_4_078E
        cmp    byte ptr [esi + 0x9d], 0
        jne    loc_4_078E
        mov    byte ptr [esi + 0x9d], 1
        push   esi
        call   sub_4_07E0
        mov    ecx, dword ptr [edi + 0x2c]
        lea    eax, [edi + 0x24]
        lea    edx, [ebp - 0x14]
        mov    dword ptr [ebp - 0xc], eax
        push   edx
        lea    eax, [ebp - 0x18]
        push   eax
        mov    dword ptr [ebp - 0x10], ecx
        push   0
        mov    dword ptr [esi + 0x82], edi
        push   0
        push   edi
        call   sub_1_04F8
loc_4_06D5:
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   edx
        push   eax
        call   sub_1_051C
loc_4_06E6:
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_4_0703
        cmp    dword ptr [ebp - 4], 6
        jb     loc_4_0756
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   eax
        push   ecx
        push   ecx
        jmp    loc_4_074B
loc_4_0703:
        cmp    dword ptr [ebp - 4], 0xde
        jae    loc_4_072E
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 4], eax
        je     loc_4_072E
        lea    ebx, [esi + 0x19f]
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   0xd0
        push   0xe
        push   edi
        call   sub_4_0810
        jmp    loc_4_0738
loc_4_072E:
        sub    dword ptr [ebp - 4], 0xe
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0xe
loc_4_0738:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, 0xe
        mov    edx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        push   ebx
        push   0xe
        push   edx
        push   edx
loc_4_074B:
        push   0
        mov    eax, dword ptr [esi + 0x6a]
        push   eax
        call   sub_1_00B0
loc_4_0756:
        mov    eax, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [eax + 0xf]
        inc    cl
        mov    byte ptr [eax + 0xf], cl
        cmp    cl, 3
        jne    loc_4_0787
        push   0
        mov    eax, dword ptr [ebp - 0x10]
        push   edi
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        call   sub_1_044A
loc_4_0775:
        mov    ecx, dword ptr [ebp - 0x10]
        dec    dword ptr [ecx + 0x14]
        dec    dword ptr [esi + 0x16e]
        dec    dword ptr [esi + 0x11e]
loc_4_0787:
        mov    byte ptr [esi + 0x9d], 0
loc_4_078E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        inc    dword ptr [eax + 0x15a]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x28], 0
        cmp    dword ptr [eax + 0x7a], 0
        jne    loc_4_07C2
        mov    dword ptr [eax + 0x7a], ecx
        jmp    loc_4_07C8
loc_4_07C2:
        mov    edx, dword ptr [eax + 0x7e]
        mov    dword ptr [edx + 0x28], ecx
loc_4_07C8:
        pop    ebp
        mov    dword ptr [eax + 0x7e], ecx
        inc    dword ptr [eax + 0x172]
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07E0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x7a]
        add    eax, 0x24
        cmp    dword ptr [eax + 4], 0
        jne    loc_4_07FA
        mov    dword ptr [ecx + 0x7e], 0
loc_4_07FA:
        mov    eax, dword ptr [eax + 4]
        pop    ebp
        mov    dword ptr [ecx + 0x7a], eax
        dec    dword ptr [ecx + 0x172]
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0810:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_4_08D9
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_04F8
loc_4_083D:
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_4_08D9
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_051C
loc_4_0857:
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_4_08D4
        mov    edi, dword ptr [ebp + 0xc]
loc_4_085F:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_088B
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_0540
loc_4_0872:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_08D4
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_051C
loc_4_0889:
        jmp    loc_4_08CF
loc_4_088B:
        test   edi, edi
        je     loc_4_08A8
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_4_08A0
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_4_08CF
loc_4_08A0:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_4_08A8:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_4_08B6
        mov    ebx, eax
loc_4_08B6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        add    esi, ebx
        call   sub_1_05B4
loc_4_08C6:
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_4_08CF:
        cmp    dword ptr [ebp + 0x10], esi
        ja     loc_4_085F
loc_4_08D4:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_4_08D9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_08F0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_4_09BC
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   esi
        push   edx
        call   sub_1_04F8
loc_4_091D:
        cmp    dword ptr [ebp - 0x14], esi
        je     loc_4_09BC
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_051C
loc_4_0937:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], esi
        mov    dword ptr [ebp - 0x10], ecx
        je     loc_4_09B7
        mov    edi, dword ptr [ebp + 0x14]
loc_4_0945:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0971
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_0540
loc_4_0958:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_09B7
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_051C
loc_4_096F:
        jmp    loc_4_09B2
loc_4_0971:
        test   edi, edi
        je     loc_4_098E
        cmp    dword ptr [ebp - 4], edi
        jae    loc_4_0986
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_4_09B2
loc_4_0986:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_4_098E:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_4_099C
        mov    ebx, eax
loc_4_099C:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        add    esi, ebx
        call   sub_1_05B4
loc_4_09AC:
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_4_09B2:
        cmp    esi, dword ptr [ebp + 0xc]
        jb     loc_4_0945
loc_4_09B7:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_4_09BC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_09D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 0x4a]
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_0A25
        cmp    byte ptr [eax + 0x40], 0
        jne    loc_4_0A1E
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ecx + 0x10]
        test   edx, edx
        jl     loc_4_09FD
        cmp    edx, 1
        jle    loc_4_0A04
loc_4_09FD:
        mov    edi, 0xc00000bb
        jmp    loc_4_0A2A
loc_4_0A04:
        push   ecx
        mov    edi, 0x103
        push   esi
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0
        call   sub_4_0A70
        jmp    loc_4_0A2A
loc_4_0A1E:
        mov    edi, 0xc0010002
        jmp    loc_4_0A2A
loc_4_0A25:
        mov    edi, 0xc001000d
loc_4_0A2A:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_0A60
        cmp    dword ptr [esi + 0x4a], 2
        jne    loc_4_0A3F
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_0A49
loc_4_0A3F:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_0A60
loc_4_0A49:
        push   0
        add    esi, 0x18
        push   esi
        mov    byte ptr [esi - 3], 1
        call   sub_1_04AA
loc_4_0A58:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_4_0A60:
        mov    eax, edi
        dec    dword ptr [esi + 0x4a]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_4_0A70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x6e], 0
        je     loc_4_0A8C
        mov    edx, dword ptr [eax + 0x72]
        mov    ecx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 0x72], ecx
        ret    8
loc_4_0A8C:
        cmp    byte ptr [eax + 0x95], 0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x6e], ecx
        mov    dword ptr [eax + 0x72], ecx
        jne    loc_4_0AA4
        push   eax
        call   sub_4_0AB0
loc_4_0AA4:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_4_0ABC:
        mov    edi, dword ptr [esi + 0x6e]
        test   edi, edi
        je     loc_4_0C91
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_0CA1
        mov    byte ptr [esi + 0x95], 1
        mov    eax, dword ptr [edi + 0x10]
        cmp    eax, 4
        ja     loc_4_0C3F
        jmp    dword ptr [eax*4]             ; -> obj4:0x0CB4
loc_4_0AEE:
        push   0
        lea    eax, [edi + 0x24]
        push   eax
        lea    ecx, [edi + 0x20]
        push   ecx
        mov    edx, dword ptr [edi + 0x1c]
        push   edx
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    ecx, dword ptr [edi + 0x14]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_4_0D60
        mov    dword ptr [ebp - 4], eax
        jmp    loc_4_0C3F
loc_4_0B16:
        lea    eax, [edi + 0x24]
        lea    ecx, [edi + 0x20]
        push   eax
        mov    edx, dword ptr [edi + 0x1c]
        push   ecx
        mov    eax, dword ptr [edi + 0x18]
        push   edx
        mov    ecx, dword ptr [edi + 0x14]
        push   eax
        mov    edx, dword ptr [edi + 4]
        push   ecx
        push   edx
        push   esi
        call   sub_4_12F0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_4_0C3F
loc_4_0B3C:
        push   1
        lea    eax, [edi + 0x24]
        push   eax
        lea    ecx, [edi + 0x20]
        push   ecx
        mov    edx, dword ptr [edi + 0x1c]
        push   edx
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        mov    ecx, dword ptr [edi + 0x14]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_4_0D60
        mov    dword ptr [ebp - 4], eax
        jmp    loc_4_0C3F
loc_4_0B64:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0xc]
        mov    ecx, dword ptr [ebx + 0x10]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        push   ecx
        push   ebx
        mov    eax, dword ptr [edx + 0x6a]
        push   eax
        call   sub_2_0170
loc_4_0B7C:
        test   al, al
        jne    loc_4_0BB3
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   0xc0000001
        push   eax
        call   sub_1_043E
loc_4_0B90:
        push   8
        mov    eax, dword ptr [esi + 0x66]
        push   1
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_05A0
loc_4_0BA4:
        add    esp, 0x14
        push   0
        push   0x45
        push   ebx
        call   sub_1_04D4
loc_4_0BB1:
        jmp    loc_4_0BD9
loc_4_0BB3:
        lea    eax, [esi + 0x4e]
        push   0
        push   0
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esi + 0x52]
        mov    dword ptr [ebx + 4], ecx
        mov    ecx, dword ptr [esi + 0x52]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 0x52], ebx
        inc    dword ptr [esi + 0x56]
        inc    dword ptr [ebx + 0x14]
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_043E
loc_4_0BD9:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_4_0C3F
loc_4_0BE2:
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    ecx, dword ptr [esi + 0x6a]
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        push   ecx
        call   sub_2_0240
loc_4_0BF4:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_0C00
        dec    dword ptr [ebx + 0x14]
        jmp    loc_4_0C16
loc_4_0C00:
        mov    eax, 0x103
        cmp    dword ptr [ebp - 4], eax
        je     loc_4_0C16
        cmp    dword ptr [ebp - 4], 0xc001000e
        jne    loc_4_0C16
        mov    dword ptr [ebp - 4], eax
loc_4_0C16:
        mov    byte ptr [ebx + 0x40], 1
        dec    dword ptr [ebx + 0x14]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 0x5a]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 0x5e]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 0x5e]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 0x5e], ebx
loc_4_0C3F:
        cmp    dword ptr [ebp - 4], 0x103
        je     loc_4_0CA8
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x6e], eax
        mov    eax, dword ptr [edi + 0x10]
        test   eax, eax
        je     loc_4_0C78
        cmp    eax, 1
        je     loc_4_0C78
        cmp    eax, 2
        jne    loc_4_0ABC
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x66]
        push   eax
        push   edi
        push   ecx
        call   sub_1_047A
loc_4_0C70:
        dec    dword ptr [esi + 0x4a]
        jmp    loc_4_0ABC
loc_4_0C78:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_045C
loc_4_0C89:
        dec    dword ptr [ebx + 0x14]
        jmp    loc_4_0ABC
loc_4_0C91:
        mov    byte ptr [esi + 0x95], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0CA1:
        mov    byte ptr [esi + 0x95], 0
loc_4_0CA8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_4_0AEE
        dd  loc_4_0B16
        dd  loc_4_0B3C
        dd  loc_4_0B64
        dd  loc_4_0BE2
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_0CD0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x4a]
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_0D12
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x10], 2
        je     loc_4_0CF4
        mov    edi, 0xc00000bb
        jmp    loc_4_0D17
loc_4_0CF4:
        push   eax
        mov    edi, 0x103
        mov    dword ptr [eax + 4], 0
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [esi + 0x4a]
        call   sub_4_0A70
        jmp    loc_4_0D17
loc_4_0D12:
        mov    edi, 0xc001000d
loc_4_0D17:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_0D4D
        cmp    dword ptr [esi + 0x4a], 2
        jne    loc_4_0D2C
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_0D36
loc_4_0D2C:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_0D4D
loc_4_0D36:
        push   0
        add    esi, 0x18
        push   esi
        mov    byte ptr [esi - 3], 1
        call   sub_1_04AA
loc_4_0D45:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_4_0D4D:
        mov    eax, edi
        dec    dword ptr [esi + 0x4a]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0D60:
        push   ebp
        mov    cl, byte ptr [esp + 0x24]
        mov    ebp, esp
        sub    esp, 0x14
        test   cl, cl
        push   ebx
        push   esi
        push   edi
        je     loc_4_0D7D
        mov    edx, 0                        ; -> obj5:0x0020
        mov    edi, 0x34
        jmp    loc_4_0D87
loc_4_0D7D:
        mov    edx, 0                        ; -> obj5:0x00F0
        mov    edi, 0x18
loc_4_0D87:
        xor    esi, esi
        test   edi, edi
        jle    loc_4_0DA0
        mov    ebx, edx
        mov    eax, dword ptr [ebp + 0x10]
loc_4_0D92:
        cmp    dword ptr [ebx], eax
        je     loc_4_0DA3
        add    ebx, 4
        inc    esi
        cmp    edi, esi
        jg     loc_4_0D92
        jmp    loc_4_0DA3
loc_4_0DA0:
        mov    eax, dword ptr [ebp + 0x10]
loc_4_0DA3:
        cmp    edi, esi
        jne    loc_4_0DBE
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0xc0010017
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_4_0DBE:
        lea    ebx, [ebp - 8]
        mov    esi, 4
        cmp    eax, 0x20101
        ja     loc_4_0DE8
        je     loc_4_0FBF
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_4_120E
        jmp    dword ptr [eax*4]             ; -> obj4:0x1244
loc_4_0DE8:
        cmp    eax, 0x20201
        ja     loc_4_0E0A
        je     loc_4_1005
        sub    eax, 0x20102
        cmp    eax, 3
        ja     loc_4_120E
        jmp    dword ptr [eax*4]             ; -> obj4:0x1290
loc_4_0E0A:
        cmp    eax, 0x1010101
        ja     loc_4_0E2C
        je     loc_4_10E7
        sub    eax, 0x20202
        cmp    eax, 0xc
        ja     loc_4_120E
        jmp    dword ptr [eax*4]             ; -> obj4:0x12A0
loc_4_0E2C:
        cmp    eax, 0x1020101
        ja     loc_4_0E5F
        je     loc_4_119F
        cmp    eax, 0x1010102
        je     loc_4_10F7
        cmp    eax, 0x1010103
        je     loc_4_1107
        cmp    eax, 0x1010104
        je     loc_4_1196
        jmp    loc_4_120E
loc_4_0E5F:
        cmp    eax, 0x1020201
        ja     loc_4_0E87
        je     loc_4_11C0
        cmp    eax, 0x1020102
        je     loc_4_11AA
        cmp    eax, 0x1020103
        je     loc_4_11B5
        jmp    loc_4_120E
loc_4_0E87:
        sub    eax, 0x1020202
        cmp    eax, 5
        ja     loc_4_120E
        jmp    dword ptr [eax*4]             ; -> obj4:0x12D4
loc_4_0E9C:
        mov    ebx, edx
        lea    esi, [edi*4]
        jmp    loc_4_120E
loc_4_0EAA:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_4_120E
loc_4_0EB6:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_4_120E
loc_4_0EC2:
        xor    eax, eax
        test   cl, cl
        mov    dword ptr [ebp - 8], eax
        je     loc_4_120E
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x56], eax
        jne    loc_4_120E
        xor    esi, esi
        jmp    loc_4_120E
loc_4_0EE2:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_120E
loc_4_0EEE:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_4_120E
loc_4_0EFA:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_4_120E
loc_4_0F06:
        mov    dword ptr [ebp - 8], 0xbd4
        jmp    loc_4_120E
loc_4_0F12:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_120E
loc_4_0F1E:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_120E
loc_4_0F2A:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_120E
loc_4_0F36:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6
        lea    eax, [ebp - 0x14]
        push   edi
        lea    ebx, [ebp - 0x14]
        push   eax
        mov    esi, 4
        call   sub_1_05B4
loc_4_0F50:
        mov    byte ptr [ebp - 0x11], 0
        jmp    loc_4_120E
loc_4_0F59:
        mov    ebx, 0                        ; -> obj5:0x0000
        mov    esi, 0x1e
        jmp    loc_4_120E
loc_4_0F68:
        test   cl, cl
        je     loc_4_0F77
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x76]
        jmp    loc_4_120B
loc_4_0F77:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_4_120E
loc_4_0F88:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_120E
loc_4_0F94:
        lea    ebx, [ebp - 2]
        mov    esi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_4_120E
loc_4_0FA7:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_4_120E
loc_4_0FB3:
        mov    dword ptr [ebp - 8], 7
        jmp    loc_4_120E
loc_4_0FBF:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xbe]
        jmp    loc_4_120B
loc_4_0FCD:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xc2]
        jmp    loc_4_120B
loc_4_0FDB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xc6]
        jmp    loc_4_120B
loc_4_0FE9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xca]
        jmp    loc_4_120B
loc_4_0FF7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xce]
        jmp    loc_4_120B
loc_4_1005:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0xd2
        jmp    loc_4_120E
loc_4_1018:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x102]
        jmp    loc_4_120B
loc_4_1026:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0xda
        jmp    loc_4_120E
loc_4_1039:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x106]
        jmp    loc_4_120B
loc_4_1047:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0xe2
        jmp    loc_4_120E
loc_4_105A:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x10a]
        jmp    loc_4_120B
loc_4_1068:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0xea
        jmp    loc_4_120E
loc_4_107B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x10e]
        jmp    loc_4_120B
loc_4_1089:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0xf2
        jmp    loc_4_120E
loc_4_109C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x112]
        jmp    loc_4_120B
loc_4_10AA:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0xfa
        jmp    loc_4_120E
loc_4_10BD:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x116]
        jmp    loc_4_120B
loc_4_10CB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x11a]
        jmp    loc_4_120B
loc_4_10D9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x11e]
        jmp    loc_4_120B
loc_4_10E7:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 6
        add    ebx, 6
        jmp    loc_4_120E
loc_4_10F7:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 6
        add    ebx, 0xc
        jmp    loc_4_120E
loc_4_1107:
        test   cl, cl
        je     loc_4_113C
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x18]
        push   ecx
        mov    edi, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 0x10]
        mov    eax, dword ptr [edi + 0x6a]
        push   eax
        push   ecx
        call   sub_2_0760
loc_4_1127:
        mov    ebx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 0xc]
        lea    eax, [ecx + ecx*2]
        lea    esi, [eax*2]
        jmp    loc_4_120E
loc_4_113C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [edi + 0x6a]
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        lea    eax, [ebp - 0x10]
        push   edx
        push   eax
        call   sub_2_06A0
loc_4_115F:
        cmp    dword ptr [ebp - 0x10], 0
        mov    ebx, dword ptr [ebp + 0x14]
        jne    loc_4_117A
        mov    eax, dword ptr [ebp - 0xc]
        lea    ecx, [eax + eax*2]
        lea    esi, [ecx*2]
        jmp    loc_4_120E
loc_4_117A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [edi + 0x6a]
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        push   edx
        call   sub_2_0660
loc_4_118A:
        lea    ecx, [eax + eax*2]
        lea    esi, [ecx*2]
        jmp    loc_4_120E
loc_4_1196:
        mov    dword ptr [ebp - 8], 0x10
        jmp    loc_4_120E
loc_4_119F:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x122]
        jmp    loc_4_120B
loc_4_11AA:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x126]
        jmp    loc_4_120B
loc_4_11B5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x12a]
        jmp    loc_4_120B
loc_4_11C0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x12e]
        jmp    loc_4_120B
loc_4_11CB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x132]
        jmp    loc_4_120B
loc_4_11D6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x136]
        jmp    loc_4_120B
loc_4_11E1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x13a]
        jmp    loc_4_120B
loc_4_11EC:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x13e]
        jmp    loc_4_120B
loc_4_11F7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x142]
        jmp    loc_4_120B
loc_4_1202:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x146]
loc_4_120B:
        mov    dword ptr [ebp - 8], eax
loc_4_120E:
        cmp    esi, dword ptr [ebp + 0x18]
        jbe    loc_4_1226
        mov    eax, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [eax], esi
        mov    eax, 0xc0010014
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_4_1226:
        push   esi
        mov    eax, dword ptr [ebp + 0x14]
        push   ebx
        push   eax
        call   sub_1_05B4
loc_4_1231:
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (42 entries) ----
        dd  loc_4_0E9C
        dd  loc_4_0EAA
        dd  loc_4_0EB6
        dd  loc_4_0EC2
        dd  loc_4_0EE2
        dd  loc_4_0EEE
        dd  loc_4_0EFA
        dd  loc_4_0F06
        dd  loc_4_0F12
        dd  loc_4_0F1E
        dd  loc_4_0F2A
        dd  loc_4_0F36
        dd  loc_4_0F59
        dd  loc_4_0F68
        dd  loc_4_0F88
        dd  loc_4_0F94
        dd  loc_4_0FA7
        dd  loc_4_120E
        dd  loc_4_0FB3
        dd  loc_4_0FCD
        dd  loc_4_0FDB
        dd  loc_4_0FE9
        dd  loc_4_0FF7
        dd  loc_4_1018
        dd  loc_4_1026
        dd  loc_4_1039
        dd  loc_4_1047
        dd  loc_4_105A
        dd  loc_4_1068
        dd  loc_4_107B
        dd  loc_4_1089
        dd  loc_4_109C
        dd  loc_4_10AA
        dd  loc_4_10BD
        dd  loc_4_10CB
        dd  loc_4_10D9
        dd  loc_4_11CB
        dd  loc_4_11D6
        dd  loc_4_11E1
        dd  loc_4_11EC
        dd  loc_4_11F7
        dd  loc_4_1202
        db  08Dh, 064h, 024h, 000h
sub_4_12F0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 0x1010103
        push   esi
        ja     loc_4_1325
        je     loc_4_13F5
        cmp    eax, 0x1010e
        je     loc_4_1331
        cmp    eax, 0x1010f
        je     loc_4_139B
        cmp    eax, 0x10112
        je     loc_4_13AD
loc_4_1325:
        mov    eax, 0xc0010017
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_1331:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_4_134C
        mov    eax, dword ptr [ebp + 0x20]
        pop    esi
        mov    dword ptr [eax], 4
        mov    eax, 0xc0010014
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_134C:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_05B4
loc_4_135B:
        test   dword ptr [ebp - 4], 0xf050
        je     loc_4_1370
        mov    eax, 0xc00000bb
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_1370:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xc]
        mov    ecx, dword ptr [eax + 0x6a]
        push   edx
        push   ecx
        call   sub_2_05E0
loc_4_138B:
        mov    ecx, dword ptr [ebp + 0x1c]
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_139B:
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_13AD:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_4_13D1
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x20]
        pop    esi
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax], 0
        mov    eax, 0xc0010014
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_13D1:
        push   4
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x41
        push   eax
        call   sub_1_05B4
loc_4_13E3:
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_13F5:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, esi
        sub    edx, edx
        div    ecx
        test   edx, edx
        je     loc_4_1413
        mov    eax, 0xc0010015
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_1413:
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        mov    edx, dword ptr [ecx + 0x6a]
        push   eax
        push   edx
        call   sub_2_0300
loc_4_143A:
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1450:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_14B6
        add    esi, 0x1789
        push   2
        push   esi
        call   sub_4_26B0
        mov    edi, dword ptr [ebp + 0xc]
        test   edi, 8
        je     loc_4_1486
        push   3
        push   esi
        call   sub_4_26B0
loc_4_1486:
        test   edi, 2
        je     loc_4_1496
        push   4
        push   esi
        call   sub_4_26B0
loc_4_1496:
        test   edi, 4
        je     loc_4_14A6
        push   5
        push   esi
        call   sub_4_26B0
loc_4_14A6:
        test   edi, 0x20
        je     loc_4_14B6
        push   6
        push   esi
        call   sub_4_26B0
loc_4_14B6:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_4_14C0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_14FD
        add    esi, 0x1789
        push   0
        push   0
        push   esi
        call   sub_4_2950
        mov    edi, dword ptr [ebp + 0x10]
        test   edi, edi
        je     loc_4_14FD
        mov    ebx, dword ptr [ebp + 0x14]
loc_4_14F0:
        push   ebx
        push   esi
        add    ebx, 6
        call   sub_4_2A30
        dec    edi
        jne    loc_4_14F0
loc_4_14FD:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1510:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x14]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_1520:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], 0x103
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x4a]
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_16B2
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_4_16A9
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        inc    dword ptr [edi + 0x14]
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_04F8
loc_4_1566:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_169D
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_4_169D
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 0x30], cx
        add    ebx, 0x24
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 8], edi
        push   eax
        push   edi
        push   esi
        call   sub_4_1710
        test   byte ptr [edi + 0x41], 2
        je     loc_4_15BB
        cmp    eax, 1
        jne    loc_4_15AA
        mov    eax, 2
        jmp    loc_4_15BB
loc_4_15AA:
        cmp    eax, 3
        jne    loc_4_15C5
        mov    dword ptr [ebp - 4], 0
        mov    eax, 4
loc_4_15BB:
        cmp    eax, 3
        jne    loc_4_15C5
        mov    eax, 1
loc_4_15C5:
        cmp    eax, 4
        jae    loc_4_15D6
        inc    dword ptr [esi + 0x11e]
        inc    dword ptr [esi + 0x16e]
loc_4_15D6:
        cmp    eax, 1
        jne    loc_4_1627
        mov    byte ptr [ebx + 0xf], 1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_4_17F0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_4_07A0
        cmp    byte ptr [esi + 0x9c], 0
        jne    loc_4_16B9
        cmp    dword ptr [esi + 0x9e], 0
        je     loc_4_16B9
        cmp    byte ptr [esi + 0x98], 0
        je     loc_4_16B9
        push   0
        push   esi
        call   sub_4_1830
        jmp    loc_4_16B9
loc_4_1627:
        cmp    eax, 2
        jne    loc_4_165F
        mov    byte ptr [ebx + 0xf], 2
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_4_17F0
        cmp    byte ptr [esi + 0x9c], 0
        jne    loc_4_16B9
        cmp    dword ptr [esi + 0x9e], 0
        je     loc_4_16B9
        cmp    byte ptr [esi + 0x98], 0
        je     loc_4_16B9
        push   0
        push   esi
        call   sub_4_1830
        jmp    loc_4_16B9
loc_4_165F:
        cmp    eax, 3
        jne    loc_4_16B9
        mov    byte ptr [ebx + 0xf], 2
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        call   sub_4_07A0
        lea    ecx, [esi + 0xd2]
        xor    edx, edx
        xor    eax, eax
        inc    dword ptr [esi + 0xbe]
        inc    dword ptr [esi + 0x102]
        mov    dx, word ptr [ebx + 0xc]
        add    edx, dword ptr [ecx]
        mov    dword ptr [ecx], edx
        mov    ax, word ptr [ebx + 0xc]
        cmp    eax, edx
        jbe    loc_4_16B9
        inc    dword ptr [ecx + 4]
        jmp    loc_4_16B9
loc_4_169D:
        mov    dword ptr [ebp - 4], 0xc000009a
        dec    dword ptr [edi + 0x14]
        jmp    loc_4_16B9
loc_4_16A9:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_4_16B9
loc_4_16B2:
        mov    dword ptr [ebp - 4], 0xc001000d
loc_4_16B9:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_16F3
        cmp    dword ptr [esi + 0x4a], 2
        jne    loc_4_16CE
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_16D8
loc_4_16CE:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_16F3
loc_4_16D8:
        push   0
        add    esi, 0x18
        push   esi
        mov    byte ptr [esi - 3], 1
        call   sub_1_04AA
loc_4_16E7:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_16F3:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 0x4a]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1710:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        lea    ecx, [ebp - 8]
        lea    esi, [eax + 0x24]
        push   0
        push   ecx
        push   0
        push   0
        push   eax
        call   sub_1_04F8
loc_4_172E:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_051C
loc_4_173F:
        mov    edx, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx]
        test   cl, 1
        je     loc_4_176C
        cmp    dword ptr [edx + 2], -1
        jne    loc_4_1763
        cmp    cl, 0xff
        jne    loc_4_1763
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_4_1763
        mov    byte ptr [esi + 0xe], 3
        add    esi, 0xe
        jmp    loc_4_17B4
loc_4_1763:
        mov    byte ptr [esi + 0xe], 2
        add    esi, 0xe
        jmp    loc_4_17B4
loc_4_176C:
        mov    byte ptr [esi + 0xe], 1
        add    esi, 0xe
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        add    ecx, 6
        mov    eax, dword ptr [eax + 2]
        cmp    edx, eax
        jbe    loc_4_178D
        mov    eax, 1
        jmp    loc_4_17AD
loc_4_178D:
        jae    loc_4_1796
        mov    eax, 0xffffffff
        jmp    loc_4_17AD
loc_4_1796:
        mov    cx, word ptr [ecx]
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jbe    loc_4_17AB
        mov    eax, 1
        jmp    loc_4_17AD
loc_4_17AB:
        sbb    eax, eax
loc_4_17AD:
        test   eax, eax
        jne    loc_4_17B4
        mov    byte ptr [esi], 4
loc_4_17B4:
        cmp    byte ptr [esi], 4
        jne    loc_4_17C5
        mov    eax, 3
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_17C5:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    edx, dword ptr [ecx + 0x6a]
        push   edx
        call   sub_1_0330
loc_4_17D5:
        pop    esi
        cmp    al, 1
        mov    eax, 2
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_17F0:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x24], 0
        mov    edx, dword ptr [eax + 0xa2]
        test   edx, edx
        jne    loc_4_1813
        mov    dword ptr [eax + 0x9e], ecx
        jmp    loc_4_1816
loc_4_1813:
        mov    dword ptr [edx + 0x24], ecx
loc_4_1816:
        pop    ebp
        mov    dword ptr [eax + 0xa2], ecx
        inc    dword ptr [eax + 0x176]
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x98], 0
        je     loc_4_1899
        cmp    byte ptr [esi + 0x9c], 0
        jne    loc_4_1899
        mov    eax, dword ptr [esi + 0x9e]
        test   eax, eax
        je     loc_4_1899
        cmp    byte ptr [esi + 0x13], 0
        jne    loc_4_1899
        mov    byte ptr [esi + 0x9c], 1
        mov    cl, byte ptr [ebp + 0xc]
        push   ecx
        push   eax
        push   esi
        call   sub_4_18A0
        test   al, al
        je     loc_4_1892
        mov    eax, dword ptr [esi + 0x9e]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x9e], eax
        test   eax, eax
        jne    loc_4_188C
        mov    dword ptr [esi + 0xa2], 0
loc_4_188C:
        dec    dword ptr [esi + 0x176]
loc_4_1892:
        mov    byte ptr [esi + 0x9c], 0
loc_4_1899:
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_18A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x18a], bl
        je     loc_4_18C1
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_18C1:
        lea    esi, [edi + 0x1789]
        push   esi
        call   sub_4_1F90
        test   ax, ax
        je     loc_4_18DD
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_18DD:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   ecx
        call   sub_1_04F8
loc_4_18F0:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_192B
loc_4_18F6:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_051C
loc_4_1907:
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        push   esi
        call   sub_4_1FB0
        add    ebx, dword ptr [ebp - 8]
        lea    ecx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0540
loc_4_1925:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_18F6
loc_4_192B:
        push   esi
        call   sub_4_1F70
        mov    word ptr [edi + 0x195], bx
        push   esi
        call   sub_4_19C0
        cmp    byte ptr [edi + 0x18b], 0
        mov    eax, dword ptr [ebp + 0xc]
        je     loc_4_1968
        push   esi
        mov    dword ptr [edi + 0x19b], eax
        mov    byte ptr [edi + 0x18a], 1
        call   sub_4_19D0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_1968:
        push   esi
        mov    dword ptr [edi + 0x197], eax
        call   sub_4_19D0
        push   esi
        inc    dword ptr [edi + 0x18d]
        mov    dword ptr [edi + 0x182], 4
        mov    dword ptr [edi + 0x186], 0
        mov    byte ptr [edi + 0x18b], 1
        call   sub_4_19C0
        push   esi
        call   sub_4_1FF0
        push   esi
        call   sub_4_19D0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19C0:
        cli    
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19D0:
        sti    
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
loc_4_19E3:
        mov    edx, 0x2fd
        in     al, dx
        test   al, 0x20
        je     loc_4_19E3
        mov    edx, 0x2f8
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        lea    eax, [ebp - 0x10]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 8]
        mov    edi, dword ptr [esi]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_0480
loc_4_1A22:
        mov    cl, byte ptr [ebp - 0xe]
        mov    byte ptr [edi + 0x17e2], cl
        mov    dl, byte ptr [ebp - 0xd]
        mov    byte ptr [edi + 0x17e3], dl
        mov    al, byte ptr [ebp - 0xc]
        mov    byte ptr [edi + 0x17e4], al
        mov    cl, byte ptr [ebp - 0xb]
        mov    byte ptr [edi + 0x17e5], cl
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_1A58
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1A58:
        mov    al, byte ptr [esi + 0x5a]
        and    al, 0xe
        shr    al, 1
        and    eax, 0xff
        cmp    eax, 7
        ja     loc_4_1A71
        jmp    dword ptr [eax*4]             ; -> obj4:0x1B10
loc_4_1A71:
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1A7D:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1A8F:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x1300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AA1:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x2300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AB3:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x3300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AC5:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x4300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AD7:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x5300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AE9:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x6300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AFB:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0xc], 0x7300
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        ; ---- dispatch table (8 entries) ----
        dd  loc_4_1A7D
        dd  loc_4_1A8F
        dd  loc_4_1AA1
        dd  loc_4_1AB3
        dd  loc_4_1AC5
        dd  loc_4_1AD7
        dd  loc_4_1AE9
        dd  loc_4_1AFB
sub_4_1B30:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x179b]
        lea    edi, [esi + 0x14e]
        shl    eax, 4
        mov    dword ptr [esi + 0x14a], eax
        cmp    word ptr [esi + 0x17a1], 0x4000
        jne    loc_4_1B68
        push   0x4000
        jmp    loc_4_1B6D
loc_4_1B68:
        push   0x10000
loc_4_1B6D:
        push   eax
        lea    ecx, [ebp - 4]
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        push   edi
        push   ecx
        call   sub_1_059A
loc_4_1B7C:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_1B8E
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1B8E:
        mov    eax, dword ptr [edi]
        pop    edi
        mov    dword ptr [esi + 0x179d], eax
        pop    esi
        xor    ax, ax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  066h, 08Bh, 04Ch, 024h, 010h, 08Bh, 0ECh, 083h, 0ECh, 004h, 080h, 0F1h, 001h, 08Dh, 045h, 0FCh
        db  056h, 050h, 08Bh, 075h, 014h, 056h, 08Bh, 045h, 008h, 051h, 08Bh, 04Dh, 00Ch, 051h, 08Bh, 010h
        db  08Bh, 04Ah, 066h, 051h, 0E8h
        dd  loc_1_0492               ; fixup
        db  08Bh, 055h, 0FCh, 08Bh, 04Dh, 018h, 089h, 011h, 083h, 03Eh, 001h, 05Eh, 01Bh, 0C9h, 08Bh, 0E5h
        db  0F7h, 0D9h, 066h, 08Bh, 0C1h, 05Dh, 0C2h, 014h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh
        db  0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1C00:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        lea    ecx, [eax + eax*4]
        lea    edx, [ecx + ecx*4]
        lea    eax, [edx + edx*4]
        shl    eax, 3
        push   eax
        call   sub_1_04C8
loc_4_1C19:
        xor    ax, ax
        pop    ebp
        ret    8
sub_4_1C20:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    al, byte ptr [esp + 0x10]
        out    dx, al
        xor    ax, ax
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 054h
        db  024h, 00Ch, 08Bh, 0ECh, 08Bh, 044h, 024h, 010h, 025h, 0FFh, 0FFh, 000h, 000h, 05Dh, 066h, 0EFh
        db  066h, 033h, 0C0h, 0C2h, 00Ch, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 054h
        db  024h, 00Ch, 08Bh, 0ECh, 08Bh, 044h, 024h, 010h, 0EFh, 066h, 033h, 0C0h, 05Dh, 0C2h, 00Ch, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1C80:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        in     al, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 066h, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h
        db  000h, 08Dh, 049h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 014h, 08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh
        db  055h, 00Ch, 051h, 052h, 0E8h
        dd  loc_1_03F1               ; fixup
        db  066h, 033h, 0C0h, 05Dh, 0C2h, 010h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 014h
        db  08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh, 055h, 00Ch, 051h, 052h, 0E8h
        dd  loc_1_03B4               ; fixup
        db  066h, 033h, 0C0h, 05Dh, 0C2h, 010h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 018h
        db  08Bh, 0ECh, 08Bh, 008h, 051h, 08Bh, 055h, 014h, 052h, 08Bh, 045h, 00Ch, 06Ah, 000h, 08Bh, 04Dh
        db  008h, 050h, 08Bh, 011h, 08Bh, 042h, 066h, 050h, 0E8h
        dd  loc_1_0498               ; fixup
        db  066h, 033h, 0C0h, 05Dh, 0C2h, 014h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh
        db  044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 05Dh, 0FFh, 081h, 07Eh, 001h, 000h, 000h, 0C2h, 004h
        db  000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh
        db  044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 05Dh, 0C6h, 081h, 08Ch, 001h, 000h, 000h, 001h, 0C2h
        db  004h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh
        db  044h, 024h, 008h, 08Bh, 0ECh, 06Ah, 000h, 08Bh, 048h, 00Ch, 083h, 0C1h, 00Fh, 051h, 050h, 0E8h
        db  0AAh, 0FEh, 0FFh, 0FFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 055h, 08Bh
        db  044h, 024h, 008h, 08Bh, 0ECh, 06Ah, 01Fh, 08Bh, 048h, 00Ch, 083h, 0C1h, 00Fh, 051h, 050h, 0E8h
        db  08Ah, 0FEh, 0FFh, 0FFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1DA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0x23
        push   eax
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C80
        mov    byte ptr [esi + 0x3e], al
        and    al, 0x1f
        push   eax
        mov    byte ptr [esi + 0x3e], al
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        mov    al, byte ptr [esi + 0x3e]
        pop    esi
        and    eax, 0xffff00ff
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_19C0
        push   0x62
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C80
        mov    bl, al
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   esi
        call   sub_4_19D0
        mov    al, byte ptr [esi + 0x3c]
        cmp    bl, al
        jne    loc_4_1E53
        mov    ax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1E53:
        xor    ecx, ecx
        mov    cl, al
        shl    ecx, 8
        add    ecx, dword ptr [esi + 0x14]
        mov    dl, byte ptr [ecx]
        mov    byte ptr [esi + 0x3f], dl
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [esi + 0x3d], al
        mov    ax, word ptr [ecx + 2]
        sub    ax, 4
        mov    word ptr [ebp - 2], ax
        cmp    ax, 0x5ea
        ja     loc_4_1EE5
        test   dl, 6
        jne    loc_4_1EE5
        and    eax, 0xffff
        mov    edi, dword ptr [esi + 0x50]
        mov    edx, ecx
        mov    dword ptr [ebp - 8], eax
        sub    edx, edi
        lea    eax, [edx + eax + 4]
        test   eax, eax
        jle    loc_4_1EB3
        mov    ebx, dword ptr [esi + 0x54]
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        test   eax, eax
        jle    loc_4_1EB3
loc_4_1EA4:
        mov    dx, word ptr [ebx]
        add    ebx, 2
        mov    word ptr [edi], dx
        add    edi, 2
        dec    eax
        jne    loc_4_1EA4
loc_4_1EB3:
        add    ecx, 4
        mov    esi, dword ptr [esi + 0x40]
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        cdq    
        sub    eax, edx
        sar    eax, 1
        test   ax, ax
        je     loc_4_1ED8
loc_4_1EC8:
        mov    dx, word ptr [ecx]
        add    ecx, 2
        mov    word ptr [esi], dx
        add    esi, 2
        dec    ax
        jne    loc_4_1EC8
loc_4_1ED8:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1EE5:
        push   esi
        call   sub_4_1F00
        mov    ax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1F00:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [ecx + 0x3d]
        mov    byte ptr [ecx + 0x3c], al
        cmp    byte ptr [ecx + 0x38], al
        jne    loc_4_1F2C
        mov    al, byte ptr [ecx + 0x3a]
        dec    al
        push   eax
        mov    eax, dword ptr [ecx + 0xc]
        add    eax, 3
        push   eax
        push   ecx
        call   sub_4_1C20
        xor    ax, ax
        pop    ebp
        ret    4
loc_4_1F2C:
        dec    al
        push   eax
        mov    eax, dword ptr [ecx + 0xc]
        add    eax, 3
        push   eax
        push   ecx
        call   sub_4_1C20
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1F50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0x1f
        mov    ecx, dword ptr [eax + 0xc]
        add    ecx, 0xf
        push   ecx
        push   eax
        call   sub_4_1C20
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_1F70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 0x4c], 0x3c
        mov    ebp, esp
        jae    loc_4_1F84
        mov    word ptr [eax + 0x4c], 0x3c
loc_4_1F84:
        xor    ax, ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1F90:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x4c], 0
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x44]
        pop    ebp
        mov    dword ptr [eax + 0x48], ecx
        xor    ax, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1FB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [esp + 0x10]
        mov    ebp, esp
        test   dx, dx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x48]
        je     loc_4_1FD7
        mov    di, dx
        mov    ecx, dword ptr [ebp + 0xc]
loc_4_1FCD:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [esi], bl
        inc    esi
        dec    di
        jne    loc_4_1FCD
loc_4_1FD7:
        add    word ptr [eax + 0x4c], dx
        pop    edi
        mov    dword ptr [eax + 0x48], esi
        pop    esi
        xor    ax, ax
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1FF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x4c]
        mov    ebx, dword ptr [esi + 0x44]
        movzx  edi, byte ptr [esi + 0x39]
        shl    edi, 8
        inc    eax
        cdq    
        add    edi, dword ptr [esi + 0x14]
        sub    eax, edx
        sar    eax, 1
        test   ax, ax
        je     loc_4_2028
loc_4_2018:
        mov    cx, word ptr [ebx]
        add    ebx, 2
        mov    word ptr [edi], cx
        add    edi, 2
        dec    ax
        jne    loc_4_2018
loc_4_2028:
        mov    ax, word ptr [esi + 0x4c]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 5
        push   eax
        push   esi
        call   sub_4_1C20
        mov    ax, word ptr [esi + 0x4c]
        shr    ax, 8
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x39]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 4
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x26
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        xor    byte ptr [esi + 0x39], 0xe
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_2080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0
        mov    ecx, dword ptr [eax + 0xc]
        add    ecx, 0xf
        push   ecx
        push   eax
        call   sub_4_1C20
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_20A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0x1f
        mov    ecx, dword ptr [eax + 0xc]
        add    ecx, 0xf
        push   ecx
        push   eax
        call   sub_4_1C20
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_20C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0xa]
        add    al, 8
        push   eax
        push   0x96
        push   esi
        call   sub_4_1C20
        push   0x101
        push   esi
        call   sub_4_1C80
        cmp    al, 0x56
        jne    loc_4_213F
        push   0x100
        push   esi
        call   sub_4_1C80
        test   al, al
        je     loc_4_2108
        cmp    al, 2
        je     loc_4_2108
        cmp    al, 4
        je     loc_4_2108
        cmp    al, 6
        jne    loc_4_211C
loc_4_2108:
        mov    word ptr [esi + 0x18], 0x4000
        mov    di, 1
        mov    cx, word ptr [esi + 0xa]
        add    cl, 8
        mov    byte ptr [esi + 0x58], cl
loc_4_211C:
        cmp    al, 1
        je     loc_4_212C
        cmp    al, 3
        je     loc_4_212C
        cmp    al, 5
        je     loc_4_212C
        cmp    al, 7
        jne    loc_4_213F
loc_4_212C:
        mov    word ptr [esi + 0x18], 0
        mov    di, 1
        mov    ax, word ptr [esi + 0xa]
        add    al, 8
        mov    byte ptr [esi + 0x58], al
loc_4_213F:
        push   0
        push   0x96
        push   esi
        call   sub_4_1C20
        test   di, di
        jne    loc_4_215C
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_215C:
        mov    al, byte ptr [esi + 0x58]
        push   eax
        push   0x96
        push   esi
        call   sub_4_1C20
        push   0x81
        push   0x102
        push   esi
        call   sub_4_1C20
        push   0x103
        push   esi
        call   sub_4_1C80
        shr    al, 1
        and    eax, 7
        dec    eax
        cmp    eax, 6
        ja     loc_4_2199
        jmp    dword ptr [eax*4]             ; -> obj4:0x232C
loc_4_2199:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_21A4:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x1300
        jmp    loc_4_21EF
loc_4_21AF:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x2300
        jmp    loc_4_21EF
loc_4_21BA:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x3300
        jmp    loc_4_21EF
loc_4_21C5:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x4300
        jmp    loc_4_21EF
loc_4_21D0:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x5300
        jmp    loc_4_21EF
loc_4_21DB:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x6300
        jmp    loc_4_21EF
loc_4_21E6:
        lea    edi, [esi + 0xc]
        mov    dword ptr [edi], 0x7300
loc_4_21EF:
        push   0x103
        push   esi
        call   sub_4_1C80
        shr    al, 4
        and    eax, 7
        cmp    eax, 7
        ja     loc_4_220C
        jmp    dword ptr [eax*4]             ; -> obj4:0x2348
loc_4_220C:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2217:
        mov    word ptr [esi + 0x12], 0xc000
        jmp    loc_4_2255
loc_4_221F:
        mov    word ptr [esi + 0x12], 0xc400
        jmp    loc_4_2255
loc_4_2227:
        mov    word ptr [esi + 0x12], 0xc800
        jmp    loc_4_2255
loc_4_222F:
        mov    word ptr [esi + 0x12], 0xcc00
        jmp    loc_4_2255
loc_4_2237:
        mov    word ptr [esi + 0x12], 0xd000
        jmp    loc_4_2255
loc_4_223F:
        mov    word ptr [esi + 0x12], 0xd400
        jmp    loc_4_2255
loc_4_2247:
        mov    word ptr [esi + 0x12], 0xd800
        jmp    loc_4_2255
loc_4_224F:
        mov    word ptr [esi + 0x12], 0xdc00
loc_4_2255:
        push   esi
        call   sub_4_1B30
        test   ax, ax
        je     loc_4_226B
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_226B:
        push   0x104
        push   esi
        call   sub_4_1C80
        and    eax, 3
        cmp    eax, 3
        ja     loc_4_2285
        jmp    dword ptr [eax*4]             ; -> obj4:0x2368
loc_4_2285:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2290:
        mov    word ptr [esi + 0x10], 3
        jmp    loc_4_22AE
loc_4_2298:
        mov    word ptr [esi + 0x10], 7
        jmp    loc_4_22AE
loc_4_22A0:
        mov    word ptr [esi + 0x10], 9
        jmp    loc_4_22AE
loc_4_22A8:
        mov    word ptr [esi + 0x10], 0xc
loc_4_22AE:
        xor    eax, eax
        mov    edx, dword ptr [esi + 0x14]
        mov    word ptr [esi + 0x1a], ax
loc_4_22B7:
        mov    ecx, eax
        add    edx, 2
        inc    ax
        and    ecx, 0xffff
        cmp    ax, 6
        lea    ebx, [ecx + esi]
        mov    cl, byte ptr [edx - 2]
        mov    byte ptr [ebx + 0x22], cl
        mov    byte ptr [ebx + 0x1c], cl
        jb     loc_4_22B7
        push   0
        push   0x96
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [edi]
        push   eax
        push   esi
        call   sub_4_1C80
        cmp    al, 0xff
        jne    loc_4_22FB
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_22FB:
        push   0x21
        mov    eax, dword ptr [edi]
        push   eax
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [edi]
        push   eax
        push   esi
        call   sub_4_1C80
        cmp    al, 0x21
        je     loc_4_2322
        cmp    al, 0x23
        je     loc_4_2322
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2322:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        ; ---- dispatch table (19 entries) ----
        dd  loc_4_21A4
        dd  loc_4_21AF
        dd  loc_4_21BA
        dd  loc_4_21C5
        dd  loc_4_21D0
        dd  loc_4_21DB
        dd  loc_4_21E6
        dd  loc_4_2217
        dd  loc_4_221F
        dd  loc_4_2227
        dd  loc_4_222F
        dd  loc_4_2237
        dd  loc_4_223F
        dd  loc_4_2247
        dd  loc_4_224F
        dd  loc_4_2290
        dd  loc_4_2298
        dd  loc_4_22A0
        dd  loc_4_22A8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2380:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x38], 1
        mov    byte ptr [esi + 0x3c], 2
        mov    dx, word ptr [esi + 0x18]
        inc    edx
        mov    ecx, dword ptr [esi + 0x14]
        shr    edx, 8
        lea    eax, [ecx + 0x100]
        and    dl, 0xfe
        mov    dword ptr [esi + 0x54], eax
        sub    dl, 0xc
        xor    eax, eax
        mov    byte ptr [esi + 0x39], dl
        sub    dl, 4
        mov    al, dl
        push   0
        shl    eax, 8
        mov    byte ptr [esi + 0x3a], dl
        add    eax, ecx
        mov    dword ptr [esi + 0x50], eax
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x21
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0x49
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xe
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xc
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x38]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        inc    eax
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x38]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 3
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x3a]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 2
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x61
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        inc    eax
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x1d]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 2
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x1e]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 3
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x1f]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 4
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 5
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x21]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x3c]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xe
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x12
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0x20
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0x23
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0x22
        push   eax
        push   esi
        call   sub_4_1C20
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_25C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C80
        mov    bl, al
        push   0x21
        and    bl, 4
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        push   2
        push   esi
        call   sub_4_1C00
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        test   bl, bl
        jne    loc_4_2615
        mov    word ptr [esi + 0x4e], 0
        jmp    loc_4_2642
loc_4_2615:
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C80
        and    ax, 0xff
        mov    cl, byte ptr [esi + 0x3e]
        mov    word ptr [esi + 0x4e], 0
        and    ecx, 0xffff00ff
        or     ax, cx
        test   al, 0xa
        jne    loc_4_2642
        mov    word ptr [esi + 0x4e], 1
loc_4_2642:
        push   2
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        xor    ax, ax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2670:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x10
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        cmp    word ptr [esi + 0x4e], 0
        je     loc_4_26A8
        push   0x26
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
loc_4_26A8:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
sub_4_26B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    di, word ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x34], di
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        movsx  eax, di
        dec    eax
        cmp    eax, 5
        ja     loc_4_2920
        jmp    dword ptr [eax*4]             ; -> obj4:0x2938
loc_4_26E3:
        push   0x21
        mov    eax, dword ptr [esi + 0xc]
        jmp    loc_4_2919
loc_4_26ED:
        push   0x22
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        jmp    loc_4_2913
loc_4_2700:
        push   0x22
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        jmp    loc_4_2913
loc_4_2713:
        push   0x62
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        mov    al, byte ptr [esi + 0x28]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x29]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x2a]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x2b]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x2c]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x2d]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x2e]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 0xe
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 0x2f]
        mov    eax, dword ptr [esi + 0xc]
        push   ecx
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0xc
        jmp    loc_4_2913
loc_4_27BA:
        push   0x62
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xe
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0xc
        jmp    loc_4_2913
loc_4_2869:
        push   0x62
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xe
        push   eax
        push   esi
        call   sub_4_1C20
        push   0xff
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0x1f
loc_4_2913:
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xc
loc_4_2919:
        push   eax
        push   esi
        call   sub_4_1C20
loc_4_2920:
        push   0x1f
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        ; ---- dispatch table (6 entries) ----
        dd  loc_4_26E3
        dd  loc_4_26ED
        dd  loc_4_2700
        dd  loc_4_2713
        dd  loc_4_27BA
        dd  loc_4_2869
sub_4_2950:
        push   ebp
        cmp    word ptr [esp + 0x10], 0
        mov    ebp, esp
        push   esi
        jne    loc_4_2A24
        mov    esi, dword ptr [ebp + 8]
        push   0
        lea    eax, [esi + 0x28]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 6], 0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x62
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xa
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xb
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xc
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xd
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xe
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
        push   0x22
        mov    ecx, dword ptr [esi + 0xc]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0x1f
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 0xf
        push   eax
        push   esi
        call   sub_4_1C20
loc_4_2A24:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_2A30:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        call   sub_4_2B40
        mov    bx, ax
        mov    edi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xf
        push   eax
        push   edi
        call   sub_4_1C20
        push   0x62
        mov    ecx, dword ptr [edi + 0xc]
        push   ecx
        push   edi
        call   sub_4_1C20
        mov    ax, bx
        sar    ax, 3
        movsx  esi, ax
        mov    eax, dword ptr [edi + 0xc]
        and    bl, 7
        add    eax, esi
        add    eax, 8
        push   eax
        push   edi
        call   sub_4_1C80
        mov    dl, 1
        mov    cl, bl
        shl    dl, cl
        or     dl, al
        mov    eax, dword ptr [edi + 0xc]
        push   edx
        add    eax, esi
        add    eax, 8
        push   eax
        push   edi
        call   sub_4_1C20
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 8
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x28], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 9
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x29], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xa
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x2a], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xb
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x2b], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xc
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x2c], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xd
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x2d], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xe
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 0x2e], al
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xf
        push   eax
        push   edi
        call   sub_4_1C80
        push   0x22
        mov    ecx, dword ptr [edi + 0xc]
        push   ecx
        mov    byte ptr [edi + 0x2f], al
        push   edi
        call   sub_4_1C20
        push   0x1f
        mov    eax, dword ptr [edi + 0xc]
        add    eax, 0xf
        push   eax
        push   edi
        call   sub_4_1C20
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2B40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax]
        push   ebx
        mov    ax, word ptr [eax + 4]
        push   esi
        push   edi
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [edx], ecx
        mov    word ptr [edx + 4], ax
        mov    eax, 0xffffffff
loc_4_2B67:
        movsx  ecx, word ptr [ebp - 2]
        mov    si, 8
        lea    ecx, [ebp + ecx - 8]
loc_4_2B73:
        mov    edx, eax
        and    edx, 0x80000000
        cmp    edx, 1
        mov    dl, byte ptr [ecx]
        sbb    edi, edi
        mov    bl, dl
        inc    di
        add    eax, eax
        shr    dl, 1
        and    bx, 1
        mov    byte ptr [ecx], dl
        xor    di, bx
        test   di, di
        je     loc_4_2BA3
        movsx  edx, di
        xor    eax, 0x4c11db7
        or     eax, edx
loc_4_2BA3:
        dec    si
        jne    loc_4_2B73
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 6
        jl     loc_4_2B67
        and    eax, 0xff000000
        pop    edi
        shr    eax, 0x1a
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2BE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x4a]
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_2C52
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_4_2C4B
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        inc    dword ptr [edi + 0x14]
        test   ecx, ecx
        jne    loc_4_2C2F
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 0x82]
        add    eax, 0xe
        mov    edx, dword ptr [ebp + 0x14]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_0546
loc_4_2C28:
        dec    dword ptr [edi + 0x14]
        xor    edi, edi
        jmp    loc_4_2C57
loc_4_2C2F:
        push   0
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, ecx
        push   eax
        call   sub_4_08F0
        dec    dword ptr [edi + 0x14]
        xor    edi, edi
        jmp    loc_4_2C57
loc_4_2C4B:
        mov    edi, 0xc001000c
        jmp    loc_4_2C57
loc_4_2C52:
        mov    edi, 0xc001000d
loc_4_2C57:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_2C8D
        cmp    dword ptr [esi + 0x4a], 2
        jne    loc_4_2C6C
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_2C76
loc_4_2C6C:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_2C8D
loc_4_2C76:
        push   0
        add    esi, 0x18
        push   esi
        mov    byte ptr [esi - 3], 1
        call   sub_1_04AA
loc_4_2C85:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_4_2C8D:
        mov    eax, edi
        dec    dword ptr [esi + 0x4a]
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        mov    word ptr [ebp - 0x24], 0
        mov    word ptr [ebp - 0x22], 8
        mov    word ptr [ebp - 0x20], 0x5600
        mov    word ptr [ebp - 0x1e], 0x5601
        mov    word ptr [ebp - 0x1c], 0x5602
        mov    word ptr [ebp - 0x1a], 0x5603
        mov    word ptr [ebp - 0x18], 0x5604
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    ecx, [ebp - 0x24]
        push   ecx
        lea    edx, [ebp - 4]
        mov    word ptr [ebp - 0x16], 0x5605
        mov    word ptr [ebp - 0x14], 0x5606
        mov    word ptr [ebp - 0x12], 0x5607
        push   edx
        call   sub_1_041A
loc_4_2CF5:
        push   -1
        lea    ecx, [ebp - 0xc]
        push   0
        push   8
        push   ecx
        call   sub_1_04CE
loc_4_2D04:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_4_2D16
        mov    eax, 0xc000009a
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2D16:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x5c], 3
        mov    byte ptr [ebp - 0x5b], 0
        mov    dword ptr [ebp - 0x56], 0     ; -> loc_4_3150
        mov    dword ptr [ebp - 0x52], 0     ; -> loc_4_33A0
        mov    dword ptr [ebp - 0x4e], 0     ; -> loc_4_1520
        mov    dword ptr [ebp - 0x4a], 0     ; -> loc_4_2BE0
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_4_36A0
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_4_09D0
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_4_0CD0
        mov    edx, dword ptr [0]            ; -> obj5:0x0150
        mov    eax, dword ptr [4]            ; -> obj5:0x0154
        push   0x36
        mov    dword ptr [ebp - 0x2e], edx
        lea    ecx, [ebp - 0x5c]
        mov    dword ptr [ebp - 0x2a], eax
        push   ecx
        mov    edx, dword ptr [ebp - 0xc]
        push   edx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_4_3810
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_4_3280
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_4_37A0
        lea    eax, [edx + 4]
        push   eax
        push   ecx
        call   sub_1_0426
loc_4_2D98:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_2DA6
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2DA6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0420
loc_4_2DB1:
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_2DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        lea    eax, [esi + 0x1789]
        push   eax
        mov    dword ptr [esi + 0x1789], esi
        mov    dword ptr [esi + 0x178d], ebx
        call   sub_4_1A00
        test   ax, ax
        je     loc_4_2DF0
        mov    byte ptr [ebp + 0x14], 1
loc_4_2DF0:
        lea    edi, [ebp - 0x2c]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0x2c], eax
        mov    byte ptr [ebp - 0x28], al
        mov    byte ptr [ebp - 0x27], al
        mov    dword ptr [ebp - 0x26], eax
        mov    dword ptr [ebp - 0x1a], 0x5ea
        mov    dword ptr [ebp - 0x16], 1
        mov    dword ptr [ebp - 0xe], 4
        lea    edx, [ebp - 0x2c]
        mov    dword ptr [ebp - 0x1e], eax
        lea    eax, [esi + 0x1795]
        push   edx
        mov    dword ptr [ebp - 0x22], 3
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0xa], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x12], ecx
        push   eax
        mov    ecx, dword ptr [esi]
        push   ebx
        lea    edx, [esi + 0x66]
        push   esi
        push   ecx
        push   edx
        call   sub_1_0432
loc_4_2E4D:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_3138
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_4_2E88
        push   0
        mov    eax, dword ptr [esi + 0x66]
        push   0xaa00000c
        push   eax
        call   sub_1_05A0
loc_4_2E6E:
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_0438
loc_4_2E7A:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_2E88:
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_20C0
        test   ax, ax
        je     loc_4_2ED0
        cmp    ax, 2
        push   0
        jne    loc_4_2EA8
        push   0xaa000001
        jmp    loc_4_2EAD
loc_4_2EA8:
        push   0xaa000003
loc_4_2EAD:
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_05A0
loc_4_2EB6:
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_0438
loc_4_2EC2:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_2ED0:
        lea    edi, [esi + 0x17ab]
        push   6
        push   edi
        lea    eax, [esi + 6]
        push   eax
        call   sub_1_05B4
loc_4_2EE2:
        push   6
        lea    eax, [esi + 0xc]
        push   edi
        xor    ebx, ebx
        push   eax
        call   sub_1_05B4
loc_4_2EF0:
        lea    eax, [esi + 0x4e]
        mov    dword ptr [esi + 0x56], ebx
        mov    dword ptr [esi + 0x52], eax
        mov    byte ptr [esi + 5], bl
        mov    dword ptr [eax], eax
        mov    byte ptr [esi + 0x12], bl
        lea    eax, [esi + 0x5a]
        mov    byte ptr [esi + 0x13], bl
        mov    dword ptr [esi + 0x5e], eax
        mov    byte ptr [esi + 0x15], bl
        mov    dword ptr [eax], eax
        mov    byte ptr [esi + 0x16], bl
        mov    byte ptr [esi + 0x17], bl
        mov    dword ptr [esi + 0x7a], ebx
        mov    dword ptr [esi + 0x7e], ebx
        mov    dword ptr [esi + 0x86], ebx
        mov    eax, 1
        mov    dword ptr [esi + 0x8a], ebx
        mov    dword ptr [esi + 0xb6], ebx
        mov    dword ptr [esi + 0xba], ebx
        mov    byte ptr [esi + 0x96], al
        mov    byte ptr [esi + 0x97], al
        mov    byte ptr [esi + 0x98], al
        mov    byte ptr [esi + 0x99], al
        mov    byte ptr [esi + 0x9a], bl
        mov    byte ptr [esi + 0x9b], bl
        mov    byte ptr [esi + 0x9c], bl
        mov    byte ptr [esi + 0x9d], bl
        mov    dword ptr [esi + 0x9e], ebx
        mov    dword ptr [esi + 0xa2], ebx
        mov    dword ptr [esi + 0xa6], ebx
        mov    dword ptr [esi + 0xaa], ebx
        mov    dword ptr [esi + 0xae], ebx
        mov    dword ptr [esi + 0xb2], ebx
        mov    dword ptr [esi + 0x4a], eax
        mov    byte ptr [esi + 0x92], bl
        lea    eax, [esi + 0xbe]
        mov    byte ptr [esi + 0x93], bl
        mov    byte ptr [esi + 0x94], bl
        mov    dword ptr [esi + 0x8e], ebx
        lea    edi, [esi + 0xd2]
        mov    byte ptr [esi + 4], bl
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0xc], ebx
        mov    dword ptr [eax + 0x10], ebx
        xor    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        mov    ecx, 8
        lea    edi, [esi + 0x102]
        rep stosd dword ptr es:[edi], eax
        mov    ecx, 7
        lea    eax, [esi + 0x122]
        lea    edi, [esi + 0x12e]
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0x76], ebx
        mov    dword ptr [esi + 0x18], ebx
        mov    dword ptr [esi + 0x20], esi
        mov    dword ptr [esi + 0x24], ebx
        lea    ecx, [esi + 0x6a]
        lea    edx, [esi + 0x62]
        push   ecx
        lea    eax, [esi + 0xc]
        push   edx
        mov    dword ptr [esi + 0x2c], esi
        mov    dword ptr [esi + 0x1c], 0     ; -> loc_4_0160
        mov    dword ptr [esi + 0x28], 0     ; -> loc_4_0230
        push   eax
        push   0                             ; -> loc_4_1510
        push   0                             ; -> loc_4_1450
        push   0                             ; -> loc_4_14C0
        push   0x10
        call   sub_2_0000
loc_4_3036:
        test   al, al
        jne    loc_4_3067
        push   2
        mov    eax, dword ptr [esi + 0x66]
        push   ebx
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_05A0
loc_4_304D:
        add    esp, 0x14
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_0438
loc_4_3059:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3067:
        xor    eax, eax
        lea    edi, [esi + 0x30]
        mov    ax, word ptr [esi + 0x1799]
        push   2
        push   1
        mov    ecx, dword ptr [esi + 0x66]
        push   eax
        push   eax
        push   0                             ; -> loc_4_0040
        lea    eax, [ebp - 4]
        push   esi
        push   0                             ; -> loc_4_0000
        push   ecx
        push   edi
        push   eax
        call   sub_1_04B6
loc_4_3092:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_30CD
        push   1
        mov    eax, dword ptr [esi + 0x66]
        push   0
        push   2
        push   0xaa000005
        push   eax
        call   sub_1_05A0
loc_4_30AC:
        add    esp, 0x14
        mov    eax, dword ptr [esi + 0x6a]
        push   eax
        call   sub_2_0120
loc_4_30B8:
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_0438
loc_4_30C1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_30CD:
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_2380
        test   ax, ax
        je     loc_4_3104
        push   edi
        call   sub_1_04BC
loc_4_30E4:
        mov    eax, dword ptr [esi + 0x6a]
        push   eax
        call   sub_2_0120
loc_4_30ED:
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_0438
loc_4_30F6:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_3104:
        lea    eax, [esi + 0x789]
        lea    ecx, [esi + 0xf89]
        lea    edx, [esi + 0x1789]
        mov    dword ptr [esi + 0x17c9], eax
        mov    dword ptr [esi + 0x17cd], ecx
        push   edx
        add    esi, 0x24
        call   sub_4_20A0
        push   0x3e8
        push   esi
        call   sub_1_04AA
loc_4_3136:
        xor    eax, eax
loc_4_3138:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3150:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 5], 0
        je     loc_4_316E
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_316E:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, edx
        je     loc_4_318B
        mov    eax, dword ptr [ebp + 0x14]
loc_4_317A:
        cmp    dword ptr [eax], 0
        je     loc_4_3187
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     loc_4_317A
loc_4_3187:
        cmp    ecx, edx
        jne    loc_4_3198
loc_4_318B:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_3198:
        mov    eax, dword ptr [ebp + 0x10]
        push   -1
        push   0
        lea    ecx, [ebp - 4]
        push   0x45
        mov    dword ptr [eax], edx
        push   ecx
        inc    dword ptr [esi + 0x4a]
        call   sub_1_04CE
loc_4_31AF:
        mov    edi, eax
        test   edi, edi
        jne    loc_4_3223
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x40], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x41], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x28], 3
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x18], 0
        add    eax, 0x18
        mov    edi, 0x103
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x18
        push   eax
        push   esi
        call   sub_4_0A70
        jmp    loc_4_323A
loc_4_3223:
        push   9
        mov    eax, dword ptr [esi + 0x66]
        push   1
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_05A0
loc_4_3237:
        add    esp, 0x14
loc_4_323A:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_3272
        cmp    dword ptr [esi + 0x4a], 2
        jne    loc_4_324F
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_3259
loc_4_324F:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_3272
loc_4_3259:
        push   0
        add    esi, 0x18
        push   esi
        mov    byte ptr [esi - 3], 1
        call   sub_1_04AA
loc_4_3268:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_3272:
        mov    eax, edi
        dec    dword ptr [esi + 0x4a]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
loc_4_3280:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   -1
        lea    eax, [ebp - 4]
        push   ebx
        mov    dword ptr [ebp - 8], ebx
        push   0x1807
        push   eax
        call   sub_1_04CE
loc_4_329F:
        mov    dword ptr [ebp - 8], eax
        cmp    eax, ebx
        je     loc_4_32B4
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_32B4:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x601
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_3840
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 4]
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        mov    dword ptr [ecx], edx
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_054C
loc_4_32E8:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_330C
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x1807
        push   eax
        call   sub_1_04D4
loc_4_32FE:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_330C:
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   esi
        push   edx
        call   sub_1_0480
loc_4_331E:
        mov    cl, byte ptr [ebp - 0x16]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 0x17e2], cl
        mov    cl, byte ptr [ebp - 0x15]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 0x17e3], cl
        mov    cl, byte ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 0x17e4], cl
        mov    cl, byte ptr [ebp - 0x13]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 0x17e5], cl
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0x10]
        dec    ax
        mov    word ptr [edx + 0x1793], ax
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_3365
        mov    bl, 1
loc_4_3365:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_0558
loc_4_336E:
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   esi
        push   ecx
        call   sub_4_2DC0
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_4_3394
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x1807
        push   eax
        call   sub_1_04D4
loc_4_3394:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_33A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x4a]
        cmp    byte ptr [edi + 0x40], 0
        jne    loc_4_33DB
        mov    dword ptr [edi + 0x28], 4
        mov    dword ptr [edi + 0x1c], edi
        lea    eax, [edi + 0x18]
        push   eax
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        call   sub_4_0A70
        dec    dword ptr [edi + 0x14]
        mov    edi, 0x103
        jmp    loc_4_33E0
loc_4_33DB:
        mov    edi, 0xc0010002
loc_4_33E0:
        cmp    byte ptr [esi + 0x15], 0
        jne    loc_4_3416
        cmp    dword ptr [esi + 0x4a], 2
        jne    loc_4_33F5
        cmp    byte ptr [esi + 0x92], 0
        jne    loc_4_33FF
loc_4_33F5:
        mov    eax, dword ptr [esi + 0x5a]
        sub    eax, esi
        cmp    eax, 0x5a
        je     loc_4_3416
loc_4_33FF:
        push   0
        add    esi, 0x18
        push   esi
        mov    byte ptr [esi - 3], 1
        call   sub_1_04AA
loc_4_340E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_3416:
        mov    eax, edi
        dec    dword ptr [esi + 0x4a]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3430:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_2080
        push   esi
        lea    eax, [esi + 0x30]
        push   0                             ; -> loc_4_3780
        push   eax
        call   sub_1_04C2
loc_4_3454:
        xor    eax, eax
        lea    edi, [esi + 0x6e]
        mov    byte ptr [esi + 0x96], al
        mov    byte ptr [esi + 0x97], al
        mov    ecx, dword ptr [edi]
        mov    byte ptr [esi + 0x98], al
        mov    byte ptr [esi + 0x99], al
        test   ecx, ecx
        mov    byte ptr [esi + 0x92], 1
        je     loc_4_34CB
        cmp    dword ptr [ecx + 0x10], 4
        jne    loc_4_3490
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [ecx + 4]
        dec    dword ptr [ecx + 0x14]
        mov    ecx, dword ptr [edi]
loc_4_3490:
        test   ecx, ecx
        je     loc_4_34C4
loc_4_3494:
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 4
        je     loc_4_34BC
        cmp    eax, 3
        je     loc_4_34BC
        mov    eax, dword ptr [ecx]
        push   0xc001000d
        push   ecx
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_045C
loc_4_34B7:
        dec    dword ptr [ebx + 0x14]
        jmp    loc_4_34BE
loc_4_34BC:
        mov    edi, ecx
loc_4_34BE:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        jne    loc_4_3494
loc_4_34C4:
        mov    byte ptr [esi + 0x95], 0
loc_4_34CB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_34E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_2080
        xor    eax, eax
        mov    ecx, 1
        mov    word ptr [esi + 0x48], ax
        mov    byte ptr [esi + 0x96], cl
        mov    byte ptr [esi + 0x97], cl
        mov    byte ptr [esi + 0x98], cl
        mov    byte ptr [esi + 0x99], cl
        mov    byte ptr [esi + 0x9a], al
        mov    byte ptr [esi + 0x9b], al
        mov    byte ptr [esi + 0x9c], al
        mov    byte ptr [esi + 0x9d], al
        cmp    byte ptr [esi + 0x18b], cl
        jne    loc_4_3570
        mov    eax, dword ptr [esi + 0x197]
        mov    edi, dword ptr [eax + 0x2c]
        mov    cl, byte ptr [eax + 0x33]
        lea    edx, [eax + 0x24]
        inc    cl
        cmp    cl, 3
        mov    byte ptr [edx + 0xf], cl
        jne    loc_4_3569
        push   0xc001000c
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_044A
loc_4_3560:
        dec    dword ptr [esi + 0x11e]
        dec    dword ptr [edi + 0x14]
loc_4_3569:
        mov    byte ptr [esi + 0x18b], 0
loc_4_3570:
        cmp    byte ptr [esi + 0x18a], 1
        jne    loc_4_35B1
        mov    eax, dword ptr [esi + 0x19b]
        mov    edi, dword ptr [eax + 0x2c]
        mov    cl, byte ptr [eax + 0x33]
        lea    edx, [eax + 0x24]
        inc    cl
        cmp    cl, 3
        mov    byte ptr [edx + 0xf], cl
        jne    loc_4_35AA
        push   0xc001000c
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_044A
loc_4_35A1:
        dec    dword ptr [esi + 0x11e]
        dec    dword ptr [edi + 0x14]
loc_4_35AA:
        mov    byte ptr [esi + 0x18a], 0
loc_4_35B1:
        mov    ebx, dword ptr [esi + 0x9e]
        test   ebx, ebx
        je     loc_4_35ED
loc_4_35BB:
        mov    edi, dword ptr [ebx + 0x2c]
        mov    al, byte ptr [ebx + 0x33]
        mov    ecx, ebx
        lea    edx, [ebx + 0x24]
        inc    al
        mov    ebx, dword ptr [edx]
        mov    byte ptr [edx + 0xf], al
        cmp    al, 3
        jne    loc_4_35E9
        push   0xc001000c
        mov    eax, dword ptr [edi + 0x10]
        push   ecx
        push   eax
        call   sub_1_044A
loc_4_35E0:
        dec    dword ptr [esi + 0x11e]
        dec    dword ptr [edi + 0x14]
loc_4_35E9:
        test   ebx, ebx
        jne    loc_4_35BB
loc_4_35ED:
        mov    ebx, dword ptr [esi + 0x7a]
        test   ebx, ebx
        je     loc_4_3627
loc_4_35F4:
        mov    edi, dword ptr [ebx + 0x2c]
        mov    al, byte ptr [ebx + 0x33]
        mov    ecx, ebx
        lea    edx, [ebx + 0x24]
        inc    al
        mov    ebx, dword ptr [edx + 4]
        mov    byte ptr [edx + 0xf], al
        cmp    al, 3
        jne    loc_4_3623
        push   0xc001000c
        mov    eax, dword ptr [edi + 0x10]
        push   ecx
        push   eax
        call   sub_1_044A
loc_4_361A:
        dec    dword ptr [esi + 0x11e]
        dec    dword ptr [edi + 0x14]
loc_4_3623:
        test   ebx, ebx
        jne    loc_4_35F4
loc_4_3627:
        xor    eax, eax
        mov    dword ptr [esi + 0x82], eax
        mov    dword ptr [esi + 0x7a], eax
        mov    dword ptr [esi + 0x7e], eax
        mov    dword ptr [esi + 0x86], eax
        mov    dword ptr [esi + 0x8a], eax
        mov    dword ptr [esi + 0xae], eax
        mov    dword ptr [esi + 0xb2], eax
        mov    dword ptr [esi + 0xa6], eax
        mov    dword ptr [esi + 0xaa], eax
        mov    dword ptr [esi + 0x9e], eax
        push   2
        mov    dword ptr [esi + 0xa2], eax
        lea    ecx, [esi + 0x1789]
        mov    dword ptr [esi + 0xb6], eax
        mov    dword ptr [esi + 0xba], eax
        mov    dword ptr [esi + 0x11e], eax
        push   ecx
        add    esi, 0x1789
        call   sub_4_26B0
        push   esi
        call   sub_4_20A0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_36A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 0x4a]
        cmp    byte ptr [edi + 0x92], 0
        jne    loc_4_372B
        cmp    byte ptr [edi + 0x93], 0
        jne    loc_4_372B
        cmp    byte ptr [ebx + 0x40], 0
        jne    loc_4_3724
        mov    byte ptr [edi + 0x93], 1
        cmp    byte ptr [edi + 0x94], 0
        jne    loc_4_3714
        mov    esi, dword ptr [edi + 0x4e]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, -0x4e
        je     loc_4_3706
loc_4_36E3:
        push   0
        inc    dword ptr [esi + 0x14]
        push   0
        mov    eax, dword ptr [esi + 0x10]
        push   0x40010004
        push   eax
        call   sub_1_046E
loc_4_36F8:
        mov    eax, edi
        dec    dword ptr [esi + 0x14]
        mov    esi, dword ptr [esi]
        sub    eax, esi
        cmp    eax, -0x4e
        jne    loc_4_36E3
loc_4_3706:
        mov    byte ptr [edi + 0x93], 0
        push   ebx
        push   edi
        call   sub_4_3430
loc_4_3714:
        mov    esi, 0x103
        inc    dword ptr [ebx + 0x14]
        mov    dword ptr [edi + 0x8e], ebx
        jmp    loc_4_3730
loc_4_3724:
        mov    esi, 0xc0010002
        jmp    loc_4_3730
loc_4_372B:
        mov    esi, 0xc001000d
loc_4_3730:
        cmp    byte ptr [edi + 0x15], 0
        jne    loc_4_3767
        cmp    dword ptr [edi + 0x4a], 2
        jne    loc_4_3745
        cmp    byte ptr [edi + 0x92], 0
        jne    loc_4_374F
loc_4_3745:
        mov    eax, dword ptr [edi + 0x5a]
        sub    eax, edi
        cmp    eax, 0x5a
        je     loc_4_3767
loc_4_374F:
        push   0
        add    edi, 0x18
        push   edi
        mov    byte ptr [edi - 3], 1
        call   sub_1_04AA
loc_4_375E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_3767:
        mov    eax, esi
        dec    dword ptr [edi + 0x4a]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3780:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_2080
        mov    word ptr [esi + 0x48], 0
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_4_37A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [esi + 5], 1
        lea    eax, [esi + 0x1789]
        push   eax
        call   sub_4_2080
        lea    eax, [esi + 0x30]
        lea    edi, [esi + 0x24]
        push   eax
        call   sub_1_04BC
loc_4_37CD:
        mov    eax, dword ptr [esi + 0x6a]
        push   eax
        call   sub_2_0120
loc_4_37D6:
        mov    byte ptr [ebp - 1], bl
loc_4_37D9:
        lea    eax, [ebp - 1]
        push   eax
        push   edi
        call   sub_1_04B0
loc_4_37E3:
        cmp    byte ptr [ebp - 1], bl
        je     loc_4_37D9
        mov    eax, dword ptr [esi + 0x66]
        push   eax
        call   sub_1_0438
loc_4_37F1:
        push   0
        push   0x1807
        push   esi
        call   sub_1_04D4
loc_4_37FE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_1_042C
loc_4_3827:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_0420
loc_4_3831:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_3840:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x152], ecx
        mov    dword ptr [eax + 0x156], ecx
        mov    dword ptr [eax + 0x15a], ecx
        mov    dword ptr [eax + 0x15e], ecx
        mov    dword ptr [eax + 0x162], ecx
        mov    dword ptr [eax + 0x166], ecx
        mov    dword ptr [eax + 0x16a], ecx
        mov    dword ptr [eax + 0x16e], ecx
        mov    dword ptr [eax + 0x172], ecx
        mov    dword ptr [eax + 0x176], ecx
        mov    dword ptr [eax + 0x17a], ecx
        mov    dword ptr [eax + 0x17e], ecx
        mov    byte ptr [eax + 0x18a], cl
        mov    byte ptr [eax + 0x18b], cl
        mov    word ptr [eax + 0x195], cx
        mov    byte ptr [eax + 0x18c], cl
        pop    ebp
        mov    dword ptr [eax + 0x18d], ecx
        mov    dword ptr [eax + 0x191], ecx
        mov    dword ptr [eax + 0x182], ecx
        ret    4
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x158  RW-
        db  04Eh, 049h, 043h, 020h, 045h, 074h, 068h, 065h, 072h, 06Eh, 065h, 074h, 020h, 041h, 064h, 061h
        db  070h, 074h, 065h, 072h, 020h, 028h, 044h, 04Eh, 049h, 045h, 058h, 058h, 029h, 000h, 000h, 000h
        db  001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h
        db  005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h
        db  012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h
        db  00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h
        db  010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 001h, 001h, 002h, 000h
        db  002h, 001h, 002h, 000h, 003h, 001h, 002h, 000h, 004h, 001h, 002h, 000h, 005h, 001h, 002h, 000h
        db  001h, 002h, 002h, 000h, 002h, 002h, 002h, 000h, 003h, 002h, 002h, 000h, 004h, 002h, 002h, 000h
        db  005h, 002h, 002h, 000h, 006h, 002h, 002h, 000h, 007h, 002h, 002h, 000h, 008h, 002h, 002h, 000h
        db  009h, 002h, 002h, 000h, 00Ah, 002h, 002h, 000h, 00Bh, 002h, 002h, 000h, 00Ch, 002h, 002h, 000h
        db  00Dh, 002h, 002h, 000h, 00Eh, 002h, 002h, 000h, 001h, 001h, 001h, 001h, 002h, 001h, 001h, 001h
        db  003h, 001h, 001h, 001h, 004h, 001h, 001h, 001h, 001h, 001h, 002h, 001h, 002h, 001h, 002h, 001h
        db  003h, 001h, 002h, 001h, 001h, 002h, 002h, 001h, 002h, 002h, 002h, 001h, 003h, 002h, 002h, 001h
        db  004h, 002h, 002h, 001h, 005h, 002h, 002h, 001h, 006h, 002h, 002h, 001h, 007h, 002h, 002h, 001h
        db  001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h
        db  005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h
        db  012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h
        db  00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h
        db  010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 00Eh, 002h, 002h, 000h
        db  001h, 001h, 001h, 001h, 002h, 001h, 001h, 001h, 003h, 001h, 001h, 001h, 004h, 001h, 001h, 001h
        db  006h, 000h, 006h, 031h
        dd  loc_1_0670               ; fixup
VxD__LDATA ENDS
