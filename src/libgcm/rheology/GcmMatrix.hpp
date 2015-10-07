#ifndef _GCM_GcmMatrix_H
#define _GCM_GcmMatrix_H  1

#include <iostream>

#include "libgcm/util/Logging.hpp"
#include "libgcm/util/Types.hpp"
#include "libgcm/util/Assertion.hpp"

namespace gcm {

    class GcmMatrix {
    public:
        // Just a remark: we do not use templates here because of
        // http://stackoverflow.com/questions/1724036/splitting-templated-c-classes-into-hpp-cpp-files-is-it-possible
        GcmMatrix(uchar _size);
		GcmMatrix(const GcmMatrix& origin);
        ~GcmMatrix();
        
        // Basic operators
        
        // Get matrix size
        uchar getSize() const;
        // Assign matrix
        GcmMatrix& operator=(const GcmMatrix &A);
        // Check for matrix equality
        bool operator==(const GcmMatrix &A) const;
        bool operator!=(const GcmMatrix &A) const;
        // Get elements - read-write and read-only options
        gcm::real& operator()(int i, int j);
        gcm::real get(int i, int j) const;
        // Matrix operations
        GcmMatrix operator+(const GcmMatrix &A) const;
        GcmMatrix operator-(const GcmMatrix &A) const;
        GcmMatrix operator*(const GcmMatrix &A) const;
        GcmMatrix operator/(const GcmMatrix &A) const;
        // Multiply and divide matrix by constant
        GcmMatrix operator*(const gcm::real &a) const;
        GcmMatrix operator/(const gcm::real &a) const;
        
        // Divide two matrixes element by element (e.g. c[i][j] = a[i][j] / b[i][j])
        GcmMatrix operator%(const GcmMatrix &A) const;
        // Get inverse matrix
        GcmMatrix inv(uchar realSize) const;

        // TODO: do we really need these functions?
        // They are meant to reduce the number of code lines around matrixes.
        // Recheck if we really use them now.
        
        // Set all elements to zero
        void clear();
        // Create unit matrix
        void createE();
        // Set matrix column to certain values
        void setColumn(double *Clmn, uchar numOfClmn, uchar numOfStrings);

    protected:
        // Real matrix data lives here
        gcm::real** p;
        uchar size;
        
    private:
        USE_LOGGER;
    };
}

namespace std {

    inline std::ostream& operator<<(std::ostream &os, const gcm::GcmMatrix &matrix) {
        unsigned char size = matrix.getSize();
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++)
                os << matrix.get(r, c) << " ";
            os << std::endl;
        }
        return os;
    };
}

#endif