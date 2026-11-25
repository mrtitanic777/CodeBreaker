// jrt.rs -- the runtime of the Rust translation of a Java program (backends/rust_java).
//
// What the translation leans on, and how Java's behaviour is kept:
//
//   * a reference is `Option<&'static T>`: one machine word (two for a trait object), `Copy`, null is `None`. Objects are made in the
//     runtime's heap and freed by its collector when nothing reaches them, so a value can be passed, stored and returned
//     without ownership bookkeeping, and `this` is just `self`. A class is a trait object (`Ref<dyn Animal>`); a call on a
//     reference goes through `.nn()`, which throws NullPointerException for null.
//   * a String is `Str`: Java's UTF-16 text, immutable, shared, nullable. Its methods carry the Java names.
//   * integer arithmetic wraps (`wrapping_*`); `/` and `%` by zero throw ArithmeticException; shifts mask their count as Java
//     does; Rust's `as` between the numeric types is Java's cast (saturating from floating point, NaN to 0).
//   * an exception is a Rust unwind carrying a `Thrown`; `try_catch` and `finally` catch it, so try/catch/finally in the
//     translation read as they do in Java. Every Java exception class from the library is a `dyn Throwable`; the run-time
//     errors (null, bounds, division, casts, number formats) are made here.
//   * `Cat` is Java's string conversion (`"" + x`), so concatenation, `append` and `println` take any value.
//
// Memory is reclaimed by a conservative collector over the runtime's own heap (see "memory" below); that and the buffers it serves are the only
// unsafe code. No crate is used.
#![allow(dead_code, non_snake_case, non_camel_case_types, non_upper_case_globals, unused_macros, unused_imports, unused_variables, unreachable_code)]

use std::any::Any;
use std::cell::{Cell, RefCell};
use std::io::Write;

// ---- the macros (before everything that uses them) ----


// ---- memory: a conservative mark-and-sweep collector ---------------------------------------------------------------------------------------
//
// Objects, arrays, strings and the buffers of the collections are made here, in blocks of this module's own heap, so that a program that makes
// garbage as it runs does not grow without bound, while the generated code stays free of ownership: a reference is still a plain `&'static`.
//
//   * small blocks (up to 2 KB) come from 1 MB chunks, each cut into blocks of one size class; a block is found from any address inside it (a
//     reference to a field of an object points into the middle of it); bigger blocks are made one by one and kept in an ordered map.
//   * a collection starts at an allocation of generated code (`gc_new`, `new_array`, a concatenation, a string method ... see `gc_poll`) once as
//     many bytes have been made since the last one as were live after it (64 MB at least). The runtime's own allocations never start one, so a
//     function of this file that holds a fresh object only in a Rust `Vec` is never interrupted.
//   * the roots are the stack of the program's thread, the registers (callee-saved ones are spilled to the stack first) and the ranges the
//     runtime registers (the static fields, the exception being thrown). Every word of a root and of a reached block that holds the address of
//     a block keeps that block: nothing needs to tell the collector where a reference is.
//   * a block holds no destructors: what is made here is plain data (`Cell`s, words, the buffers of `GcVec`), and a swept block is just reused.
//     A chunk with nothing live in it goes back to the system.
//
// The scan reads the stack and the blocks as words, which is why this one part of the runtime is `unsafe`. It is written for x86-64; elsewhere
// `gc_poll` does nothing and the program keeps what it makes, as before.
use std::alloc::{GlobalAlloc, Layout, System as SysAlloc};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
use std::sync::Mutex;

const CHUNK: usize = 1 << 20;
const CLASSES: [usize; 14] = [16, 32, 48, 64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048];

struct Chunk {
    bs: usize,
    n: usize,
    alloc: Vec<u64>,
    mark: Vec<u64>,
    cursor: usize,
    free: usize,
}
struct Heap {
    chunks: std::collections::HashMap<usize, Chunk>,
    by_class: Vec<Vec<usize>>,
    hint: Vec<usize>,
    large: std::collections::BTreeMap<usize, (usize, bool)>,
    roots: Vec<(usize, usize)>,
    stack_base: usize,
    lo: usize,
    hi: usize,
}
static HEAP: Mutex<Option<Heap>> = Mutex::new(None);
static SINCE: AtomicUsize = AtomicUsize::new(0);
static THRESHOLD: AtomicUsize = AtomicUsize::new(64 << 20);
static ENABLED: AtomicBool = AtomicBool::new(false);
pub static COLLECTIONS: AtomicUsize = AtomicUsize::new(0);

fn heap_of(g: &mut Option<Heap>) -> &mut Heap {
    g.get_or_insert_with(|| Heap {
        chunks: std::collections::HashMap::new(),
        by_class: vec![Vec::new(); CLASSES.len()],
        hint: vec![0; CLASSES.len()],
        large: std::collections::BTreeMap::new(),
        roots: Vec::new(),
        stack_base: 0,
        lo: usize::MAX,
        hi: 0,
    })
}
fn bit(v: &[u64], i: usize) -> bool {
    v[i / 64] >> (i % 64) & 1 != 0
}
fn set_bit(v: &mut [u64], i: usize) {
    v[i / 64] |= 1 << (i % 64);
}

// A zeroed block of at least `size` bytes, 16-aligned, that lives until a collection finds nothing pointing into it.
pub fn gc_alloc_internal(size: usize) -> *mut u8 {
    let mut guard = HEAP.lock().unwrap_or_else(|e| e.into_inner());
    let heap = heap_of(&mut guard);
    let size = size.max(1);
    SINCE.fetch_add(size, Ordering::Relaxed);
    if size > CLASSES[CLASSES.len() - 1] {
        let size = (size + 15) & !15;
        let p = unsafe { SysAlloc.alloc_zeroed(Layout::from_size_align(size, 16).unwrap()) };
        if p.is_null() {
            throw_out_of_memory();
        }
        heap.large.insert(p as usize, (size, false));
        heap.lo = heap.lo.min(p as usize);
        heap.hi = heap.hi.max(p as usize + size);
        return p;
    }
    let class = CLASSES.iter().position(|c| *c >= size).unwrap();
    loop {
        let at = heap.hint[class];
        if at >= heap.by_class[class].len() {
            let p = unsafe { SysAlloc.alloc_zeroed(Layout::from_size_align(CHUNK, CHUNK).unwrap()) };
            if p.is_null() {
                throw_out_of_memory();
            }
            let bs = CLASSES[class];
            let n = CHUNK / bs;
            heap.chunks.insert(p as usize, Chunk { bs, n, alloc: vec![0; n / 64 + 1], mark: vec![0; n / 64 + 1], cursor: 0, free: n });
            heap.by_class[class].push(p as usize);
            heap.lo = heap.lo.min(p as usize);
            heap.hi = heap.hi.max(p as usize + CHUNK);
            continue;
        }
        let base = heap.by_class[class][at];
        let ch = heap.chunks.get_mut(&base).unwrap();
        if ch.free == 0 {
            heap.hint[class] += 1;
            continue;
        }
        let mut w = ch.cursor / 64;
        while w < ch.alloc.len() {
            if ch.alloc[w] != !0 {
                let idx = w * 64 + (!ch.alloc[w]).trailing_zeros() as usize;
                if idx < ch.n {
                    set_bit(&mut ch.alloc, idx);
                    ch.cursor = idx;
                    ch.free -= 1;
                    let p = (base + idx * ch.bs) as *mut u8;
                    unsafe { std::ptr::write_bytes(p, 0, ch.bs) };
                    return p;
                }
            }
            w += 1;
        }
        ch.free = 0;
    }
}

fn throw_out_of_memory() -> ! {
    eprintln!("Exception in thread \"main\" java.lang.OutOfMemoryError: Java heap space");
    std::process::exit(1)
}

pub fn gc_new_internal<T>(v: T) -> &'static T {
    assert!(std::mem::align_of::<T>() <= 16);
    let p = gc_alloc_internal(std::mem::size_of::<T>()) as *mut T;
    unsafe {
        p.write(v);
        &*p
    }
}
// The same, where a collection may come first (nothing of the caller's is held in a Rust container then).
pub fn gc_new<T>(v: T) -> &'static T {
    gc_poll();
    gc_new_internal(v)
}
pub fn gc_slice<T: Copy>(items: &[T]) -> &'static [T] {
    if items.is_empty() {
        return &[];
    }
    let p = gc_alloc_internal(std::mem::size_of_val(items)) as *mut T;
    unsafe {
        std::ptr::copy_nonoverlapping(items.as_ptr(), p, items.len());
        std::slice::from_raw_parts(p, items.len())
    }
}
fn gc_cells<T: Copy>(n: usize, mut f: impl FnMut(usize) -> T) -> &'static [Cell<T>] {
    if n == 0 {
        return &[];
    }
    let p = gc_alloc_internal(n * std::mem::size_of::<Cell<T>>()) as *mut Cell<T>;
    unsafe {
        for i in 0..n {
            p.add(i).write(Cell::new(f(i)));
        }
        std::slice::from_raw_parts(p, n)
    }
}

// A range of memory outside this heap (a static field's cell) that holds references.
pub fn register_root(addr: usize, len: usize) {
    let mut guard = HEAP.lock().unwrap_or_else(|e| e.into_inner());
    heap_of(&mut guard).roots.push((addr, len));
}

// A collection, if enough has been made since the last one.
#[inline]
pub fn gc_poll() {
    if SINCE.load(Ordering::Relaxed) >= THRESHOLD.load(Ordering::Relaxed) && ENABLED.load(Ordering::Relaxed) {
        collect();
    }
}

#[cfg(target_arch = "x86_64")]
#[inline(never)]
fn spill_registers(buf: &mut [usize; 32]) {
    // the callee-saved registers of Windows and of System V: a reference one of them holds is found on the stack
    unsafe {
        std::arch::asm!(
            "mov [{0}], rbx", "mov [{0} + 8], rbp", "mov [{0} + 16], rdi", "mov [{0} + 24], rsi",
            "mov [{0} + 32], r12", "mov [{0} + 40], r13", "mov [{0} + 48], r14", "mov [{0} + 56], r15",
            "movups [{0} + 64], xmm6", "movups [{0} + 80], xmm7", "movups [{0} + 96], xmm8", "movups [{0} + 112], xmm9",
            "movups [{0} + 128], xmm10", "movups [{0} + 144], xmm11", "movups [{0} + 160], xmm12", "movups [{0} + 176], xmm13",
            "movups [{0} + 192], xmm14", "movups [{0} + 208], xmm15",
            in(reg) buf.as_mut_ptr(), options(nostack)
        );
    }
}

fn mark_word(heap: &mut Heap, addr: usize, work: &mut Vec<(usize, usize)>) {
    if addr < heap.lo || addr >= heap.hi {
        return;
    }
    if let Some(ch) = heap.chunks.get_mut(&(addr & !(CHUNK - 1))) {
        let idx = (addr & (CHUNK - 1)) / ch.bs;
        if idx < ch.n && bit(&ch.alloc, idx) && !bit(&ch.mark, idx) {
            set_bit(&mut ch.mark, idx);
            work.push(((addr & !(CHUNK - 1)) + idx * ch.bs, ch.bs));
        }
        return;
    }
    if let Some((start, entry)) = heap.large.range_mut(..=addr).next_back() {
        if addr < *start + entry.0 && !entry.1 {
            entry.1 = true;
            work.push((*start, entry.0));
        }
    }
}
fn scan_range(heap: &mut Heap, from: usize, to: usize, work: &mut Vec<(usize, usize)>) {
    let mut p = (from + 7) & !7;
    while p + 8 <= to {
        let word = unsafe { std::ptr::read_volatile(p as *const usize) };
        mark_word(heap, word, work);
        p += 8;
    }
}

// Mark everything reachable from the roots, free the rest.
#[inline(never)]
pub fn collect() {
    #[cfg(target_arch = "x86_64")]
    {
        let mut regs = [0usize; 32];
        spill_registers(&mut regs);
        let low = std::hint::black_box(&regs) as *const _ as usize;
        let mut guard = HEAP.lock().unwrap_or_else(|e| e.into_inner());
        let heap = heap_of(&mut guard);
        if heap.stack_base == 0 || low >= heap.stack_base {
            return;
        }
        COLLECTIONS.fetch_add(1, Ordering::Relaxed);
        let mut work: Vec<(usize, usize)> = Vec::new();
        let base = heap.stack_base;
        scan_range(heap, low, base, &mut work);
        let roots = heap.roots.clone();
        for (addr, len) in roots {
            scan_range(heap, addr, addr + len, &mut work);
        }
        while let Some((start, size)) = work.pop() {
            scan_range(heap, start, start + size, &mut work);
        }
        // sweep: what was not marked is free
        let mut live = 0usize;
        let mut empty: Vec<usize> = Vec::new();
        for (base, ch) in heap.chunks.iter_mut() {
            let count: usize = ch.mark.iter().map(|w| w.count_ones() as usize).sum();
            ch.alloc.copy_from_slice(&ch.mark);
            for w in ch.mark.iter_mut() {
                *w = 0;
            }
            ch.free = ch.n - count;
            ch.cursor = 0;
            live += count * ch.bs;
            if count == 0 {
                empty.push(*base);
            }
        }
        for base in empty {
            heap.chunks.remove(&base);
            unsafe { SysAlloc.dealloc(base as *mut u8, Layout::from_size_align(CHUNK, CHUNK).unwrap()) };
        }
        let chunks = &heap.chunks;
        for list in heap.by_class.iter_mut() {
            list.retain(|b| chunks.contains_key(b));
        }
        for h in heap.hint.iter_mut() {
            *h = 0;
        }
        let dead: Vec<usize> = heap.large.iter().filter(|(_, v)| !v.1).map(|(k, _)| *k).collect();
        for k in dead {
            let (size, _) = heap.large.remove(&k).unwrap();
            unsafe { SysAlloc.dealloc(k as *mut u8, Layout::from_size_align(size, 16).unwrap()) };
        }
        for v in heap.large.values_mut() {
            v.1 = false;
            live += v.0;
        }
        SINCE.store(0, Ordering::Relaxed);
        THRESHOLD.store((live).max(64 << 20), Ordering::Relaxed);
    }
}

// The program's thread calls this first: the collector may run from then on, and knows where its stack ends.
pub fn gc_enable(stack_top: usize) {
    let mut guard = HEAP.lock().unwrap_or_else(|e| e.into_inner());
    heap_of(&mut guard).stack_base = stack_top;
    ENABLED.store(cfg!(target_arch = "x86_64"), Ordering::Relaxed);
}

// A growable array of plain values in this heap (the buffer of a list, a builder, a map).
pub struct GcVec<T: Copy + 'static> {
    ptr: Cell<*mut T>,
    len: Cell<usize>,
    cap: Cell<usize>,
}
impl<T: Copy + 'static> GcVec<T> {
    pub fn new() -> Self {
        GcVec { ptr: Cell::new(std::ptr::null_mut()), len: Cell::new(0), cap: Cell::new(0) }
    }
    pub fn from_slice(items: &[T]) -> Self {
        let v = GcVec::new();
        v.extend_from_slice(items);
        v
    }
    pub fn len(&self) -> usize {
        self.len.get()
    }
    fn reserve(&self, extra: usize) {
        let need = self.len.get() + extra;
        if need <= self.cap.get() {
            return;
        }
        let cap = need.max(self.cap.get() * 2).max(4);
        let p = gc_alloc_internal(cap * std::mem::size_of::<T>()) as *mut T;
        unsafe {
            if self.len.get() > 0 {
                std::ptr::copy_nonoverlapping(self.ptr.get(), p, self.len.get());
            }
        }
        self.ptr.set(p);
        self.cap.set(cap);
    }
    pub fn push(&self, v: T) {
        self.reserve(1);
        unsafe { self.ptr.get().add(self.len.get()).write(v) };
        self.len.set(self.len.get() + 1);
    }
    pub fn get(&self, i: usize) -> T {
        assert!(i < self.len.get());
        unsafe { *self.ptr.get().add(i) }
    }
    pub fn set(&self, i: usize, v: T) {
        assert!(i < self.len.get());
        unsafe { self.ptr.get().add(i).write(v) };
    }
    pub fn insert(&self, i: usize, v: T) {
        assert!(i <= self.len.get());
        self.reserve(1);
        unsafe {
            let p = self.ptr.get();
            std::ptr::copy(p.add(i), p.add(i + 1), self.len.get() - i);
            p.add(i).write(v);
        }
        self.len.set(self.len.get() + 1);
    }
    pub fn remove(&self, i: usize) -> T {
        assert!(i < self.len.get());
        unsafe {
            let p = self.ptr.get();
            let v = *p.add(i);
            std::ptr::copy(p.add(i + 1), p.add(i), self.len.get() - i - 1);
            self.len.set(self.len.get() - 1);
            v
        }
    }
    pub fn clear(&self) {
        self.len.set(0);
    }
    pub fn extend_from_slice(&self, items: &[T]) {
        self.reserve(items.len());
        unsafe { std::ptr::copy_nonoverlapping(items.as_ptr(), self.ptr.get().add(self.len.get()), items.len()) };
        self.len.set(self.len.get() + items.len());
    }
    pub fn resize(&self, n: usize, fill: T) {
        if n > self.len.get() {
            self.reserve(n - self.len.get());
            for i in self.len.get()..n {
                unsafe { self.ptr.get().add(i).write(fill) };
            }
        }
        self.len.set(n);
    }
    pub fn to_vec(&self) -> Vec<T> {
        if self.len.get() == 0 {
            return Vec::new();
        }
        unsafe { std::slice::from_raw_parts(self.ptr.get(), self.len.get()) }.to_vec()
    }
    pub fn assign(&self, items: &[T]) {
        self.len.set(0);
        self.extend_from_slice(items);
    }
}

// A reference as another type it converts to (a class as its base or interface).
macro_rules! up {
    ($x:expr, $t:ty) => {
        $x.map(|r| r as &'static $t)
    };
}

// A static field: made on first use, read with `get`, written with `set`.
macro_rules! jstatic {
    ($name:ident : $t:ty = $init:expr) => {
        pub struct $name;
        impl $name {
            fn cell() -> &'static std::cell::Cell<$t> {
                thread_local! { static CELL: &'static std::cell::Cell<$t> = static_cell(std::cell::Cell::new($init)); }
                CELL.with(|c| *c)
            }
            pub fn get() -> $t {
                Self::cell().get()
            }
            pub fn set(v: $t) {
                Self::cell().set(v)
            }
        }
    };
}

// "" + a + b ...: each part in Java's string conversion, evaluated left to right
macro_rules! cat {
    ($($x:expr),+ $(,)?) => {{
        let mut v: Vec<u16> = Vec::new();
        $( Cat::cat(&$x, &mut v); )+
        gc_poll();
        Str::from_units(v)
    }};
}

// ---- references ----------------------------------------------------------------------------------------------------------

pub type Ref<T> = Option<&'static T>;

pub trait Nn<T: ?Sized> {
    fn nn(self) -> &'static T;
}
impl<T: ?Sized> Nn<T> for Option<&'static T> {
    fn nn(self) -> &'static T {
        match self {
            Some(r) => r,
            None => throw_new("java.lang.NullPointerException", None),
        }
    }
}


// Whether two references are the same object (Java's == on references).
pub fn same<T: ?Sized, U: ?Sized>(a: Option<&'static T>, b: Option<&'static U>) -> bool {
    match (a, b) {
        (None, None) => true,
        (Some(x), Some(y)) => std::ptr::addr_eq(x as *const T, y as *const U),
        _ => false,
    }
}


// The cell of a static field: outside the collected heap, and a root of it.
pub fn static_cell<T>(cell: Cell<T>) -> &'static Cell<T> {
    let c: &'static Cell<T> = Box::leak(Box::new(cell));
    register_root(c as *const Cell<T> as usize, std::mem::size_of::<Cell<T>>());
    c
}

// ---- Object ---------------------------------------------------------------------------------------------------------------

pub trait Object: Any {
    fn toString(&'static self) -> Str {
        cat!(lit(self.class_name()), lit("@"), Integer_toHexString(self.hashCode()))
    }
    fn equals(&'static self, other: Ref<dyn Object>) -> bool {
        match other {
            None => false,
            Some(o) => std::ptr::eq(self as *const Self as *const (), o as *const dyn Object as *const ()),
        }
    }
    fn hashCode(&'static self) -> i32 {
        ((self as *const Self as *const () as usize) >> 4) as i32
    }
    // the class's name as getClass().getName() gives it, and the names of everything it is (itself, its bases and interfaces)
    fn class_name(&'static self) -> &'static str;
    fn class_chain(&'static self) -> &'static [&'static str];
    fn as_any(&'static self) -> &'static dyn Any;
    fn as_object(&'static self) -> &'static dyn Object;
}

impl dyn Object {
    pub fn instance_of(&'static self, name: &str) -> bool {
        self.class_chain().iter().any(|n| *n == name)
    }
}

pub fn instance_of(o: Ref<dyn Object>, name: &str) -> bool {
    match o {
        None => false,
        Some(r) => r.instance_of(name),
    }
}

// ---- strings --------------------------------------------------------------------------------------------------------------

#[derive(Clone, Copy, Default)]
pub struct Str(Option<&'static [u16]>);

thread_local! {
    static INTERNED: RefCell<std::collections::HashMap<&'static str, Str>> = RefCell::new(std::collections::HashMap::new());
}

// A string literal: one object per text, as Java's constant pool makes it (so `==` on two literals is true).
pub fn lit(text: &'static str) -> Str {
    INTERNED.with(|m| *m.borrow_mut().entry(text).or_insert_with(|| Str::permanent(text)))
}

fn is_ws(c: u16) -> bool {
    c == 32 || (9..=13).contains(&c) || (28..=31).contains(&c)
}

impl Str {
    pub const NULL: Str = Str(None);
    pub fn from_units(v: Vec<u16>) -> Str {
        Str(Some(gc_slice(&v)))
    }
    // text that is never freed (a literal)
    pub fn permanent(s: &str) -> Str {
        let v: Vec<u16> = s.encode_utf16().collect();
        Str(Some(Box::leak(v.into_boxed_slice())))
    }
    pub fn from_rs(s: &str) -> Str {
        Str::from_units(s.encode_utf16().collect())
    }
    pub fn to_rs(&self) -> String {
        String::from_utf16_lossy(self.u())
    }
    pub fn is_null(&self) -> bool {
        self.0.is_none()
    }
    fn u(&self) -> &'static [u16] {
        match self.0 {
            Some(s) => s,
            None => throw_new("java.lang.NullPointerException", None),
        }
    }
    fn sub(&self, a: usize, b: usize) -> Str {
        Str::from_units(self.u()[a..b].to_vec())
    }
    pub fn length(&self) -> i32 {
        self.u().len() as i32
    }
    pub fn isEmpty(&self) -> bool {
        self.u().is_empty()
    }
    pub fn isBlank(&self) -> bool {
        self.u().iter().all(|c| is_ws(*c))
    }
    pub fn charAt(&self, i: i32) -> u16 {
        let u = self.u();
        if i < 0 || i as usize >= u.len() {
            throw_new("java.lang.StringIndexOutOfBoundsException", Some(format!("Index {} out of bounds for length {}", i, u.len())));
        }
        u[i as usize]
    }
    pub fn substring(&self, a: i32) -> Str {
        self.substring2(a, self.length())
    }
    pub fn substring2(&self, a: i32, b: i32) -> Str {
        let n = self.length();
        if a < 0 || b > n || a > b {
            throw_new("java.lang.StringIndexOutOfBoundsException", Some(format!("begin {}, end {}, length {}", a, b, n)));
        }
        self.sub(a as usize, b as usize)
    }
    pub fn concat(&self, o: Str) -> Str {
        let mut v = self.u().to_vec();
        v.extend_from_slice(o.u());
        Str::from_units(v)
    }
    // equals(Object) where the argument is a String (anything else is never equal)
    pub fn equals(&self, o: Str) -> bool {
        self.u() == o.0.unwrap_or(&[]) && o.0.is_some()
    }
    pub fn equalsIgnoreCase(&self, o: Str) -> bool {
        o.0.is_some() && self.u().len() == o.u().len() && self.u().iter().zip(o.u()).all(|(a, b)| lower_u16(*a) == lower_u16(*b))
    }
    pub fn compareTo(&self, o: Str) -> i32 {
        let (a, b) = (self.u(), o.u());
        for i in 0..a.len().min(b.len()) {
            if a[i] != b[i] {
                return a[i] as i32 - b[i] as i32;
            }
        }
        a.len() as i32 - b.len() as i32
    }
    pub fn compareToIgnoreCase(&self, o: Str) -> i32 {
        let (a, b) = (self.u(), o.u());
        for i in 0..a.len().min(b.len()) {
            let (x, y) = (lower_u16(upper_u16(a[i])), lower_u16(upper_u16(b[i])));
            if x != y {
                return x as i32 - y as i32;
            }
        }
        a.len() as i32 - b.len() as i32
    }
    pub fn hashCode(&self) -> i32 {
        self.u().iter().fold(0i32, |h, c| h.wrapping_mul(31).wrapping_add(*c as i32))
    }
    pub fn startsWith(&self, p: Str) -> bool {
        self.u().starts_with(p.u())
    }
    pub fn startsWith_from(&self, p: Str, from: i32) -> bool {
        from >= 0 && (from as usize) <= self.u().len() && self.u()[from as usize..].starts_with(p.u())
    }
    pub fn endsWith(&self, p: Str) -> bool {
        self.u().ends_with(p.u())
    }
    fn find(&self, p: &[u16], from: i32) -> i32 {
        let u = self.u();
        let from = from.max(0) as usize;
        if p.is_empty() {
            return from.min(u.len()) as i32;
        }
        if p.len() > u.len() {
            return -1;
        }
        (from..=u.len() - p.len()).find(|i| &u[*i..*i + p.len()] == p).map_or(-1, |i| i as i32)
    }
    fn rfind(&self, p: &[u16], from: i32) -> i32 {
        let u = self.u();
        if p.len() > u.len() || from < 0 {
            return -1;
        }
        let last = (u.len() - p.len()).min(from as usize);
        (0..=last).rev().find(|i| &u[*i..*i + p.len()] == p).map_or(-1, |i| i as i32)
    }
    pub fn contains(&self, p: Str) -> bool {
        self.find(p.u(), 0) >= 0
    }
    pub fn indexOf_str(&self, p: Str) -> i32 {
        self.find(p.u(), 0)
    }
    pub fn indexOf_str_from(&self, p: Str, from: i32) -> i32 {
        self.find(p.u(), from)
    }
    pub fn indexOf_char(&self, c: i32) -> i32 {
        self.find(&utf16_of(c), 0)
    }
    pub fn indexOf_char_from(&self, c: i32, from: i32) -> i32 {
        self.find(&utf16_of(c), from)
    }
    pub fn lastIndexOf_str(&self, p: Str) -> i32 {
        self.rfind(p.u(), i32::MAX)
    }
    pub fn lastIndexOf_char(&self, c: i32) -> i32 {
        self.rfind(&utf16_of(c), i32::MAX)
    }
    pub fn lastIndexOf_char_from(&self, c: i32, from: i32) -> i32 {
        self.rfind(&utf16_of(c), from)
    }
    pub fn toUpperCase(&self) -> Str {
        Str::from_rs(&self.to_rs().to_uppercase())
    }
    pub fn toLowerCase(&self) -> Str {
        Str::from_rs(&self.to_rs().to_lowercase())
    }
    pub fn trim(&self) -> Str {
        let u = self.u();
        let a = u.iter().position(|c| *c > 32).unwrap_or(u.len());
        let b = u.iter().rposition(|c| *c > 32).map_or(a, |i| i + 1);
        self.sub(a, b)
    }
    pub fn strip(&self) -> Str {
        let u = self.u();
        let a = u.iter().position(|c| !is_ws(*c)).unwrap_or(u.len());
        let b = u.iter().rposition(|c| !is_ws(*c)).map_or(a, |i| i + 1);
        self.sub(a, b)
    }
    pub fn replace_char(&self, a: u16, b: u16) -> Str {
        Str::from_units(self.u().iter().map(|c| if *c == a { b } else { *c }).collect())
    }
    pub fn replace_str(&self, a: Str, b: Str) -> Str {
        let (u, p, r) = (self.u(), a.u(), b.u());
        let mut out = Vec::with_capacity(u.len());
        let mut i = 0;
        if p.is_empty() {
            for c in u {
                out.extend_from_slice(r);
                out.push(*c);
            }
            out.extend_from_slice(r);
            return Str::from_units(out);
        }
        while i < u.len() {
            if u[i..].starts_with(p) {
                out.extend_from_slice(r);
                i += p.len();
            } else {
                out.push(u[i]);
                i += 1;
            }
        }
        Str::from_units(out)
    }
    pub fn repeat(&self, n: i32) -> Str {
        if n < 0 {
            throw_new("java.lang.IllegalArgumentException", Some(format!("count is negative: {}", n)));
        }
        let mut v = Vec::new();
        for _ in 0..n {
            v.extend_from_slice(self.u());
        }
        Str::from_units(v)
    }
    pub fn toCharArray(&self) -> Arr<u16> {
        array_of(self.u())
    }
    pub fn chars_len(&self) -> usize {
        self.u().len()
    }
    pub fn toString(&self) -> Str {
        *self
    }
    // split(regex): a regex of plain text, "\\s+", or one escaped character; trailing empty strings are dropped, as Java does
    pub fn split(&self, regex: Str) -> Arr<Str> {
        let u = self.u();
        let re = regex.to_rs();
        let mut parts: Vec<Str> = Vec::new();
        if re == "\\s+" {
            let mut i = 0;
            let mut start = 0;
            while i < u.len() {
                if is_ws(u[i]) {
                    let mut j = i;
                    while j < u.len() && is_ws(u[j]) {
                        j += 1;
                    }
                    if !(i == 0 && j > 0 && start == 0 && i == start && false) {
                        parts.push(self.sub(start, i));
                    }
                    start = j;
                    i = j;
                } else {
                    i += 1;
                }
            }
            parts.push(self.sub(start, u.len()));
        } else {
            let lit_text = if re.len() == 2 && re.starts_with('\\') { re[1..].to_string() } else { re.clone() };
            if lit_text.chars().any(|c| "\\[](){}.*+?^$|".contains(c)) {
                throw_new("java.lang.UnsupportedOperationException", Some(format!("split on the pattern {}", re)));
            }
            let p: Vec<u16> = lit_text.encode_utf16().collect();
            if p.is_empty() {
                for i in 0..u.len() {
                    parts.push(self.sub(i, i + 1));
                }
            } else {
                let mut start = 0;
                let mut i = 0;
                while i + p.len() <= u.len() {
                    if u[i..].starts_with(&p) {
                        parts.push(self.sub(start, i));
                        i += p.len();
                        start = i;
                    } else {
                        i += 1;
                    }
                }
                parts.push(self.sub(start, u.len()));
            }
        }
        if parts.len() > 1 && parts[0].u().is_empty() && u.is_empty() {
            parts.truncate(1);
        }
        while parts.len() > 1 && parts[parts.len() - 1].u().is_empty() {
            parts.pop();
        }
        array_of(&parts)
    }
}

fn utf16_of(c: i32) -> Vec<u16> {
    match char::from_u32(c as u32) {
        Some(ch) => {
            let mut b = [0u16; 2];
            ch.encode_utf16(&mut b).to_vec()
        }
        None => vec![c as u16],
    }
}

fn lower_u16(c: u16) -> u16 {
    match char::from_u32(c as u32) {
        Some(ch) => {
            let mut it = ch.to_lowercase();
            match (it.next(), it.next()) {
                (Some(l), None) if (l as u32) < 0x10000 => l as u32 as u16,
                _ => c,
            }
        }
        None => c,
    }
}
fn upper_u16(c: u16) -> u16 {
    match char::from_u32(c as u32) {
        Some(ch) => {
            let mut it = ch.to_uppercase();
            match (it.next(), it.next()) {
                (Some(l), None) if (l as u32) < 0x10000 => l as u32 as u16,
                _ => c,
            }
        }
        None => c,
    }
}

// ---- string conversion ----------------------------------------------------------------------------------------------------

pub trait Cat {
    fn cat(&self, out: &mut Vec<u16>);
}
fn push_str(out: &mut Vec<u16>, s: &str) {
    out.extend(s.encode_utf16());
}
macro_rules! cat_int {
    ($($t:ty),*) => { $( impl Cat for $t { fn cat(&self, out: &mut Vec<u16>) { push_str(out, &self.to_string()); } } )* };
}
cat_int!(i8, i16, i32, i64);
impl Cat for u16 {
    fn cat(&self, out: &mut Vec<u16>) {
        out.push(*self);
    }
}
impl Cat for bool {
    fn cat(&self, out: &mut Vec<u16>) {
        push_str(out, if *self { "true" } else { "false" });
    }
}
impl Cat for f64 {
    fn cat(&self, out: &mut Vec<u16>) {
        push_str(out, &double_to_string(*self));
    }
}
impl Cat for f32 {
    fn cat(&self, out: &mut Vec<u16>) {
        push_str(out, &float_to_string(*self));
    }
}
impl Cat for Str {
    fn cat(&self, out: &mut Vec<u16>) {
        match self.0 {
            Some(u) => out.extend_from_slice(u),
            None => push_str(out, "null"),
        }
    }
}
impl<T: ?Sized + Object> Cat for Option<&'static T> {
    fn cat(&self, out: &mut Vec<u16>) {
        match self {
            Some(o) => o.toString().cat(out),
            None => push_str(out, "null"),
        }
    }
}


// Digits and exponent of the shortest text that reads back as the same number, as Java prints it.
fn java_float_text(digits_exp: String, negative: bool) -> String {
    // digits_exp is Rust's `{:e}` form: "d.ddde-5" or "de5"
    let (mant, exp) = digits_exp.split_once('e').unwrap();
    let exp: i32 = exp.parse().unwrap();
    let digits: String = mant.chars().filter(|c| *c != '.').collect();
    let mut out = String::new();
    if negative {
        out.push('-');
    }
    if (-3..7).contains(&exp) {
        if exp >= 0 {
            let int_len = exp as usize + 1;
            if digits.len() <= int_len {
                out.push_str(&digits);
                for _ in digits.len()..int_len {
                    out.push('0');
                }
                out.push_str(".0");
            } else {
                out.push_str(&digits[..int_len]);
                out.push('.');
                out.push_str(&digits[int_len..]);
            }
        } else {
            out.push_str("0.");
            for _ in 0..(-exp - 1) {
                out.push('0');
            }
            out.push_str(&digits);
        }
    } else {
        out.push_str(&digits[..1]);
        out.push('.');
        if digits.len() > 1 {
            out.push_str(&digits[1..]);
        } else {
            out.push('0');
        }
        out.push('E');
        out.push_str(&exp.to_string());
    }
    out
}

pub fn double_to_string(x: f64) -> String {
    if x.is_nan() {
        return "NaN".into();
    }
    if x.is_infinite() {
        return if x > 0.0 { "Infinity".into() } else { "-Infinity".into() };
    }
    if x == 0.0 {
        return if x.is_sign_negative() { "-0.0".into() } else { "0.0".into() };
    }
    java_float_text(format!("{:e}", x.abs()), x < 0.0)
}
pub fn float_to_string(x: f32) -> String {
    if x.is_nan() {
        return "NaN".into();
    }
    if x.is_infinite() {
        return if x > 0.0 { "Infinity".into() } else { "-Infinity".into() };
    }
    if x == 0.0 {
        return if x.is_sign_negative() { "-0.0".into() } else { "0.0".into() };
    }
    java_float_text(format!("{:e}", x.abs()), x < 0.0)
}

// ---- output ---------------------------------------------------------------------------------------------------------------

thread_local! {
    static STDOUT_BUF: RefCell<Vec<u8>> = RefCell::new(Vec::with_capacity(1 << 16));
}

pub fn flush_stdout() {
    STDOUT_BUF.with(|b| {
        let mut b = b.borrow_mut();
        let out = std::io::stdout();
        let mut lock = out.lock();
        let _ = lock.write_all(&b);
        let _ = lock.flush();
        b.clear();
    });
}

fn emit(err: bool, units: &[u16], newline: bool) {
    let mut text = String::from_utf16_lossy(units);
    if newline {
        text.push('\n');
    }
    if err {
        flush_stdout();
        let _ = std::io::stderr().write_all(text.as_bytes());
    } else {
        STDOUT_BUF.with(|b| {
            let mut b = b.borrow_mut();
            b.extend_from_slice(text.as_bytes());
            if b.len() > (1 << 16) {
                drop(b);
                flush_stdout();
            }
        });
    }
}

#[derive(Clone, Copy)]
pub struct PrintStream {
    err: bool,
}
pub struct System;
impl System {
    pub fn out() -> PrintStream {
        PrintStream { err: false }
    }
    pub fn err() -> PrintStream {
        PrintStream { err: true }
    }
    pub fn exit(code: i32) -> ! {
        flush_stdout();
        std::process::exit(code)
    }
    pub fn currentTimeMillis() -> i64 {
        std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH).map_or(0, |d| d.as_millis() as i64)
    }
    pub fn nanoTime() -> i64 {
        thread_local! { static START: std::time::Instant = std::time::Instant::now(); }
        START.with(|s| s.elapsed().as_nanos() as i64)
    }
    pub fn lineSeparator() -> Str {
        lit("\n")
    }
    pub fn identityHashCode(o: Ref<dyn Object>) -> i32 {
        match o {
            None => 0,
            Some(r) => r.hashCode(),
        }
    }
}
impl PrintStream {
    pub fn println<T: Cat>(&self, v: T) {
        let mut u = Vec::new();
        v.cat(&mut u);
        emit(self.err, &u, true);
    }
    pub fn print<T: Cat>(&self, v: T) {
        let mut u = Vec::new();
        v.cat(&mut u);
        emit(self.err, &u, false);
    }
    pub fn println0(&self) {
        emit(self.err, &[], true);
    }
    pub fn println_chars(&self, v: Arr<u16>) {
        emit(self.err, &v.units(), true);
    }
    pub fn print_chars(&self, v: Arr<u16>) {
        emit(self.err, &v.units(), false);
    }
    pub fn printf(&self, format: Str, args: &[FArg]) {
        emit(self.err, jformat(format, args).u(), false);
    }
    pub fn flush(&self) {
        if !self.err {
            flush_stdout();
        }
    }
}

// ---- arrays ---------------------------------------------------------------------------------------------------------------

pub struct JArray<T: Copy + 'static> {
    cells: &'static [Cell<T>],
}
pub type Arr<T> = Option<&'static JArray<T>>;

fn bounds_error(i: i32, n: usize) -> ! {
    throw_new("java.lang.ArrayIndexOutOfBoundsException", Some(format!("Index {} out of bounds for length {}", i, n)))
}

pub trait ArrOps<T: Copy + 'static> {
    fn at(self, i: i32) -> T;
    fn put(self, i: i32, v: T);
    fn length(self) -> i32;
    fn units(self) -> Vec<T>;
    fn copy_of(self) -> Self;
}
impl<T: Copy + 'static> ArrOps<T> for Option<&'static JArray<T>> {
    fn at(self, i: i32) -> T {
        let a = self.nn();
        match a.cells.get(i as u32 as usize) {
            Some(c) if i >= 0 => c.get(),
            _ => bounds_error(i, a.cells.len()),
        }
    }
    fn put(self, i: i32, v: T) {
        let a = self.nn();
        match a.cells.get(i as u32 as usize) {
            Some(c) if i >= 0 => c.set(v),
            _ => bounds_error(i, a.cells.len()),
        }
    }
    fn length(self) -> i32 {
        self.nn().cells.len() as i32
    }
    fn units(self) -> Vec<T> {
        self.nn().cells.iter().map(|c| c.get()).collect()
    }
    fn copy_of(self) -> Self {
        array_of(&self.units())
    }
}

pub fn new_array<T: Copy + Default + 'static>(n: i32) -> Arr<T> {
    if n < 0 {
        throw_new("java.lang.NegativeArraySizeException", Some(n.to_string()));
    }
    Some(gc_new(JArray { cells: gc_cells(n as usize, |_| T::default()) }))
}
pub fn array_of<T: Copy + 'static>(items: &[T]) -> Arr<T> {
    Some(gc_new_internal(JArray { cells: gc_cells(items.len(), |i| items[i]) }))
}
pub fn new_array2<T: Copy + Default + 'static>(a: i32, b: i32) -> Arr<Arr<T>> {
    let outer = new_array::<Arr<T>>(a);
    for i in 0..a {
        outer.put(i, new_array::<T>(b));
    }
    outer
}
pub fn new_array3<T: Copy + Default + 'static>(a: i32, b: i32, c: i32) -> Arr<Arr<Arr<T>>> {
    let outer = new_array::<Arr<Arr<T>>>(a);
    for i in 0..a {
        outer.put(i, new_array2::<T>(b, c));
    }
    outer
}

// ---- arithmetic -----------------------------------------------------------------------------------------------------------

fn div_zero() -> ! {
    throw_new("java.lang.ArithmeticException", Some("/ by zero".into()))
}
pub trait JInt: Sized + Copy {
    fn jdiv(self, o: Self) -> Self;
    fn jrem(self, o: Self) -> Self;
}
impl JInt for i32 {
    fn jdiv(self, o: i32) -> i32 {
        if o == 0 {
            div_zero()
        }
        self.wrapping_div(o)
    }
    fn jrem(self, o: i32) -> i32 {
        if o == 0 {
            div_zero()
        }
        self.wrapping_rem(o)
    }
}
impl JInt for i64 {
    fn jdiv(self, o: i64) -> i64 {
        if o == 0 {
            div_zero()
        }
        self.wrapping_div(o)
    }
    fn jrem(self, o: i64) -> i64 {
        if o == 0 {
            div_zero()
        }
        self.wrapping_rem(o)
    }
}

// ---- StringBuilder --------------------------------------------------------------------------------------------------------

pub struct StringBuilder {
    v: GcVec<u16>,
}
fn sb_index(i: i32, n: usize) -> usize {
    if i < 0 || i as usize >= n {
        throw_new("java.lang.StringIndexOutOfBoundsException", Some(format!("index {},length {}", i, n)));
    }
    i as usize
}
impl StringBuilder {
    pub fn new() -> Ref<StringBuilder> {
        Some(gc_new(StringBuilder { v: GcVec::new() }))
    }
    pub fn from_str(s: Str) -> Ref<StringBuilder> {
        Some(gc_new(StringBuilder { v: GcVec::from_slice(s.u()) }))
    }
    pub fn append<T: Cat>(&'static self, x: T) -> Ref<StringBuilder> {
        let mut piece = Vec::new();
        x.cat(&mut piece);
        self.v.extend_from_slice(&piece);
        Some(self)
    }
    pub fn append_chars(&'static self, x: Arr<u16>) -> Ref<StringBuilder> {
        self.v.extend_from_slice(&x.units());
        Some(self)
    }
    pub fn length(&'static self) -> i32 {
        self.v.len() as i32
    }
    pub fn isEmpty(&'static self) -> bool {
        self.v.len() == 0
    }
    pub fn charAt(&'static self, i: i32) -> u16 {
        self.v.get(sb_index(i, self.v.len()))
    }
    pub fn setCharAt(&'static self, i: i32, c: u16) {
        self.v.set(sb_index(i, self.v.len()), c);
    }
    pub fn deleteCharAt(&'static self, i: i32) -> Ref<StringBuilder> {
        self.v.remove(sb_index(i, self.v.len()));
        Some(self)
    }
    pub fn delete(&'static self, a: i32, b: i32) -> Ref<StringBuilder> {
        let mut v = self.v.to_vec();
        let n = v.len();
        let b = (b.max(0) as usize).min(n);
        if a < 0 || a as usize > b {
            throw_new("java.lang.StringIndexOutOfBoundsException", Some(format!("start {}, end {}, length {}", a, b, n)));
        }
        v.drain(a as usize..b);
        self.v.assign(&v);
        Some(self)
    }
    pub fn insert<T: Cat>(&'static self, at: i32, x: T) -> Ref<StringBuilder> {
        let mut piece = Vec::new();
        x.cat(&mut piece);
        let mut v = self.v.to_vec();
        if at < 0 || at as usize > v.len() {
            throw_new("java.lang.StringIndexOutOfBoundsException", Some(format!("offset {}, length {}", at, v.len())));
        }
        let tail = v.split_off(at as usize);
        v.extend(piece);
        v.extend(tail);
        self.v.assign(&v);
        Some(self)
    }
    pub fn reverse(&'static self) -> Ref<StringBuilder> {
        let mut v = self.v.to_vec();
        v.reverse();
        self.v.assign(&v);
        Some(self)
    }
    pub fn setLength(&'static self, n: i32) {
        self.v.resize(n.max(0) as usize, 0);
    }
    pub fn indexOf(&'static self, s: Str) -> i32 {
        Str::from_units(self.v.to_vec()).indexOf_str(s)
    }
    pub fn substring(&'static self, a: i32) -> Str {
        Str::from_units(self.v.to_vec()).substring(a)
    }
    pub fn substring2(&'static self, a: i32, b: i32) -> Str {
        Str::from_units(self.v.to_vec()).substring2(a, b)
    }
}
impl Object for StringBuilder {
    fn toString(&'static self) -> Str {
        gc_poll();
        Str::from_units(self.v.to_vec())
    }
    fn class_name(&'static self) -> &'static str {
        "java.lang.StringBuilder"
    }
    fn class_chain(&'static self) -> &'static [&'static str] {
        &["java.lang.StringBuilder", "java.lang.Object"]
    }
    fn as_any(&'static self) -> &'static dyn Any {
        self
    }
    fn as_object(&'static self) -> &'static dyn Object {
        self
    }
}

// ---- exceptions -----------------------------------------------------------------------------------------------------------

pub struct ThrowableData {
    pub message: Cell<Str>,
    pub cause: Cell<Ref<dyn Throwable>>,
}
impl Default for ThrowableData {
    fn default() -> Self {
        ThrowableData { message: Cell::new(Str::NULL), cause: Cell::new(None) }
    }
}
impl ThrowableData {
    // the constructors Throwable and its library subclasses have: (), (String), (String, Throwable), (Throwable)
    pub fn init(this: &'static dyn Throwable) {}
    pub fn init_message(this: &'static dyn Throwable, message: Str) {
        this.Throwable_fields().message.set(message);
    }
    pub fn init_message_cause(this: &'static dyn Throwable, message: Str, cause: Ref<dyn Throwable>) {
        this.Throwable_fields().message.set(message);
        this.Throwable_fields().cause.set(cause);
    }
    pub fn init_cause(this: &'static dyn Throwable, cause: Ref<dyn Throwable>) {
        this.Throwable_fields().cause.set(cause);
        this.Throwable_fields().message.set(match cause {
            Some(c) => c.toString(),
            None => Str::NULL,
        });
    }
}
pub trait Throwable: Object {
    fn Throwable_fields(&'static self) -> &'static ThrowableData;
    fn getMessage(&'static self) -> Str {
        self.Throwable_fields().message.get()
    }
    fn getLocalizedMessage(&'static self) -> Str {
        self.getMessage()
    }
    fn getCause(&'static self) -> Ref<dyn Throwable> {
        self.Throwable_fields().cause.get()
    }
}
impl dyn Throwable {
    pub fn printStackTrace(&'static self) {
        emit(true, self.toString().u(), true);
    }
}
// toString of a throwable that does not override it: its class and, when it has one, its message
pub fn throwable_to_string(t: &'static dyn Throwable) -> Str {
    match t.getLocalizedMessage().0 {
        Some(_) => cat!(lit(t.class_name()), lit(": "), t.getLocalizedMessage()),
        None => lit(t.class_name()),
    }
}

// A library exception made by the runtime or by `new X(...)` of a library class.
pub struct LibException {
    data: ThrowableData,
    name: &'static str,
    chain: &'static [&'static str],
}
const LIB_PARENT: &[(&str, &str)] = &[
    ("java.lang.Throwable", "java.lang.Object"),
    ("java.lang.Exception", "java.lang.Throwable"),
    ("java.lang.Error", "java.lang.Throwable"),
    ("java.lang.RuntimeException", "java.lang.Exception"),
    ("java.lang.IllegalArgumentException", "java.lang.RuntimeException"),
    ("java.lang.IllegalStateException", "java.lang.RuntimeException"),
    ("java.lang.ArithmeticException", "java.lang.RuntimeException"),
    ("java.lang.NullPointerException", "java.lang.RuntimeException"),
    ("java.lang.ClassCastException", "java.lang.RuntimeException"),
    ("java.lang.IndexOutOfBoundsException", "java.lang.RuntimeException"),
    ("java.lang.ArrayIndexOutOfBoundsException", "java.lang.IndexOutOfBoundsException"),
    ("java.lang.StringIndexOutOfBoundsException", "java.lang.IndexOutOfBoundsException"),
    ("java.lang.NegativeArraySizeException", "java.lang.RuntimeException"),
    ("java.lang.ArrayStoreException", "java.lang.RuntimeException"),
    ("java.lang.NumberFormatException", "java.lang.IllegalArgumentException"),
    ("java.lang.UnsupportedOperationException", "java.lang.RuntimeException"),
    ("java.lang.CloneNotSupportedException", "java.lang.Exception"),
    ("java.lang.InterruptedException", "java.lang.Exception"),
    ("java.lang.AssertionError", "java.lang.Error"),
    ("java.lang.VirtualMachineError", "java.lang.Error"),
    ("java.lang.StackOverflowError", "java.lang.VirtualMachineError"),
    ("java.lang.OutOfMemoryError", "java.lang.VirtualMachineError"),
    ("java.util.NoSuchElementException", "java.lang.RuntimeException"),
    ("java.util.InputMismatchException", "java.util.NoSuchElementException"),
    ("java.util.ConcurrentModificationException", "java.lang.RuntimeException"),
    ("java.util.EmptyStackException", "java.lang.RuntimeException"),
    ("java.io.IOException", "java.lang.Exception"),
    ("java.io.UncheckedIOException", "java.lang.RuntimeException"),
    ("java.io.FileNotFoundException", "java.io.IOException"),
];
fn lib_chain(name: &'static str) -> &'static [&'static str] {
    thread_local! { static CHAINS: RefCell<std::collections::HashMap<&'static str, &'static [&'static str]>> = RefCell::new(std::collections::HashMap::new()); }
    CHAINS.with(|m| {
        *m.borrow_mut().entry(name).or_insert_with(|| {
            let mut chain = vec![name];
            let mut at = name;
            while let Some((_, p)) = LIB_PARENT.iter().find(|(c, _)| *c == at) {
                chain.push(*p);
                at = p;
            }
            if at != "java.lang.Object" {
                chain.push("java.lang.Object");
            }
            &*Box::leak(chain.into_boxed_slice())
        })
    })
}
impl LibException {
    pub fn make(name: &'static str, message: Str, cause: Ref<dyn Throwable>) -> &'static LibException {
        let e: &'static LibException = gc_new_internal(LibException { data: ThrowableData::default(), name, chain: lib_chain(name) });
        e.data.message.set(message);
        e.data.cause.set(cause);
        e
    }
}
impl Object for LibException {
    fn toString(&'static self) -> Str {
        throwable_to_string(self)
    }
    fn class_name(&'static self) -> &'static str {
        self.name
    }
    fn class_chain(&'static self) -> &'static [&'static str] {
        self.chain
    }
    fn as_any(&'static self) -> &'static dyn Any {
        self
    }
    fn as_object(&'static self) -> &'static dyn Object {
        self
    }
}
impl Throwable for LibException {
    fn Throwable_fields(&'static self) -> &'static ThrowableData {
        &self.data
    }
}

// `new X(message)`, `new X(message, cause)`, `new X(cause)` and `new X()` of a library exception class.
pub fn new_exception(name: &'static str, message: Str, cause: Ref<dyn Throwable>) -> Ref<dyn Throwable> {
    Some(LibException::make(name, message, cause))
}
pub fn new_exception_cause(name: &'static str, cause: Ref<dyn Throwable>) -> Ref<dyn Throwable> {
    let message = match cause {
        Some(c) => c.toString(),
        None => Str::NULL,
    };
    Some(LibException::make(name, message, cause))
}

// What an unwind carries when Java code throws: a marker; the exception itself waits in `THROWN` (a reference is not Send).
pub struct Thrown;
thread_local! {
    static THROWN: &'static Cell<Option<&'static dyn Throwable>> = static_cell(Cell::new(None));
}
fn take_thrown() -> &'static dyn Throwable {
    THROWN.with(|t| t.take()).expect("a Java exception in flight")
}

pub fn throw_new(name: &'static str, message: Option<String>) -> ! {
    throw(Some(LibException::make(name, message.map_or(Str::NULL, |m| Str::from_rs(&m)), None)))
}
pub fn throw(t: Ref<dyn Throwable>) -> ! {
    {
        let t = t.nn();
        THROWN.with(|c| c.set(Some(t)));
        std::panic::resume_unwind(Box::new(Thrown))
    }
}

// try { body } : the exception it threw, if it threw one
pub fn try_catch<R>(body: impl FnOnce() -> R) -> Result<R, &'static dyn Throwable> {
    match std::panic::catch_unwind(std::panic::AssertUnwindSafe(body)) {
        Ok(r) => Ok(r),
        Err(payload) => match payload.downcast::<Thrown>() {
            Ok(_) => Err(take_thrown()),
            Err(other) => std::panic::resume_unwind(other),
        },
    }
}
// try { body } finally { cleanup }: the cleanup runs however the body ends
pub fn with_finally<R>(body: impl FnOnce() -> R, cleanup: impl FnOnce()) -> R {
    match std::panic::catch_unwind(std::panic::AssertUnwindSafe(body)) {
        Ok(r) => {
            cleanup();
            r
        }
        Err(payload) => {
            cleanup();
            std::panic::resume_unwind(payload)
        }
    }
}

// What leaves a try body: it ended, or it returned, broke or continued out of a statement around it.
pub enum Flow<R> {
    Next,
    Return(R),
    Break(u32),
    Continue(u32),
}

pub fn check_cast(o: Ref<dyn Object>, name: &str, from: &str) {
    if let Some(r) = o {
        if !r.instance_of(name) {
            throw_new("java.lang.ClassCastException", Some(format!("class {} cannot be cast to class {}", r.class_name(), name)));
        }
    }
}

// The program's entry: runs `main` (on a thread with a big stack, so that deep recursion Java copes with does here), then ends the process as the
// JVM does (an exception nobody caught is reported and exits 1).
pub fn run(main: impl FnOnce() + Send + 'static) -> ! {
    let worker = std::thread::Builder::new().stack_size(256 << 20).spawn(move || {
        let top = 0usize;
        gc_enable(std::hint::black_box(&top) as *const usize as usize + 128);
        let r = std::panic::catch_unwind(std::panic::AssertUnwindSafe(main));
        flush_stdout();
        match r {
            Ok(()) => 0,
            Err(payload) => match payload.downcast::<Thrown>() {
                Ok(_) => {
                    let mut text = String::from("Exception in thread \"main\" ");
                    text.push_str(&take_thrown().toString().to_rs());
                    text.push('\n');
                    let _ = std::io::stderr().write_all(text.as_bytes());
                    1
                }
                Err(_) => 1,
            },
        }
    });
    let code = match worker {
        Ok(handle) => handle.join().unwrap_or(1),
        Err(_) => 1,
    };
    std::process::exit(code)
}

// ---- formatting (String.format, printf) -----------------------------------------------------------------------------------

#[derive(Clone, Copy)]
pub enum FArg {
    Int(i64),
    Float(f64),
    Text(Str),
    Char(u16),
    Bool(bool),
    Null,
}
pub trait FmtArg {
    fn fa(self) -> FArg;
}
impl FmtArg for i8 {
    fn fa(self) -> FArg {
        FArg::Int(self as i64)
    }
}
impl FmtArg for i16 {
    fn fa(self) -> FArg {
        FArg::Int(self as i64)
    }
}
impl FmtArg for i32 {
    fn fa(self) -> FArg {
        FArg::Int(self as i64)
    }
}
impl FmtArg for i64 {
    fn fa(self) -> FArg {
        FArg::Int(self)
    }
}
impl FmtArg for f64 {
    fn fa(self) -> FArg {
        FArg::Float(self)
    }
}
impl FmtArg for f32 {
    fn fa(self) -> FArg {
        FArg::Float(self as f64)
    }
}
impl FmtArg for u16 {
    fn fa(self) -> FArg {
        FArg::Char(self)
    }
}
impl FmtArg for bool {
    fn fa(self) -> FArg {
        FArg::Bool(self)
    }
}
impl FmtArg for Str {
    fn fa(self) -> FArg {
        FArg::Text(self)
    }
}
impl<T: ?Sized + Object> FmtArg for Option<&'static T> {
    fn fa(self) -> FArg {
        match self {
            Some(o) => FArg::Text(o.toString()),
            None => FArg::Null,
        }
    }
}
pub fn fa<T: FmtArg>(x: T) -> FArg {
    x.fa()
}

// A double as Java's %.Nf writes it: the digits of its shortest text, rounded half up.
fn fixed_half_up(x: f64, prec: usize) -> String {
    let sci = format!("{:e}", x.abs());
    let (mant, exp) = sci.split_once('e').unwrap();
    let exp: i32 = exp.parse().unwrap();
    let all: Vec<u8> = mant.bytes().filter(|c| *c != b'.').collect();
    // the digits as an integer part and a fraction
    let (int_part, frac): (Vec<u8>, Vec<u8>) = if exp >= 0 {
        let n = exp as usize + 1;
        let mut ip = all.clone();
        while ip.len() < n {
            ip.push(b'0');
        }
        let fr = ip.split_off(n);
        (ip, fr)
    } else {
        let mut fr = vec![b'0'; (-exp - 1) as usize];
        fr.extend(&all);
        (vec![b'0'], fr)
    };
    let mut digits: Vec<u8> = int_part;
    let int_len0 = digits.len();
    for i in 0..prec {
        digits.push(*frac.get(i).unwrap_or(&b'0'));
    }
    if frac.get(prec).map_or(false, |d| *d >= b'5') {
        let mut i = digits.len();
        loop {
            if i == 0 {
                digits.insert(0, b'1');
                break;
            }
            i -= 1;
            if digits[i] == b'9' {
                digits[i] = b'0';
            } else {
                digits[i] += 1;
                break;
            }
        }
    }
    let int_len = digits.len() - prec;
    let mut s = String::from_utf8(digits[..int_len].to_vec()).unwrap();
    if prec > 0 {
        s.push('.');
        s.push_str(std::str::from_utf8(&digits[int_len..]).unwrap());
    }
    s
}
fn group_thousands(int_digits: &str) -> String {
    let mut out = String::new();
    for (i, c) in int_digits.chars().enumerate() {
        if i > 0 && (int_digits.len() - i) % 3 == 0 {
            out.push(',');
        }
        out.push(c);
    }
    out
}

pub fn jformat(format: Str, args: &[FArg]) -> Str {
    let f = format.to_rs();
    let chars: Vec<char> = f.chars().collect();
    let mut out = String::new();
    let mut i = 0;
    let mut next = 0;
    while i < chars.len() {
        if chars[i] != '%' {
            out.push(chars[i]);
            i += 1;
            continue;
        }
        i += 1;
        let (mut left, mut zero, mut plus, mut comma, mut space) = (false, false, false, false, false);
        let mut index: Option<usize> = None;
        let start = i;
        let mut n = 0usize;
        while i < chars.len() && chars[i].is_ascii_digit() {
            n = n * 10 + chars[i] as usize - '0' as usize;
            i += 1;
        }
        if i < chars.len() && chars[i] == '$' && i > start {
            index = Some(n - 1);
            i += 1;
        } else {
            i = start;
        }
        while i < chars.len() && "-0+, #(".contains(chars[i]) {
            match chars[i] {
                '-' => left = true,
                '0' => zero = true,
                '+' => plus = true,
                ',' => comma = true,
                ' ' => space = true,
                _ => throw_new("java.util.UnsupportedOperationException", Some("format flag".into())),
            }
            i += 1;
        }
        let mut width = 0usize;
        while i < chars.len() && chars[i].is_ascii_digit() {
            width = width * 10 + chars[i] as usize - '0' as usize;
            i += 1;
        }
        let mut prec: Option<usize> = None;
        if i < chars.len() && chars[i] == '.' {
            i += 1;
            let mut p = 0;
            while i < chars.len() && chars[i].is_ascii_digit() {
                p = p * 10 + chars[i] as usize - '0' as usize;
                i += 1;
            }
            prec = Some(p);
        }
        if i >= chars.len() {
            throw_new("java.util.UnknownFormatConversionException", Some("%".into()));
        }
        let conv = chars[i];
        i += 1;
        if conv == 'n' {
            out.push('\n');
            continue;
        }
        if conv == '%' {
            out.push('%');
            continue;
        }
        let arg = {
            let k = index.unwrap_or_else(|| {
                next += 1;
                next - 1
            });
            match args.get(k) {
                Some(a) => *a,
                None => throw_new("java.util.MissingFormatArgumentException", Some(format!("Format specifier '%{}'", conv))),
            }
        };
        let mut body = match (conv, arg) {
            ('d', FArg::Int(v)) => {
                let digits = v.unsigned_abs().to_string();
                let digits = if comma { group_thousands(&digits) } else { digits };
                let sign = if v < 0 { "-" } else if plus { "+" } else if space { " " } else { "" };
                if zero && !left && width > sign.len() + digits.len() {
                    format!("{}{}{}", sign, "0".repeat(width - sign.len() - digits.len()), digits)
                } else {
                    format!("{}{}", sign, digits)
                }
            }
            ('x', FArg::Int(v)) | ('X', FArg::Int(v)) => {
                let t = if v < 0 && (v as i32) as i64 == v { format!("{:x}", v as i32) } else { format!("{:x}", v) };
                let t = if conv == 'X' { t.to_uppercase() } else { t };
                if zero && !left && width > t.len() { format!("{}{}", "0".repeat(width - t.len()), t) } else { t }
            }
            ('o', FArg::Int(v)) => {
                let t = if v < 0 && (v as i32) as i64 == v { format!("{:o}", v as i32) } else { format!("{:o}", v) };
                if zero && !left && width > t.len() { format!("{}{}", "0".repeat(width - t.len()), t) } else { t }
            }
            ('e', FArg::Float(v)) | ('E', FArg::Float(v)) => {
                let t = format!("{:.*e}", prec.unwrap_or(6), v);
                let (m, e) = t.split_once('e').unwrap();
                let e: i32 = e.parse().unwrap();
                let t = format!("{}e{}{:02}", m, if e < 0 { '-' } else { '+' }, e.abs());
                if conv == 'E' { t.to_uppercase() } else { t }
            }
            ('f', FArg::Float(v)) => {
                if v.is_nan() {
                    "NaN".to_string()
                } else if v.is_infinite() {
                    (if v > 0.0 { "Infinity" } else { "-Infinity" }).to_string()
                } else {
                    let t = fixed_half_up(v, prec.unwrap_or(6));
                    let (ip, fp) = match t.split_once('.') {
                        Some((a, b)) => (a.to_string(), format!(".{}", b)),
                        None => (t.clone(), String::new()),
                    };
                    let ip = if comma { group_thousands(&ip) } else { ip };
                    let neg = v.is_sign_negative() && (v != 0.0 || true) && v < 0.0;
                    let sign = if neg { "-" } else if plus { "+" } else if space { " " } else { "" };
                    let digits = format!("{}{}", ip, fp);
                    if zero && !left && width > sign.len() + digits.len() {
                        format!("{}{}{}", sign, "0".repeat(width - sign.len() - digits.len()), digits)
                    } else {
                        format!("{}{}", sign, digits)
                    }
                }
            }
            ('s', a) | ('S', a) => {
                let t = match a {
                    FArg::Text(s) => s.to_rs_or_null(),
                    FArg::Int(v) => v.to_string(),
                    FArg::Float(v) => double_to_string(v),
                    FArg::Char(c) => String::from_utf16_lossy(&[c]),
                    FArg::Bool(b) => b.to_string(),
                    FArg::Null => "null".to_string(),
                };
                let t = match prec {
                    Some(p) => t.chars().take(p).collect(),
                    None => t,
                };
                if conv == 'S' { t.to_uppercase() } else { t }
            }
            ('c', FArg::Char(c)) => String::from_utf16_lossy(&[c]),
            ('c', FArg::Int(c)) => char::from_u32(c as u32).map_or(String::new(), |c| c.to_string()),
            ('b', FArg::Bool(b)) | ('B', FArg::Bool(b)) => b.to_string(),
            ('b', FArg::Null) => "false".to_string(),
            ('b', _) => "true".to_string(),
            _ => throw_new("java.util.IllegalFormatConversionException", Some(format!("{} != argument", conv))),
        };
        let len = body.chars().count();
        if width > len {
            let pad = " ".repeat(width - len);
            body = if left { format!("{}{}", body, pad) } else { format!("{}{}", pad, body) };
        }
        out.push_str(&body);
    }
    Str::from_rs(&out)
}
impl Str {
    fn to_rs_or_null(&self) -> String {
        if self.is_null() { "null".to_string() } else { self.to_rs() }
    }
}

// ---- Math and the number classes ------------------------------------------------------------------------------------------

pub struct Math;
impl Math {
    pub const PI: f64 = std::f64::consts::PI;
    pub const E: f64 = std::f64::consts::E;
    pub fn abs_i32(a: i32) -> i32 {
        a.wrapping_abs()
    }
    pub fn abs_i64(a: i64) -> i64 {
        a.wrapping_abs()
    }
    pub fn abs_f64(a: f64) -> f64 {
        a.abs()
    }
    pub fn abs_f32(a: f32) -> f32 {
        a.abs()
    }
    pub fn max_f64(a: f64, b: f64) -> f64 {
        if a.is_nan() || b.is_nan() {
            f64::NAN
        } else if a == 0.0 && b == 0.0 {
            if a.is_sign_negative() { b } else { a }
        } else if a > b {
            a
        } else {
            b
        }
    }
    pub fn min_f64(a: f64, b: f64) -> f64 {
        if a.is_nan() || b.is_nan() {
            f64::NAN
        } else if a == 0.0 && b == 0.0 {
            if a.is_sign_negative() { a } else { b }
        } else if a < b {
            a
        } else {
            b
        }
    }
    pub fn round_f64(a: f64) -> i64 {
        (a + 0.5).floor() as i64
    }
    pub fn round_f32(a: f32) -> i32 {
        (a as f64 + 0.5).floor() as i32
    }
    pub fn signum(a: f64) -> f64 {
        if a == 0.0 || a.is_nan() { a } else { a.signum() }
    }
    pub fn floorDiv_i32(a: i32, b: i32) -> i32 {
        let q = a.jdiv(b);
        if (a ^ b) < 0 && q.wrapping_mul(b) != a { q - 1 } else { q }
    }
    pub fn floorMod_i32(a: i32, b: i32) -> i32 {
        let m = a.jrem(b);
        if (m ^ b) < 0 && m != 0 { m + b } else { m }
    }
    pub fn floorDiv_i64(a: i64, b: i64) -> i64 {
        let q = a.jdiv(b);
        if (a ^ b) < 0 && q.wrapping_mul(b) != a { q - 1 } else { q }
    }
    pub fn floorMod_i64(a: i64, b: i64) -> i64 {
        let m = a.jrem(b);
        if (m ^ b) < 0 && m != 0 { m + b } else { m }
    }
    pub fn toRadians(a: f64) -> f64 {
        a / 180.0 * std::f64::consts::PI
    }
    pub fn toDegrees(a: f64) -> f64 {
        a * 180.0 / std::f64::consts::PI
    }
    pub fn addExact_i32(a: i32, b: i32) -> i32 {
        a.checked_add(b).unwrap_or_else(|| throw_new("java.lang.ArithmeticException", Some("integer overflow".into())))
    }
    pub fn multiplyExact_i32(a: i32, b: i32) -> i32 {
        a.checked_mul(b).unwrap_or_else(|| throw_new("java.lang.ArithmeticException", Some("integer overflow".into())))
    }
    pub fn addExact_i64(a: i64, b: i64) -> i64 {
        a.checked_add(b).unwrap_or_else(|| throw_new("java.lang.ArithmeticException", Some("long overflow".into())))
    }
    pub fn multiplyExact_i64(a: i64, b: i64) -> i64 {
        a.checked_mul(b).unwrap_or_else(|| throw_new("java.lang.ArithmeticException", Some("long overflow".into())))
    }
    pub fn pow(a: f64, b: f64) -> f64 {
        if b == 0.0 {
            1.0
        } else if b.is_nan() || (a.is_nan()) {
            f64::NAN
        } else {
            a.powf(b)
        }
    }
}

fn number_format(s: Str) -> ! {
    match s.0 {
        Some(_) => throw_new("java.lang.NumberFormatException", Some(format!("For input string: \"{}\"", s.to_rs()))),
        None => throw_new("java.lang.NumberFormatException", Some("Cannot parse null string: null".into())),
    }
}
pub fn Integer_parseInt(s: Str) -> i32 {
    Integer_parseInt_radix(s, 10)
}
pub fn Integer_parseInt_radix(s: Str, radix: i32) -> i32 {
    let t = match s.0 {
        Some(_) => s.to_rs(),
        None => number_format(s),
    };
    let ok = !t.is_empty() && t.chars().enumerate().all(|(i, c)| (i == 0 && (c == '-' || c == '+') && t.len() > 1) || c.is_digit(radix as u32));
    if !ok {
        number_format(s);
    }
    i32::from_str_radix(&t, radix as u32).unwrap_or_else(|_| number_format(s))
}
pub fn Long_parseLong(s: Str) -> i64 {
    let t = match s.0 {
        Some(_) => s.to_rs(),
        None => number_format(s),
    };
    let ok = !t.is_empty() && t.chars().enumerate().all(|(i, c)| (i == 0 && (c == '-' || c == '+') && t.len() > 1) || c.is_ascii_digit());
    if !ok {
        number_format(s);
    }
    t.parse::<i64>().unwrap_or_else(|_| number_format(s))
}
pub fn Double_parseDouble(s: Str) -> f64 {
    let t = match s.0 {
        Some(_) => s.to_rs().trim().to_string(),
        None => throw_new("java.lang.NullPointerException", None),
    };
    match t.as_str() {
        "NaN" => f64::NAN,
        "Infinity" | "+Infinity" => f64::INFINITY,
        "-Infinity" => f64::NEG_INFINITY,
        _ => {
            let body = t.strip_suffix(|c| "dDfF".contains(c)).unwrap_or(&t);
            if body.is_empty() || body.contains("inf") || body.contains("nan") || body.contains("NaN") {
                throw_new("java.lang.NumberFormatException", Some(if t.is_empty() { "empty String".to_string() } else { format!("For input string: \"{}\"", s.to_rs()) }));
            }
            body.parse::<f64>().unwrap_or_else(|_| throw_new("java.lang.NumberFormatException", Some(format!("For input string: \"{}\"", s.to_rs()))))
        }
    }
}
pub fn Boolean_parseBoolean(s: Str) -> bool {
    !s.is_null() && s.equalsIgnoreCase(lit("true"))
}
pub fn Integer_toString_radix(v: i32, radix: i32) -> Str {
    Long_toString_radix(v as i64, radix)
}
pub fn Long_toString_radix(v: i64, radix: i32) -> Str {
    let mut n = v.unsigned_abs();
    if n == 0 {
        return lit("0");
    }
    let mut d = Vec::new();
    while n > 0 {
        d.push(std::char::from_digit((n % radix as u64) as u32, radix as u32).unwrap());
        n /= radix as u64;
    }
    if v < 0 {
        d.push('-');
    }
    d.reverse();
    Str::from_rs(&d.into_iter().collect::<String>())
}
pub fn Integer_toHexString(v: i32) -> Str {
    Str::from_rs(&format!("{:x}", v))
}
pub fn Integer_toBinaryString(v: i32) -> Str {
    Str::from_rs(&format!("{:b}", v))
}
pub fn Integer_toOctalString(v: i32) -> Str {
    Str::from_rs(&format!("{:o}", v))
}
pub fn Long_toHexString(v: i64) -> Str {
    Str::from_rs(&format!("{:x}", v))
}
pub fn Long_toBinaryString(v: i64) -> Str {
    Str::from_rs(&format!("{:b}", v))
}
pub fn Integer_compare(a: i32, b: i32) -> i32 {
    (a > b) as i32 - (a < b) as i32
}
pub fn Long_compare(a: i64, b: i64) -> i32 {
    (a > b) as i32 - (a < b) as i32
}
pub fn Double_compare(a: f64, b: f64) -> i32 {
    if a < b {
        -1
    } else if a > b {
        1
    } else {
        let (x, y) = (a.to_bits() as i64, b.to_bits() as i64);
        let (x, y) = (if a.is_nan() { 0x7ff8000000000000 } else { x }, if b.is_nan() { 0x7ff8000000000000 } else { y });
        (x > y) as i32 - (x < y) as i32
    }
}
pub fn Character_isDigit(c: u16) -> bool {
    char::from_u32(c as u32).map_or(false, |c| c.is_numeric() && c.to_digit(10).is_some() || c.is_ascii_digit())
}
pub fn Character_isLetter(c: u16) -> bool {
    char::from_u32(c as u32).map_or(false, |c| c.is_alphabetic())
}
pub fn Character_isLetterOrDigit(c: u16) -> bool {
    Character_isLetter(c) || Character_isDigit(c)
}
pub fn Character_isUpperCase(c: u16) -> bool {
    char::from_u32(c as u32).map_or(false, |c| c.is_uppercase())
}
pub fn Character_isLowerCase(c: u16) -> bool {
    char::from_u32(c as u32).map_or(false, |c| c.is_lowercase())
}
pub fn Character_isWhitespace(c: u16) -> bool {
    is_ws(c)
}
pub fn Character_toUpperCase(c: u16) -> u16 {
    upper_u16(c)
}
pub fn Character_toLowerCase(c: u16) -> u16 {
    lower_u16(c)
}
pub fn Character_getNumericValue(c: u16) -> i32 {
    char::from_u32(c as u32).and_then(|c| c.to_digit(36)).map_or(-1, |d| d as i32)
}
pub fn Character_toString(c: u16) -> Str {
    Str::from_units(vec![c])
}
pub fn String_valueOf<T: Cat>(v: T) -> Str {
    let mut u = Vec::new();
    v.cat(&mut u);
    Str::from_units(u)
}
pub fn String_valueOf_chars(v: Arr<u16>) -> Str {
    Str::from_units(v.units())
}
pub fn String_copyValueOf(v: Arr<u16>) -> Str {
    Str::from_units(v.units())
}
pub fn String_join(sep: Str, parts: &[Str]) -> Str {
    let mut out = Vec::new();
    for (i, p) in parts.iter().enumerate() {
        if i > 0 {
            out.extend_from_slice(sep.u());
        }
        p.cat(&mut out);
    }
    Str::from_units(out)
}

// ---- the rest of what Java's `==`, try/finally and java.util.Arrays need ------------------------------------------------------

pub fn same_str(a: Str, b: Str) -> bool {
    match (a.0, b.0) {
        (None, None) => true,
        (Some(x), Some(y)) => std::ptr::eq(x.as_ptr(), y.as_ptr()) && x.len() == y.len(),
        _ => false,
    }
}

// try { body } finally { ... }: whatever the body did (it returned, or something was thrown), for the finally to run after it
pub fn catch_all<R>(body: impl FnOnce() -> R) -> Result<R, Box<dyn Any + Send>> {
    std::panic::catch_unwind(std::panic::AssertUnwindSafe(body))
}
pub fn rethrow(payload: Box<dyn Any + Send>) -> ! {
    std::panic::resume_unwind(payload)
}

pub fn arraycopy<T: Copy + 'static>(src: Arr<T>, sp: i32, dst: Arr<T>, dp: i32, n: i32) {
    let (s, d) = (src.nn(), dst.nn());
    if sp < 0 || dp < 0 || n < 0 || sp as i64 + n as i64 > s.cells.len() as i64 || dp as i64 + n as i64 > d.cells.len() as i64 {
        throw_new(
            "java.lang.ArrayIndexOutOfBoundsException",
            Some(format!("arraycopy: last source index {} out of bounds for length {}", sp as i64 + n as i64, s.cells.len())),
        );
    }
    let items: Vec<T> = (0..n as usize).map(|i| s.cells[sp as usize + i].get()).collect();
    for (i, v) in items.into_iter().enumerate() {
        d.cells[dp as usize + i].set(v);
    }
}
pub fn Arrays_toString<T: Cat + Copy + 'static>(a: Arr<T>) -> Str {
    match a {
        None => lit("null"),
        Some(arr) => {
            let mut v: Vec<u16> = vec!['[' as u16];
            for (i, c) in arr.cells.iter().enumerate() {
                if i > 0 {
                    v.extend(", ".encode_utf16());
                }
                c.get().cat(&mut v);
            }
            v.push(']' as u16);
            Str::from_units(v)
        }
    }
}
pub fn Arrays_fill<T: Copy + 'static>(a: Arr<T>, v: T) {
    for c in a.nn().cells.iter() {
        c.set(v);
    }
}
pub fn Arrays_copyOf<T: Copy + Default + 'static>(a: Arr<T>, n: i32) -> Arr<T> {
    if n < 0 {
        throw_new("java.lang.NegativeArraySizeException", Some(n.to_string()));
    }
    let src = a.nn();
    let items: Vec<T> = (0..n as usize).map(|i| src.cells.get(i).map_or(T::default(), |c| c.get())).collect();
    array_of(&items)
}
pub fn Arrays_copyOfRange<T: Copy + Default + 'static>(a: Arr<T>, from: i32, to: i32) -> Arr<T> {
    let src = a.nn();
    if from < 0 || from as usize > src.cells.len() {
        throw_new("java.lang.ArrayIndexOutOfBoundsException", Some(format!("Index {} out of bounds for length {}", from, src.cells.len())));
    }
    if from > to {
        throw_new("java.lang.IllegalArgumentException", Some(format!("{} > {}", from, to)));
    }
    let items: Vec<T> = (from as usize..to as usize).map(|i| src.cells.get(i).map_or(T::default(), |c| c.get())).collect();
    array_of(&items)
}
pub fn Arrays_equals<T: Copy + PartialEq + 'static>(a: Arr<T>, b: Arr<T>) -> bool {
    match (a, b) {
        (None, None) => true,
        (Some(x), Some(y)) => x.cells.len() == y.cells.len() && x.cells.iter().zip(y.cells.iter()).all(|(p, q)| p.get() == q.get()),
        _ => false,
    }
}
pub fn Arrays_sort<T: Copy + PartialOrd + 'static>(a: Arr<T>) {
    let mut items = a.units();
    items.sort_by(|x, y| x.partial_cmp(y).unwrap_or(std::cmp::Ordering::Equal));
    for (c, v) in a.nn().cells.iter().zip(items) {
        c.set(v);
    }
}

// ---- boxed numbers: a java.lang.Integer is an `Option<i32>` (null is None) ---------------------------------------------------------

pub trait Unbox<T> {
    fn unbox(self) -> T;
}
macro_rules! boxed {
    ($($t:ty),*) => { $(
        impl Unbox<$t> for Option<$t> {
            fn unbox(self) -> $t {
                match self {
                    Some(v) => v,
                    None => throw_new("java.lang.NullPointerException", None),
                }
            }
        }
        impl Cat for Option<$t> {
            fn cat(&self, out: &mut Vec<u16>) {
                match self {
                    Some(v) => v.cat(out),
                    None => push_str(out, "null"),
                }
            }
        }
        impl FmtArg for Option<$t> {
            fn fa(self) -> FArg {
                match self {
                    Some(v) => v.fa(),
                    None => FArg::Null,
                }
            }
        }
        impl JEq for Option<$t> {
            fn jeq(&self, o: &Self) -> bool {
                self == o
            }
        }
        impl JHash for Option<$t> {
            fn jhash(&self) -> i32 {
                match self {
                    Some(v) => v.jhash(),
                    None => 0,
                }
            }
        }
        impl JNull for Option<$t> {
            fn null() -> Self {
                None
            }
        }
    )* };
}

// ---- equality, hash codes and "no value" as Java's collections use them ----------------------------------------------------------------

pub trait JEq {
    fn jeq(&self, other: &Self) -> bool;
}
pub trait JHash {
    fn jhash(&self) -> i32;
}
pub trait JNull {
    fn null() -> Self;
}
macro_rules! prim_collection_traits {
    ($($t:ty => $h:expr),*) => { $(
        impl JEq for $t {
            fn jeq(&self, o: &Self) -> bool {
                self == o
            }
        }
        impl JHash for $t {
            fn jhash(&self) -> i32 {
                let f: fn(&$t) -> i32 = $h;
                f(self)
            }
        }
    )* };
}
prim_collection_traits!(
    i8 => |v| *v as i32,
    i16 => |v| *v as i32,
    i32 => |v| *v,
    i64 => |v| (*v ^ ((*v as u64) >> 32) as i64) as i32,
    u16 => |v| *v as i32,
    bool => |v| if *v { 1231 } else { 1237 },
    f64 => |v| { let b = v.to_bits(); (b ^ (b >> 32)) as i32 },
    f32 => |v| v.to_bits() as i32
);
boxed!(i8, i16, i32, i64, u16, bool, f64, f32);
impl JEq for Str {
    fn jeq(&self, o: &Self) -> bool {
        match (self.0, o.0) {
            (None, None) => true,
            (Some(a), Some(b)) => a == b,
            _ => false,
        }
    }
}
impl JHash for Str {
    fn jhash(&self) -> i32 {
        if self.is_null() { 0 } else { self.hashCode() }
    }
}
impl JNull for Str {
    fn null() -> Self {
        Str::NULL
    }
}
impl<T: ?Sized + Object> JEq for Option<&'static T> {
    fn jeq(&self, o: &Self) -> bool {
        match (self, o) {
            (None, None) => true,
            (Some(a), Some(b)) => {
                a.equals(Some(b.as_object()))
            }
            _ => false,
        }
    }
}
impl<T: ?Sized + Object> JHash for Option<&'static T> {
    fn jhash(&self) -> i32 {
        match self {
            Some(o) => o.as_object().hashCode(),
            None => 0,
        }
    }
}
impl<T: ?Sized + Object> JNull for Option<&'static T> {
    fn null() -> Self {
        None
    }
}
// `==` on two boxes: Java makes one box per value from -128 to 127 (and per char up to 127, and for the booleans) and a new one otherwise, so the
// same value is the same box only in that range. (Two names of one big box compare unequal here, which Java says are equal.)
pub trait BoxCache: Copy + PartialEq {
    fn cached(self) -> bool;
}
macro_rules! box_cache_ints {
    ($($t:ty),*) => { $( impl BoxCache for $t { fn cached(self) -> bool { (-128..=127).contains(&(self as i64)) } } )* };
}
box_cache_ints!(i8, i16, i32, i64, u16);
impl BoxCache for bool {
    fn cached(self) -> bool {
        true
    }
}
impl BoxCache for f64 {
    fn cached(self) -> bool {
        false
    }
}
impl BoxCache for f32 {
    fn cached(self) -> bool {
        false
    }
}
pub fn box_equals<T: PartialEq>(a: Option<T>, b: Option<T>) -> bool {
    match a {
        None => throw_new("java.lang.NullPointerException", None),
        Some(x) => b == Some(x),
    }
}
pub fn box_same<T: BoxCache>(a: Option<T>, b: Option<T>) -> bool {
    match (a, b) {
        (None, None) => true,
        (Some(x), Some(y)) => x == y && x.cached(),
        _ => false,
    }
}

pub fn Objects_equals<T: JEq>(a: T, b: T) -> bool {
    a.jeq(&b)
}
pub fn Objects_requireNonNull<T: JEq + JNull>(a: T) -> T {
    if a.jeq(&T::null()) {
        throw_new("java.lang.NullPointerException", None);
    }
    a
}
pub fn Objects_hash(hashes: &[i32]) -> i32 {
    hashes.iter().fold(1i32, |h, e| h.wrapping_mul(31).wrapping_add(*e))
}

// natural order (Comparable): numbers, characters, booleans and Strings
pub trait JOrd {
    fn jcmp(&self, other: &Self) -> i32;
}
macro_rules! ord_prims {
    ($($t:ty),*) => { $(
        impl JOrd for $t {
            fn jcmp(&self, o: &Self) -> i32 {
                (*self > *o) as i32 - (*self < *o) as i32
            }
        }
        impl JOrd for Option<$t> {
            fn jcmp(&self, o: &Self) -> i32 {
                match (self, o) {
                    (Some(a), Some(b)) => a.jcmp(b),
                    _ => throw_new("java.lang.NullPointerException", None),
                }
            }
        }
    )* };
}
ord_prims!(i8, i16, i32, i64, u16, bool, f64, f32);
impl JOrd for Str {
    fn jcmp(&self, o: &Self) -> i32 {
        self.compareTo(*o)
    }
}

// ---- java.util.ArrayList and its iterator ---------------------------------------------------------------------------------------------

pub struct JIter<T: Copy + 'static> {
    items: &'static [T],
    at: Cell<usize>,
}
impl<T: Copy + 'static> JIter<T> {
    pub fn of(items: Vec<T>) -> Ref<JIter<T>> {
        Some(gc_new_internal(JIter { items: gc_slice(&items), at: Cell::new(0) }))
    }
    pub fn hasNext(&'static self) -> bool {
        self.at.get() < self.items.len()
    }
    pub fn next(&'static self) -> T {
        let i = self.at.get();
        match self.items.get(i) {
            Some(v) => {
                self.at.set(i + 1);
                *v
            }
            None => throw_new("java.util.NoSuchElementException", None),
        }
    }
}

pub struct ArrayList<T: Copy + 'static> {
    v: GcVec<T>,
}
fn list_index(i: i32, n: usize) -> usize {
    if i < 0 || i as usize >= n {
        throw_new("java.lang.IndexOutOfBoundsException", Some(format!("Index {} out of bounds for length {}", i, n)));
    }
    i as usize
}
impl<T: Copy + 'static> ArrayList<T> {
    pub fn new() -> Ref<ArrayList<T>> {
        Some(gc_new(ArrayList { v: GcVec::new() }))
    }
    pub fn of(items: Vec<T>) -> Ref<ArrayList<T>> {
        Some(gc_new_internal(ArrayList { v: GcVec::from_slice(&items) }))
    }
    pub fn add(&'static self, x: T) -> bool {
        self.v.push(x);
        true
    }
    pub fn add_at(&'static self, i: i32, x: T) {
        if i < 0 || i as usize > self.v.len() {
            throw_new("java.lang.IndexOutOfBoundsException", Some(format!("Index: {}, Size: {}", i, self.v.len())));
        }
        self.v.insert(i as usize, x);
    }
    pub fn get(&'static self, i: i32) -> T {
        self.v.get(list_index(i, self.v.len()))
    }
    pub fn set(&'static self, i: i32, x: T) -> T {
        let k = list_index(i, self.v.len());
        let old = self.v.get(k);
        self.v.set(k, x);
        old
    }
    pub fn size(&'static self) -> i32 {
        self.v.len() as i32
    }
    pub fn isEmpty(&'static self) -> bool {
        self.v.len() == 0
    }
    pub fn clear(&'static self) {
        self.v.clear();
    }
    pub fn remove_at(&'static self, i: i32) -> T {
        let k = list_index(i, self.v.len());
        self.v.remove(k)
    }
    pub fn iterator(&'static self) -> Ref<JIter<T>> {
        JIter::of(self.v.to_vec())
    }
    pub fn addAll(&'static self, other: Ref<ArrayList<T>>) -> bool {
        let items = other.nn().v.to_vec();
        let changed = !items.is_empty();
        self.v.extend_from_slice(&items);
        changed
    }
    pub fn items(&'static self) -> Vec<T> {
        self.v.to_vec()
    }
}
impl<T: Copy + JOrd + 'static> ArrayList<T> {
    // list.sort(null): the natural order, stable
    pub fn sort_natural(&'static self, comparator: Ref<()>) {
        if comparator.is_some() {
            throw_new("java.lang.UnsupportedOperationException", Some("a comparator".into()));
        }
        let mut items = self.v.to_vec();
        items.sort_by(|a, b| a.jcmp(b).cmp(&0));
        self.v.assign(&items);
    }
}
pub fn Collections_sort<T: Copy + JOrd + 'static>(list: Ref<ArrayList<T>>) {
    list.nn().sort_natural(None);
}
pub fn Collections_reverse<T: Copy + 'static>(list: Ref<ArrayList<T>>) {
    let mut items = list.nn().v.to_vec();
    items.reverse();
    list.nn().v.assign(&items);
}
impl<T: Copy + JEq + 'static> ArrayList<T> {
    pub fn indexOf(&'static self, x: T) -> i32 {
        (0..self.v.len()).find(|i| self.v.get(*i).jeq(&x)).map_or(-1, |i| i as i32)
    }
    pub fn lastIndexOf(&'static self, x: T) -> i32 {
        (0..self.v.len()).rev().find(|i| self.v.get(*i).jeq(&x)).map_or(-1, |i| i as i32)
    }
    pub fn contains(&'static self, x: T) -> bool {
        self.indexOf(x) >= 0
    }
    pub fn remove_obj(&'static self, x: T) -> bool {
        let at = self.indexOf(x);
        if at >= 0 {
            self.v.remove(at as usize);
        }
        at >= 0
    }
}
fn join_items<T: Cat>(items: &[T], open: char, close: char) -> Str {
    let mut v: Vec<u16> = vec![open as u16];
    for (i, x) in items.iter().enumerate() {
        if i > 0 {
            push_str(&mut v, ", ");
        }
        x.cat(&mut v);
    }
    v.push(close as u16);
    Str::from_units(v)
}
impl<T: Copy + Cat + 'static> Object for ArrayList<T> {
    fn toString(&'static self) -> Str {
        join_items(&self.v.to_vec(), '[', ']')
    }
    fn class_name(&'static self) -> &'static str {
        "java.util.ArrayList"
    }
    fn class_chain(&'static self) -> &'static [&'static str] {
        &["java.util.ArrayList", "java.util.List", "java.util.Collection", "java.lang.Iterable", "java.lang.Object"]
    }
    fn as_any(&'static self) -> &'static dyn Any {
        self
    }
    fn as_object(&'static self) -> &'static dyn Object {
        self
    }
}

// ---- java.util.HashMap and HashSet, iterating in the order Java's do ----------------------------------------------------------------------
//
// Java walks its table bucket by bucket (a key's bucket is its spread hash masked by the table size, the table doubling when it is three
// quarters full and never shrinking), each bucket in the order its keys came in. The entries are kept in the order they came in, and
// that walk is made when somebody asks for the order. Lookup is its own open-addressing table over the entries (all of it in the collected heap).

struct MapInner<K: Copy + 'static, V: Copy + 'static> {
    slots: GcVec<Option<(K, V, i32)>>,   // in the order the keys were first put (a removed key leaves a hole)
    index: GcVec<u32>,                   // 0: empty, 1: removed, n + 2: the entry in slot n
    cap: Cell<usize>,                    // Java's table size
    live: Cell<usize>,
    used: Cell<usize>,                   // index cells that are not empty (removed ones too)
}
fn spread(h: i32) -> usize {
    let h = h as u32;
    (h ^ (h >> 16)) as usize
}
impl<K: Copy + JEq + JHash + 'static, V: Copy + 'static> MapInner<K, V> {
    fn new() -> Self {
        MapInner { slots: GcVec::new(), index: GcVec::new(), cap: Cell::new(16), live: Cell::new(0), used: Cell::new(0) }
    }
    fn find(&self, k: K, h: i32) -> Option<(usize, usize)> {
        let n = self.index.len();
        if n == 0 {
            return None;
        }
        let mask = n - 1;
        let mut pos = spread(h) & mask;
        loop {
            let e = self.index.get(pos);
            if e == 0 {
                return None;
            }
            if e >= 2 {
                if let Some((sk, _, sh)) = self.slots.get(e as usize - 2) {
                    if sh == h && sk.jeq(&k) {
                        return Some((pos, e as usize - 2));
                    }
                }
            }
            pos = (pos + 1) & mask;
        }
    }
    // the entries again, without the holes, in a table big enough for them
    fn rebuild(&self) {
        let entries: Vec<(K, V, i32)> = self.slots.to_vec().into_iter().flatten().collect();
        let mut size = 16;
        while size * 3 < (entries.len() + 1) * 4 * 2 {
            size *= 2;
        }
        self.slots.clear();
        self.index.clear();
        self.index.resize(size, 0);
        for e in entries {
            self.slots.push(Some(e));
            self.place(e.2, self.slots.len() - 1);
        }
        self.used.set(self.slots.len());
    }
    fn place(&self, h: i32, slot: usize) {
        let mask = self.index.len() - 1;
        let mut pos = spread(h) & mask;
        while self.index.get(pos) >= 2 {
            pos = (pos + 1) & mask;
        }
        self.index.set(pos, slot as u32 + 2);
    }
    fn put(&self, k: K, v: V) -> Option<V> {
        let h = k.jhash();
        if let Some((_, slot)) = self.find(k, h) {
            let (sk, old, sh) = self.slots.get(slot).unwrap();
            self.slots.set(slot, Some((sk, v, sh)));
            return Some(old);
        }
        if self.index.len() == 0 || (self.used.get() + 1) * 4 > self.index.len() * 3 || self.slots.len() > self.live.get() * 2 + 16 {
            self.rebuild();
        }
        self.slots.push(Some((k, v, h)));
        let mask = self.index.len() - 1;
        let mut pos = spread(h) & mask;
        while self.index.get(pos) >= 2 {
            pos = (pos + 1) & mask;
        }
        if self.index.get(pos) == 0 {
            self.used.set(self.used.get() + 1);
        }
        self.index.set(pos, self.slots.len() as u32 + 1);
        self.live.set(self.live.get() + 1);
        if self.live.get() * 4 > self.cap.get() * 3 {
            self.cap.set(self.cap.get() * 2);
        }
        None
    }
    fn get(&self, k: K) -> Option<V> {
        self.find(k, k.jhash()).map(|(_, slot)| self.slots.get(slot).unwrap().1)
    }
    fn remove(&self, k: K) -> Option<V> {
        let (pos, slot) = self.find(k, k.jhash())?;
        self.index.set(pos, 1);
        self.live.set(self.live.get() - 1);
        let old = self.slots.get(slot).unwrap().1;
        self.slots.set(slot, None);
        Some(old)
    }
    fn clear(&self) {
        self.slots.clear();
        self.index.clear();
        self.live.set(0);
        self.used.set(0);
    }
    // the entries in the order a Java HashMap yields them (or, linked, as they came in)
    fn ordered(&self, linked: bool) -> Vec<(K, V)> {
        let mut live: Vec<(K, V, i32)> = self.slots.to_vec().into_iter().flatten().collect();
        if !linked {
            let mask = self.cap.get() - 1;
            live.sort_by_key(|e| spread(e.2) & mask);
        }
        live.into_iter().map(|e| (e.0, e.1)).collect()
    }
}

pub struct MapEntry<K: Copy + 'static, V: Copy + 'static> {
    k: K,
    v: V,
}
impl<K: Copy + 'static, V: Copy + 'static> MapEntry<K, V> {
    pub fn getKey(&'static self) -> K {
        self.k
    }
    pub fn getValue(&'static self) -> V {
        self.v
    }
}
impl<K: Copy + Cat + 'static, V: Copy + Cat + 'static> Object for MapEntry<K, V> {
    fn toString(&'static self) -> Str {
        cat!(self.k, lit("="), self.v)
    }
    fn class_name(&'static self) -> &'static str {
        "java.util.HashMap$Node"
    }
    fn class_chain(&'static self) -> &'static [&'static str] {
        &["java.util.HashMap$Node", "java.util.Map.Entry", "java.lang.Object"]
    }
    fn as_any(&'static self) -> &'static dyn Any {
        self
    }
    fn as_object(&'static self) -> &'static dyn Object {
        self
    }
}

pub struct HashMap<K: Copy + 'static, V: Copy + 'static> {
    m: MapInner<K, V>,
    linked: bool,
}
impl<K: Copy + JEq + JHash + 'static, V: Copy + JNull + 'static> HashMap<K, V> {
    pub fn new() -> Ref<HashMap<K, V>> {
        Some(gc_new(HashMap { m: MapInner::new(), linked: false }))
    }
    pub fn new_linked() -> Ref<HashMap<K, V>> {
        Some(gc_new(HashMap { m: MapInner::new(), linked: true }))
    }
    pub fn put(&'static self, k: K, v: V) -> V {
        self.m.put(k, v).unwrap_or_else(V::null)
    }
    pub fn get(&'static self, k: K) -> V {
        self.m.get(k).unwrap_or_else(V::null)
    }
    pub fn getOrDefault(&'static self, k: K, d: V) -> V {
        self.m.get(k).unwrap_or(d)
    }
    pub fn containsKey(&'static self, k: K) -> bool {
        self.m.get(k).is_some()
    }
    pub fn remove(&'static self, k: K) -> V {
        self.m.remove(k).unwrap_or_else(V::null)
    }
    pub fn putIfAbsent(&'static self, k: K, v: V) -> V {
        match self.m.get(k) {
            Some(old) => old,
            None => {
                self.m.put(k, v);
                V::null()
            }
        }
    }
    pub fn size(&'static self) -> i32 {
        self.m.live.get() as i32
    }
    pub fn isEmpty(&'static self) -> bool {
        self.m.live.get() == 0
    }
    pub fn clear(&'static self) {
        self.m.clear();
    }
    pub fn keySet(&'static self) -> Ref<HashSet<K>> {
        let keys: Vec<K> = self.m.ordered(self.linked).into_iter().map(|e| e.0).collect();
        HashSet::from_ordered(keys)
    }
    pub fn values(&'static self) -> Ref<ArrayList<V>> {
        ArrayList::of(self.m.ordered(self.linked).into_iter().map(|e| e.1).collect())
    }
}
impl<K: Copy + JEq + JHash + Cat + 'static, V: Copy + JNull + Cat + 'static> HashMap<K, V> {
    pub fn entrySet(&'static self) -> Ref<HashSet<Ref<MapEntry<K, V>>>> {
        let entries: Vec<Ref<MapEntry<K, V>>> =
            self.m.ordered(self.linked).into_iter().map(|(k, v)| -> Ref<MapEntry<K, V>> { Some(gc_new_internal(MapEntry { k, v })) }).collect();
        HashSet::from_ordered(entries)
    }
}
impl<K: Copy + JEq + JHash + 'static, V: Copy + JEq + 'static> HashMap<K, V> {
    pub fn containsValue(&'static self, v: V) -> bool {
        self.m.slots.to_vec().into_iter().flatten().any(|e| e.1.jeq(&v))
    }
}
impl<K: Copy + JEq + JHash + Cat + 'static, V: Copy + Cat + 'static> Object for HashMap<K, V> {
    fn toString(&'static self) -> Str {
        let mut v: Vec<u16> = vec!['{' as u16];
        for (i, (k, val)) in self.m.ordered(self.linked).into_iter().enumerate() {
            if i > 0 {
                push_str(&mut v, ", ");
            }
            k.cat(&mut v);
            v.push('=' as u16);
            val.cat(&mut v);
        }
        v.push('}' as u16);
        Str::from_units(v)
    }
    fn class_name(&'static self) -> &'static str {
        "java.util.HashMap"
    }
    fn class_chain(&'static self) -> &'static [&'static str] {
        &["java.util.HashMap", "java.util.Map", "java.lang.Object"]
    }
    fn as_any(&'static self) -> &'static dyn Any {
        self
    }
    fn as_object(&'static self) -> &'static dyn Object {
        self
    }
}

pub struct HashSet<T: Copy + 'static> {
    m: MapInner<T, ()>,
    linked: bool,
}
impl<T: Copy + JEq + JHash + 'static> HashSet<T> {
    pub fn new() -> Ref<HashSet<T>> {
        Some(gc_new(HashSet { m: MapInner::new(), linked: false }))
    }
    pub fn new_linked() -> Ref<HashSet<T>> {
        Some(gc_new(HashSet { m: MapInner::new(), linked: true }))
    }
    // a set that keeps the order of what it was made from (a view of a map's keys)
    pub fn from_ordered(items: Vec<T>) -> Ref<HashSet<T>> {
        let set = gc_new_internal(HashSet { m: MapInner::new(), linked: true });
        for x in items {
            set.m.put(x, ());
        }
        Some(set)
    }
    pub fn add(&'static self, x: T) -> bool {
        self.m.put(x, ()).is_none()
    }
    pub fn contains(&'static self, x: T) -> bool {
        self.m.get(x).is_some()
    }
    pub fn remove_obj(&'static self, x: T) -> bool {
        self.m.remove(x).is_some()
    }
    pub fn size(&'static self) -> i32 {
        self.m.live.get() as i32
    }
    pub fn isEmpty(&'static self) -> bool {
        self.m.live.get() == 0
    }
    pub fn clear(&'static self) {
        self.m.clear();
    }
    pub fn iterator(&'static self) -> Ref<JIter<T>> {
        JIter::of(self.m.ordered(self.linked).into_iter().map(|e| e.0).collect())
    }
}
impl<T: Copy + JEq + JHash + Cat + 'static> Object for HashSet<T> {
    fn toString(&'static self) -> Str {
        let items: Vec<T> = self.m.ordered(self.linked).into_iter().map(|e| e.0).collect();
        join_items(&items, '[', ']')
    }
    fn class_name(&'static self) -> &'static str {
        "java.util.HashSet"
    }
    fn class_chain(&'static self) -> &'static [&'static str] {
        &["java.util.HashSet", "java.util.Set", "java.util.Collection", "java.lang.Iterable", "java.lang.Object"]
    }
    fn as_any(&'static self) -> &'static dyn Any {
        self
    }
    fn as_object(&'static self) -> &'static dyn Object {
        self
    }
}
