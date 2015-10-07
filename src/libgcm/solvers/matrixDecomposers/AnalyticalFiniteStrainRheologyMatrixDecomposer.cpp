#include "libgcm/solvers/matrixDecomposers/AnalyticalFiniteStrainRheologyMatrixDecomposer.hpp"

using namespace gcm;

AnalyticalFiniteStrainRheologyMatrixDecomposer::
                              AnalyticalFiniteStrainRheologyMatrixDecomposer() {
}

AnalyticalFiniteStrainRheologyMatrixDecomposer::
                              ~AnalyticalFiniteStrainRheologyMatrixDecomposer() {
}

void AnalyticalFiniteStrainRheologyMatrixDecomposer::decomposeX(const GcmMatrix& a, 
                                                                      GcmMatrix& u, 
                                                                      GcmMatrix& l, 
                                                                      GcmMatrix& u1) const
{
    l.clear();
    u.clear();
    u1.clear();
    
	// AnalyticalRheologyMatrixDecomposer is used for decomposition of 
	// 9x9 submatrix of matrix a which fortunately help in this case to  
	// decompose all 10x10 matrix a.
    AnalyticalRheologyMatrixDecomposer decomposer9;
    decomposer9.decomposeX(a, u, l, u1);
    
    l(9, 9) = 0;
    u1(9, 9) = u(9, 9) = 1;
    
    for(int i = 0; i < 6; i++)
        u1(9, i) = a.get(9, 0) * u1(0, i) / l(i, i);
        
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 6; j++)
            u(9, i) -= u1(9, j) * u(j, i);
}

void AnalyticalFiniteStrainRheologyMatrixDecomposer::decomposeY(const GcmMatrix& a, 
                                                                      GcmMatrix& u, 
                                                                      GcmMatrix& l, 
                                                                      GcmMatrix& u1) const
{
    l.clear();
    u.clear();
    u1.clear();
    
	// AnalyticalRheologyMatrixDecomposer is used for decomposition of 
	// 9x9 submatrix of matrix a which fortunately help in this case to  
	// decompose all 10x10 matrix a.
    AnalyticalRheologyMatrixDecomposer decomposer9;
    decomposer9.decomposeY(a, u, l, u1);
    
    l(9, 9) = 0;
    u1(9, 9) = u(9, 9) = 1;
    
    for(int i = 0; i < 6; i++)
        u1(9, i) = a.get(9, 1) * u1(1, i) / l(i, i);
        
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 6; j++)
            u(9, i) -= u1(9, j) * u(j, i);
}

void AnalyticalFiniteStrainRheologyMatrixDecomposer::decomposeZ(const GcmMatrix& a, 
                                                                      GcmMatrix& u, 
                                                                      GcmMatrix& l, 
                                                                      GcmMatrix& u1) const
{
    l.clear();
    u.clear();
    u1.clear();
    
	// AnalyticalRheologyMatrixDecomposer is used for decomposition of 
	// 9x9 submatrix of matrix a which fortunately help in this case to  
	// decompose all 10x10 matrix a.
    AnalyticalRheologyMatrixDecomposer decomposer9;
    decomposer9.decomposeZ(a, u, l, u1);
    
    l(9, 9) = 0;
    u1(9, 9) = u(9, 9) = 1;
    
    for(int i = 0; i < 6; i++)
        u1(9, i) = a.get(9, 2) * u1(2, i) / l(i, i);
        
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 6; j++)
            u(9, i) -= u1(9, j) * u(j, i);
}