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
# MetaCallable interface

## Overview

`MetaCallable` is a meta interface to invoke meta callable (function, member function, constructor, std::function, etc).  

## Header

```c++
#include "metapp/interfaces/metacallable.h"
```

## Get MetaCallable interface

We can call `MetaType::getMetaCallable()` to get the `MetaCallable` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaCallable * metaCallable = metaType->getMetaCallable();
```

## Implemented built-in meta types

Constructor (tkConstructor)  
Function (tkFunction)  
Member function pointer, T (C::*)(arguments...) (tkMemberPointer)  
`std::function` (tkStdFunction)  
Default arguments function (tkDefaultArgsFunction)  
Variadic function (tkVariadicFunction)  

## MetaCallable constructor

```c++
MetaCallable(
	const MetaType * (*getClassType)(const Variant & callable),
	int (*getParameterCount)(const Variant & callable),
	const MetaType * (*getReturnType)(const Variant & callable),
	const MetaType * (*getParameterType)(const Variant & callable, const int index),
	int (*rankInvoke)(const Variant & callable, const Variant * arguments, const int argumentCount),
	bool (*canInvoke)(const Variant & callable, const Variant * arguments, const int argumentCount),
	Variant (*invoke)(const Variant & callable, void * instance, const Variant * arguments, const int argumentCount)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

## MetaCallable member functions

The first parameter in all of the member functions is `const Variant & callable`. It's the Variant which meta type implements `MetaCallable`, and hold the proper data such as function pointer. The member functions operate on the data.  
We can treat `callable` as the C++ object instance which class implements an interface called `MetaCallable`.  
`callable` can be a value, a reference, or a pointer.  

#### getClassType

```c++
const MetaType * getClassType(const Variant & callable);
```

Returns the meta type of the class that the callable belongs to, or to say, the class declares the callable. 
If the function returns meta type of `void` (MetaType::isVoid() is true), the callable doesn't belong to any class,
or the callable is a static member function. When invoking the callable, the `instance` can be nullptr.  
If the function returns non-void meta type, the callable belongs to the class of the meta type.
When invoking the callable, the `instance` must be pointer to a valid object.  

#### getParameterCount

```c++
int getParameterCount(const Variant & callable);
```

Returns the parameter count.  
For variadic function (tkVariadicFunction), returns 0.  

#### getReturnType

```c++
const MetaType * getReturnType(const Variant & callable);
```

Returns meta type of the callable return type.  
For constructor (tkConstructor), the return type is pointer to the class.  

#### getParameterType

```c++
const MetaType * getParameterType(const Variant & callable, const int index);
```

Returns the meta type of parameter at `index`.  
For variadic function (tkVariadicFunction), the function always returns nullptr.  

#### rankInvoke

```c++
int rankInvoke(const Variant & callable, const Variant * arguments, const int argumentCount);
```

Returns the rank value of whether each argument in array `arguments` matches the parameter in the callable.  
If the return value is 0, then the arguments can't be used to invoke the callable. Otherwise, the larger the return value, the more matching.  

Parameter `arguments` is a pointer to an array of Variant, each element is an argument. The first element is the left-most argument in the callable.  
Parameter `argumentCount` is the number of argument in `arguments`.  

#### canInvoke

```c++
bool canInvoke(const Variant & callable, const Variant * arguments, const int argumentCount);
```

Returns true if the `arguments` can be used to invoke the callable, false if not.  
This is similar to check if the result of `rankInvoke` is larger than 0, but it's more efficient than `rankInvoke`.

#### invoke

```c++
Variant invoke(const Variant & callable, void * instance, const Variant * arguments, const int argumentCount);
```

Invokes the callable, returns the result of the callable. If the callable doesn't return any value (the result type is void), then empty Variant is returned (Variant::isEmpty() is true).  

#### isStatic

```c++
bool isStatic(const Variant & callable) const;
```

Returns true if the accessible is static or non-member, false if the accessbile is class member.  
The function is equivalent to `return getClassType(callable)->isVoid();`.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaCallable`.  
Usually you should prefer the utility functions to calling `MetaCallable` member function directly. However, if you need to call functions on a single `MetaCallable` more than one times in a high performance application, you may store `callable.getMetaType()->getMetaCallable()` to a local variable, then use the variable to call the member functions. This is because `getMetaCallable()` has slightly performance overhead (the overhead is neglect most time).

#### callableGetClassType

```c++
inline const MetaType * callableGetClassType(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getClassType(callable);
}
```

Shortcut for `MetaCallable::getClassType()`.

#### callableGetParameterCount

```c++
inline int callableGetParameterCount(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getParameterCount(callable);
}
```

Shortcut for `MetaCallable::getParameterCount()`.

#### callableGetReturnType

```c++
inline const MetaType * callableGetReturnType(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->getReturnType(callable);
}
```

Shortcut for `MetaCallable::getReturnType()`.

#### callableGetParameterType

```c++
inline const MetaType * callableGetParameterType(const Variant & callable, const int index)
{
	return callable.getMetaType()->getMetaCallable()->getParameterType(callable, index);
}
```

Shortcut for `MetaCallable::getParameterType()`.

#### callableRankInvoke

```c++
template <typename ...Args>
int callableRankInvoke(const Variant & callable, Args ...args);
```

Converts `args` to Variant array then calls `MetaCallable::rankInvoke()` and returns the result.

#### callableCanInvoke

```c++
template <typename ...Args>
bool callableCanInvoke(const Variant & callable, Args ...args);
```

Converts `args` to Variant array then calls `MetaCallable::canInvoke()` and returns the result.

#### callableInvoke

```c++
template <typename ...Args>
Variant callableInvoke(const Variant & callable, void * instance, Args ...args);
```

Converts `args` to Variant array then calls `MetaCallable::invoke()` and returns the result.

#### callableInvoke on container

```c++
template <template <typename, typename> class Container, typename T, typename Allocator, typename ...Args>
Variant callableInvoke(const Container<T, Allocator> & callableList, void * instance, Args ...args);
```

Finds a best matched callable in `callableList`, then invokes the matched callable and returns the result.  
If no matched callable, exception `metapp::IllegalArgumentException` is thrown.  
`callableList` can be STL iterable containers, such as `std::vector`, `std::deque`, or `std::list`. The value type T can be `metapp::Variant` that holds a callable.  
`callableList` can also be `MetaItemList` or `MetaItemList` obtained from MetaClass or MetaRepo.

#### findCallable

```c++
template <typename Iterator>
Iterator findCallable(
	Iterator first,
	Iterator last,
	const Variant * arguments,
	const int argumentCount
);
```

Returns an iterator to the element that's best matched to `arguments` in the range [first, last).  
If no matched callable, `last` is returned.  
`Iterator` must be the iterator to `Variant`, `MetaItem`, or `MetaItem`.

#### callableIsStatic

```c++
inline bool callableIsStatic(const Variant & callable)
{
	return callable.getMetaType()->getMetaCallable()->isStatic(callable);
}
```

Shortcut for `MetaCallable::isStatic()`.

desc*/
