#ifndef __AOP_SRC_HAS_MEMBER_H__
#define __AOP_SRC_HAS_MEMBER_H__
#include <type_traits>

#define HAS_MEMBER_FUNCTION(member) \
template<typename T, typename ... Args> \
struct has_member_##member \
{ \
private: \
	template<typename U> \
	static auto test(void*) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	\
	template<typename U>\
	static std::false_type test(...);\
	\
public:\
	static const bool value = decltype(test<T>(nullptr))::value;\
};

#endif