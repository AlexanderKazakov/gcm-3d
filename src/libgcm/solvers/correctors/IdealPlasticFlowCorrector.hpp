#ifndef GCM_IDEALPLASTICFLOWCORRECTOR_HPP
#define GCM_IDEALPLASTICFLOWCORRECTOR_HPP 

#include "libgcm/solvers/correctors/Corrector.hpp"

namespace gcm
{
	class IdealPlasticFlowCorrector : public Corrector {
	public:
		void apply(Mesh *mesh) override;
		
	};
}
#endif /* GCM_IDEALPLASTICFLOWCORRECTOR_HPP */
