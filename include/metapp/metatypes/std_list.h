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

#ifndef METAPP_STD_LIST_H_969872685611
#define METAPP_STD_LIST_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/bases/metaiterablebase.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/utilities/utility.h"

#include <list>

namespace metapp {

template <typename T, typename Allocator>
struct DeclareMetaTypeBase <std::list<T, Allocator> >
	: MetaIterableBase<std::list<T, Allocator> >
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkStdList;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGetValueType,
			&metaIndexableResize,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

private:
	using ValueType = T;
	using ContainerType = std::list<T, Allocator>;

	static size_t metaIndexableGetSize(const Variant & var)
	{
		return var.depointer().get<ContainerType &>().size();
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*var*/, const size_t /*index*/)
	{
		return getMetaType<ValueType>();
	}

	static void metaIndexableResize(const Variant & var, const size_t size)
	{
		var.depointer().get<ContainerType &>().resize(size);
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		const Variant ref = var.depointer();

		if(index >= metaIndexableGetSize(ref)) {
			errorInvalidIndex();
		}
		auto & list = ref.get<ContainerType &>();
		auto it = list.begin();
		std::advance(it, index);
		return Variant::create<ValueType>(*it);
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		const Variant ref = var.depointer();

		internal_::verifyVariantWritable(ref);

		if(index >= metaIndexableGetSize(ref)) {
			errorInvalidIndex();
		}
		else {
			auto & list = ref.get<ContainerType &>();
			auto it = list.begin();
			std::advance(it, index);
			internal_::assignValue(*it, value.cast<ValueType &>().template get<ValueType &>());
		}
	}

};


} // namespace metapp


#endif

