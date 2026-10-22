# Instructions first used by dash.xex 2.0.6719 (the Blades devkit dashboard). Expected values follow the Power ISA
# definitions (no hardware capture): vsubsbs subtracts each signed byte with saturation to [-128, 127]; vcmpgtub. and
# vcmpgtuh. compare unsigned bytes / halfwords and set CR6 (read back with mfcr): LT (0x80) when every element is
# true, EQ (0x20) when none is.

test_vsubsbs:
  #_ REGISTER_IN v3 [7F800001, 10F07F80, 00000000, 01020304]
  #_ REGISTER_IN v4 [FF017F02, 20100180, 00000000, 04030201]
  vsubsbs v5, v3, v4
  blr
  #_ REGISTER_OUT v3 [7F800001, 10F07F80, 00000000, 01020304]
  #_ REGISTER_OUT v4 [FF017F02, 20100180, 00000000, 04030201]
  #_ REGISTER_OUT v5 [7F8081FF, F0E07E00, 00000000, FDFF0103]

test_vcmpgtuh_record_all:
  #_ REGISTER_IN v3 [FFFF8000, 00020003, 7FFF0001, 12345678]
  #_ REGISTER_IN v4 [FFFE7FFF, 00010002, 7FFE0000, 12335677]
  vcmpgtuh. v5, v3, v4
  mfcr r7
  blr
  #_ REGISTER_OUT v5 [FFFFFFFF, FFFFFFFF, FFFFFFFF, FFFFFFFF]
  #_ REGISTER_OUT r7 0x0000000000000080

test_vcmpgtuh_record_some:
  #_ REGISTER_IN v3 [00010000, 00000000, 00000000, 00000000]
  #_ REGISTER_IN v4 [00000001, 00000000, 00000000, 00000000]
  vcmpgtuh. v5, v3, v4
  mfcr r7
  blr
  #_ REGISTER_OUT v5 [FFFF0000, 00000000, 00000000, 00000000]
  #_ REGISTER_OUT r7 0x0000000000000000

test_vcmpgtuh_record_none:
  #_ REGISTER_IN v3 [12345678, 00000000, FFFFFFFF, 00010002]
  #_ REGISTER_IN v4 [12345678, 00000000, FFFFFFFF, 00010002]
  vcmpgtuh. v5, v3, v4
  mfcr r7
  blr
  #_ REGISTER_OUT v5 [00000000, 00000000, 00000000, 00000000]
  #_ REGISTER_OUT r7 0x0000000000000020

test_vcmpgtub_record_all:
  #_ REGISTER_IN v3 [FF808101, 02030405, 06070809, 0A0B0C0D]
  #_ REGISTER_IN v4 [7F7F8000, 01020304, 05060708, 090A0B0C]
  vcmpgtub. v5, v3, v4
  mfcr r7
  blr
  #_ REGISTER_OUT v5 [FFFFFFFF, FFFFFFFF, FFFFFFFF, FFFFFFFF]
  #_ REGISTER_OUT r7 0x0000000000000080
