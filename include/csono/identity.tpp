#ifndef CSONO_IDENTITY_TPP
#define CSONO_IDENTITY_TPP

#include <array>

#include <cstdint>



namespace csono::uid {

	class Key {
	public:
		using int_t = uint64_t;

	private:
		std::array<int_t, 4> v;

	public:
		constexpr Key(
				int_t v0 = 0,
				int_t v1 = 0,
				int_t v2 = 0,
				int_t v3 = 0
		): v{v0, v1, v2, v3} { }

		constexpr Key& operator += (Key r) {
			v[0] += r.v[0];
			if(v[0] < r.v[0])  ++(v[1]);
			v[1] += r.v[1];
			if(v[1] < r.v[1])  ++(v[2]);
			v[2] += r.v[2];
			if(v[2] < r.v[2])  ++(v[3]);
			v[3] += r.v[3];
			return *this;
		}
		constexpr Key operator + (Key r) const { r += *this;  return r; }
		constexpr Key& operator ++ () { return (*this) += Key(1); }

		constexpr bool operator == (Key r) const
			{ return ((v[0])==(r.v[0])) && ((v[1])==(r.v[1])) && ((v[2])==(r.v[2])) && ((v[3])==(r.v[3])); }
		constexpr bool operator != (Key r) const { return !((*this) == r); }

		constexpr bool operator < (Key r) const
			{ return ((v[0])<(r.v[0])) || ((v[1])<=(r.v[1])) || ((v[2])<=(r.v[2])) || ((v[3])<=(r.v[3])); }
		constexpr bool operator > (Key r) const
			{ return ((v[0])>(r.v[0])) || ((v[1])>=(r.v[1])) || ((v[2])>=(r.v[2])) || ((v[3])>=(r.v[3])); }

		constexpr bool operator <= (Key r) const { return ! ((*this) > r); }
		constexpr bool operator >= (Key r) const { return ! ((*this) < r); }

		constexpr int_t& operator [] (unsigned int index) { return v[index]; }
		constexpr const int_t & operator [] (unsigned int index) const { return v[index]; }

		constexpr operator int_t () const { return v[0] ^ v[1] ^ v[2] ^ v[3]; }
	};


	/* All generated values are unique between all instances generated
	 * by a Keygen with the same id family. */
	template<class id_type, class id_family>
	class Keygen {
	private:
		static id_type id_mem;
	public:
		static inline id_type newId() { return ++id_mem; }
		static constexpr id_type nullId() { return 0; }
	};

	template<class id_type, class id_family>
	id_type Keygen<id_type, id_family>::id_mem = 0;


	template<typename T, class generator = T>
	class Unique {
	public:
		using key_t = T;
		using gen_t = generator;

	private:
		T id;

		inline Unique(T value): id(value) { }

	public:
		static inline const key_t null_key = gen_t::nullId();
		static inline const Unique<T, generator> null = Unique(null_key);

		inline Unique(): id(gen_t::newId()) { }
		inline Unique(const Unique &): id(gen_t::newId()) { }
		inline Unique(Unique&& mov): id(std::move(mov.id)) { }

		inline T uid() const { return id; }
		inline operator T () const { return id; }

		inline bool isNull() const { return id == gen_t::nullId(); }

		inline Unique& operator = (const Unique &) {
			id = gen_t::newId();  return *this;
		}

		inline Unique& operator = (Unique&& mov) {
			id = std::move(mov.id);
			return *this;
		}
	};


	template<class id_family>
	using Uid = Unique<Key, Keygen<Key, id_family>>;

	using UniversalUid = Uid<void>;

}

#endif
