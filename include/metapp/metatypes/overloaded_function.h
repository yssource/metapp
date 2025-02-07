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

#ifndef METAPP_OVERLOADED_FUNCTION_H_969872685611
#define METAPP_OVERLOADED_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/utilities/utility.h"

#include <deque>

namespace metapp {

class OverloadedFunction
{
public:
	OverloadedFunction() : callableList() {}

	const std::deque<Variant> & getCallableList() const {
		return callableList;
	}

	void addCallable(const Variant & callable) {
		callableList.push_back(callable);
	}

private:
	std::deque<Variant> callableList;
};

template <>
struct DeclareMetaType <OverloadedFunction >
{
public:
	static constexpr TypeKind typeKind = tkOverloadedFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetClassType,
			&metaCallableGetParameterCount,
			&metaCallableGetReturnType,
			&metaCallableGetParameterType,
			&metaCallableRankInvoke,
			&metaCallableCanInvoke,
			&metaCallableInvoke
		);
		return &metaCallable;
	}

	static const MetaType * metaCallableGetClassType(const Variant & func)
	{
		const auto & callableList = func.get<const OverloadedFunction &>().getCallableList();
		if(! callableList.empty()) {
			const auto & callable = callableList.front();
			return callable.getMetaType()->getMetaCallable()->getClassType(callable);
		}
		return voidMetaType;
	}

	static int metaCallableGetParameterCount(const Variant & /*func*/)
	{
		return 0;
	}

	static const MetaType * metaCallableGetReturnType(const Variant & /*func*/)
	{
		return voidMetaType;
	}

	static const MetaType * metaCallableGetParameterType(const Variant & /*func*/, const int /*index*/)
	{
		return voidMetaType;
	}

	static int metaCallableRankInvoke(const Variant & func, const Variant & instance, const ArgumentSpan & arguments)
	{
		int maxRank;
		const auto & callableList = func.get<const OverloadedFunction &>().getCallableList();
		findCallable(callableList.begin(), callableList.end(), instance, arguments, &maxRank);
		return maxRank;
	}

	static bool metaCallableCanInvoke(const Variant & func, const Variant & instance, const ArgumentSpan & arguments)
	{
		const auto & callableList = func.get<const OverloadedFunction &>().getCallableList();
		for(const auto & callable : callableList) {
			if(callable.getMetaType()->getMetaCallable()->canInvoke(callable, instance, arguments)) {
				return true;
			}
		}
		return false;
	}

	static Variant metaCallableInvoke(const Variant & func, const Variant & instance, const ArgumentSpan & arguments)
	{
		const auto & callableList = func.get<const OverloadedFunction &>().getCallableList();
		auto it = findCallable(callableList.begin(), callableList.end(), instance, arguments, nullptr);
		if(it != callableList.end()) {
			return (*it).getMetaType()->getMetaCallable()->invoke(*it, instance, arguments);
		}
		errorIllegalArgument();
		return Variant();
	}

};


} // namespace metapp


#endif

