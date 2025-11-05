Alright. You want the microscope. No metaphors. No cute stories. **Real mechanics of the compiler pipeline** for templates in C++.

We're going to dissect the exact moment the compiler shifts from
“template blueprint” to **actual generated machine code**.

The specific target:

> Two-phase compilation: definition-time vs instantiation-time.

Let's expand this to the bones.

---

## **Phase 1: Template Definition Time (Parsing + partial semantic analysis)**

When the compiler sees a template definition, it does **not** generate code.

It performs:

### ✅ **Syntax checking**

It ensures the template is grammatically valid C++:

```cpp
template<typename T>
T add(T a, T b) { return a + b; }
```

* braces correct
* keywords valid
* expressions form valid syntax

No type substitution yet.

---

### ✅ **Type checking for non-dependent constructs**

"Non-dependent" means: the compiler can fully know what it refers to **without knowing T**.

Example:

```cpp
template <typename T>
void foo() {
    int x = 0;    // OK: type known
    ++x;          // OK: operator ++ on int known
}
```

These are checked **now**.

---

### ✅ **Unqualified non-dependent name lookup happens now**

If a name doesn’t depend on T, it must be valid now:

```cpp
int g();

template<typename T>
void f() { g(); }   // g must exist at template DEFINITION time
```

If `g()` is not declared before the template definition, this is an error **even if not instantiated**.

---

### ❌ **No checking of dependent expressions**

Dependent = depends on a template parameter.

```cpp
template <typename T>
void foo(T a) {
    a.bar();     // dependent expression -> NOT checked now
    a + 5;       // operator+ not checked for T now
}
```

Compiler marks them as “deferred semantic analysis” to handle later.

It basically says:

> I’ll only check this when I know what T is.

---

## **Phase 2: Template Instantiation Time (Substitution + full semantic analysis + codegen)**

Triggered when the template is **used** with a concrete type.

### ✔ Substitution

Compiler creates a **specialized copy** of the template structure where every `T` becomes `int`, `double`, etc.

```cpp
foo<int>(10);   // T = int
foo<MyType>(obj);
```

---

### ✔ Dependent name resolution (delayed lookup)

This is when:

* `T::member`
* `T::type`
* `a + b` where `a` is T

get checked.

If invalid: **compile error here**, not earlier.

Example:

```cpp
struct NoPlus {};

foo(NoPlus{}); // NOW compiler checks a + 5 -> error
```

---

### ✔ Full semantic checking

Like any normal function or class.
Every expression must type-check now.

---

### ✔ Machine code emission⁩ (or inline instantiation)

After the type checks succeed, the compiler generates code.

This is why **multiple file instantiations lead to duplicate code** unless optimized by the compiler (COMDAT folding, `extern template`, etc).

---

## Why C++ uses two-phase lookup

Because templates let you write code that **is not required to be valid for all T**.
They defer checks until the type is known.

Without this, templates would be useless for generic programming.

---

## Visual timeline

```
(Template seen in translation unit)
┌────────────────────────┐
| Phase 1: Definition    |
| - parse syntax         |
| - check non-dependent  |
| - bind non-dependent names
| - DO NOT check code using T
└────────────────────────┘

(Template called with type)
           ↓
┌────────────────────────┐
| Phase 2: Instantiation |
| - substitute T         |
| - resolve dependent names
| - type-check everything|
| - generate real code   |
└────────────────────────┘
```

---

## Concrete Example to Show the Split

```cpp
int g(){ return 1; }

template<class T>
void test(T x) {
    g();         // looked up & validated in phase 1
    x.m();       // dependent: checked in phase 2
}
```

Even if `T::m()` doesn’t exist, **no error until instantiation**:

```cpp
struct A { void m() {} };
test(A{});      // OK

struct B {};
test(B{});      // error NOW: B has no m()
```

---

## Proving the rule: Non-dependent name fails even if never instantiated

```cpp
template<typename T>
void bad() { unknown(); } // error even if never used

int main() {
    // never calls bad
}
```

Because `unknown()` does *not* depend on T.

---

## Final practical takeaways

| At definition                     | At instantiation                   |
| --------------------------------- | ---------------------------------- |
| Template is parsed                | Template arguments substituted     |
| Non-dependent names resolved      | Dependent names resolved           |
| Non-dependent expressions checked | Dependent expressions type-checked |
| No code generated                 | Code generated                     |

---

## Want the reality underneath (AST + compiler logs)?

If you want, next I can show:

* Clang `-Xclang -ast-dump` demonstration of both phases
* Compiler trace with `-ftime-trace`
* Disassembly showing when instantiations trigger codegen
* Live example with `extern template` controlling code emission
* Example where template errors appear miles deep in instantiation stack and how to decode them

---