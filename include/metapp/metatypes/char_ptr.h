#ifndef METAPP_CHAR_PTR_H_969872685611
#define METAPP_CHAR_PTR_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metatypes/utils/declareutil.h"
// Need the specialization to be included first
#include "metapp/metatypes/pointer.h"

#include <string>

namespace metapp {

template <typename T>
struct DeclareCharPtrBase
{
private:
	using super = DeclareMetaTypeObject <T>;

public:
	using UpType = typename std::remove_pointer<typename std::decay<T>::type>::type;
	static constexpr TypeKind typeKind = tkCharPtr;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return super::canCast(value, toMetaType)
			|| canCastToStdString(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToStdString(toMetaType)) {
			return std::string(value.get<const char *>());
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToStdString(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkStdString;
	}
};

template <>
struct DeclareMetaType <const char *> : public DeclareCharPtrBase <const char *>
{
};

template <>
struct DeclareMetaType <char *> : public DeclareCharPtrBase <char *>
{
};

template <>
struct DeclareMetaType <char[]> : public DeclareCharPtrBase <char[]>
{
};

template <size_t length>
struct DeclareMetaType <char[length]> : public DeclareCharPtrBase <char[length]>
{
};


template <typename T>
struct DeclareWideCharPtrBase
{
private:
	using super = DeclareMetaTypeObject <T>;

public:
	using UpType = typename std::remove_pointer<typename std::decay<T>::type>::type;
	static constexpr TypeKind typeKind = tkWideCharPtr;

	static bool canCast(const Variant & value, const MetaType * toMetaType) {
		return super::canCast(value, toMetaType)
			|| canCastToStdWstring(toMetaType);
	}

	static Variant cast(const Variant & value, const MetaType * toMetaType) {
		if(canCastToStdWstring(toMetaType)) {
			return std::wstring(value.get<const wchar_t *>());
		}
		else {
			return super::cast(value, toMetaType);
		}
	}

private:
	static bool canCastToStdWstring(const MetaType * toMetaType) {
		return toMetaType->getTypeKind() == tkStdWideString;
	}
};

template <>
struct DeclareMetaType <const wchar_t *> : public DeclareWideCharPtrBase <const wchar_t *>
{
};

template <>
struct DeclareMetaType <wchar_t *> : public DeclareWideCharPtrBase <wchar_t *>
{
};

template <>
struct DeclareMetaType <wchar_t[]> : public DeclareWideCharPtrBase <wchar_t[]>
{
};

template <size_t length>
struct DeclareMetaType <wchar_t[length]> : public DeclareWideCharPtrBase <wchar_t[length]>
{
};


} // namespace metapp


#endif

