#ifndef ELEMENT_HPP
#define	ELEMENT_HPP

#include "libgcm/util/Types.hpp"

namespace gcm {
	/**
	 * Base class to inherit all mesh elements (tetrahedrons, cubes, etc.)
	 */
	class Element {
	public:
		Element();
		uint number;
		
	private:

	};
}

#endif	/* ELEMENT_HPP */

