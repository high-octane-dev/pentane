// https://github.com/dragazo/rustex/blob/master/rustex.h
/*
	MIT License

	Copyright (c) 2020 Cruz Jean

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#pragma once

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <type_traits>
#include <optional>

namespace util
{
	// a rust-style mutex - holds a value of type T and manages access via guards around SharedLockable.
	// in order to ensure the soundness of the data contract, you should not access the stored value except directly via the handles.
	template<typename T, typename SharedLockable = std::shared_mutex>
	class Mutex
	{
	private: // -- detail -- //

		template<typename T_cv, template<typename> typename Locker>
		class guard_common
		{
		private: // -- detail -- //

			T_cv* ptr;
			Locker<SharedLockable> lock;

			friend class Mutex;
			explicit guard_common(const Mutex& m) : ptr(std::addressof(m.raw_val)), lock(m.raw_mut) {}
			explicit guard_common(const Mutex& m, std::adopt_lock_t) : ptr(std::addressof(m.raw_val)), lock(m.raw_mut, std::adopt_lock) {}

		public: // -- access -- //

			T_cv* operator->() const noexcept { return ptr; }
			T_cv& operator*() const noexcept { return *ptr; }
		};

		mutable SharedLockable raw_mut;
		mutable T raw_val;

	public: // -- guards -- //

		// when these guards are moved-from they are considered invalid and should not be used at all afterwards (like rust)
		typedef guard_common<T, std::unique_lock>       mut_guard;
		typedef guard_common<const T, std::shared_lock> guard;

	public: // -- intrinsics -- //

		// constructs the wrapped object of type T with the specified arguments
		template<typename ...Args, std::enable_if_t<std::is_constructible_v<T, Args...>, int> = 0>
		constexpr explicit Mutex(Args &&...args) : raw_val(std::forward<Args>(args)...) {}

		Mutex(const Mutex&) = delete;
		Mutex(Mutex&&) = delete;

		Mutex& operator=(const Mutex&) = delete;
		Mutex& operator=(Mutex&&) = delete;

	public: // -- access -- //

		// returns a handle that facilitates shared readonly access of the wrapped value
		[[nodiscard]] guard lock() const { return guard{ *this }; }
		// returns a handle that facilitates unique read/write access of the wrapped value
		[[nodiscard]] mut_guard lock_mut() const { return mut_guard{ *this }; }

		// attempts to return a handle that facilitates shared readonly access of the wrapped value. returns nullopt on failure.
		[[nodiscard]] std::optional<guard> try_lock() const
		{
			if (raw_mut.try_lock_shared()) return guard{ *this, std::adopt_lock };
			else return std::nullopt;
		}
		// attempts to return a handle that facilitates unique read/write access of the wrapped value. returns nullopt on failure.
		[[nodiscard]] std::optional<mut_guard> try_lock_mut() const
		{
			if (raw_mut.try_lock()) return mut_guard{ *this, std::adopt_lock };
			else return std::nullopt;
		}
	};
}