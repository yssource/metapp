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

#ifndef METAPP_DECLAREUTIL_I_H_969872685611
#define METAPP_DECLAREUTIL_I_H_969872685611

#include "metapp/variant.h"
#include "metapp/metatype.h"
#include "metapp/interfaces/metacallable.h"

#include <type_traits>
#include <numeric>
#include <algorithm>

namespace metapp {

static constexpr int invokeRankMatch = 1000;
static constexpr int invokeRankCast = 1;
static constexpr int invokeRankNone = 0;

namespace internal_ {

template <typename TL, size_t N>
bool canCastArgument(const ArgumentSpan & arguments)
{
	if(N >= arguments.size()) {
		return true;
	}
	return arguments[N].template canCast<typename TypeListGetAt<TL, N>::Type>();
}

template <typename TL, size_t N>
int rankArgumentMatching(const ArgumentSpan & arguments)
{
	if(N >= arguments.size()) {
		return invokeRankMatch;
	}
	using To = typename TypeListGetAt<TL, N>::Type;
	if(arguments[N].template canGet<To>()) {
		return invokeRankMatch;
	}
	if(arguments[N].template canCast<To>()) {
		return invokeRankCast;
	}
	return invokeRankNone;
}

template <typename TL, int N>
Variant castArgument(const ArgumentSpan & arguments)
{
	return arguments[N].template cast<typename TypeListGetAt<TL, N>::Type>();
}

template <typename TL, int N>
auto getArgument(const ArgumentSpan & arguments) -> typename TypeListGetAt<TL, N>::Type
{
	return arguments[N].template get<typename TypeListGetAt<TL, N>::Type>();
}

template <typename ArgList>
struct MetaCallableInvokeChecker
{
	using ArgumentTypeList = ArgList;
	static constexpr size_t argCount = TypeListCount<ArgumentTypeList>::value;

	static bool canInvoke(const ArgumentSpan & arguments) {
		using Sequence = typename MakeIntSequence<argCount>::Type;
		return doCanInvoke(arguments, Sequence());
	}

	template <int ...Indexes>
	static bool doCanInvoke(const ArgumentSpan & arguments, IntConstantList<Indexes...>) {
		std::array<bool, argCount> canCastList {
			canCastArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)canCastList;
		return std::find(std::begin(canCastList), std::end(canCastList), false) == std::end(canCastList);
	}

	static int rankInvoke(const ArgumentSpan & arguments) {
		using Sequence = typename MakeIntSequence<argCount>::Type;
		return doRankInvoke(arguments, Sequence());
	}

	template <int ...Indexes>
	static int doRankInvoke(const ArgumentSpan & arguments, IntConstantList<Indexes...>) {
		if(argCount == arguments.size() && arguments.empty()) {
			return invokeRankMatch;
		}
		std::array<int, argCount> rankList {
			rankArgumentMatching<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)rankList;
		int rank = 0;
		for(auto it = std::begin(rankList); it != std::end(rankList); ++it) {
			if(*it == invokeRankNone) {
				return invokeRankNone;
			}
			rank += *it;
		}
		return rank;
	}
};

template <typename Class, typename RT, typename ArgList>
struct MetaCallableInvoker;

template <typename RT, typename ArgList>
struct MetaCallableInvoker <void, RT, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr int argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(FT && func, void * instance, const ArgumentSpan & arguments) {
		using Sequence = typename MakeIntSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, int ...Indexes>
	static Variant doInvoke(FT func, void * /*instance*/, const ArgumentSpan & arguments, IntConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant(func(getArgument<ArgumentTypeList, Indexes>(castedArguments)...));
	}
};

template <typename ArgList>
struct MetaCallableInvoker <void, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr int argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(FT && func, void * instance, const ArgumentSpan & arguments) {
		using Sequence = typename MakeIntSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, int ...Indexes>
	static Variant doInvoke(FT func, void * /*instance*/, const ArgumentSpan & arguments, IntConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		func(getArgument<ArgumentTypeList, Indexes>(castedArguments)...);
		return Variant();
	}
};

template <typename Class, typename RT, typename ArgList>
struct MetaCallableInvoker
{
	using ArgumentTypeList = ArgList;
	static constexpr int argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(FT && func, void * instance, const ArgumentSpan & arguments) {
		using Sequence = typename MakeIntSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, int ...Indexes>
	static Variant doInvoke(FT func, void * instance, const ArgumentSpan & arguments, IntConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		return Variant((static_cast<Class *>(instance)->*func)(getArgument<ArgumentTypeList, Indexes>(castedArguments)...));
	}
};

template <typename Class, typename ArgList>
struct MetaCallableInvoker <Class, void, ArgList>
{
	using ArgumentTypeList = ArgList;
	static constexpr int argCount = TypeListCount<ArgumentTypeList>::value;

	template <typename FT>
	static Variant invoke(FT && func, void * instance, const ArgumentSpan & arguments) {
		using Sequence = typename MakeIntSequence<argCount>::Type;
		return doInvoke(std::forward<FT>(func), instance, arguments, Sequence());
	}

	template <typename FT, int ...Indexes>
	static Variant doInvoke(FT func, void * instance, const ArgumentSpan & arguments, IntConstantList<Indexes...>) {
		std::array<Variant, argCount> castedArguments {
			castArgument<ArgumentTypeList, Indexes>(arguments)...
		};
		// avoid unused warning if there is no arguments
		(void)arguments;
		(void)castedArguments;
		(static_cast<Class *>(instance)->*func)(getArgument<ArgumentTypeList, Indexes>(castedArguments)...);
		return Variant();
	}
};


} // namespace internal_

} // namespace metapp

#endif
