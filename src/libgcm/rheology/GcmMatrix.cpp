#include "libgcm/rheology/GcmMatrix.hpp"
#include "libgcm/util/Assertion.hpp"

// For fabs())
#include <cmath>
// For memset())
#include <cstring>

#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

using namespace gcm;

GcmMatrix::GcmMatrix(uchar _size) {
    INIT_LOGGER("gcm.GcmMatrix");
    size = _size;
    p = new gcm::real*[size];
    for(int i = 0; i < size; i++) {
        p[i] = new gcm::real[size];
    }
};

GcmMatrix::GcmMatrix(const GcmMatrix& origin) {
	size = origin.getSize();
    p = new gcm::real*[size];
    for(int i = 0; i < size; i++) {
        p[i] = new gcm::real[size];
    }
	for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            p[i][j] = origin.get(i, j);
};

GcmMatrix::~GcmMatrix()
{
    for(int i = 0; i < size; i++) {
        delete[] p[i];
    }
    delete[] p;
};

uchar GcmMatrix::getSize() const {
    return size;
};

GcmMatrix& GcmMatrix::operator=(const GcmMatrix &A)
{
    assert_true(size == A.size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            p[i][j] = A.p[i][j];
    return *this;
};

gcm::real& GcmMatrix::operator()(int i, int j)
{
    return p[i][j];
};

gcm::real GcmMatrix::get(int i, int j) const
{
    return p[i][j];
};

void GcmMatrix::createE()
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            p[i][j] = (i == j ? 1 : 0);
};

bool GcmMatrix::operator==(const GcmMatrix &A) const
{
    assert_true(size == A.size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            // If element is greater than chosen floating point epsilon ...
            if ( (fabs(p[i][j]) > EQUALITY_TOLERANCE || fabs(A.p[i][j]) > EQUALITY_TOLERANCE )
                        // .. and difference is greater than 1% ...
                        && fabs(p[i][j] - A.p[i][j]) > 0.005 * fabs(p[i][j] + A.p[i][j]))
            {
                // .. matrices are not equal
                return false;
            }
    return true;
};

bool GcmMatrix::operator!=(const GcmMatrix &A) const
{
    return !(*this == A);
};

GcmMatrix GcmMatrix::operator*(const GcmMatrix &A) const
{
    assert_true(size == A.size);
    GcmMatrix res_matrix(size);
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            res_matrix.p[r][c] = 0;
            for (int i = 0; i < size; i++) {
                res_matrix.p[r][c] += this->p[r][i] * A.p[i][c];
            }
        }
    }
    return res_matrix;
};

GcmMatrix GcmMatrix::operator/(const GcmMatrix &A) const
{
    assert_true(size == A.size);
    return (*this * A.inv(size));
};

GcmMatrix GcmMatrix::operator*(const gcm::real &a) const
{
    GcmMatrix res_matrix(size);
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            res_matrix.p[r][c] = this->p[r][c] * a;
        }
    }
    return res_matrix;
};

GcmMatrix GcmMatrix::operator/(const gcm::real &a) const
{
    if( a == 0.0 )
        THROW_INVALID_ARG("Can not divide matrix by zero");
    return (*this * (1/a));
};

GcmMatrix GcmMatrix::operator%(const GcmMatrix &A) const
{
    assert_true(size == A.size);
    GcmMatrix res_matrix(size);
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            res_matrix.p[r][c] = this->p[r][c] / A.p[r][c];
        }
    }
    return res_matrix;
};

GcmMatrix GcmMatrix::operator+(const GcmMatrix &A) const
{
    assert_true(size == A.size);
    GcmMatrix res_matrix(size);
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            res_matrix.p[r][c] = this->p[r][c] + A.p[r][c];
    return res_matrix;
};

GcmMatrix GcmMatrix::operator-(const GcmMatrix &A) const
{
    assert_true(size == A.size);
    GcmMatrix res_matrix(size);
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            res_matrix.p[r][c] = this->p[r][c] - A.p[r][c];
    return res_matrix;
};

void GcmMatrix::clear() {
	for(int i = 0; i < size; i++)
		memset(p[i], 0, size * sizeof (gcm::real));
};

void GcmMatrix::setColumn(double *Clmn, uchar numOfClmn, uchar numOfStrings)
{
    for (int i = 0; i < numOfStrings; i++)
        p[i][numOfClmn] = Clmn[i];
	for (int i = numOfStrings; i < size; i++)
		p[i][numOfClmn] = 0;
};

GcmMatrix GcmMatrix::inv(uchar realSize) const
{
    // Invert the matrix using gsl library
    gsl_set_error_handler_off();

    gsl_matrix* Z1 = gsl_matrix_alloc(realSize, realSize);
    gsl_matrix* Z = gsl_matrix_alloc(realSize, realSize);
    gsl_permutation* perm = gsl_permutation_alloc(realSize);
    int k;

    for (int i = 0; i < realSize; i++)
        for (int j = 0; j < realSize; j++)
            gsl_matrix_set(Z1, i, j, p[i][j]);

    int status = gsl_linalg_LU_decomp(Z1, perm, &k);
    if (status) {
        LOG_DEBUG("gsl_linalg_LU_decomp failed");
        THROW_INVALID_ARG("gsl_linalg_LU_decomp failed");
    }
    status = gsl_linalg_LU_invert(Z1, perm, Z);
    if (status) {
        LOG_DEBUG("gsl_linalg_LU_invert failed");
        THROW_INVALID_ARG("gsl_linalg_LU_invert failed");
    }
	
	GcmMatrix res_matrix(size);
	res_matrix.clear();
    for (int i = 0; i < realSize; i++)
        for (int j = 0; j < realSize; j++)
            res_matrix.p[i][j] = gsl_matrix_get(Z, i, j);

    gsl_permutation_free(perm);
    gsl_matrix_free(Z);
    gsl_matrix_free(Z1);

    return res_matrix;
};