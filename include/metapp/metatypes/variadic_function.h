#ifndef METAPP_VARIADIC_FUNCTION_H_969872685611
#define METAPP_VARIADIC_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/variant.h"
#include "metapp/interfaces/metacallable.h"
#include "metapp/metatypes/utils/declareutil.h"
#include "metapp/utils/utility.h"

namespace metapp {

template <typename FT>
class VariadicFunction
{
public:
	VariadicFunction() : func() {
	}

	explicit VariadicFunction(FT func)
		: func(func) {
	}

	const Variant & getFunc() const {
		return func;
	}

private:
	Variant func;
};

template <typename FT>
auto createVariadicFunction(FT && func) -> VariadicFunction<FT>
{
	return VariadicFunction<FT>(std::forward<FT>(func));
}

namespace internal_ {

} // namespace internal_

template <typename FT>
struct DeclareMetaType <VariadicFunction<FT> >
{
private:
	using Underlying = DeclareMetaType<FT>;
	using FunctionType = VariadicFunction<FT>;
	using ClassType = typename Underlying::ClassType;
	using ReturnType = typename Underlying::ReturnType;
	using ArgumentTypeList = typename Underlying::ArgumentTypeList;

	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

public:
	using UpType = FT;
	static constexpr TypeKind typeKind = tkVariadicFunction;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&metaCallableGetParamCount,
			&metaCallableGetReturnType,
			&metaCallableGetParamType,
			&metaCallableRankInvoke,
			&metaCallableCanInvoke,
			&metaCallableInvoke
		);
		return &metaCallable;
	}

	static size_t metaCallableGetParamCount()
	{
		return 0;
	}

	static const MetaType * metaCallableGetReturnType()
	{
		return getMetaType<ReturnType>();
	}

	static const MetaType * metaCallableGetParamType(const size_t /*index*/)
	{
		return nullptr;
	}

	static int metaCallableRankInvoke(const Variant * /*arguments*/, const size_t /*argumentCount*/)
	{
		return 1;
	}

	static bool metaCallableCanInvoke(const Variant * /*arguments*/, const size_t /*argumentCount*/)
	{
		return true;
	}

	static Variant metaCallableInvoke(const Variant & func, void * instance, const Variant * arguments, const size_t argumentCount)
	{
		Variant newArguments[2] = { arguments, argumentCount };

		const FunctionType & variadicFunc = func.get<FunctionType &>();
		const Variant & underlyingFunc = variadicFunc.getFunc();
		return underlyingFunc.getMetaType()->getMetaCallable()->invoke(underlyingFunc, instance, newArguments, 2);
	}

};


} // namespace metapp


#endif

