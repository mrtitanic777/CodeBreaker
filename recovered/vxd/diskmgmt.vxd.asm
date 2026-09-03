; DISKMGMT -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 376 instructions recovered by recursive descent

Declare_Virtual_Device DISKMGMT, 1, 0, DISKMGMT_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x440  R-X
DISKMGMT_Control:
        cmp    eax, 2
        jne    loc_1_000E
        call   sub_2_0000
        cmp    eax, 1
        ret    
loc_1_000E:
        cmp    eax, 0x1b
        jne    loc_1_001C
        call   sub_2_0000
        cmp    eax, 1
        ret    
loc_1_001C:
        cmp    eax, 0x1c
        jne    loc_1_002A
        call   sub_1_0377
        cmp    eax, 1
        ret    
loc_1_002A:
        cmp    eax, 0x23
        jne    loc_1_003C
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_0070
        cmp    eax, 1
        ret    
loc_1_003C:
        clc    
        ret    
loc_1_003E:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 2
        je     loc_1_0057
        mov    word ptr [ebx + 2], 0xffff
loc_1_0057:
        ret    
sub_1_0058:
        VxDCall  BLOCKDEV.Service_8007h          ; cd2007801000
sub_1_005E:
        push   esi
        mov    esi, dword ptr [0x3b8]        ; -> loc_1_03B8
        VxDCall  BLOCKDEV.Service_0011h          ; cd2011001000
        pop    esi
        ret    
        db  000h, 000h, 000h
sub_1_0070:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        jne    loc_1_007C
        sub    eax, eax
        jmp    loc_1_00A5
loc_1_007C:
        cmp    ecx, -1
        jne    loc_1_0088
        call   sub_1_0382
        jmp    loc_1_00A5
loc_1_0088:
        cmp    ecx, 5
        mov    eax, 0x32
        ja     loc_1_00A5
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   dword ptr [ecx*4 + 0x420]     ; -> obj1:0x0420
loc_1_00A5:
        ret    0x10
loc_1_00A8:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        cmp    dword ptr [eax + 0x1c], 4
        jae    loc_1_00BC
        mov    eax, 0xea
        jmp    loc_1_00CD
loc_1_00BC:
        or     ecx, ecx
        mov    eax, 0x57
        je     loc_1_00CD
        mov    dword ptr [ecx], 0x1000
        sub    eax, eax
loc_1_00CD:
        ret    0xc
loc_1_00D0:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [eax + 0x18]
        mov    eax, dword ptr [0x3bc]        ; -> loc_1_03BC
        mov    eax, dword ptr [eax + 0x30]
        or     eax, eax
        mov    dword ptr [edx], 0
        je     loc_1_0100
        mov    ecx, 0x2000
loc_1_00EE:
        test   dword ptr [eax + 0x20], ecx
        jne    loc_1_00F9
        cmp    dword ptr [eax], eax
        jne    loc_1_00F9
        inc    dword ptr [edx]
loc_1_00F9:
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        jne    loc_1_00EE
loc_1_0100:
        sub    eax, eax
        ret    0xc
loc_1_0105:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [eax + 0x18]
        mov    dword ptr [esp + 0xc], 0
        cmp    dword ptr [eax + 0x1c], 0x121
        jae    loc_1_012D
        mov    eax, 0xea
        jmp    loc_1_0256
loc_1_012D:
        or     esi, esi
        jne    loc_1_013B
        mov    eax, 0x57
        jmp    loc_1_0256
loc_1_013B:
        push   0
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_0388
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0157
        jmp    loc_1_0251
loc_1_0157:
        mov    eax, dword ptr [0x3bc]        ; -> loc_1_03BC
        sub    dx, dx
        mov    ebx, dword ptr [eax + 0x30]
        or     ebx, ebx
        je     loc_1_0251
        mov    eax, 0x2000
loc_1_016F:
        test   dword ptr [ebx + 0x20], eax
        jne    loc_1_0183
        cmp    dword ptr [ebx], ebx
        jne    loc_1_0183
        inc    dx
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [ebx + 0x70], ecx
        je     loc_1_018A
loc_1_0183:
        mov    ebx, dword ptr [ebx + 0xc]
        or     ebx, ebx
        jne    loc_1_016F
loc_1_018A:
        or     ebx, ebx
        je     loc_1_0251
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x48
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    dword ptr [esi], 0x4d4f4547
        dec    dl
        mov    byte ptr [esi + 0x104], dl
        mov    al, byte ptr [ebx + 0x74]
        inc    al
        mov    byte ptr [esi + 0x120], al
        mov    al, byte ptr [ebx + 0x40]
        mov    byte ptr [esi + 0x11f], al
        mov    ecx, dword ptr [ebx + 0x20]
        test   ecx, 0x80000
        jne    loc_1_0211
        mov    eax, dword ptr [ebx + 0x54]
        mov    dword ptr [esi + 0x107], eax
        mov    eax, dword ptr [ebx + 0x58]
        mov    dword ptr [esi + 0x10b], eax
        mov    eax, dword ptr [ebx + 0x60]
        mov    dword ptr [esi + 0x113], eax
        mov    eax, dword ptr [ebx + 0x64]
        mov    dword ptr [esi + 0x117], eax
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [esi + 0x11b], eax
        mov    eax, dword ptr [ebx + 0x5c]
        or     eax, eax
        mov    dword ptr [esi + 0x10f], eax
        jne    loc_1_0211
        mov    eax, dword ptr [ebx + 0xdb]
        mov    dword ptr [esi + 0x10f], eax
loc_1_0211:
        mov    edx, dword ptr [ebx + 0x10]
        sub    ax, ax
        or     edx, edx
        je     loc_1_0224
loc_1_021B:
        mov    edx, dword ptr [edx + 0x10]
        inc    ax
        or     edx, edx
        jne    loc_1_021B
loc_1_0224:
        or     ax, ax
        mov    byte ptr [esi + 0x106], al
        jne    loc_1_0236
        mov    byte ptr [esi + 0x106], 1
loc_1_0236:
        test   cl, 4
        je     loc_1_0246
        mov    al, byte ptr [ebx + 0x14]
        mov    byte ptr [esi + 0x105], al
        jmp    loc_1_024D
loc_1_0246:
        mov    byte ptr [esi + 0x105], 0xff
loc_1_024D:
        sub    eax, eax
        jmp    loc_1_0256
loc_1_0251:
        mov    eax, 0x3f4
loc_1_0256:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
loc_1_025F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [eax + 0x10]
        mov    edx, dword ptr [eax + 0x18]
        cmp    dword ptr [eax + 0x1c], 1
        jae    loc_1_027C
        mov    eax, 0xea
        jmp    loc_1_02E2
loc_1_027C:
        or     edx, edx
        jne    loc_1_0287
        mov    eax, 0x57
        jmp    loc_1_02E2
loc_1_0287:
        mov    eax, dword ptr [0x3bc]        ; -> loc_1_03BC
        mov    esi, dword ptr [eax + 0x30]
        mov    eax, esi
        or     esi, esi
        je     loc_1_02A9
        mov    cl, byte ptr [ecx]
loc_1_0297:
        cmp    byte ptr [eax + 0x14], cl
        je     loc_1_02A5
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        jne    loc_1_0297
        jmp    loc_1_02A9
loc_1_02A5:
        mov    edi, dword ptr [eax]
        jmp    loc_1_02AD
loc_1_02A9:
        mov    edi, dword ptr [esp + 0xc]
loc_1_02AD:
        or     eax, eax
        je     loc_1_02DD
        sub    bx, bx
        or     esi, esi
        je     loc_1_02D9
        mov    ecx, 0x2000
loc_1_02BD:
        mov    eax, dword ptr [esi]
        cmp    eax, edi
        je     loc_1_02D7
        test   dword ptr [esi + 0x20], ecx
        jne    loc_1_02CE
        cmp    esi, eax
        jne    loc_1_02CE
        inc    bx
loc_1_02CE:
        mov    esi, dword ptr [esi + 0xc]
        or     esi, esi
        jne    loc_1_02BD
        jmp    loc_1_02D9
loc_1_02D7:
        mov    byte ptr [edx], bl
loc_1_02D9:
        sub    eax, eax
        jmp    loc_1_02E2
loc_1_02DD:
        mov    eax, 0x3f4
loc_1_02E2:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
loc_1_02EB:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [0x3bc]        ; -> loc_1_03BC
        mov    edx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [eax + 0x30]
        sub    ax, ax
        or     ebx, ebx
        je     loc_1_0368
loc_1_030B:
        test   byte ptr [ebx + 0x21], 0x20
        jne    loc_1_0321
        cmp    dword ptr [ebx], ebx
        jne    loc_1_0321
        movzx  ecx, byte ptr [edx]
        movzx  ebp, ax
        cmp    ecx, ebp
        je     loc_1_0328
        inc    ax
loc_1_0321:
        mov    ebx, dword ptr [ebx + 0xc]
        or     ebx, ebx
        jne    loc_1_030B
loc_1_0328:
        or     ebx, ebx
        je     loc_1_0368
        push   0
        lea    eax, [esp + 0x14]
        push   dword ptr [ebx + 0x70]
        push   eax
        call   sub_1_0390
        inc    dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        cmp    dword ptr [esi + 0x1c], eax
        jae    loc_1_0352
        mov    eax, 0xea
        jmp    loc_1_036D
loc_1_0352:
        push   0
        push   dword ptr [esp + 0x14]
        push   edi
        push   dword ptr [ebx + 0x70]
        call   sub_1_0398
        add    esp, 0x10
        sub    eax, eax
        jmp    loc_1_036D
loc_1_0368:
        mov    eax, 0x3f4
loc_1_036D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
sub_1_0377:
        call   sub_1_005E
        mov    eax, 1
        ret    
sub_1_0382:
        mov    eax, 1
        ret    
sub_1_0388:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        add    byte ptr [eax], al
sub_1_0390:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID_Size ; cd2006803300
        add    byte ptr [eax], al
sub_1_0398:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        add    byte ptr [eax], al
loc_1_03A0:
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
loc_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
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
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DISKMGMT'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DISKMGMT_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_1_00A8               ; fixup
        dd  loc_1_00D0               ; fixup
        dd  loc_1_0105               ; fixup
        dd  loc_1_025F               ; fixup
        dd  loc_1_02EB               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x4c  R-X
sub_2_0000:
        push   0x14                          ; -> loc_2_0014
        call   sub_1_0058
        add    esp, 4
        mov    eax, 1
        ret    
        db  000h
loc_2_0014:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr ds:[eax], al
        add    byte ptr [eax + 0x44000003], ah ; -> loc_1_03A0
        dec    ecx
        push   ebx
        dec    ebx
        dec    ebp
        inc    edi
        dec    ebp
        push   esp
        and    byte ptr [esi + 0x53], dl
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
        dec    ecx
        push   ebx
        dec    ebx
        dec    ebp
        inc    edi
        dec    ebp
        push   esp
        and    byte ptr [esi + 0x69], dl
        jb     0xba
        jne    0xa9
        insb   byte ptr es:[edi], dx
        db  020h, 044h, 065h
VxD__LTEXT ENDS
