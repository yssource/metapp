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
# MetaAccessible interface

## Overview

`MetaAccessible` is a meta interface to get and set value.  

## Header

```c++
#include "metapp/interfaces/metaaccessible.h"
```

## Get MetaAccessible interface

We can call `MetaType::getMetaAccessible()` to get the `MetaAccessible` interface. If the type doesn't implement the interface, `nullptr` is returned.

```c++
const metapp::MetaType * metaType = metapp::getMetaType<std::vector<int> >();
const metapp::MetaAccessible * metaAccessible = metaType->getMetaAccessible();
```

## Implemented built-in meta types

Pointer, T * (tkPointer)  
Member data pointer, T C::* (tkMemberPointer)  
Accessor (tkAccessor)  

## MetaAccessible constructor

```c++
MetaAccessible(
	const MetaType * (*getValueType)(const Variant & accessible),
	bool (*isReadOnly)(const Variant & accessible),
	const MetaType * (*getClassType)(const Variant & accessible),
	Variant (*get)(const Variant & accessible, const void * instance),
	void (*set)(const Variant & accessible, void * instance, const Variant & value)
);
```

All arguments are function pointers. All pointers must point to valid function.  
The meaning of each functions are same as the member functions listed below.

## MetaIterable member functions

The first parameter in all of the member functions is `const Variant & accessible`. It's the Variant which meta type implements `MetaIndexable`, and hold the proper data such as `std::vector`. The member functions operate on the data.  
We can treat `accessible` as the C++ object instance which class implements an interface called `MetaIndexable`.  
`accessible` can be a value, a reference, or a pointer.  

#### getValueType

```c++
const MetaType * getValueType(const Variant & accessible);
```

Returns the meta type of the value.  
For Accessor, returns the meta type of `Accessor::ValueType`.  

#### isReadOnly

```c++
bool isReadOnly(const Variant & accessible);
```

Returns `true` if the value is read only.  
For pointer `T *` and member data pointer `T C::*`, returns `true` if T is const.  
For Accessor, returns `Accessor::isReadOnly()`.  

#### getClassType

```c++
const MetaType * getClassType(const Variant & accessible);
```

Returns the meta type of the class that the accessible belongs to, or to say, the class declares the accessible. 
If the function returns meta type of `void` (MetaType::isVoid() is true), the accessible doesn't belong to any class,
or the accessible is a static member. When getting/setting value in the accessble, the `instance` can be nullptr.  
If the function returns non-void meta type, the accessible belongs to the class of the meta type.
When getting/setting value in the accessble, the `instance` must be pointer to a valid object.  

#### get

```c++
Variant get(const Variant & accessible, const void * instance);
```

Returns the value.  
The parameter `instance` is the object instance, if any.  
For pointer `T *`, `instance` is ignored. The returned Variant is a reference to the value.  
For member data pointer `T C::*`, `instance` is used to access the value in the object. The returned Variant is a reference to the value.  
For Accessor, `instance` is passed to the accessor. The returned Variant is the value get from the accessor.  

#### set

```c++
void set(const Variant & accessible, void * instance, const Variant & value);
```

Set a new value.  

#### isStatic

```c++
bool isStatic(const Variant & accessible) const;
```

Returns true if the accessible is static or non-member, false if the accessbile is class member.  
The function is equivalent to `return getClassType(accessible)->isVoid();`.  

## Non-member utility functions

Below free functions are shortcut functions to use the member functions in `MetaAccessible`.  
Usually you should prefer the utility functions to calling `MetaAccessible` member function directly. However, if you need to call functions on a single `MetaAccessible` more than one times in a high performance application, you may store `accessible.getMetaType()->getMetaAccessible()` to a local variable, then use the variable to call the member functions. This is because `getMetaAccessible()` has slightly performance overhead (the overhead is neglect most time).

```c++
inline const MetaType * accessibleGetValueType(const Variant & accessible)
{
	return accessible.getMetaType()->getMetaAccessible()->getValueType(accessible);
}

inline bool accessibleIsReadOnly(const Variant & accessible)
{
	return accessible.getMetaType()->getMetaAccessible()->isReadOnly(accessible);
}

inline const MetaType * accessibleGetClassType(const Variant & accessible)
{
	return accessible.getMetaType()->getMetaAccessible()->getClassType(accessible);
}

inline Variant accessibleGet(const Variant & accessible, const void * instance)
{
	return accessible.getMetaType()->getMetaAccessible()->get(accessible, instance);
}

inline void accessibleSet(const Variant & accessible, void * instance, const Variant & value)
{
	accessible.getMetaType()->getMetaAccessible()->set(accessible, instance, value);
}

inline bool accessibleIsStatic(const Variant & accessible)
{
	return accessible.getMetaType()->getMetaAccessible()->isStatic(accessible);
}
```

desc*/
