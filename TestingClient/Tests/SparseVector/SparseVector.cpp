#include "SparseVector.hpp"

#include "ButchersToolbox/SparseVector.hpp"


namespace SparseVector_tests {

	void testCorrectness()
	{
		SparseVector<uint32_t> vec;
		vec.resize(4);
		vec[0] = 0;
		vec[1] = 1;
		vec[2] = 2;
		vec[3] = 3;

		// Test looping
		uint32_t i = 0;
		for (auto val : vec) {
			switch (i) {
			case 0:
				assert(val == 0);
				break;
			case 1:
				assert(val == 1);
				break;
			case 2:
				assert(val == 2);
				break;
			case 3:
				assert(val == 3);
				break;
			default:
				break;
			}
			i++;
		}

		// Test erase
		vec.erase(1);

		for (auto val : vec) {
			switch (val) {
			case 0:
				assert(val == 0);
				break;
			case 1:
				throw std::exception();
				break;
			case 2:
				assert(val == 2);
				break;
			case 3:
				assert(val == 3);
				break;
			default:
				break;
			}
		}
	}
}