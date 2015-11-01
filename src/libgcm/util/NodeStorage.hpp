#ifndef NODESTORAGE_HPP
#define	NODESTORAGE_HPP

#include <vector>
#include <unordered_map>
#include <libgcm/node/Node.hpp>
#include "libgcm/util/Types.hpp"


namespace gcm {
	/**
	 * The storage of nodes, their local-global index map
	 * and dynamical memory.
	 * Represents std::vector<Node>, std::unordered_map
	 * and real* together.
	 */
	class NodeStorage {
	public:
		NodeStorage();
		/**
		 * Clear containers and delete allocated memory
		 */
		~NodeStorage();
		/**
		 * @return size of %nodes
		 */
		uint getSize() const;
		/**
		 * @return reference to %nodes[i]
		 */
		Node& getNodeByLocalIndex(const uint i);
		/**
		 * @return reference to node with global index == i
		 */
		Node& getNodeByGlobalIndex(const uint i);
		/**
		 * Add node without memory to %nodes
		 */
		void addNode(const Node& node);
		/**
		 * If node with global index i is stored
		 */
		bool hasNodeWithGlobalIndex(const uint i) const;
		/**
		 * @return local index of node with global index == i
		 */
		uint getNodeLocalIndex(const uint i) const;
		/**
		 * Allocate memory for existing nodes.
		 */
		void createMemory();
		/**
		 * Free %memory and clear %nodes and %nodesMap.
		 */
		void clear();
		/**
		 * Reserve memory in %nodes
		 */
		void reserve(uint n);

	private:
		/* The nodes; nodes.size() is corresponding to size of allocated memory */
		std::vector<Node> nodes;
		/* The memory pointer; nodes.size() is corresponding to size of allocated memory */
		real* memory = nullptr;
		/* The map between global and local indexes of nodes */
		std::unordered_map<uint, uint> nodesMap;


		USE_LOGGER;

	};
}

#endif	/* NODESTORAGE_HPP */

