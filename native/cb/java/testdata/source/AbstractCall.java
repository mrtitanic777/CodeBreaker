package t;

public abstract class AbstractCall {
    protected int id;

    public AbstractCall() {
        entityInit();
        this.setup();
        int n = compute(2);
        other.entityNever();
    }

    protected abstract void entityInit();
    protected abstract int setup();
    protected abstract Foo compute(int a);
    protected abstract void entityNever();
    protected abstract void unused();
}
