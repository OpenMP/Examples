/*
* @@name:	directive_syntax_attribute.2
* @@type:	C++
* @@operation:	view
* @@expect:	none
* @@version:	omp_6.0
*/
// Case 1
[[ omp::decl(threadprivate) ]] int u, v;
// equivalent to
int u ,v;
#pragma omp threadprivate(u, v)

// Case 2
int a[100] [[ omp::decl(declare_target) ]],
    b[100] [[ omp::decl(declare_target, link) ]];
// equivalent to
int a[100], b[100];
#pragma omp declare_target(a)
#pragma omp declare_target link(b)

// Case 3
[[ omp::decl(declare_target) ]] void f( int c );
// equivalent to
void f( int c );
#pragma omp declare_target(f)

// Case 4
template<typename T>
[[ omp::decl(declare_target) ]]
void foo(T);
// equivalent to
#pragma omp begin declare_target
template<typename T>
void foo(T);
#pragma omp end declare_target
