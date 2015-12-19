//
// structs used to record per-track information in TTrees
// 
#ifndef TrkInfo_HH
#define TrkInfo_HH
#include "DataProducts/inc/threevec.hh"
#include "TrkDiag/inc/helixpar.hh"
#include "Rtypes.h"
namespace mu2e
{
// information about the track fit at a particular place
  struct TrkFitInfo {
    Float_t _fitmom,_fitmomerr, _fltlen;
    helixpar _fitpar;
    helixpar _fitparerr;
    TrkFitInfo() { reset(); }
    void reset() { _fitmom=_fitmomerr=_fltlen-1000.0; _fitpar.reset(); _fitparerr.reset(); }
    static std::string const& leafnames() { static const std::string leaves =
      std::string("mom/F:momerr/F:fltlen/F:")+helixpar::leafnames()+std::string(":d0err/F:p0err/F:omerr/F:z0err/F:tderr/F");
      return leaves;
    }
  };

// general information about a track
  struct TrkInfo {
    Int_t _fitstatus;
    Int_t _fitpart;
    Int_t _nhits;
    Int_t _ndof;
    Int_t _nactive;
    Int_t _ndouble,_ndactive;
    Float_t _t0;
    Float_t _t0err;
    Float_t _chisq;
    Float_t _fitcon;
    Float_t _radlen;
    Float_t _firstflt, _lastflt;
    Float_t _seedmom;
    Float_t _trkqual;
    TrkFitInfo _ent; // fit information at tracker entrance
    TrkInfo() { reset(); }
    void reset() { 
      _fitstatus = -1000;
      _nhits = _nactive = _ndouble = _ndactive = _ndof = -1;
      _seedmom = _t0 = _t0err = _chisq = _fitcon = _radlen = _firstflt = _lastflt = -1.0;
      _trkqual=-1000.0;
      _ent.reset();
    }
    static std::string const& leafnames() { static const std::string leaves =
    std::string("status/I:pdg/I:nhits/I:ndof/I:nactive/I:ndouble/I:ndactive/I:t0/F:t0err/F:chisq/F:con/F:radlen/F:firstflt/F:lastflt/F:seedmom/F:trkqual/F:")+TrkFitInfo::leafnames();
     return leaves;
    }
  };

// general NC info about a track (particle)
  struct TrkInfoMC {
    Int_t _ndigi, _ndigigood;
    Int_t _nhits, _nactive, _ngood, _nambig;
    Int_t _pdg, _gen, _proc;
    Int_t _ppdg, _pgen, _pproc;
    TrkInfoMC() { reset(); }
    void reset() { _ndigi = _ndigigood = _nactive = _nhits = _ngood = _nambig = _pdg = _gen  = _proc = _ppdg = _pgen = _pproc = -1; }
    static std::string const& leafnames() { static const std::string leaves =
      std::string("ndigi/I:ndigigood/I:nhits/I:nactive/I:ngood/I:nambig/I:pdg/I:gen/I:proc/I:ppdg/I:pgen/I:pproc/I");
      return leaves;
    }
  };
//  MC information about a track (particle) for a specific point/time
  struct TrkInfoMCStep {
    Float_t _time;
    Float_t _mom;
    threevec _pos;
    helixpar _hpar;
    TrkInfoMCStep() { reset(); }
    void reset() { _time = _mom = -1; _pos.reset(); _hpar.reset(); }
    static std::string const& leafnames() { static const std::string leaves =
      std::string("t0/F:mom/F:")+threevec::leafnames() +std::string(":")+helixpar::leafnames();
      return leaves;
    }
};
}
#endif
