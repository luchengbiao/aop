#ifndef __AOP_SRC_ASPECT_H__
#define __AOP_SRC_ASPECT_H__
#include "has_member.h"
#include "macros.h"

AOP_NAMESPACE_BEGIN

HAS_MEMBER_FUNCTION(Before);
HAS_MEMBER_FUNCTION(After);

template<typename Func, typename... Args>
struct Aspect
{
	Aspect(Func&& func) : func_(std::forward<Func>(func)) {}
	Aspect(const Aspect&) = delete;
	Aspect& operator=(const Aspect&) = delete;

	template<typename T, typename = std::enable_if_t<has_member_Before<T, Args...>::value>>
	void InvokeBefore(T&& aspect, Args&&... args)
	{
		aspect.Before(std::forward<Args>(args)...);
	}

	void InvokeBefore(...) {}

	template<typename T, typename = std::enable_if_t<has_member_After<T, Args...>::value>>
	void InvokeAfter(T&& aspect, Args&&... args)
	{
		aspect.After(std::forward<Args>(args)...);
	}

	void InvokeAfter(...) {}

	template<typename T>
	void Invoke(Args&&... args, T&& aspect)
	{
		InvokeBefore(std::forward<T>(aspect), std::forward<Args>(args)...);
		func_(std::forward<Args>(args)...);
		InvokeAfter(std::forward<T>(aspect), std::forward<Args>(args)...);
	}

	template<typename Head, typename... Tail>
	void Invoke(Args&&... args, Head&& head, Tail&&... tail)
	{
		InvokeBefore(std::forward<Head>(head), std::forward<Args>(args)...);
		Invoke(std::forward<Args>(args)..., std::forward<Tail>(tail)...);
		InvokeAfter(std::forward<Head>(head), std::forward<Args>(args)...);
	}

private:
	Func func_;
};

template<typename T>
using identity_t = typename std::identity<T>::type;

template<typename... AP, typename Func, typename... Args>
void Invoke(Func&& func, Args&&... args)
{
	Aspect<Func, Args...> asp(std::forward<Func>(func));
	asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}

template<typename... AP, typename Func, typename... Args>
void InvokeWithAspects(AP&&...ap, Func&& func, Args&&... args)
{
	Aspect<Func, Args...> asp(std::forward<Func>(func));
	asp.Invoke(std::forward<Args>(args)..., std::forward<AP>(ap)...);
}

AOP_NAMESPACE_END

#endif