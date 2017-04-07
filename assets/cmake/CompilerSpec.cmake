# Feature test macros
include(CheckCXXSourceCompiles)

# Test support for noexcept specifier - negative: VS 2013
CHECK_CXX_SOURCE_COMPILES("
void func() noexcept { }
int main() { }
" HAS_NOEXCEPT_SPECIFIER)

# N3656 - negative: libstdc++ prior 4.9.0
CHECK_CXX_SOURCE_COMPILES("
#include <memory>
int main() { std::unique_ptr<int> foo = std::make_unique<int>(4); }
" HAS_MAKE_UNIQUE)

# Default template arguments for function templates - negative: VS 2012
CHECK_CXX_SOURCE_COMPILES("
template <typename Fu = int> void func() { }
int main() { }
" HAS_DEFAULT_TEMPLATE_ARGS_FOR_FUNCTIONS)

# Thread local storage - partial: VS 2012
CHECK_CXX_SOURCE_COMPILES("
int main() { thread_local int i; }
" HAS_THREAD_LOCAL_STORAGE)

# N2326 defaulted and deleted functions - negative: VS 2012 of course
CHECK_CXX_SOURCE_COMPILES("
struct Fu { Fu() = default; Fu(const Fu&) = delete; };
int main() { }
" HAS_DEFAULT_AND_DELETE)

# Variadic templates - negative: VS 2012
CHECK_CXX_SOURCE_COMPILES("
template <typename... T> void func(T...) { }
int main() { }
" HAS_VARIADIC_TEMPLATES)

# Non-buggy <type_traits> implementation - negative VS 2013
CHECK_CXX_SOURCE_COMPILES("
#include <type_traits>
struct Fu { Fu(const Fu&) = delete; };
static_assert(!std::is_copy_constructible<Fu>::value, \"\");
int main() { }
" HAS_NON_BUGGY_TYPE_TRAITS)

# N3671 - negative: _MSC_VER <= 1800, libstdc++ prior 4.9.0
CHECK_CXX_SOURCE_COMPILES("
#include <algorithm>
#include <cstring>
static const char source[] = \"foo\";
static const char dest[] = \"bar\";
int main() { std::equal(source, source + std::strlen(source),
                        dest, dest + std::strlen(dest)); }
" HAS_ROBUST_NONMODIFYING_SEQ_OPS)

# Test support for initializer lists - negative: VS 2012
CHECK_CXX_SOURCE_COMPILES("
#include <vector>
int main() { std::vector<int> vec{1, 2, 3, 4, 5}; }
" HAS_INITIALIZER_LISTS)

# N2326 defaulted and deleted functions - negative: VS 2012 of course
CHECK_CXX_SOURCE_COMPILES("
explicit struct Fu {  };
int main() { }
" HAS_EXPLICIT_SPECIFIER)

# Generate compspec.h file needed for the plugin
configure_file("${CMAKE_CURRENT_LIST_DIR}/compspec.h.in" "${CMAKE_SOURCE_DIR}/src/compspec.h" @ONLY)


