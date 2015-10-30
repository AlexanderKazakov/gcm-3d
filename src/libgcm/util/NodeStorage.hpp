#ifndef NODESTORAGE_HPP
#define	NODESTORAGE_HPP

#include "libgcm/util/Singleton.hpp"
#include "libgcm/util/Types.hpp"


namespace gcm {
	/**
	 * The storage of nodes and their dynamical memory.
	 * Represents std::vector<Node> and real* together. 
	 * There are stupid support for nodes number increase.
	 */
	class NodeStorage {
	public:
		NodeStorage();
	private:
		/* The memory pointer */
		real* memory = nullptr;
		/* Size of preallocated at this very moment memory */
		uint preallocatedSize;
		/* Size of used at this very moment memory. It's less than preallocatedSize */
		uint usedSize;
		/* When usedSize goes to be more than preallocatedSize, 
		 * increase the last one by this ratio */
		const real storageOnDemandGrowRatio = 1.25;
		
	};
}

#endif	/* NODESTORAGE_HPP */

