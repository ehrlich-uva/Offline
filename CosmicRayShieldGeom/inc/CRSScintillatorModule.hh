#ifndef CosmicRayShieldGeom_CRSScintillatorModule_hh
#define CosmicRayShieldGeom_CRSScintillatorModule_hh
//
// Representation of one Scintillator Module in CosmicRayShield
//

//
// $Id: CRSScintillatorModule.hh,v 1.6 2011/12/06 22:53:01 gandr Exp $
// $Author: gandr $
// $Date: 2011/12/06 22:53:01 $
//
// Original author KLG somewhat based on Rob Kutschke' Sector
//

#include <vector>

#include "CosmicRayShieldGeom/inc/CRSScintillatorModuleId.hh"
#include "CosmicRayShieldGeom/inc/CRSScintillatorLayer.hh"

#include "CLHEP/Vector/ThreeVector.h"


namespace mu2e {

  class CosmicRayShield;

  class CRSScintillatorModule{

    friend class CRSScintillatorShield;
    friend class CosmicRayShieldMaker;

  public:

    CRSScintillatorModule();

    CRSScintillatorModule(CRSScintillatorModuleId const & id,
                          int const nBarsPerLayer);

    CRSScintillatorModule(CRSScintillatorModuleId const & id,
                          int                     const nBarsPerLayer,
                          std::vector<double>     const & globalRotationAngles,
                          CLHEP::Hep3Vector       const & globalOffset // offset in Mu2e
                          );

    // Accept the compiler generated destructor, copy constructor and
    // assignment operators

    const CRSScintillatorModuleId& id() const { return _id;}

    const std::vector<CRSScintillatorLayer>& getLayers() const{
      return _layers;
    }

    int nLayers() const{
      return _layers.size();
    }

    const CRSScintillatorLayer& getLayer ( int n ) const {
      return _layers.at(n);
    }

    const CRSScintillatorLayer& getLayer ( CRSScintillatorLayerId const & lid) const {
      return _layers.at(lid.getLayerNumber());
    }

    CRSScintillatorBar const & getBar ( const CRSScintillatorBarId& moduleid ) const{
      return _layers.at(moduleid.getLayerNumber()).getBar(moduleid);
    }

    // Formatted string embedding the id of the module.
    std::string name( std::string const & base ) const;

    const std::vector<double>& halfLengths() const { return _halfLengths; }

    std::vector<double> const & getGlobalRotationAngles() const { return _globalRotationAngles;}

    CLHEP::Hep3Vector const & getGlobalOffset() const {return _globalOffset;}

    std::vector<double> const & getHalfLengths() {
      return _halfLengths;
    }

    void setHalfLengths( std::vector<double> const & v ){
      _halfLengths = v;
    }

    // On readback from persistency, recursively recompute mutable members.
    //    void fillPointers ( const CosmicRayShield& cosmicRayShield ) const;

  private:

    CRSScintillatorModuleId _id;

    // this defines if this is full or half module
    int _nBarsPerLayer;

    // Properties of the enclosing logical volume (box).

    std::vector<double> _globalRotationAngles;

    // Mid-point
    CLHEP::Hep3Vector _globalOffset;

    std::vector<CRSScintillatorLayer> _layers;

    // Half lengths of the outer box; not used for now

    // the shape is more complicated though it is splice of three
    // boxes, but we will skip the unistruts in the first
    // approximation

    std::vector<double> _halfLengths;

  };

}  //namespace mu2e
#endif /* CosmicRayShieldGeom_CRSScintillatorModule_hh */
