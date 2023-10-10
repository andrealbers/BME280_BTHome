/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_KOBJECT_H
#define Z_INCLUDE_SYSCALLS_KOBJECT_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void z_impl_k_object_access_grant(const void * object, struct k_thread * thread);

__pinned_func
static inline void k_object_access_grant(const void * object, struct k_thread * thread)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const void * val; } parm0 = { .val = object };
		union { uintptr_t x; struct k_thread * val; } parm1 = { .val = thread };
		(void) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_K_OBJECT_ACCESS_GRANT);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_object_access_grant(object, thread);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define k_object_access_grant(object, thread) do { 	sys_port_trace_syscall_enter(K_SYSCALL_K_OBJECT_ACCESS_GRANT, k_object_access_grant, object, thread); 	k_object_access_grant(object, thread); 	sys_port_trace_syscall_exit(K_SYSCALL_K_OBJECT_ACCESS_GRANT, k_object_access_grant, object, thread); } while(false)
#endif
#endif


extern void z_impl_k_object_release(const void * object);

__pinned_func
static inline void k_object_release(const void * object)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const void * val; } parm0 = { .val = object };
		(void) arch_syscall_invoke1(parm0.x, K_SYSCALL_K_OBJECT_RELEASE);
		return;
	}
#endif
	compiler_barrier();
	z_impl_k_object_release(object);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define k_object_release(object) do { 	sys_port_trace_syscall_enter(K_SYSCALL_K_OBJECT_RELEASE, k_object_release, object); 	k_object_release(object); 	sys_port_trace_syscall_exit(K_SYSCALL_K_OBJECT_RELEASE, k_object_release, object); } while(false)
#endif
#endif


extern void * z_impl_k_object_alloc(enum k_objects otype);

__pinned_func
static inline void * k_object_alloc(enum k_objects otype)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; enum k_objects val; } parm0 = { .val = otype };
		return (void *) arch_syscall_invoke1(parm0.x, K_SYSCALL_K_OBJECT_ALLOC);
	}
#endif
	compiler_barrier();
	return z_impl_k_object_alloc(otype);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define k_object_alloc(otype) ({ 	void * retval; 	sys_port_trace_syscall_enter(K_SYSCALL_K_OBJECT_ALLOC, k_object_alloc, otype); 	retval = k_object_alloc(otype); 	sys_port_trace_syscall_exit(K_SYSCALL_K_OBJECT_ALLOC, k_object_alloc, otype, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
