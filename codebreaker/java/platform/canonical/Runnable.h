#ifndef RUNNABLE_H
#define RUNNABLE_H
// java.lang.Runnable -- declared `class` (not struct) so it agrees with every forward
// declaration of it (the header emitter forward-declares reference types as `class X;`,
// and MinecraftApplet's local fwd-decl does too) -> no C4099 struct/class mismatch.
class Runnable {
public:
	virtual ~Runnable() {}
	virtual void run() = 0;
};
#endif // RUNNABLE_H
