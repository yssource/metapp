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

#ifndef METAPP_METAITERABLEBASE_H_969872685611
#define METAPP_METAITERABLEBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaiterable.h"

namespace metapp {

template <typename ContainerType>
struct MetaIterableBase
{
	static const MetaIterable * getMetaIterable() {
		static MetaIterable metaIterable(
			&metaIterableForEach
		);
		return &metaIterable;
	}

private:
	static void metaIterableForEach(const Variant & iterable, MetaIterable::Callback callback)
	{
		ContainerType & container = iterable.depointer().get<ContainerType &>();
		for(auto & item : container) {
			if(! callback(Variant::create<decltype(item)>(item))) {
				break;
			}
		}
	}

};


} // namespace metapp


#endif

