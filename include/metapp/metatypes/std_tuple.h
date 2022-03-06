#ifndef METAPP_STD_TUPLE_H_969872685611
#define METAPP_STD_TUPLE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metaindexable.h"
#include "metapp/interfaces/metaiterable.h"
#include "metapp/utils/utility.h"

#include <tuple>
#include <array>

namespace metapp {

template <typename ...Types>
struct DeclareMetaTypeBase <std::tuple<Types...> >
	: public DeclareMetaTypeObject <std::tuple<Types...> >
{
	using UpType = TypeList<Types...>;
	static constexpr TypeKind typeKind = tkStdTuple;

	static const MetaIndexable * getMetaIndexable() {
		static MetaIndexable metaIndexable(
			&metaIndexableGetSize,
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

	static Variant metaIndexableGet(const Variant & var, const size_t index)
	{
		using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
		return doGetAt(var, index, Sequence());
	}

	static void metaIndexableSet(const Variant & var, const size_t index, const Variant & value)
	{
		if(index >= metaIndexableGetSize(var)) {
			errorInvalidIndex();
		}
		else {
			using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
			doSetAt(var, index, value, Sequence());
		}
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

		assignValue(std::get<index>(var.get<TupleType &>()), value.get<ValueType &>());
	}

	static void metaIterableForEach(const Variant & value, MetaIterable::Callback callback)
	{
		using Sequence = typename internal_::MakeSizeSequence<sizeof...(Types)>::Type;
		doForEach(value, callback, Sequence());
	}

	template <size_t ...Indexes>
	static void doForEach(const Variant & value, MetaIterable::Callback callback, internal_::SizeConstantList<Indexes...>)
	{
		using Func = bool (*)(const Variant & value, MetaIterable::Callback callback);

		std::array<Func, sizeof...(Types)> funcList {
			&doForEachHelper<Indexes>...
		};
		for(auto func : funcList) {
			if(! func(value, callback)) {
				break;
			}
		}
	}

	template <size_t index>
	static bool doForEachHelper(const Variant & value, MetaIterable::Callback callback)
	{
		using TupleType = std::tuple<Types...>;

		return callback(Variant::create<
			typename std::tuple_element<index, TupleType>::type &
			>(
				std::get<index>(value.get<TupleType &>())
		));
	}

};


} // namespace metapp


#endif

