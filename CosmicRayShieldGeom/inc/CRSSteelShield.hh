#ifndef CosmicRayShieldGeom_CRSSteelShield_hh
#define CosmicRayShieldGeom_CRSSteelShield_hh

//
// Representation of CRSSteelShield aka the flux return yoke
//
// $Id: CRSSteelShield.hh,v 1.6 2011/12/06 22:53:01 gandr Exp $
// $Author: gandr $
// $Date: 2011/12/06 22:53:01 $
//
// Original author KLG
//
// c++ includes
#include <string>
#include <vector>

#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Vector/ThreeVector.h"

namespace mu2e {

  class CRSSteelShield {

    friend class CosmicRayShieldMaker;

  public:

    CRSSteelShield() :
      _name(),
      _rotation(),
      _globalOffset(),
      _halfLengths(),
      _holeRadius()
    {}

    // accept default d'tor etc...

    CRSSteelShield(std::string const name,
                               CLHEP::HepRotation* rotation,
                               CLHEP::Hep3Vector   globalOffset,
                               double const        halfLengths[3],
                               double              holeRadius = 0.):
      _name(name),
      _rotation(rotation),
      _globalOffset(globalOffset),
      _holeRadius(holeRadius)
    {
      _halfLengths[0] = halfLengths[0];
      _halfLengths[1] = halfLengths[1];
      _halfLengths[2] = halfLengths[2];
    };

    std::string         const & name() const { return _name;};
    CLHEP::Hep3Vector   const & getGlobalOffset() const { return _globalOffset; };
    CLHEP::HepRotation* const  getRotation()     const { return _rotation; };

    // Return halfLengths, yes nestBox can take it directly as an argument
    double const* getHalfLengths()  const { return _halfLengths; };

    // we assume that the hole is in the center of the given shield...
    double const & getHoleRadius()                const { return _holeRadius; };

  private:

    std::string _name;

    // unit vector along the shield=bars direction
    CLHEP::Hep3Vector _uv; // not used for now

    // rotation in the parent frame; 0 for now
    CLHEP::HepRotation* _rotation;

    // position of the center in the global Mu2e frame
    CLHEP::Hep3Vector _globalOffset;

    double _halfLengths[3];

    double _holeRadius;

  };

}

#endif /* CosmicRayShieldGeom_CRSSteelShield_hh */
