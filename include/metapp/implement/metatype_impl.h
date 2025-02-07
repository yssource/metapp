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

#ifndef METAPP_METATYPE_IMPL_H_969872685611
#define METAPP_METATYPE_IMPL_H_969872685611

#include "metapp/implement/internal/construct_i.h"

#include <atomic>

namespace metapp {

namespace internal_ {

template <typename T>
struct UpTypeGetter;

template <typename Type0, typename ...Types>
struct UpTypeGetter <TypeList<Type0, Types...> >
{
	static const MetaType ** makeUpTypeList()
	{
		static std::array<const MetaType *, sizeof...(Types) + 1> upTypeList {
			getMetaType<Type0>(),
			getMetaType<Types>()...,
		};
		return upTypeList.data();
	}

	static UpTypeData getUpType() {
		return {
			(int)(sizeof...(Types) + 1),
			makeUpTypeList()
		};
	}
};

template <>
struct UpTypeGetter <TypeList<> >
{
	static UpTypeData getUpType() {
		return {
			0,
			nullptr
		};
	}
};

template <typename T>
struct UpTypeGetter
{
	static UpTypeData getUpType() {
		return UpTypeGetter<TypeList<T> >::getUpType();
	}
};

template <typename T, bool has>
using SelectDeclareClass = typename std::conditional<
	has,
	DeclareMetaType<T>,
	CommonDeclareMetaType<T>
>::type;

template <typename T> struct DeepRemoveCv { using Type = T; };
template <typename T> struct DeepRemoveCv <T const> : DeepRemoveCv<T> {};
template <typename T> struct DeepRemoveCv <T volatile> : DeepRemoveCv<T> {};
template <typename T> struct DeepRemoveCv <T const volatile> : DeepRemoveCv<T> {};
template <typename T> struct DeepRemoveCv <T *> { using Type = typename DeepRemoveCv<T>::Type *; };
template <typename T> struct DeepRemoveCv <T &> { using Type = typename DeepRemoveCv<T>::Type &; };
template <typename T> struct DeepRemoveCv <T &&> { using Type = typename DeepRemoveCv<T>::Type &&; };

template <typename T>
const void * unifiedDataGetter()
{
	return (const void *)&commonCast;
}

template <typename T>
const MetaType * doGetMetaTypeStorage()
{
	using M = DeclareMetaType<T>;

	static const MetaType metaType(
		&unifiedDataGetter<typename DeepRemoveCv<T>::Type>,
		doGetUnifiedType<typename std::remove_cv<T>::type>(),
		SelectDeclareClass<T, HasMember_typeFlags<M>::value>::typeFlags | CommonDeclareMetaType<T>::typeFlags
	);
	return &metaType;
}

template <typename T>
constexpr  auto doGetMetaType()
	-> typename std::enable_if<! HasMember_setup<DeclareMetaType<T> >::value, const MetaType *>::type
{
	return doGetMetaTypeStorage<T>();
}

template <typename T>
auto doGetMetaType()
	-> typename std::enable_if<HasMember_setup<DeclareMetaType<T> >::value,	const MetaType *>::type
{
	static std::atomic_flag hasCalledSetup;
	const MetaType * metaType = doGetMetaTypeStorage<T>();
	if(! hasCalledSetup.test_and_set()) {
		DeclareMetaType<T>::setup();
	}
	return metaType;
}

template <typename T>
const UnifiedType * doGetUnifiedType()
{
	using M = DeclareMetaType<T>;

	static const UnifiedType unifiedType(
		SelectDeclareClass<T, HasMember_typeKind<M>::value>::typeKind,
		UnifiedMetaTable{
			SelectDeclareClass<T, HasMember_constructData<M>::value>::constructData,
			SelectDeclareClass<T, HasMember_destroy<M>::value>::destroy,
			SelectDeclareClass<T, HasMember_cast<M>::value>::cast,
			SelectDeclareClass<T, HasMember_castFrom<M>::value>::castFrom,

			MakeMetaInterfaceData<T>::getMetaInterfaceData(),
		},
		UpTypeGetter<
			typename SelectDeclareClass<T, HasMember_UpType<M>::value>::UpType
		>::getUpType()
	);
	return &unifiedType;
}

} // namespace internal_

template <typename T>
inline void * CommonDeclareMetaType<T>::constructData(VariantData * data, const void * copyFrom)
{
	if(data != nullptr) {
		data->construct<Underlying>(copyFrom);
		return nullptr;
	}
	else {
		return internal_::constructOnHeap<Underlying>(copyFrom);
	}
}

template <typename T>
inline void CommonDeclareMetaType<T>::destroy(void * instance)
{
#if defined(METAPP_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
#if defined(METAPP_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-abstract-non-virtual-dtor"
#endif
	delete static_cast<Underlying *>(instance);
#if defined(METAPP_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif
#if defined(METAPP_COMPILER_GCC)
#pragma GCC diagnostic pop
#endif
}

template <typename T>
inline bool CommonDeclareMetaType<T>::cast(Variant * result, const Variant & value, const MetaType * toMetaType)
{
	return commonCast(result, value, getMetaType<T>(), toMetaType);
}

template <typename T>
inline constexpr const MetaType * getMetaType()
{
	return internal_::doGetMetaType<T>();
}


} // namespace metapp


#endif
