#ifndef GCM_CORRECTOR_HPP
#define GCM_CORRECTOR_HPP

#include "libgcm/meshes/Mesh.hpp"

namespace gcm {
	class Mesh;
	
	class Corrector {
	public:
		virtual void apply(Mesh *mesh) = 0;	
	private:

	};
	
	
	typedef std::shared_ptr<Corrector> CorrectorPtr;

	template<typename T, typename...Args>
	std::shared_ptr<T> makeCorrectorPtr(Args...args) {
		return std::make_shared<T>(args...);
	}
}

#endif /* GCM_CORRECTOR_HPP */