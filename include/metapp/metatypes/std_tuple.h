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

#ifndef METAPP_STD_TUPLE_H_969872685611
#define METAPP_STD_TUPLE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/interfaces/metaiterable.h"
#include "metapp/utilities/utility.h"

#include <tuple>
#include <array>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::tuple<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdTuple;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
			&metaIndexableGetValueType,
			nullptr,
			&metaIndexableGet,
			&metaIndexableSet
		);
		return &metaIndexable;
	}

	static const MetaIterable * getMetaIterable() {
		static MetaIterable metaIterable(
			&metaIterableForEach
		);
		return &metaIterable;
	}

private:
	static size_t metaIndexableGetSize(const Variant & /*var*/)
	{
		return sizeof...(Types);
	}

	static const MetaType * metaIndexableGetValueType(const Variant & /*var*/, const size_t index)
	{
		using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
		return doGetValueType(index, Sequence());
	}

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
		return doGetAt(var.depointer(), index, Sequence());
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		if(index >= metaIndexableGetSize(var)) {
			errorInvalidIndex();
		}
		else {
			using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
			doSetAt(var.depointer(), index, value, Sequence());
		}
	}

	template <size_t ...Indexes>
	static const MetaType * doGetValueType(const size_t index, internal_::SizeConstantList<Indexes...>)
	{
		std::array<const MetaType *, sizeof...(Types)> valutTypeList {
			getMetaType<typename std::tuple_element<Indexes, std::tuple<Types...> >::type>()...
		};
		return valutTypeList[index];
	}

	template <size_t ...Indexes>
	static Variant doGetAt(const Variant & var, const size_t index, internal_::SizeConstantList<Indexes...>)
	{
		using Func = Variant (*)(const Variant & value);

		std::array<Func, sizeof...(Types)> funcList {
			&doGetAtHelper<Indexes>...
		};
		return funcList[index](var);
	}

	template <size_t index>
	static Variant doGetAtHelper(const Variant & var)
	{
		using TupleType = std::tuple<Types...>;

		return std::get<index>(var.get<TupleType &>());
	}

	template <size_t ...Indexes>
	static void doSetAt(const Variant & var, const size_t index, const Variant & value, internal_::SizeConstantList<Indexes...>)
	{
		using Func = void (*)(const Variant & var, const Variant & value);

		std::array<Func, sizeof...(Types)> funcList {
			&doSetAtHelper<Indexes>...
		};
		funcList[index](var, value);
	}

	template <size_t index>
	static void doSetAtHelper(const Variant & var, const Variant & value)
	{
		using TupleType = std::tuple<Types...>;
		using ValueType = typename std::tuple_element<index, TupleType>::type;

		internal_::assignValue(std::get<index>(var.get<TupleType &>()), value.get<ValueType &>());
	}

	static void metaIterableForEach(const Variant & var, MetaIterable::Callback callback)
	{
		using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
		doForEach(var.depointer(), callback, Sequence());
	}

	template <size_t ...Indexes>
	static void doForEach(const Variant & var, MetaIterable::Callback callback, internal_::SizeConstantList<Indexes...>)
	{
		using Func = bool (*)(const Variant & value, MetaIterable::Callback callback);

		std::array<Func, sizeof...(Types)> funcList {
			&doForEachHelper<Indexes>...
		};
		for(auto func : funcList) {
			if(! func(var, callback)) {
				break;
			}
		}
	}

	template <size_t index>
	static bool doForEachHelper(const Variant & var, MetaIterable::Callback callback)
	{
		using TupleType = std::tuple<Types...>;

		return callback(Variant::create<
			typename std::tuple_element<index, TupleType>::type &
			>(
				std::get<index>(var.get<TupleType &>())
		));
	}

};


} // namespace metapp


#endif

