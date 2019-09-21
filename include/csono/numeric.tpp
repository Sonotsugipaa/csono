#ifndef CSONO_NUMERIC_TPP
#define CSONO_NUMERIC_TPP



inline namespace csono {
	inline namespace num {

		template<typename num>
		constexpr num range_morph(
				num value,
				num from_low, num from_high,
				num to_low,   num to_high
		) {
			value *= (to_high   - to_low);
			value /= (from_high - from_low);
			value +=  to_low    - from_low;
			return value;
		}

	}
}

#endif
