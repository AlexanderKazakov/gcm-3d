#include "libgcm/rheology/RheologyMatrix.hpp"

using namespace gcm;

RheologyMatrix::RheologyMatrix(uchar size, const MaterialPtr& material, 
	const SetterPtr& setter, const DecomposerPtr& decomposer): 
	size(size), material(material), setter(setter), decomposer(decomposer)
{
    auto n = setter->getNumberOfStates(); 
    if (n == 0)
    {
        n = 1;
        immutable = false;
    }
    for (auto& m: matrices)
    {
        m.a.resize(n, GcmMatrix(size));
        m.u.resize(n, GcmMatrix(size));
        m.l.resize(n, GcmMatrix(size));
        m.u1.resize(n, GcmMatrix(size));
        m.cached.resize(n, false);
    }
}

RheologyMatrix::~RheologyMatrix()
{
}

uchar RheologyMatrix::getSize() const {
    return size;
};

real RheologyMatrix::getMaxEigenvalue() const
{
    float res = 0;
    uchar size = getL().getSize();
    for (unsigned i = 0; i < size; ++i)
    {
        real la = getL(i, i);
        if(la > res)
            res = la;
    }
    return res;
}

real RheologyMatrix::getMinEigenvalue() const
{
    float res = std::numeric_limits<real>::infinity();
    uchar size = getL().getSize();
    for (unsigned i = 0; i < size; ++i)
    {
        real la = getL(i, i);
        if(la > 0 && la < res)
            res = la;
    }
    return res;
}

const GcmMatrix& RheologyMatrix::getA() const
{
    assert_true(matrices[direction].cached[index]);
    return matrices[direction].a[index];
}

real RheologyMatrix::getA(unsigned int i, unsigned int j) const
{
    return getA().get(i, j);
}

const GcmMatrix& RheologyMatrix::getL() const
{
    assert_true(matrices[direction].cached[index]);
    return matrices[direction].l[index];
}

real RheologyMatrix::getL(unsigned int i, unsigned int j) const
{
    return getL().get(i, j);
}

const GcmMatrix& RheologyMatrix::getU() const
{
    assert_true(matrices[direction].cached[index]);
    return matrices[direction].u[index];
}

real RheologyMatrix::getU(unsigned int i, unsigned int j) const
{
    return getU().get(i, j);
}

const GcmMatrix& RheologyMatrix::getU1() const
{
    assert_true(matrices[direction].cached[index]);
    return matrices[direction].u1[index];
}

real RheologyMatrix::getU1(unsigned int i, unsigned int j) const
{
    return getU1().get(i, j);
}

const MaterialPtr& RheologyMatrix::getMaterial() const
{
    return material;
}

void RheologyMatrix::decomposeX(const Node& node)
{
    decompose(node, 0);
}

void RheologyMatrix::decomposeY(const Node& node)
{
    decompose(node, 1);
}

void RheologyMatrix::decomposeZ(const Node& node)
{
    decompose(node, 2);
}

void RheologyMatrix::decompose(const Node& node, unsigned int direction)
{
    assert_le(direction, 2);

    auto s = setter->getStateForNode(node);
    if (!matrices[direction].cached[s] || !immutable)
    {
        switch(direction)
        {
            case 0: setter->setX(matrices[0].a[s], material, node); break;
            case 1: setter->setY(matrices[1].a[s], material, node); break;
            case 2: setter->setZ(matrices[2].a[s], material, node); break;

        }
        switch (direction)
        {
            case 0: decomposer->decomposeX(matrices[0].a[s], matrices[0].u[s], matrices[0].l[s], matrices[0].u1[s]); break;
            case 1: decomposer->decomposeY(matrices[1].a[s], matrices[1].u[s], matrices[1].l[s], matrices[1].u1[s]); break;
            case 2: decomposer->decomposeZ(matrices[2].a[s], matrices[2].u[s], matrices[2].l[s], matrices[2].u1[s]); break;
        }
        matrices[direction].cached[s] = true;
    }

    this->direction = direction;
    this->index = s;
}