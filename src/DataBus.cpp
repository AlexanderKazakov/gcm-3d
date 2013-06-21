#include "DataBus.h"
#include "Body.h"
#include "mesh/TetrMeshSecondOrder.h"

gcm::DataBus::DataBus() {
	INIT_LOGGER("gcm.DataBus");
	MPI_NODE_TYPES = NULL;
	local_numbers = NULL;
	rank = MPI::COMM_WORLD.Get_rank();
	numberOfWorkers = MPI::COMM_WORLD.Get_size();
	createStaticTypes();
}

gcm::DataBus::~DataBus() {
	if (MPI_NODE_TYPES != NULL) {
		LOG_TRACE("Cleaning old types");
		for (int i = 0; i < numberOfWorkers; i++)
		{
			for (int j = 0; j < numberOfWorkers; j++)
			{
				LOG_TRACE("Cleaning type " << i << " " << j );
				LOG_TRACE("Size " << i << " " << j << " = " << local_numbers[i][j].size());
				if (local_numbers[i][j].size() > 0)
					MPI_NODE_TYPES[i][j].Free();
			}
		}
		delete[] MPI_NODE_TYPES;
	}

	if (local_numbers != NULL) {
		for (int i = 0; i < numberOfWorkers; i++)
			delete[] local_numbers[i];
		delete[] local_numbers;
	}
}

void gcm::DataBus::setEngine(IEngine* engine) {
	this->engine = engine;
}

IEngine* gcm::DataBus::getEngine() {
	return engine;
}

void gcm::DataBus::syncTimeStep(float* tau) {
	MPI::COMM_WORLD.Barrier();
	MPI::COMM_WORLD.Allreduce(tau, tau, 1, MPI::FLOAT, MPI::MIN);
}

void gcm::DataBus::syncNodes(float tau)
{
	LOG_DEBUG("Creating dynamic types");
	createDynamicTypes();
	LOG_DEBUG("Creating dynamic types done");
	
	vector<MPI::Request> reqs;
	MPI::COMM_WORLD.Barrier();
	LOG_DEBUG("Starting nodes sync");
	
	Body* body = engine->getBody(0);
	TetrMeshSecondOrder* mesh = (TetrMeshSecondOrder*)body->getMeshes();
	
	for (int i = 0; i < numberOfWorkers; i++)
		for (int j = 0; j < numberOfWorkers; j++)
			if (local_numbers[i][j].size() && i != rank)
			{
				LOG_DEBUG("Sending nodes from zone " << j << " to zone " << i);
				reqs.push_back(
					MPI::COMM_WORLD.Isend(
						&mesh->nodes[0],
						1,
						MPI_NODE_TYPES[i][j],
						i,
						TAG_SYNC_NODE+100*i+j
					)
				);
			}
	
	for (int i = 0; i < numberOfWorkers; i++)
		for (int j = 0; j < numberOfWorkers; j++)
			if (local_numbers[i][j].size() && i == rank)
			{
				LOG_DEBUG("Receiving nodes from zone " << j << " to zone " << i);
				reqs.push_back(
					MPI::COMM_WORLD.Irecv(
						&mesh->nodes[0],
						1,
						MPI_NODE_TYPES[i][j],
						j,
						TAG_SYNC_NODE+100*i+j
					)
				);
			}

	MPI::Request::Waitall(reqs.size(), &reqs[0]);
	
	LOG_DEBUG("Nodes sync done");
	
	/*for(int i = 0; i < engine->getNumberOfBodies(); i++)
	{
		Body* body = engine->getBody(i);
		TetrMeshSecondOrder* mesh = (TetrMeshSecondOrder*)body->getMeshes();
		LOG_DEBUG( "Syncing remote data for mesh " << i );
		float reqH = tau * mesh->getMaxLambda();
		
		LOG_DEBUG( "Mesh sync done" );
	}*/
}

void gcm::DataBus::createStaticTypes()
{
	AABB outlines[2];
	TetrMeshSecondOrder meshes[2];
	TriangleSecondOrder faces[2];
	TetrSecondOrder tetrs[2];

	MPI::Datatype outl_types[] = {
		MPI::LB,
		MPI::FLOAT,
		MPI::FLOAT,
		MPI::UB
	};

	int outl_lengths[] = {
		1,
		3,
		3,
		1
	};

	MPI::Aint outl_displacements[] = {
		MPI::Get_address(&outlines[0]),
		MPI::Get_address(&outlines[0].min_coords[0]),
		MPI::Get_address(&outlines[0].max_coords[0]),
		MPI::Get_address(&outlines[1])
	};

	for (int i = 3; i >=0; i--)
		outl_displacements[i] -= MPI::Get_address(&outlines[0]);

	MPI_OUTLINE = MPI::Datatype::Create_struct(
		4,
		outl_lengths,
		outl_displacements,
		outl_types
	);
	
	MPI::Datatype mesh_outl_types[] = {
		MPI::LB,
		MPI::FLOAT,
		MPI::FLOAT,
		MPI::UB
	};
	
	int mesh_outl_lengths[] = {
		1,
		3,
		3,
		1				
	};
	
	MPI::Aint mesh_outl_displacements[] = {
		MPI::Get_address(&meshes[0]),
		MPI::Get_address(&meshes[0].outline.min_coords[0]),
		MPI::Get_address(&meshes[0].outline.max_coords[0]),
		MPI::Get_address(&meshes[1])
	};
	
	for (int i = 3; i >=0; i--)
		mesh_outl_displacements[i] -= mesh_outl_displacements[0];
	
	MPI_MESH_OUTLINE = MPI::Datatype::Create_struct(
		4,
		mesh_outl_lengths,
		mesh_outl_displacements,
		mesh_outl_types
	);
	
	MPI::Datatype face_types[] = 
	{
		MPI::LB,
		MPI::INT,
		MPI::INT,
		MPI::UB
	};
	
	int face_lens[] = {
		1,
		1,
		3,
		1
	};
	
	MPI::Aint face_displ[] = {
		MPI::Get_address(&faces[0]),
		MPI::Get_address(&faces[0].number),
		MPI::Get_address(&faces[0].verts[0]),
		MPI::Get_address(&faces[1])
	};
	
	for (int i = 3; i >= 0; i--)
		face_displ[i] -= face_displ[0];
	
	MPI_FACE_NUMBERED = MPI::Datatype::Create_struct(
		4,
		face_lens,
		face_displ,
		face_types
	);
	
	MPI::Datatype tetr_types[] = 
	{
		MPI::LB,
		MPI::INT,
		MPI::INT,
		MPI::INT,
		MPI::UB
	};
	
	int tetr_lens[] = {
		1,
		1,
		4,
		6,
		1
	};
	
	MPI::Aint tetr_displ[] = {
		MPI::Get_address(&tetrs[0]),
		MPI::Get_address(&tetrs[0].number),
		MPI::Get_address(&tetrs[0].verts[0]),
		MPI::Get_address(&tetrs[0].addVerts[0]),
		MPI::Get_address(&tetrs[1])
	};
	
	for (int i = 4; i >= 0; i--)
		tetr_displ[i] -= tetr_displ[0];
	
	MPI_TETR_NUMBERED = MPI::Datatype::Create_struct(
		5,
		tetr_lens,
		tetr_displ,
		tetr_types
	);
	
	ElasticNode elnodes[2];
	MPI::Datatype elnode_types[] = {
		MPI::LB,
		MPI::FLOAT,
		MPI::FLOAT,
		MPI::FLOAT,
		MPI::UNSIGNED,
		MPI::UB
	};
	
	int elnode_lens[] = {
		1,
		9,
		3,
		3,
		1,
		1
	};
	
	MPI::Aint elnode_displs[] = {
		MPI::Get_address(&elnodes[0]),
		MPI::Get_address(&elnodes[0].values[0]),
		MPI::Get_address(&elnodes[0].coords[0]),
		MPI::Get_address(&elnodes[0].elasticRheologyProperties[0]),
		MPI::Get_address(&elnodes[0].publicFlags),
		MPI::Get_address(&elnodes[1])
	};
	for (int i = 5; i >= 0; i--)
		elnode_displs[i] -= elnode_displs[0];
	
	MPI_ELNODE = MPI::Datatype::Create_struct(
		6,
		elnode_lens,
		elnode_displs,
		elnode_types
	);
	MPI_ELNODE.Commit();
	
	MPI::Datatype elnoden_types[] = {
		MPI::LB,
		MPI::INT,
		MPI::FLOAT,
		MPI::FLOAT,
		MPI::FLOAT,
		MPI::UNSIGNED,
		MPI::UB
	};
	
	int elnoden_lens[] = {
		1,
		1,
		9,
		3,
		3,
		1,
		1
	};
	
	MPI::Aint elnoden_displs[] = {
		MPI::Get_address(&elnodes[0]),
		MPI::Get_address(&elnodes[0].number),
		MPI::Get_address(&elnodes[0].values[0]),
		MPI::Get_address(&elnodes[0].coords[0]),
		MPI::Get_address(&elnodes[0].elasticRheologyProperties[0]),
		MPI::Get_address(&elnodes[0].publicFlags),
		MPI::Get_address(&elnodes[1])
	};
	for (int i = 6; i >= 0; i--)
		elnoden_displs[i] -= elnoden_displs[0];
	
	MPI_ELNODE_NUMBERED = MPI::Datatype::Create_struct(
		7,
		elnoden_lens,
		elnoden_displs,
		elnoden_types
	);
	
	MPI_ELNODE_NUMBERED.Commit();
	MPI_FACE_NUMBERED.Commit();
	MPI_TETR_NUMBERED.Commit();
	MPI_MESH_OUTLINE.Commit();
	MPI_OUTLINE.Commit();
}

void gcm::DataBus::createDynamicTypes()
{
	LOG_DEBUG("Building dynamic MPI types for fast node sync");
	GCMDispatcher* dispatcher = engine->getDispatcher();
	Body* body = engine->getBody(0);
	TetrMeshSecondOrder* mesh = (TetrMeshSecondOrder*)body->getMeshes();
	
	// TODO add more cleanup code here to prevent memory leaks
	if (MPI_NODE_TYPES != NULL) {
		LOG_TRACE("Cleaning old types");
		for (int i = 0; i < numberOfWorkers; i++)
		{
			for (int j = 0; j < numberOfWorkers; j++)
			{
				LOG_TRACE("Cleaning type " << i << " " << j );
				LOG_TRACE("Size " << i << " " << j << " = " << local_numbers[i][j].size());
				if (local_numbers[i][j].size() > 0)
					MPI_NODE_TYPES[i][j].Free();
			}
		}
		delete[] MPI_NODE_TYPES;
	}

	if (local_numbers != NULL) {
		for (int i = 0; i < numberOfWorkers; i++)
			delete[] local_numbers[i];
		delete[] local_numbers;
	}

	// FIXME
	// it's overhead
	local_numbers = new vector<int>*[numberOfWorkers];
	vector<int> **remote_numbers = new vector<int>*[numberOfWorkers];	
	MPI_NODE_TYPES = new MPI::Datatype*[numberOfWorkers];	
	
	for (int i = 0; i < numberOfWorkers; i++)
	{
		local_numbers[i] = new vector<int>[numberOfWorkers];
		remote_numbers[i] = new vector<int>[numberOfWorkers];
		MPI_NODE_TYPES[i] = new MPI::Datatype[numberOfWorkers];
	}		
	
	ElasticNode* node;
	// find all remote nodes
	for (int j = 0; j < mesh->getNodesNumber(); j++)
	{
		node = &(mesh->nodes[j]);
		if ( node->isRemote() )
		{
			int owner = dispatcher->getOwner(node->coords);
			assert( owner != rank );
			local_numbers[rank][owner].push_back( mesh->nodesMap[node->number] );
			remote_numbers[rank][owner].push_back(node->number);
		}
	}
	MPI::COMM_WORLD.Barrier();
	
	LOG_DEBUG("Requests prepared:");
	for (int i = 0; i < numberOfWorkers; i++)
		for (int j = 0; j < numberOfWorkers; j++)
			LOG_DEBUG("Request size from #" << i << " to #" << j << ": " << local_numbers[i][j].size());

	// sync types
	unsigned int max_len = 0;
	for (int i = 0; i < numberOfWorkers; i++)
		for (int j = 0; j < numberOfWorkers; j++)
			if (local_numbers[i][j].size() > max_len)
				max_len = local_numbers[i][j].size();

	vector<int> lengths;
	for (unsigned int i = 0; i < max_len; i++)
		lengths.push_back(1);
	
	int info[3];
	
	vector<MPI::Request> reqs;
	
	for (int i = 0; i < numberOfWorkers; i++)
		for (int j = 0; j < numberOfWorkers; j++)
			if (local_numbers[i][j].size() > 0)
			{
				info[0] = remote_numbers[i][j].size();
				info[1] = i;
				info[2] = j;
				MPI_NODE_TYPES[i][j] =  MPI_ELNODE.Create_indexed(
					local_numbers[i][j].size(),
					&lengths[0],
					&local_numbers[i][j][0]
				);
				MPI_NODE_TYPES[i][j].Commit();
				reqs.push_back(
					MPI::COMM_WORLD.Isend(
						&remote_numbers[i][j][0],
						remote_numbers[i][j].size(),
						MPI::INT,
						j,
						TAG_SYNC_NODE_TYPES
					)
				);
				reqs.push_back(
					MPI::COMM_WORLD.Isend(
						info,
						3,
						MPI::INT,
						j,
						TAG_SYNC_NODE_TYPES_I
					)
				);
			}

	MPI::COMM_WORLD.Barrier();
	
	MPI::Status status;	
	
	while (MPI::COMM_WORLD.Iprobe(MPI::ANY_SOURCE, TAG_SYNC_NODE_TYPES_I, status))
	{
		MPI::COMM_WORLD.Recv(
			info,
			3,
			MPI::INT,
			status.Get_source(),
			TAG_SYNC_NODE_TYPES_I
		);
		local_numbers[info[1]][info[2]].resize(info[0]);
		MPI::COMM_WORLD.Recv(
			&local_numbers[info[1]][info[2]][0],
			info[0],
			MPI::INT,
			status.Get_source(),
			TAG_SYNC_NODE_TYPES
		);
		if (lengths.size() < (unsigned)info[0])
			for (int i = lengths.size(); i < info[0]; i++)
				lengths.push_back(1);
		for(int i = 0; i < info[0]; i++)
			local_numbers[info[1]][info[2]][i] = mesh->nodesMap[ local_numbers[info[1]][info[2]][i] ];
		MPI_NODE_TYPES[info[1]][info[2]] =  MPI_ELNODE.Create_indexed(
			info[0],
			&lengths[0],
			&local_numbers[info[1]][info[2]][0]
		);
		MPI_NODE_TYPES[info[1]][info[2]].Commit();
	}

	MPI::Request::Waitall(reqs.size(), &reqs[0]);
	MPI::COMM_WORLD.Barrier();
	
	for (int i = 0 ; i < numberOfWorkers; i++)
		delete[] remote_numbers[i];
	delete[] remote_numbers;
	LOG_DEBUG("Building dynamic MPI types for fast node sync done");
}

void gcm::DataBus::syncOutlines()
{
	MPI::COMM_WORLD.Barrier();
	AABB* outlines = engine->getDispatcher()->getOutline(0);
	if( outlines == NULL )
		return;
	AABB* buf = new AABB[numberOfWorkers];
	LOG_DEBUG("Syncing outlines");
	MPI::COMM_WORLD.Allgather(
		&outlines[rank],
		1, MPI_OUTLINE, 
		outlines,
		1, MPI_OUTLINE
	);
	LOG_DEBUG("Outlines synced");
	delete[] buf;
}

void gcm::DataBus::syncMissedNodes(float tau)
{
	bool transferRequired = false;
	AABB reqZones[numberOfWorkers][numberOfWorkers];
	
	GCMDispatcher* dispatcher = engine->getDispatcher();
	
	// FIXME@avasyukov
	// Workaround for SphProxyDispatcher
	// But we still need this 
	if( dispatcher->getOutline(0) == NULL )
		return;
	
	Body* body = engine->getBody(0);
	TetrMeshSecondOrder* mesh = (TetrMeshSecondOrder*)body->getMeshes();
	AABB* areaOfInterest = &(mesh->areaOfInterest);
	if( (mesh->syncedArea).includes( areaOfInterest ) )
	{
		LOG_DEBUG("We need nothing");
	}
	else
	{
		LOG_DEBUG("Yes, we need additional nodes");
		for (int i = 0 ; i < numberOfWorkers; i++)
			if( i != rank)
				areaOfInterest->findIntersection(dispatcher->getOutline(i), &reqZones[rank][i]);
	}
	
	MPI::COMM_WORLD.Barrier();
	MPI::COMM_WORLD.Allgather(
		reqZones[rank],
		numberOfWorkers, MPI_OUTLINE, 
		reqZones,
		numberOfWorkers, MPI_OUTLINE
	);
	
	vector<AABB> *_reqZones = new vector<AABB>[numberOfWorkers];
	for (int i = 0 ; i < numberOfWorkers; i++)
		for (int j = 0 ; j < numberOfWorkers; j++)
		{
			_reqZones[i].push_back(reqZones[i][j]);
			if( !isinf(reqZones[i][j].minX) )
			{
				transferRequired = true;
			}
		}
	
	if(transferRequired)
	{
		transferNodes(_reqZones);
		MPI::COMM_WORLD.Barrier();
		LOG_DEBUG("Rebuilding data types");
		createDynamicTypes();
		LOG_DEBUG("Processing mesh after the sync");
		mesh->preProcess();
		mesh->checkTopology(tau);
		//FIXME@avasyukov - rethink it
		for( int z = 0; z < 3; z++ )
		{
			(mesh->syncedArea).min_coords[z] = (mesh->areaOfInterest).min_coords[z] - EQUALITY_TOLERANCE;
			(mesh->syncedArea).max_coords[z] = (mesh->areaOfInterest).max_coords[z] + EQUALITY_TOLERANCE;
		}
	}
	
	for (int i = 0 ; i < numberOfWorkers; i++)
		_reqZones[i].clear();
	delete[] _reqZones;
}

void gcm::DataBus::transferNodes(vector<AABB>* _reqZones)
{
	AABB reqZones[numberOfWorkers][numberOfWorkers];
	for (int i = 0 ; i < numberOfWorkers; i++)
		for (int j = 0 ; j < numberOfWorkers; j++)
			if( !isinf(_reqZones[i][j].minX) )
			{
				reqZones[i][j] = _reqZones[i].at(j);
				LOG_DEBUG("CPU " << i << " asks from CPU " << j << " area: " << reqZones[i][j]);
			}
	
	Body* body = engine->getBody(0);
	TetrMeshSecondOrder* mesh = (TetrMeshSecondOrder*)body->getMeshes();
	
	int numberOfNodes[numberOfWorkers][numberOfWorkers];
	int numberOfTetrs[numberOfWorkers][numberOfWorkers];
	for (int i = 0 ; i < numberOfWorkers; i++)
		for (int j = 0 ; j < numberOfWorkers; j++)
		{
			numberOfNodes[i][j] = 0;
			numberOfTetrs[i][j] = 0;
		}
	
	// Looking how many nodes and tetrs we are going to send
	map<int,int>* sendNodesMap = new map<int,int>[numberOfWorkers];
	map<int,int>* addNodesMap = new map<int,int>[numberOfWorkers];
	map<int,int>* sendTetrsMap = new map<int,int>[numberOfWorkers];
	
	for (int i = 0 ; i < numberOfWorkers; i++)
	{
		if( !isinf(reqZones[i][rank].minX) )
		{
			for( int j = 0; j < mesh->nodesNumber; j++ )
			{
				ElasticNode* node = &(mesh->nodes[j]);
				if( reqZones[i][rank].isInAABB( node ) )
				{
					numberOfNodes[rank][i]++;
					sendNodesMap[i][ node->number ] = j;
				}
			}
			for( int j = 0; j < mesh->tetrsNumber; j++ )
			{
				TetrSecondOrder* tetr = mesh->getTetr2ByLocalIndex(j);
				if( sendNodesMap[i].find(tetr->verts[0]) != sendNodesMap[i].end() 
					|| sendNodesMap[i].find(tetr->verts[1]) != sendNodesMap[i].end() 
					|| sendNodesMap[i].find(tetr->verts[2]) != sendNodesMap[i].end() 
					|| sendNodesMap[i].find(tetr->verts[3]) != sendNodesMap[i].end() 
						|| sendNodesMap[i].find(tetr->addVerts[0]) != sendNodesMap[i].end() 
						|| sendNodesMap[i].find(tetr->addVerts[1]) != sendNodesMap[i].end() 
						|| sendNodesMap[i].find(tetr->addVerts[2]) != sendNodesMap[i].end() 
						|| sendNodesMap[i].find(tetr->addVerts[3]) != sendNodesMap[i].end() 
						|| sendNodesMap[i].find(tetr->addVerts[4]) != sendNodesMap[i].end() 
						|| sendNodesMap[i].find(tetr->addVerts[5]) != sendNodesMap[i].end() )
				{
					numberOfTetrs[rank][i]++;
					sendTetrsMap[i][ tetr->number ] = j;
					for( int k = 0; k < 4; k++ )
					{
						if( sendNodesMap[i].find(tetr->verts[k]) == sendNodesMap[i].end() 
								&& addNodesMap[i].find(tetr->verts[k]) == addNodesMap[i].end() )
						{
							numberOfNodes[rank][i]++;
							addNodesMap[i][ tetr->verts[k] ] = mesh->getNodeLocalIndex(tetr->verts[k]);
						}
					}
					for( int k = 0; k < 6; k++ )
					{
						if( sendNodesMap[i].find(tetr->addVerts[k]) == sendNodesMap[i].end() 
								&& addNodesMap[i].find(tetr->addVerts[k]) == addNodesMap[i].end() )
						{
							numberOfNodes[rank][i]++;
							addNodesMap[i][ tetr->addVerts[k] ] = mesh->getNodeLocalIndex(tetr->addVerts[k]);
						}
					}
				}
			}
		}
	}
	
	MPI::COMM_WORLD.Barrier();
	MPI::COMM_WORLD.Allgather(
		numberOfNodes[rank],
		numberOfWorkers, MPI_INT, 
		numberOfNodes,
		numberOfWorkers, MPI_INT
	);
	MPI::COMM_WORLD.Allgather(
		numberOfTetrs[rank],
		numberOfWorkers, MPI_INT, 
		numberOfTetrs,
		numberOfWorkers, MPI_INT
	);
	for (int i = 0 ; i < numberOfWorkers; i++)
		for (int j = 0 ; j < numberOfWorkers; j++)
			if( numberOfNodes[i][j] != 0 )
			{
				LOG_DEBUG("CPU " << i << " is going to send to CPU " << j << " " 
						<< numberOfNodes[i][j] << " nodes and " << numberOfTetrs[i][j] << " tetrs");
//				if( rank == j && mesh->getNodesNumber() == 0 )
//					mesh->createNodes(numberOfNodes[i][j]);
//				if( rank == j && mesh->getTetrsNumber() == 0 )
//					mesh->createTetrs(numberOfTetrs[i][j]);
			}
	
	
	vector<MPI::Request> reqs;
	ElasticNode** recNodes = new ElasticNode*[numberOfWorkers];
	TetrSecondOrder** recTetrs = new TetrSecondOrder*[numberOfWorkers];
	for( int i = 0; i < numberOfWorkers; i++ )
	{
		if( i != rank && numberOfNodes[i][rank] > 0 )
		{
			recNodes[i] = new ElasticNode[numberOfNodes[i][rank]];
			recTetrs[i] = new TetrSecondOrder[numberOfTetrs[i][rank]];
			reqs.push_back(
				MPI::COMM_WORLD.Irecv(
					recNodes[i],
					numberOfNodes[i][rank],
					MPI_ELNODE_NUMBERED,
					i,
					TAG_GET_TETRS_N+i
				)
			);
			reqs.push_back(
				MPI::COMM_WORLD.Irecv(
					recTetrs[i],
					numberOfTetrs[i][rank],
					MPI_TETR_NUMBERED,
					i,
					TAG_GET_TETRS_T+i
				)
			);
		}
		else
		{
			recNodes[i] = NULL;
			recTetrs[i] = NULL;
		}
	}
	
	int max_len = 0;
	for (int i = 0; i< numberOfWorkers; i++)
	{
		if (numberOfNodes[rank][i] > max_len)
			max_len = numberOfNodes[rank][i];
		if (numberOfTetrs[rank][i] > max_len)
			max_len = numberOfTetrs[rank][i];
	}
	int *lens = new int[max_len];
	for (int i = 0; i < max_len; i++)
		lens[i] = 1;
	
	MPI::Datatype n[numberOfWorkers];
	MPI::Datatype t[numberOfWorkers];
	vector<int> displ;
	map<int, int>::const_iterator itr;
	for( int i = 0; i < numberOfWorkers; i++ )
	{
		if( i != rank && numberOfNodes[rank][i] > 0 )
		{
			displ.clear();
			for( itr = sendNodesMap[i].begin(); itr != sendNodesMap[i].end(); ++itr )
				displ.push_back(itr->second);
			for( itr = addNodesMap[i].begin(); itr != addNodesMap[i].end(); ++itr )
				displ.push_back(itr->second);
			sort( displ.begin(), displ.end() );

			n[i] = MPI_ELNODE_NUMBERED.Create_indexed(numberOfNodes[rank][i], lens, &displ[0]);
			n[i].Commit();
			
			displ.clear();
			for( itr = sendTetrsMap[i].begin(); itr != sendTetrsMap[i].end(); ++itr )
				displ.push_back(itr->second);
			sort( displ.begin(), displ.end() );
			
			t[i] = MPI_TETR_NUMBERED.Create_indexed(numberOfTetrs[rank][i], lens, &displ[0]);
			t[i].Commit();
			
			reqs.push_back(
				MPI::COMM_WORLD.Isend(
					&(mesh->nodes[0]),
					1,
					n[i],
					i,
					TAG_GET_TETRS_N+rank
				)
			);
			reqs.push_back(
				MPI::COMM_WORLD.Isend(
					&(mesh->tetrs2[0]),//mesh->getTetrByLocalIndex(0),
					1,
					t[i],
					i,
					TAG_GET_TETRS_T+rank
				)
			);
		}
	}
	
	MPI::Request::Waitall(reqs.size(), &reqs[0]);
	MPI::COMM_WORLD.Barrier();
	LOG_DEBUG("Processing received data");
	for( int i = 0; i < numberOfWorkers; i++ )
	{
		if( i != rank && numberOfNodes[i][rank] > 0 )
		{
			LOG_DEBUG("Processing nodes");
			LOG_DEBUG("Worker " << rank << " data from " << i << " size " << numberOfNodes[i][rank]);
			if( mesh->getNodesNumber() == 0 )
			{
				mesh->createNodes( numberOfNodes[i][rank] );
				LOG_DEBUG("Nodes storage created");
			}
			for( int j = 0; j < numberOfNodes[i][rank]; j++ )
			{
				int num = recNodes[i][j].number;
				if( mesh->getNode(num) == NULL )
				{
					recNodes[i][j].setPlacement(Remote);
					mesh->addNode(&recNodes[i][j]);
				}
			}
			LOG_DEBUG("Processing tetrs");
			if( mesh->getTetrsNumber() == 0 )
			{
				mesh->createTetrs( numberOfTetrs[i][rank] );
				LOG_DEBUG("Tetrs storage created");
			}
			for( int j = 0; j < numberOfTetrs[i][rank]; j++ )
			{
				int num = recTetrs[i][j].number;
				if( mesh->getTetr(num) == NULL )
				{
					mesh->addTetr2(&recTetrs[i][j]);
				}
			}
		}
	}
	reqs.clear();
	for( int i = 0; i < numberOfWorkers; i++ )
	{
		if( i != rank && numberOfNodes[rank][i] > 0 )
		{
			n[i].Free();
			t[i].Free();
		}
		if( recNodes[i] != NULL )
			delete[] recNodes[i];
		if( recTetrs[i] != NULL )
			delete[] recTetrs[i];
	}
	delete[] recNodes;
	delete[] recTetrs;
	delete[] sendNodesMap;
	delete[] addNodesMap;
	delete[] sendTetrsMap;
	delete[] lens;
	LOG_DEBUG("Nodes transfer done");
}
