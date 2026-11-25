package jrt

// The library classes of java.util the translations use: ArrayList, HashMap, LinkedHashMap, HashSet, LinkedHashSet, their iterators, and Arrays.

import (
	"cmp"
	"slices"
	"strconv"
	"time"
)

func nowNanos() int64 { return time.Now().UnixNano() }

// ---- the interfaces ---------------------------------------------------------------------------------------------------------------

type Iterator interface {
	HasNext() bool
	Next() any
	Remove()
}

type Collection interface {
	Size() int32
	IsEmpty() bool
	Contains(o any) bool
	Add(o any) bool
	Remove(o any) bool
	Clear()
	AddAll(c Collection) bool
	Iterator() Iterator
	ToSlice() []any
	ToString() String
	ContainsAll(c Collection) bool
}

type List interface {
	Collection
	Get(i int32) any
	Set(i int32, o any) any
	AddAt(i int32, o any)
	RemoveAt(i int32) any
	IndexOf(o any) int32
	LastIndexOf(o any) int32
	Sort(cmp func(a, b any) int32)
}

type Set interface {
	Collection
	isSet()
}

type Map interface {
	Put(k, v any) any
	Get(k any) any
	GetOrDefault(k, d any) any
	ContainsKey(k any) bool
	ContainsValue(v any) bool
	Remove(k any) any
	PutIfAbsent(k, v any) any
	Size() int32
	IsEmpty() bool
	Clear()
	KeySet() Set
	Values() Collection
	EntrySet() Set
	ToString() String
	PutAll(m Map)
}

type Entry interface {
	GetKey() any
	GetValue() any
	SetValue(v any) any
}

type sliceIter struct {
	items  []any
	at     int
	remove func(o any)
}

func (it *sliceIter) HasNext() bool { return it.at < len(it.items) }
func (it *sliceIter) Next() any {
	if it.at >= len(it.items) {
		ThrowMsg("java.util.NoSuchElementException", "")
		panic("unreachable")
	}
	it.at++
	return it.items[it.at-1]
}
func (it *sliceIter) Remove() {
	if it.remove != nil && it.at > 0 {
		it.remove(it.items[it.at-1])
	}
}

func collectionString(items []any, self any) String {
	u := []uint16{'['}
	for i, o := range items {
		if i > 0 {
			u = append(u, ',', ' ')
		}
		if o == self && o != nil {
			u = append(u, S("(this Collection)").Chars()...)
		} else {
			u = append(u, ToStr(o).Chars()...)
		}
	}
	return FromUnits(append(u, ']'))
}

// ---- ArrayList -----------------------------------------------------------------------------------------------------------------------

type ArrayList struct{ items []any }

func NewArrayList() *ArrayList { return &ArrayList{} }
func NewArrayListCap(n int32) *ArrayList {
	if n < 0 {
		ThrowMsg("java.lang.IllegalArgumentException", "Illegal Capacity: "+strconv.Itoa(int(n)))
	}
	return &ArrayList{make([]any, 0, n)}
}
func NewArrayListOf(c Collection) *ArrayList { return &ArrayList{append([]any{}, c.ToSlice()...)} }

// ListOf is Arrays.asList and List.of.
func ListOf(items ...any) *ArrayList { return &ArrayList{append([]any{}, items...)} }

func (l *ArrayList) Size() int32          { return int32(len(l.items)) }
func (l *ArrayList) IsEmpty() bool        { return len(l.items) == 0 }
func (l *ArrayList) ToSlice() []any       { return l.items }
func (l *ArrayList) ToString() String     { return collectionString(l.items, l) }
func (l *ArrayList) Clear()               { l.items = nil }
func (l *ArrayList) Add(o any) bool       { l.items = append(l.items, o); return true }
func (l *ArrayList) isList()              {}
func (l *ArrayList) check(i int32, n int) {
	if i < 0 || int(i) >= n {
		ThrowMsg("java.lang.IndexOutOfBoundsException", "Index "+strconv.Itoa(int(i))+" out of bounds for length "+strconv.Itoa(len(l.items)))
	}
}
func (l *ArrayList) Get(i int32) any { l.check(i, len(l.items)); return l.items[i] }
func (l *ArrayList) Set(i int32, o any) any {
	l.check(i, len(l.items))
	old := l.items[i]
	l.items[i] = o
	return old
}
func (l *ArrayList) AddAt(i int32, o any) {
	if i < 0 || int(i) > len(l.items) {
		ThrowMsg("java.lang.IndexOutOfBoundsException", "Index: "+strconv.Itoa(int(i))+", Size: "+strconv.Itoa(len(l.items)))
	}
	l.items = append(l.items, nil)
	copy(l.items[i+1:], l.items[i:])
	l.items[i] = o
}
func (l *ArrayList) RemoveAt(i int32) any {
	l.check(i, len(l.items))
	old := l.items[i]
	l.items = append(l.items[:i], l.items[i+1:]...)
	return old
}
func (l *ArrayList) IndexOf(o any) int32 {
	for i, x := range l.items {
		if ObjEquals(o, x) {
			return int32(i)
		}
	}
	return -1
}
func (l *ArrayList) LastIndexOf(o any) int32 {
	for i := len(l.items) - 1; i >= 0; i-- {
		if ObjEquals(o, l.items[i]) {
			return int32(i)
		}
	}
	return -1
}
func (l *ArrayList) Contains(o any) bool { return l.IndexOf(o) >= 0 }
func (l *ArrayList) Remove(o any) bool {
	if i := l.IndexOf(o); i >= 0 {
		l.RemoveAt(i)
		return true
	}
	return false
}
func (l *ArrayList) AddAll(c Collection) bool {
	l.items = append(l.items, c.ToSlice()...)
	return c.Size() > 0
}
func (l *ArrayList) ContainsAll(c Collection) bool {
	for _, o := range c.ToSlice() {
		if !l.Contains(o) {
			return false
		}
	}
	return true
}
func (l *ArrayList) Iterator() Iterator {
	return &sliceIter{items: append([]any{}, l.items...), remove: func(o any) { l.Remove(o) }}
}
func (l *ArrayList) Sort(c func(a, b any) int32) {
	slices.SortStableFunc(l.items, func(a, b any) int { return int(c(a, b)) })
}
func (l *ArrayList) Equals(o any) bool {
	m, ok := o.(List)
	if !ok || m.Size() != l.Size() {
		return false
	}
	for i, x := range m.ToSlice() {
		if !ObjEquals(l.items[i], x) {
			return false
		}
	}
	return true
}
func (l *ArrayList) HashCode() int32 {
	h := int32(1)
	for _, x := range l.items {
		h = 31*h + hashOrZero(x)
	}
	return h
}

// ---- HashMap ---------------------------------------------------------------------------------------------------------------------------

type hmNode struct {
	hash          int32
	key, val      any
	next          *hmNode
	before, after *hmNode
}

func (n *hmNode) GetKey() any         { return n.key }
func (n *hmNode) GetValue() any       { return n.val }
func (n *hmNode) SetValue(v any) any  { o := n.val; n.val = v; return o }
func (n *hmNode) ToString() String    { return Cat(n.key, S("="), n.val) }
func (n *hmNode) Equals(o any) bool {
	e, ok := o.(Entry)
	return ok && ObjEquals(n.key, e.GetKey()) && ObjEquals(n.val, e.GetValue())
}
func (n *hmNode) HashCode() int32 { return hashOrZero(n.key) ^ hashOrZero(n.val) }

// HashMap keeps Java's table and bucket order, so iteration is in Java's order; a linked one iterates in insertion order.
type HashMap struct {
	table      []*hmNode
	size       int32
	threshold  int32
	linked     bool
	head, tail *hmNode
}

func NewHashMap() *HashMap       { return &HashMap{} }
func NewLinkedHashMap() *HashMap { return &HashMap{linked: true} }
func NewHashMapCap(n int32) *HashMap {
	if n < 0 {
		ThrowMsg("java.lang.IllegalArgumentException", "Illegal initial capacity: "+strconv.Itoa(int(n)))
	}
	return &HashMap{threshold: tableSizeFor(n)}
}
func NewHashMapOf(m Map) *HashMap {
	h := &HashMap{}
	h.PutAll(m)
	return h
}
func NewLinkedHashMapOf(m Map) *HashMap {
	h := &HashMap{linked: true}
	h.PutAll(m)
	return h
}
func tableSizeFor(c int32) int32 {
	n := int32(1)
	for n < c {
		n <<= 1
	}
	return n
}
func spread(k any) int32 {
	h := hashOrZero(k)
	return h ^ int32(uint32(h)>>16)
}
func (m *HashMap) find(k any) *hmNode {
	if len(m.table) == 0 {
		return nil
	}
	h := spread(k)
	for n := m.table[int(h)&(len(m.table)-1)]; n != nil; n = n.next {
		if n.hash == h && (n.key == k || (k != nil && n.key != nil && Equals(k, n.key))) {
			return n
		}
	}
	return nil
}
func (m *HashMap) resize() {
	old := m.table
	var newCap, newThr int32
	if len(old) > 0 {
		newCap = int32(len(old)) << 1
		if len(old) >= 16 {
			newThr = m.threshold << 1
		} else {
			newThr = newCap * 3 / 4
		}
	} else if m.threshold > 0 {
		newCap = m.threshold
		newThr = newCap * 3 / 4
	} else {
		newCap, newThr = 16, 12
	}
	m.threshold = newThr
	m.table = make([]*hmNode, newCap)
	for j, e := range old {
		var loHead, loTail, hiHead, hiTail *hmNode
		for e != nil {
			next := e.next
			e.next = nil
			if int(e.hash)&len(old) == 0 {
				if loTail == nil {
					loHead = e
				} else {
					loTail.next = e
				}
				loTail = e
			} else {
				if hiTail == nil {
					hiHead = e
				} else {
					hiTail.next = e
				}
				hiTail = e
			}
			e = next
		}
		m.table[j] = loHead
		m.table[j+len(old)] = hiHead
	}
}
func (m *HashMap) Put(k, v any) any {
	if n := m.find(k); n != nil {
		old := n.val
		n.val = v
		return old
	}
	if len(m.table) == 0 {
		m.resize()
	}
	h := spread(k)
	nn := &hmNode{hash: h, key: k, val: v}
	i := int(h) & (len(m.table) - 1)
	if m.table[i] == nil {
		m.table[i] = nn
	} else {
		t := m.table[i]
		for t.next != nil {
			t = t.next
		}
		t.next = nn
	}
	if m.tail == nil {
		m.head = nn
	} else {
		m.tail.after = nn
		nn.before = m.tail
	}
	m.tail = nn
	m.size++
	if m.size > m.threshold {
		m.resize()
	}
	return nil
}
func (m *HashMap) Get(k any) any {
	if n := m.find(k); n != nil {
		return n.val
	}
	return nil
}
func (m *HashMap) GetOrDefault(k, d any) any {
	if n := m.find(k); n != nil {
		return n.val
	}
	return d
}
func (m *HashMap) ContainsKey(k any) bool { return m.find(k) != nil }
func (m *HashMap) ContainsValue(v any) bool {
	for n := m.head; n != nil; n = n.after {
		if ObjEquals(v, n.val) {
			return true
		}
	}
	return false
}
func (m *HashMap) PutIfAbsent(k, v any) any {
	if n := m.find(k); n != nil {
		if n.val == nil {
			n.val = v
		}
		return n.val
	}
	m.Put(k, v)
	return nil
}
func (m *HashMap) Remove(k any) any {
	n := m.find(k)
	if n == nil {
		return nil
	}
	i := int(n.hash) & (len(m.table) - 1)
	if m.table[i] == n {
		m.table[i] = n.next
	} else {
		p := m.table[i]
		for p.next != n {
			p = p.next
		}
		p.next = n.next
	}
	if n.before == nil {
		m.head = n.after
	} else {
		n.before.after = n.after
	}
	if n.after == nil {
		m.tail = n.before
	} else {
		n.after.before = n.before
	}
	m.size--
	return n.val
}
func (m *HashMap) Size() int32   { return m.size }
func (m *HashMap) IsEmpty() bool { return m.size == 0 }
func (m *HashMap) Clear() {
	for i := range m.table {
		m.table[i] = nil
	}
	m.size, m.head, m.tail = 0, nil, nil
}

// nodes are the entries in iteration order.
func (m *HashMap) nodes() []*hmNode {
	out := make([]*hmNode, 0, m.size)
	if m.linked {
		for n := m.head; n != nil; n = n.after {
			out = append(out, n)
		}
		return out
	}
	for _, b := range m.table {
		for n := b; n != nil; n = n.next {
			out = append(out, n)
		}
	}
	return out
}
func (m *HashMap) PutAll(o Map) {
	for _, n := range o.EntrySet().ToSlice() {
		e := n.(Entry)
		m.Put(e.GetKey(), e.GetValue())
	}
}
func (m *HashMap) ToString() String {
	u := []uint16{'{'}
	for i, n := range m.nodes() {
		if i > 0 {
			u = append(u, ',', ' ')
		}
		u = append(u, ToStr(n.key).Chars()...)
		u = append(u, '=')
		u = append(u, ToStr(n.val).Chars()...)
	}
	return FromUnits(append(u, '}'))
}
func (m *HashMap) Equals(o any) bool {
	x, ok := o.(Map)
	if !ok || x.Size() != m.size {
		return false
	}
	for _, n := range m.nodes() {
		if !x.ContainsKey(n.key) || !ObjEquals(n.val, x.Get(n.key)) {
			return false
		}
	}
	return true
}
func (m *HashMap) HashCode() int32 {
	var h int32
	for _, n := range m.nodes() {
		h += n.HashCode()
	}
	return h
}

// the views of a map: keySet, values, entrySet
type mapView struct {
	m    *HashMap
	kind int // 0 keys, 1 values, 2 entries
}

func (v *mapView) isSet()        {}
func (v *mapView) Size() int32   { return v.m.size }
func (v *mapView) IsEmpty() bool { return v.m.size == 0 }
func (v *mapView) ToSlice() []any {
	out := make([]any, 0, v.m.size)
	for _, n := range v.m.nodes() {
		switch v.kind {
		case 0:
			out = append(out, n.key)
		case 1:
			out = append(out, n.val)
		default:
			out = append(out, n)
		}
	}
	return out
}
func (v *mapView) Contains(o any) bool {
	if v.kind == 0 {
		return v.m.ContainsKey(o)
	}
	for _, x := range v.ToSlice() {
		if ObjEquals(o, x) {
			return true
		}
	}
	return false
}
func (v *mapView) Add(o any) bool { ThrowMsg("java.lang.UnsupportedOperationException", ""); return false }
func (v *mapView) Remove(o any) bool {
	if v.kind == 0 && v.m.ContainsKey(o) {
		v.m.Remove(o)
		return true
	}
	return false
}
func (v *mapView) Clear()                  { v.m.Clear() }
func (v *mapView) AddAll(c Collection) bool { return v.Add(nil) }
func (v *mapView) ContainsAll(c Collection) bool {
	for _, o := range c.ToSlice() {
		if !v.Contains(o) {
			return false
		}
	}
	return true
}
func (v *mapView) ToString() String { return collectionString(v.ToSlice(), v) }
func (v *mapView) Iterator() Iterator {
	return &sliceIter{items: v.ToSlice(), remove: func(o any) {
		if v.kind == 0 {
			v.m.Remove(o)
		} else if n, ok := o.(*hmNode); ok {
			v.m.Remove(n.key)
		}
	}}
}
func (m *HashMap) KeySet() Set        { return &mapView{m, 0} }
func (m *HashMap) Values() Collection { return &mapView{m, 1} }
func (m *HashMap) EntrySet() Set      { return &mapView{m, 2} }

// ---- HashSet --------------------------------------------------------------------------------------------------------------------------

type HashSet struct{ m *HashMap }

func NewHashSet() *HashSet       { return &HashSet{NewHashMap()} }
func NewLinkedHashSet() *HashSet { return &HashSet{NewLinkedHashMap()} }
func NewHashSetCap(n int32) *HashSet { return &HashSet{NewHashMapCap(n)} }
func NewHashSetOf(c Collection) *HashSet {
	n := int32(float32(c.Size())/0.75) + 1
	if n < 16 {
		n = 16
	}
	s := &HashSet{NewHashMapCap(n)}
	s.AddAll(c)
	return s
}
func NewLinkedHashSetOf(c Collection) *HashSet {
	s := NewLinkedHashSet()
	s.AddAll(c)
	return s
}
func (s *HashSet) isSet()        {}
func (s *HashSet) Size() int32   { return s.m.size }
func (s *HashSet) IsEmpty() bool { return s.m.size == 0 }
func (s *HashSet) Add(o any) bool {
	if s.m.find(o) != nil {
		return false
	}
	s.m.Put(o, true)
	return true
}
func (s *HashSet) Contains(o any) bool { return s.m.find(o) != nil }
func (s *HashSet) Remove(o any) bool {
	if s.m.find(o) == nil {
		return false
	}
	s.m.Remove(o)
	return true
}
func (s *HashSet) Clear() { s.m.Clear() }
func (s *HashSet) AddAll(c Collection) bool {
	changed := false
	for _, o := range c.ToSlice() {
		if s.Add(o) {
			changed = true
		}
	}
	return changed
}
func (s *HashSet) ContainsAll(c Collection) bool {
	for _, o := range c.ToSlice() {
		if !s.Contains(o) {
			return false
		}
	}
	return true
}
func (s *HashSet) ToSlice() []any {
	out := make([]any, 0, s.m.size)
	for _, n := range s.m.nodes() {
		out = append(out, n.key)
	}
	return out
}
func (s *HashSet) ToString() String { return collectionString(s.ToSlice(), s) }
func (s *HashSet) Iterator() Iterator {
	return &sliceIter{items: s.ToSlice(), remove: func(o any) { s.Remove(o) }}
}
func (s *HashSet) Equals(o any) bool {
	x, ok := o.(Set)
	return ok && x.Size() == s.Size() && s.ContainsAll(x)
}
func (s *HashSet) HashCode() int32 {
	var h int32
	for _, o := range s.ToSlice() {
		h += hashOrZero(o)
	}
	return h
}

// ---- Arrays and System.arraycopy -------------------------------------------------------------------------------------------------

func ArraysToString[T any](a []T) String {
	if a == nil {
		return S("null")
	}
	items := make([]any, len(a))
	for i, x := range a {
		items[i] = x
	}
	return collectionString(items, nil)
}

func ArraysFill[T any](a []T, v T) {
	for i := range a {
		a[i] = v
	}
}
func ArraysCopyOf[T any](a []T, n int32) []T {
	if n < 0 {
		ThrowMsg("java.lang.NegativeArraySizeException", strconv.Itoa(int(n)))
	}
	out := make([]T, n)
	copy(out, a)
	return out
}
func ArraysCopyOfRange[T any](a []T, from, to int32) []T {
	if from > to {
		ThrowMsg("java.lang.IllegalArgumentException", strconv.Itoa(int(from))+" > "+strconv.Itoa(int(to)))
	}
	if from < 0 || int(from) > len(a) {
		ThrowMsg("java.lang.ArrayIndexOutOfBoundsException", "Array index out of range: "+strconv.Itoa(int(from)))
	}
	out := make([]T, to-from)
	copy(out, a[from:])
	return out
}
func ArraysEquals[T comparable](a, b []T) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	return slices.Equal(a, b)
}
func ArraysSort[T cmp.Ordered](a []T) { slices.Sort(a) }
func ArraysSortStrings(a []String) {
	slices.SortStableFunc(a, func(x, y String) int { return int(x.CompareTo(y)) })
}
func ArraysAsList[T any](a []T) List {
	items := make([]any, len(a))
	for i, x := range a {
		items[i] = x
	}
	return &ArrayList{items}
}

// ArrayCopy is System.arraycopy.
func ArrayCopy[T any](src []T, sp int32, dst []T, dp int32, n int32) {
	if src == nil || dst == nil {
		panic(npe())
	}
	if sp < 0 || dp < 0 || n < 0 || int(sp+n) > len(src) || int(dp+n) > len(dst) {
		ThrowMsg("java.lang.ArrayIndexOutOfBoundsException", "arraycopy: last source index "+strconv.Itoa(int(sp+n))+" out of bounds for length "+strconv.Itoa(len(src)))
	}
	copy(dst[dp:dp+n], src[sp:sp+n])
}

// StringJoin is String.join(sep, parts...).
func StringJoin(sep String, parts []String) String {
	var u []uint16
	for i, p := range parts {
		if i > 0 {
			u = append(u, sep.Chars()...)
		}
		u = append(u, ToStr(p).Chars()...)
	}
	return FromUnits(u)
}

// StringJoinOf is String.join(sep, collection).
func StringJoinOf(sep String, c Collection) String {
	var u []uint16
	for i, p := range c.ToSlice() {
		if i > 0 {
			u = append(u, sep.Chars()...)
		}
		u = append(u, ToStr(p).Chars()...)
	}
	return FromUnits(u)
}

func StringValueOfChars(c []uint16) String { return FromUnits(append([]uint16{}, c...)) }

// ---- small helpers the translations call -----------------------------------------------------------------------------------------

// ToStringOf is o.toString() (a null receiver is a NullPointerException).
func ToStringOf(o any) String {
	if o == nil {
		panic(npe())
	}
	return ToStr(o)
}

// BAnd and BOr are Java's non-short-circuit & and | on booleans.
func BAnd(a, b bool) bool { return a && b }
func BOr(a, b bool) bool  { return a || b }

// Zero32 and Zero64 are the divisors a literal zero stands for (Go refuses to compile a division by a constant zero).
var Zero32 int32
var Zero64 int64

// NewThrowableMC is new E(message, cause).
func NewThrowableMC(class string, msg String, cause Throwable) Throwable {
	t := &ThrowableBase{}
	t.Init(t, class, msg, cause)
	return t
}

// NewArr2 and NewArr3 are new T[a][b] and new T[a][b][c].
func NewArr2[T any](a, b int32) [][]T {
	out := make([][]T, a)
	for i := range out {
		out[i] = make([]T, b)
	}
	return out
}
func NewArr3[T any](a, b, c int32) [][][]T {
	out := make([][][]T, a)
	for i := range out {
		out[i] = NewArr2[T](b, c)
	}
	return out
}
