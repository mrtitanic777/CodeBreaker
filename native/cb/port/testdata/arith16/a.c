#include "hdr.h"
#define LIM 70000
#define BIG 0x8000
#define OCT 0177777
#define ADD(a,b) ((a)+(b))
#define NEG (-1)
#define CHK(x) (x)
#define ZERO 0
void f1(void) {
  if (u > -1) {}
  if (u > s) {}
  if (u < (s + t)) {}
  if ((u - 1) > 0) {}
  if (s < t) {}
  l = (int32_t)(u - 1);
  l = u + v;
  l = (long)(s * t);
  u = u / s;
  u = (u + 1) >> 2;
  s = s % t;
  if (u == 65535) {}
  if (u == 0177777) {}
  if (u == 0xFFFF) {}
  if (u < BIG) {}
  if (u > OCT) {}
  l = LIM + 1;
  if (u < ADD(s, t)) {}
  if (u > NEG) {}
  if (u > CHK(s)) {}
  if (u > ZERO) {}
  if (s & u) {}
  l = (long)(s & u);
  l = (long)(s | t);
  if ((u ^ v) > s) {}
  if (-s < u) {}
  if (~u > 0) {}
  if (u < 'a') {}
  if (c < u) {}
  if (b > s) {}
  if (u > arr[2]) {}
  if (u > st.f) {}
  if (u > ps->g) {}
  if (u > fn(3)) {}
  if (u > (s ? s : t)) {}
  if (u > (s, t)) {}
  l = (long)(u * v);
  l = (long)ul;
  ul = (unsigned long)(u - v);
  if (ul > u) {}
  if (u > 1L) {}
  if (s < 5u) {}
  if (u > *(&s)) {}
  s += 1; if (u > (s += 2)) {}
  if (u > (s = t)) {}
  if (!u > s) {}
  if (u > (int16_t)(s + t)) {}
  if (u > s++) {}
  if (u < (s << 1)) {}
  u = u >> s;
  if (u > (t + 1000)) {}
  if (u > (0 + t)) {}
  if (u >= (uint16_t)s) {}
  if (v != -s) {}
  l = (long)(~s);
  l = (long)-s;
  l = (long)(-3);
  if (u < ((s)) + 1) {}
  if ((u) > (s)) {}
}
