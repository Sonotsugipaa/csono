#include <iostream>

#include "testdefs.hpp"

#include <csono/identity.tpp>



namespace csono::test {

	bool perform(const char * what, bool outcome) {
		std::cout << (outcome? OK"  <--" : NO"  <") << "----  " << what << std::endl;
		return outcome;
	}

	bool perform() {
		using unique_t = uid::Unique<uid::Key, uid::Keygen<uid::Key, float>>;

		uid::Key k1 = uid::Key(1,2,3,4);
		uid::Key k2 = uid::Key(4,3,2,1);
		uid::Key k3 = uid::Key(1,2,3,4);
		unique_t u1, u2;
		unique_t ucopy = u1;

		return
			// Key comparison
				  perform("key comparison !=", k1 != k2)
				& perform("key comparison >", k2  > k1)
				& perform("key comparison <", k1  < k2)
				& perform("key comparison ==", k1 == k3)
			// Key member element
				& perform("key [1] == [2]", k1[1] == k2[2])
				& perform("key [2] ==  2", k2[2] == 2)
			// Unique uniqueness
				& perform("unique comparison !=", u1.uid() != u2.uid())
				& perform("unique comparison != (copy)", u1.uid() != ucopy.uid())
			// Unique null value
				& perform(
						"unique null comparison ==",
						unique_t::gen_t::nullId() ==
						unique_t::gen_t::nullId() )
				& perform(
						"unique null comparison !=",
						u1.uid() != unique_t::gen_t::nullId() )
				& perform("unique ! isNull()", ! u1.isNull())
				& perform(
						"unique   isNull()",
						unique_t::null.isNull() );
	}

}


int main(int, char**) {
	bool result = csono::test::perform();
	std::cout << "\nResult: " << (result? SUCCESS : FAILURE) << std::endl;
	return result? EXIT_SUCCESS : EXIT_FAILURE;
}
