// Win16 import ordinal -> API name (the common KERNEL/USER/GDI exports), with
// real SDK casing. A compact seed so by-ordinal imports render as names; the
// complete table comes from reading the exporting system DLLs' own export name
// tables (KRNL386/USER/GDI) with the NE loader -- a later step.
#pragma once
#include <string>

namespace win95 {

inline std::string Win16Name(const std::string& module, int ordinal) {
  std::string m = module;
  for (auto& ch : m) ch = (char)toupper((unsigned char)ch);
  if (m == "KERNEL") switch (ordinal) {
    case 15: return "GlobalAlloc";   case 16: return "GlobalReAlloc";
    case 17: return "GlobalFree";    case 18: return "GlobalLock";
    case 19: return "GlobalUnlock";  case 20: return "GlobalSize";
    case 21: return "GlobalHandle";  case 22: return "GlobalFlags";
    case 23: return "LockSegment";   case 24: return "UnlockSegment";
    case 30: return "WaitEvent";     case 50: return "GetProcAddress";
    case 81: return "_lclose";       case 82: return "_lread";
    case 83: return "_lcreat";       case 84: return "_llseek";
    case 85: return "_lopen";        case 86: return "_lwrite";
    case 87: return "lstrcpy";       case 88: return "lstrcat";
    case 89: return "lstrlen";       case 91: return "InitTask";
    case 96: return "OpenFile";      case 97: return "_lmove";
    default: break;
  }
  if (m == "USER") switch (ordinal) {
    case 1:   return "MessageBox";     case 433: return "IsCharAlpha";
    case 434: return "IsCharAlphaNumeric"; case 435: return "IsCharUpper";
    case 436: return "IsCharLower";    case 5:   return "InitApp";
    case 430: return "lstrcmp";        case 471: return "lstrcmpi";
    default: break;
  }
  return "";  // unknown -> caller falls back to MODULE_ordinal
}

}  // namespace win95
