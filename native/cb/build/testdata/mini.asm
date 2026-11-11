.386p
include vmm.inc

Declare_Virtual_Device MINI, 1, 0, Mini_Control, 0x1234, Undefined_Init_Order

VxD_LOCKED_CODE_SEG

BeginProc Mini_Control
        Control_Dispatch Device_Init, Mini_Init
        clc
        ret
EndProc Mini_Control

BeginProc Mini_Init
        clc
        ret
EndProc Mini_Init

VxD_LOCKED_CODE_ENDS

end
