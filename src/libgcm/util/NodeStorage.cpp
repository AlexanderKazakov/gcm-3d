#include "NodeStorage.hpp"

using namespace gcm;


NodeStorage::NodeStorage() {
	INIT_LOGGER("gcm.NodeStorage");
}


NodeStorage::~NodeStorage() {
	if(memory != nullptr)
		delete[] memory;
	nodes.clear();
	nodesMap.clear();
}


uint NodeStorage::getSize() const {
	return nodes.size();
}


Node &NodeStorage::getNodeByLocalIndex(const uint i) {
	assert_ge(i, 0);
	assert_lt(i, nodes.size());
	return nodes[i];
}


Node &NodeStorage::getNodeByGlobalIndex(const uint i) {
	uint localIndex = getNodeLocalIndex(i);
	assert_ge(localIndex, 0);
	return nodes[localIndex];
}


void NodeStorage::addNode(const Node &node) {
	nodes.push_back(node);
	nodesMap[node.number] = nodes.size() - 1;
}


bool NodeStorage::hasNodeWithGlobalIndex(const uint i) const {
	assert_ge(i, 0);
	auto itr = nodesMap.find(i);
	assert_true(itr != nodesMap.end());
	return itr != nodesMap.end();
}


uint NodeStorage::getNodeLocalIndex(const uint i) const {
	assert_ge(i, 0);
	auto itr = nodesMap.find(i);
	return ( itr != nodesMap.end() ? itr->second : -1 );
}


void NodeStorage::createMemory() {
	if(nodes.size() == 0) return;

	uchar sizeOfODE = nodes[0].getSizeOfODE();
	uchar sizeOfPDE = nodes[0].getSizeOfPDE();

	LOG_DEBUG("Allocate container for " <<
	          sizeOfODE + sizeOfPDE <<
	          " variables per node (both PDE and ODE) for " <<
	          nodes.size() << " nodes");

	memory = new real[nodes.size() * (sizeOfODE + sizeOfPDE)];
	assert_ne(memory, nullptr);
	memset(memory, 0, (nodes.size() * (sizeOfODE + sizeOfPDE)) * sizeof (real));

	for(uint i = 0; i < nodes.size(); i++) {
		nodes[i].initMemory(memory, i);
	}
}


void NodeStorage::clear() {
	if(memory != nullptr)
		delete[] memory;
	nodes.clear();
	nodesMap.clear();
}


void NodeStorage::reserve(uint n) {
	nodes.reserve(n);
	nodesMap.reserve(n);
}
