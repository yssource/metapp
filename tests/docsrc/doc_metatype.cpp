// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testutil.h"

#include "metapp/allmetatypes.h"

/*desc
# MetaType reference

## Overview

`metapp::MetaType` represents the meta information of arbitrary C++ type.  
We can obtain almost all information on any C++ type through `metapp::MetaType`, such as,
but not limited to, pointer, reference, const-volatile qualifier, array, function parameters, etc.  

## Header

```c++
#include "metapp/metatype.h"
```

## Obtain the MetaType for given C++ type

### Use getMetaType at compile time

```c++
template <typename T>
const MetaType * getMetaType();
```

`getMetaType` is a non-member free function.  
Return a MetaType pointer of type T.  
`getMetaType` can be used on any C++ type, the type doesn't need any registering or preprocessing.  

The pointer returned by `getMetaType()` is always the same for the same T. For example,  
desc*/

ExampleFunc
{
	//code
	ASSERT(metapp::getMetaType<int>() == metapp::getMetaType<int>());
	ASSERT(metapp::getMetaType<std::string>() == metapp::getMetaType<std::string>());
	//code
}

/*desc
MetaType is CV-aware (CV means const-volatile). That's to say, for the same T,
different CV qualified types will result different MetaType. For example,  
desc*/

ExampleFunc
{
	//code
	ASSERT(metapp::getMetaType<int>() != metapp::getMetaType<const int>());
	ASSERT(metapp::getMetaType<std::string>() != metapp::getMetaType<volatile std::string>());
	//code
}

/*desc
To identify CV-unaware meta type, use `MetaType::equal()` or `MetaType::compare()`.  

### Use MetaRepo at runtime

The class `metapp::MetaRepo` holds all registered meta types.

## Member functions

#### Not constructible

```c++
MetaType() = delete;
MetaType(const MetaType &) = delete;
MetaType(MetaType &&) = delete;
```

MetaType is not constructible. So the user can't construct a MetaType on the stack or heap.  

#### equal

```c++
bool equal(const MetaType * other) const;
```

Returns true if `this` equals to `other`.  
The comparison ignores any CV qualifiers in the types, include top level CV, or CV in the pointer or reference.  
desc*/

ExampleFunc
{
	//code
	// A type equals to itself.
	ASSERT(metapp::getMetaType<int>()->equal(metapp::getMetaType<int>()));
	// Any top level CV qualifiers are ignored.
	ASSERT(metapp::getMetaType<int>()->equal(metapp::getMetaType<const int>()));

	// Any top level CV qualifiers are ignored.
	ASSERT(metapp::getMetaType<int *>()->equal(metapp::getMetaType<int * const>()));
	// CV inside pointers or references are ignored.
	ASSERT(metapp::getMetaType<int *>()->equal(metapp::getMetaType<const int *>()));
	ASSERT(metapp::getMetaType<int const * volatile *>()->equal(metapp::getMetaType<int volatile * const *>()));
	ASSERT(metapp::getMetaType<int &>()->equal(metapp::getMetaType<const int &>()));
	ASSERT(metapp::getMetaType<int &&>()->equal(metapp::getMetaType<const int &&>()));

	// Any top level CV qualifiers are ignored.
	ASSERT(metapp::getMetaType<int[]>()->equal(metapp::getMetaType<const int[]>()));

	// Different types don't equal.
	ASSERT(! metapp::getMetaType<int>()->equal(metapp::getMetaType<long>()));
	// Different pointers are different types.
	ASSERT(! metapp::getMetaType<int *>()->equal(metapp::getMetaType<int **>()));
	//code
}

/*desc
#### compare

```c++
int compare(const MetaType * other) const;
```

Compares `this` with `other`.  
The comparison ignores any CV qualifiers in the types, include top level CV, or CV in the pointer or reference.  
Returns negative value if `this` is before `other`.  
Returns zero if `this` equals to `other`.  
Returns positive value if `this` is after `other`.  
This function is useful when putting `MetaType` in ordered containers, such as `std::map`, `std::set`, etc.

#### getUpType

```c++
const MetaType * getUpType() const; // #1
const MetaType * getUpType(const int i) const; // #2
```

Returns the pointer to UpType.  
The first form is same to `getUpType(0)`.  
A MetaType has 0, 1, or multiple UpTypes. An UpType is a pointer to MetaType object.  
A MetaType only has one TypeKind, so it can represent only one type information,
it can't represent compound information. For example, for type `int *`, the TypeKind of the MetaType is `tkPointer`,
the `int` type is missed. For `std::vector<char>`, the TypeKind of the MetaType is `tkStdVector`, the `char` type is missed.  
UpType is used to represent the missed type information.  
The actual types of UpType depends on how the meta type defines them. For example, for pointer (tkPointer),
it has only one UpType, which is the type it points to. Reference is similar to pointer.  

#### getUpTypeCount

```c++
int getUpTypeCount() const noexcept;
```

Returns the count of UpType. The result can be 0, 1, or more.  

#### getTypeKind

```c++
TypeKind getTypeKind() const noexcept;
```

Returns the TypeKind of the meta type.  

Note: whenever possible, you should prefer the type attributes and meta interfaces to TypeKind
when detecting the features of a MetaType.  
For example, instead of `metaType->getTypeKind() == metapp::tkPointer`, you should use `metaType->isPointer()`.  
Using type attributes and meta interfaces are more flexible and less error prone.  

#### isVoid

```c++
bool isVoid() const noexcept;
```

Returns true if the meta type is for `void` (tkVoid).  
`metapp` doesn't use nullptr meta type. When there is no appropriate underlying type, `void` is used.  
For example, the default constructed `Variant` (i.e, `Variant v`), has meta type of void.  

#### Get type attributes

```c++
constexpr bool isConst() const noexcept;
constexpr bool isVolatile() const noexcept;
constexpr bool isPointer() const noexcept;
constexpr bool isReference() const noexcept;
constexpr bool isClass() const noexcept;
constexpr bool isArray() const noexcept;
constexpr bool isEnum() const noexcept;
constexpr bool isMemberPointer() const noexcept;
constexpr bool isIntegral() const noexcept;
constexpr bool isFloat() const noexcept;
constexpr bool isArithmetic() const noexcept;
constexpr bool isPointerWrapper() const noexcept;
```

`isConst` returns true if the type is const qualified. It uses `std::is_const` to detect it.  
`isVolatile` returns true if the type is volatile qualified. It uses `std::is_volatile` to detect it.  
`isPointer` returns true if the type is a pointer or nullptr. It uses `std::is_pointer` and checking nullptr_t to detect it.  
`isReference` returns true if the type is a reference. It uses `std::is_reference` to detect it.  
`isClass` returns true if the type is a C++ class or struct.  It uses `std::is_class` to detect it. 
`isArray` returns true if the type is a raw array (int[3], std::string[2][3], etc). It uses `std::is_array` to detect it.  
`isEnum` returns true if the type is a C++ enum. It uses `std::is_enum` to detect it.    
`isMemberPointer` returns true if the type is a class member data or member function. It uses `std::is_member_pointer` to detect it.
To check if a MetaType is static member, check `! isMemberPointer()`.  
`isIntegral` returns true if the type is integral type. It uses `std::is_integral` to detect it.  
`isFloat` returns true if the type is float point type. It uses `std::is_floating_point` to detect it.  
`isArithmetic` returns true if the type is arithmetic type. It equals to `isIntegral() || isFloat()`.  
`isPointerWrapper` return true if the type is a pointer wrapper. A pointer wrapper is a type that acts as a pointer,
and a raw pointer can be obtained from the type. A typical pointer wrapper is smart pointer. `std::shared_ptr`
and `std::unique_ptr` are pointer wrapper.  

Note: the attributes are C++ type traits. They don't have connection to meta interface or other features in MetaType.
That's to say, `isClass()` returning true doesn't mean the MetaType implements `MetaClass` interface, etc.  

#### Get meta interfaces

```c++
const MetaClass * getMetaClass() const;
const MetaCallable * getMetaCallable() const;
const MetaAccessible * getMetaAccessible() const;
const MetaEnum * getMetaEnum() const;
const MetaIndexable * getMetaIndexable() const;
const MetaIterable * getMetaIterable() const;
const MetaStreaming * getMetaStreaming() const;
const MetaMappable * getMetaMappable() const;
const void * getMetaUser() const;
```

The functions return the meta interfaces if they are implemented.
If any interface is not implemented by the meta type, `nullptr` is returned.  
MetaType has very few public functions, most functions are in the meta interfaces.  
Please see the document for each meta interface for more detailed information.  

#### construct

```c++
void * construct() const;
```

Similar to C++ code `new T()`.  
Allocate and initialize an object on the heap, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

#### copyConstruct

```c++
void * copyConstruct(const void * copyFrom) const;
```

Similar to C++ code `new T(anotherObject)`.  
Allocate and initialize an object on the heap, copy the object pointed by `copyFrom` to the object, then returns the object pointer.  
The returned pointer can be freed using `destroy`.  

#### destroy

```c++
void destroy(void * instance) const;
```

Free an object constructed by `construct` or `copyConstruct`.  

#### getModule

```c++
const void * getModule() const noexcept;
```

Returns a pointer to represent which module the `MetaType` is from.  
Usually you don't need to use this function.  
"Module" means a dynamic library, or the main program.  
For the same module, the function always returns the same pointer for all meta types.  
For different modules, the pointers are different.  

desc*/
