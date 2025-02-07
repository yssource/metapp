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

#ifndef METAPP_METAINDEXABLE_H_969872685611
#define METAPP_METAINDEXABLE_H_969872685611

#include "metapp/variant.h"

#include <limits>

namespace metapp {

class MetaIndexable
{
public:
	static constexpr size_t unknowSize = std::numeric_limits<size_t>::max();

	MetaIndexable() = delete;

	MetaIndexable(
		size_t (*getSize)(const Variant & indexable),
		const MetaType * (*getValueType)(const Variant & indexable, const size_t index),
		void (*resize)(const Variant & indexable, const size_t size),
		Variant (*get)(const Variant & indexable, const size_t index),
		void (*set)(const Variant & indexable, const size_t index, const Variant & value)
	)
		:
			getSize(getSize),
			getValueType(getValueType),
			get(get),
			set(set),
			resize_(resize)
	{
	}

	size_t (*getSize)(const Variant & indexable);
	const MetaType * (*getValueType)(const Variant & indexable, const size_t index);
	void resize(const Variant & indexable, const size_t size) const {
		if(resize_ != nullptr) {
			resize_(indexable, size);
		}
	}
	Variant (*get)(const Variant & indexable, const size_t index);
	void (*set)(const Variant & indexable, const size_t index, const Variant & value);

private:
	void (*resize_)(const Variant & indexable, const size_t size);
};

inline size_t indexableGetSize(const Variant & indexable)
{
	return indexable.getMetaType()->getMetaIndexable()->getSize(indexable);
}

inline void indexableResize(const Variant & indexable, const size_t size)
{
	indexable.getMetaType()->getMetaIndexable()->resize(indexable, size);
}

inline Variant indexableGet(const Variant & indexable, const size_t index)
{
	return indexable.getMetaType()->getMetaIndexable()->get(indexable, index);
}

inline void indexableSet(const Variant & indexable, const size_t index, const Variant & value)
{
	indexable.getMetaType()->getMetaIndexable()->set(indexable, index, value);
}


} // namespace metapp

#endif
