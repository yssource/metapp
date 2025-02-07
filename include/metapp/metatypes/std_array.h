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

#ifndef METAPP_STD_ARRAY_H_969872685611
#define METAPP_STD_ARRAY_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/interfaces/bases/metaindexablebase.h"
#include "metapp/interfaces/bases/metaiterablebase.h"

#include <array>

namespace metapp {

template <typename T, size_t length>
struct DeclareMetaTypeBase <std::array<T, length> >
	: MetaIndexableBase<std::array<T, length> >,
		MetaIterableBase<std::array<T, length> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdArray;

	using ArrayType = std::array<T, length>;

};


} // namespace metapp


#endif

