#ifndef REFERENCE_H_969872685611
#define REFERENCE_H_969872685611

#include "metapp/metatype.h"

namespace metapp {

template <typename T>
struct DeclareMetaTypeBase <T &> : public DeclareMetaTypeRoot<T &>
{
	using UpType = T;
	static constexpr TypeKind typeKind = tkReference;

	static void * construct(MetaTypeData * data, const void * copyFrom) {
		if(data != nullptr) {
			data->podAs<T *>() = (T *)copyFrom;
		}
		return nullptr;
	}

	static void * getAddress(const MetaTypeData & data) {
		return data.podAs<void *>();
	}

	static bool canCast(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkReference;
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		return Variant(toMetaType, value.getMetaTypeData());
	}
};


} // namespace metapp


#endif

