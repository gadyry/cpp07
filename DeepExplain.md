

### 1 — Two compilation phases: definition-time vs instantiation-time

When the compiler sees a template definition, it parses and type-checks only the parts that do not depend on template parameters. It does not fully type-check dependent expressions. Later, when the template is used with specific template arguments, the compiler performs instantiation: substituting template arguments for parameters and compiling the resulting real code.

Concretely:

Definition-time (template definition)

The compiler must accept the template's syntax and check correctness for non-dependent constructs (e.g., declarations, names that do not depend on T).

Non-dependent unqualified names are resolved now (unqualified lookup).

The body is not fully checked for dependent expressions (e.g., T::foo or a + b where a/b involve T) — those are deferred.

Instantiation-time (when you use template<T> with specific T)

The compiler substitutes template arguments and type-checks the substituted code.

Dependent names and dependent expressions are resolved/checked now.

This is when real functions/classes/variables/templates are emitted.

This two-phase behavior is the reason errors about operators or members of T appear only when that particular T instantiates the template.

### 2 — Triggers: when does instantiation occur?

Implicit instantiation is performed when the program needs the template specialization to exist:

For a function template: when it is called (or when its address is taken or it needs to be emitted).

For a class template: when a member is ODR-used (e.g., a non-inline member function is used), or when the class type itself is required (for sizeof, object definition, etc.).

For member functions of class templates: members are usually instantiated only when used.

Explicit instantiation allows you to force or suppress instantiation:

template class MyTemplate<int>; — explicit instantiation definition: compiler must instantiate and emit code in this TU.

extern template class MyTemplate<int>; — explicit instantiation declaration: instructs compiler not to instantiate in this TU (useful to avoid code bloat; other TU should explicitly instantiate).

### 3 — Points/Order of instantiation (important subtleties)

The point of instantiation is after the point where the specialization is required. In practice, the compiler instantiates when it first needs the specialization; exact ordering can be complicated across translation units, but within a translation unit the instantiation happens at the point of use.

Instantiation can cause other instantiations (transitive): instantiating A<T> may instantiate A<T>::f() which uses B<T> etc.

Instantiation depth limit: compilers enforce a recursion/instantiation depth limit to prevent infinite template recursion. This is not part of the language semantics beyond being an implementation limit.

### 4 — Name lookup: two-phase lookup and dependent names

Two-phase lookup governs when unqualified names are resolved:

Unqualified non-dependent names (names that do not depend on template parameters): looked up during template definition. If they are not found then, you get an error at definition time.

Dependent names (names that depend on template parameters): lookup is deferred until instantiation. Example: T::value is dependent if it depends on T.

Qualified names like X::f where X depends on T are generally looked up at instantiation.

typename requirement: when a dependent name refers to a type, you must write typename:

template<typename T>
void f() {
    typename T::value_type x; // 'typename' required if T::value_type is a dependent type
}


If you omit typename, the code is ill-formed at template definition or instantiation depending on context.

### 5 — SFINAE (Substitution Failure Is Not An Error)

SFINAE is a narrow but critical rule:

When substituting template arguments into the immediate context of a function template (during overload resolution/deduction), if the substitution results in an invalid type or expression, that overload is removed from overload resolution rather than producing a hard error. This enables overload selection via traits, enable_if, expression checks, etc.

Important caveats:

SFINAE applies primarily during template argument deduction for function templates (and some contexts involving default template arguments and member templates).

If substitution fails outside the immediate substitution context or in a class template where deduction is not happening, it generally leads to a hard error (ill-formed).

For class templates, substitution failures are typically hard errors — you cannot rely on SFINAE to silently discard class-template specializations except in certain deduction contexts (partial ordering of function templates, alias templates with deduction guides, etc).

Example (function SFINAE):

template<typename T>
auto test(int) -> decltype(std::declval<T>().foo(), std::true_type{});

template<typename T>
std::false_type test(...);

static_assert(decltype(test<MyType>(0))::value, "MyType must have foo()");


If MyType::foo() is invalid, the first overload is discarded by SFINAE and the second is chosen.

### 6 — Member instantiation rules for class templates

Class template itself is declared at definition time but its member functions and nested types are instantiated only when used (odr-used).

Inline member functions defined inside the class body typically must be (syntactically) correct at definition time for non-dependent parts, but dependent parts are checked at instantiation.

Example:

template<typename T>
struct S {
    void f() { T::nonexistent(); } // OK at definition if T is dependent; error only when S<U>::f is instantiated and U lacks that member.
};


If you define a member outside the class:

template<typename T>
void S<T>::g() { /* ... */ }


The definition is parsed and checked similarly: dependent things are deferred; non-dependent things are checked immediately.

### 7 — Non-type template parameters and instantiation

Non-type template parameters (e.g., int N, auto N) are values known at compile-time; instantiation substitutes their value into the template. The behavior is largely the same: substitution and checks happen at instantiation for dependent expressions.

Example:

template<typename T, int N>
struct Arr { T data[N]; };

Arr<int, 5> a; // instantiate Arr<int,5> -> data[5]


auto non-type parameters (C++17/20 and later) allow more flexible compile-time values; type-checking and substitution proceed in the same instantiation phase.

### 8 — Explicit instantiation and extern template

Explicit instantiation definition:

// in one .cpp
template class MyTemplate<int>; // forces instantiation and emission of symbols for MyTemplate<int>


Use when you want to centralize instantiation and avoid multiple TUs instantiating the same specialization (reduces code size).

Explicit instantiation declaration (suppress instantiation):

extern template class MyTemplate<int>; // tells this TU: do not instantiate MyTemplate<int> here


Typically used in headers and paired with an explicit instantiation definition in one .cpp.

### 9 — Linkage, ODR, emission details

When a specialization is implicitly instantiated in multiple TUs, the compiler/linker must ensure only one emitted definition is used (ODR — one definition rule). Usually compilers mark template-generated functions as COMDAT/weak so the linker collapses duplicates.

If you explicitly instantiate in one TU and declare extern template in others, you avoid multiple instantiations and can reduce object size and compile-time.

### 10 — Diagnostics and why errors sometimes look strange

Because some checks are delayed until instantiation, template error messages can be long and obscure. The compiler is telling you the substitution result is invalid for the particular T.

The first error you see may be deep inside instantiated code (a typedef in a nested dependent template, etc.). Tracing back to the cause requires inspecting the substituted code.

### 11 — Examples that demonstrate behavior

Dependent check deferred until instantiation:

template<typename T>
void call_plus(T a, T b) { auto c = a + b; } // '+' not checked now

struct NoPlus {};

int main() {
    call_plus(1, 2);    // ok -> instantiates for int
    // call_plus(NoPlus{}, NoPlus{}); // ERROR if uncommented: NoPlus has no operator+
}


Name lookup difference:

int g(); // global g

template<typename T>
void f() {
    g(); // unqualified lookup finds global g at template definition
    T::h(); // dependent -> lookup at instantiation
}


SFINAE example (function selection):

template<typename T>
auto has_to_string(int) -> decltype(std::declval<T>().to_string(), std::true_type{});

template<typename T>
std::false_type has_to_string(...);

template<typename T>
constexpr bool has_to_string_v = decltype(has_to_string<T>(0))::value;


This compiles and yields true/false depending on whether T::to_string() exists — substitution failure for the first overload is not an error; it is excluded.

### 12 — Common gotchas and best practices

Expect errors at instantiation, not definition: if you see a cryptic error, check the substituted code for that specific specialization.

Use typename for dependent types: omission results in hard-to-follow errors.

Use extern template + single explicit instantiation to reduce binary size when many TUs instantiate the same heavy template.

Limit template recursion depth or test compiler limits if you use heavy TMP (template metaprogramming).

Prefer concept-based constraints (C++20) or enable_if to give clearer diagnostics and prevent ill-formed instantiations.

Minimize non-dependent unqualified names in templates if you expect the code to be reused in different scopes (name lookup at definition time can surprise you).

### 13 — Short checklist of where errors arise

Syntax errors in template body: reported at template definition if they do not depend on parameters.

Semantics involving dependent names/operators: reported at instantiation for the specific substituted type.

Substitution failure during deduction: may trigger SFINAE (function templates) or hard error (class templates) depending on context.

Missing typename/template keywords: cause definition-time or instantiation-time errors depending on where used.

