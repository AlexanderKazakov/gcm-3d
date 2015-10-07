#ifndef GCM_CALC_NODE_H_
#define GCM_CALC_NODE_H_

#include <cmath>
// for memset()
#include <cstring>
#include <iostream>

#include "libgcm/util/Types.hpp"
#include "libgcm/rheology/RheologyMatrix.hpp"

namespace gcm {

/**
 * Base class that contains node data. All other node implementations
 * should derive from this one. Contains basic node information such
 * as number and coordinates.
 *
 */
class Node {
protected:

	/**
	 * Public flags container type.
	 */
	typedef union {
		uchar flags;

		struct {
			uchar isUsed : 1;
			uchar isVirtual : 1;
			uchar isHighOrder : 1;
			uchar isBorder : 1;
			uchar isInContact : 1;
			uchar isDestroyed : 1;
			uchar isContactDestroyed : 1;
			uchar customFlag1 : 1;
		};
	} PublicFlags;

	/**
	 * Error flags container type.
	 */
	typedef union {
		uchar flags;

		struct {
			uchar badNeighborsX : 1;
			uchar badNeighborsY : 1;
			uchar badNeighborsZ : 1;
			uchar badOuterNormal : 1;
		};
	} ErrorFlags;

	/**
	 * Node public flags. These flags are to synchronized using MPI.
	 */
	PublicFlags publicFlags;
	/**
	 * Node flags to indicate different issues with this node during calculations.
	 */
	ErrorFlags errorFlags;

	/**
	 * Number of variables in main PDE (size of \vec{u}).
	 */
	uchar sizeOfPDE;
	/**
	 * Number of variables in supporting ODEs.
	 */
	uchar sizeOfODE;
	/**
	 * Type of node. This id must be unique for each Node subclass.
	 * This field is *really* important - all objects use it to determine node class.
	 * The list of possible types is defined in Types.hpp.
	 */
	uchar nodeType;
	
	/**
	 * Border condition that is used for this node.
	 */
	uchar borderConditionId;
	/**
	 * Contact condition that is used for this node.
	 */
	uchar contactConditionId;

	/**
	 * Identifier of the body this node belongs to.
	 */
	uchar bodyId;

	/**
	 * Node material id.
	 */
	uchar materialId;
	/**
	 * Rheology matrix pointer.
	 */
	RheologyMatrixPtr rheologyMatrix;

	/**
	 * Sets public flags.
	 */
	void setPublicFlags(uchar flags);
	/**
	 * Sets error flags.
	 */
	void setErrorFlags(uchar flags);

	/**
	 * Sets node type.
	 * Argument must be an id of some Node subclass.
	 * The list of possible types is defined in Types.hpp
	 */
	void setType(uchar type);


public:

	/**
	 * Number of the node.
	 */
	int number;
	/**
	 * Node coords.
	 */
	vector3r coords;

	/**
	 * Variables in main PDE (size of \vec{u}, >= 9)
	 */
	real *PDE;
	/**
	 * Variables in supporting ODEs (may be empty)
	 */
	real *ODE;

	Node();
	/**
	 * Default node constructor. Creates new node with all members
	 * set to default values;
	 */
	Node(uchar nodeType, uchar sizeOfValuesInPDE, uchar sizeOfValuesInODE);
	/**
	 * Copy constructor
	 */
	// CalcNode(const Node& src);
	/**
	 * Constructor. Creates node with specified number assigned.
	 *
	 * @param num Number of the node
	 */
	// CalcNode(int num);
	/**
	 * Constructor. Creates node with specified number assigned
	 * and set coordinates.
	 *
	 * @param num Number of the node
	 * @param coords Node coords
	 */
	// CalcNode(int num, const vector3r& coords);
	/**
	 * Assign operator
	 */
	void operator=(const Node& orig);
	/**
	 * Destructor
	 */
	~Node();

	/**
	 * Number of pair node in contact. FIXME
	 */
	int contactNodeNum;
	/**
	 * Contact direction. FIXME
	 */
	uchar contactDirection;

	/**
	 * Set all values in PDE and ODE in the node to zero.
	 */
	void clear();
	/**
	 * Allocate memory for values in PDE and ODE in the node from previously 
	 * pre-allocated memory
	 * @param buffer pointer to pre-allocated for all nodes memory 
	 * @param nodeNum index number of the node among all nodes
	 */
	void initMemory(real *buffer, int nodeNum);
	/** 
	 * Returns node type. The value is an id of some Node subclass.
	 * The list of possible types is defined in Types.hpp.
	 * 
	 * @return Type of the node
	 */
	uchar getType() const;

	/**
	 * @return number of variables in main PDE (size of \vec{u}, >= 9)
	 */
	uchar getSizeOfPDE() const;

	/**
	 * @return number of variables in supporting ODEs (may be empty)
	 */
	uchar getSizeOfODE() const;

	/**
	 * Sets node material id.  Sets material id and updates values for
	 * node material-related fields (e.g. rho)
	 *
	 * @param id Material id
	 */
	void setMaterialId(uchar id);
	/**
	 * Returns node material id.
	 *
	 * @return Material id.
	 */
	uchar getMaterialId() const;
	/**
	 * Returns node material.
	 *
	 * @return Material.
	 */
//	MaterialPtr getMaterial() const /*override*/;
	/**
	 * Sets rheology matrix for node.
	 */
	void setRheologyMatrix(RheologyMatrixPtr matrix);
	/**
	 * Returns rheology matrix for node. It's wrapper for corresponding
	 * material APIs.
	 *
	 * @return Rheology matrix.
	 */
	RheologyMatrixPtr getRheologyMatrix() const;

	/**
	 * Indicates whether node is a real part of mesh, is used and its state should be calculated.
	 * Example use case: imported meshes may contain nodes that were used during mesh generation 
	 * only and are not connected to any other nodes in the mesh (like centers of arcs and so on).
	 *
	 * @return True in case of used node and false otherwise.
	 */
	bool isUsed() const;
	/**
	 * Sets new used state of the node
	 *
	 * @param value Calculatable state.
	 */
	void setUsed(bool value);

	/**
	 * Indicates whether node if first order (verticle of mesh element - tetrahedron, cube, etc)
	 * or high order (addition node on edge or whatever).
	 *
	 * @return True in case of border node and false otherwise.
	 */
	bool isHighOrder() const;
	/**
	 * Sets new high order mark of the node
	 *
	 * @param value High order mark.
	 */
	void setHighOrder(bool value);

	/**
	 * Indicates whether node is real (part of mesh) or virtual (interpolated one).
	 *
	 * @return True in case of virtual node and false otherwise.
	 */
	bool isVirtual() const;
	/**
	 * Sets new virtual state of the node
	 *
	 * @param value Virtual state.
	 */
	void setVirtual(bool value);

	/**
	 * Indicates whether node belongs to border. Border means both (a) real border of the body 
	 * and (b) just border of zone of the body created for parallel computation).
	 *
	 * @return True in case of border node and false otherwise.
	 */
	bool isBorder() const;
	/**
	 * Indicates whether node does not belong to border.
	 *
	 * @return True in case of inner node and false otherwise.
	 */
	bool isInner() const;
	/**
	 * Sets new border state of the node
	 *
	 * @param value Border state.
	 */
	void setBorder(bool value);

	/**
	 * Indicates whether node is in contact or not.
	 *
	 * @return True in case of contact and false otherwise.
	 */
	bool isInContact() const;
	/**
	 * Sets new contact state of the node
	 *
	 * @param value Contact state.
	 */
	void setInContact(bool value);

	/**
	 * Indicates whether node is in destroyed area
	 *
	 * @return True in case of node belonging to destroyed area
	 */
	bool isDestroyed() const;
	/**
	 * Sets new destroyed state of the node
	 *
	 * @param value Destroyed state.
	 */
	void setDestroyed(bool value);

	/**
	 * Indicates whether node is on destroyed contact
	 *
	 * @return True in case of destroyed contact and false otherwise.
	 */
	bool isContactDestroyed() const;
	/**
	 * Sets new contact destoyed state of the node
	 *
	 * @param value Contact destoyed state.
	 */
	void setContactDestroyed(bool value);

	/**
	 * Returns value of specified custom flag.
	 *
	 * @param flag Flag to get value of.
	 * @return Flag state.
	 */
	bool getCustomFlag(uchar flag) const;
	/**
	 * Sets value of custom flag.
	 *
	 * @param flag Flag to set value of.
	 * @param value New flag value
	 */
	void setCustomFlag(uchar flag, bool value);


	// Methods to set flags regarding different non-fatal issues during computation

	/**
	 * Sets node neigh error flag depending on specified axis. TODO document
	 *
	 * @param axisName Number of axis to set error flag for.
	 */
	void setBadNeighbors(unsigned int axisNum);
	/**
	 * Sets node xneigh error flag. TODO document
	 */
	void setBadNeighborsX();
	/**
	 * Sets node yneigh error flag. TODO document
	 */
	void setBadNeighborsY();
	/**
	 * Sets node zneigh error flag. TODO document
	 */
	void setBadNeighborsZ();
	/**
	 * Sets node outer normal error flag. TODO document
	 */
	void setBadOuterNormal();
	/**
	 * Clears error flags.
	 */
	void clearErrorFlags();

	/**
	 * Returns public flags for node.
	 *
	 * @return Public flags
	 */
	uchar getPublicFlags() const;
	/**
	 * Returns error flags for node.
	 *
	 * @return Error flags
	 */
	uchar getErrorFlags() const;

	/**
	 * Sets border condition id. TODO document
	 *
	 * @param  newBorderConditionId Id of the new border condition.
	 */
	void setBorderConditionId(uchar newBorderConditionId);
	/**
	 * Returns id of the node border condition. TODO document
	 *
	 * @return Id of the border condition.
	 */
	uchar getBorderConditionId() const;

	/**
	 * Sets contact condition id. TODO document
	 *
	 * @param  newContactConditionId Id of the new contact condition.
	 */
	void setContactConditionId(uchar newContactConditionId);
	/**
	 * Returns id of the node contact condition. TODO document
	 *
	 * @return Id of the contact condition.
	 */
	uchar getContactConditionId() const;

	/**
	 * Constant to access custom flag1 using getCustomFlag / setCustomFlag.
	 */
	static const uchar FLAG_1 = 1;
};
}

namespace std {

inline std::ostream& operator<<(std::ostream &os, const gcm::Node &node) {
	os << "\n\tNode number: " << node.number << "\n";
	os << "\tCoords:";
	for (int i = 0; i < 3; i++)
		os << " " << node.coords[i];
	os << "\n\tValues in PDE:";
	for (int i = 0; i < node.getSizeOfPDE(); i++)
		os << " " << node.PDE[i];
	os << "\n\tValues in ODE:";
	for (int i = 0; i < node.getSizeOfODE(); i++)
		os << " " << node.ODE[i];
	os << "\n\tFlags: " << node.getPublicFlags();
	os << "\n\tErrors: " << node.getErrorFlags();
	return os;
}
}

#endif