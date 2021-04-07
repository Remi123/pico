#pragma once

/*	jsch is a stack-based scheduler based on Emil Fresk's crect library
 *	We do not provide any garantee as of yet.
 *
 *	The goal is to create a reactive RTOS which can be configured to 
 *	run on different hardware, with Raspberry Pi Pico's RP2040 chip based
 *	on Cortex M0+ as the first "client".
 *
 */
#include <type_traits>
#include <utility> // For pair

#include <boost/callable_traits/args.hpp>
#include <boost/heap/priority_queue.hpp> 
#include <boost/tuple/tuple.hpp>

#include <type_expr.hpp> // Compile-time composition


template<typename ... Jobs>
class jsch
{
	template<typename T> 
	using get_args = boost::callable_traits::args_t<T,te::ts_>;
public :
	using res_list = te::eval_pipe_<te::ts_<Jobs...>,
			te::transform_<te::wraptype_<std::decay>,te::wrap_<get_args>>>;
	using job_list = te::eval_pipe_<te::ts_<Jobs...>,
			te::transform_<te::wraptype_<std::decay_t>>, 
			te::wrap_<boost::tuple>>;
private :
	job_list tup_job;

public:
	jsch(Jobs& ... js) : tup_job{js...}
	{

	}
	jsch(Jobs&& ... js) : tup_job{std::forward<Jobs>(js)...}
	{

	}

};

template<typename ... Jobs>
auto make_jsch(Jobs ... js) -> jsch<Jobs...>
{
	return jsch<Jobs...>(js...);
}
