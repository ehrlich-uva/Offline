//
// TTracker Pattern Recognition based on Robust Helix Fit
//
// $Id: RobustHelixFinder_module.cc,v 1.2 2014/08/30 12:19:38 tassiell Exp $
// $Author: tassiell $
// $Date: 2014/08/30 12:19:38 $
//
// Original author D. Brown and G. Tassielli
//

// framework
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
//#include "art/Framework/Services/Optional/TFileService.h"
// data
#include "RecoDataProducts/inc/StrawHitCollection.hh"
#include "RecoDataProducts/inc/StrawHitPositionCollection.hh"
#include "RecoDataProducts/inc/StereoHitCollection.hh"
//#include "RecoDataProducts/inc/StrawHitFlagCollection.hh"
#include "RecoDataProducts/inc/StrawHit.hh"
#include "RecoDataProducts/inc/TrackerHitTimeCluster.hh"
#include "RecoDataProducts/inc/TrackerHitTimeClusterCollection.hh"
#include "RecoDataProducts/inc/HelixVal.hh"
#include "RecoDataProducts/inc/TrackSeed.hh"
#include "RecoDataProducts/inc/TrackSeedCollection.hh"
#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "MCDataProducts/inc/StrawHitMCTruth.hh"
#include "MCDataProducts/inc/StrawHitMCTruthCollection.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
// BaBar
#include "BaBar/BaBar.hh"
#include "KalmanTests/inc/TrkDef.hh"
#include "KalmanTests/inc/TrkStrawHit.hh"
#include "TrkPatRec/inc/HelixFit.hh"
// Mu2e
#include "TrkPatRec/inc/TrkPatRecUtils.hh"
//CLHEP
#include "CLHEP/Units/PhysicalConstants.h"
// C++
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <functional>
#include <float.h>
#include <vector>
#include <set>
#include <map>
using namespace std; 

namespace mu2e 
{
  class RobustHelixFinder : public art::EDProducer
  {
    public:
      explicit RobustHelixFinder(fhicl::ParameterSet const&);
      virtual ~RobustHelixFinder();
      virtual void beginJob();
      virtual void beginRun(art::Run&);
      virtual void produce(art::Event& event ); 
      void endJob();
    private:
      unsigned _iev;
      // configuration parameters
      int _diag,_debug;
      int _printfreq;
      art::Handle<mu2e::StrawHitCollection> _strawhitsH;
      art::Handle<TrackerHitTimeClusterCollection> _tclusthitH;
      // event object labels
      std::string _shLabel;
      std::string _shpLabel;
      std::string _tpkfLabel;
      // outlier cuts
      double _maxhelixdoca;
      TrkParticle _tpart; // particle type being searched for
      TrkFitDirection _fdir;  // fit direction in search
      // cache of event objects
      const StrawHitCollection* _shcol;
      const StrawHitPositionCollection* _shpcol;
      const TrackerHitTimeClusterCollection* _tccol;
      // robust helix fitter
      HelixFit _hfit;
      // cache of time peaks
      std::vector<TrkTimePeak> _tpeaks;
      std::string _iname; // data instance name
      //
      // helper functions
      bool findData(const art::Event& e);
      Int_t _eventid;
  };

  RobustHelixFinder::RobustHelixFinder(fhicl::ParameterSet const& pset) :
    _diag(pset.get<int>("diagLevel",0)),
    _debug(pset.get<int>("debugLevel",0)),
    _printfreq(pset.get<int>("printFrequency",101)),
    _shLabel(pset.get<string>("StrawHitCollectionLabel","makeSH")),
    _shpLabel(pset.get<string>("StrawHitPositionCollectionLabel","MakeStereoHits")),
    _tpkfLabel(pset.get<string>("TrackerHitTimeClusterCollection","TimePeakFinder")),
    _maxhelixdoca(pset.get<double>("MaxHelixDoca",40.0)),
    _tpart((TrkParticle::type)(pset.get<int>("fitparticle",TrkParticle::e_minus))),
    _fdir((TrkFitDirection::FitDirection)(pset.get<int>("fitdirection",TrkFitDirection::downstream))),
    _hfit(pset.get<fhicl::ParameterSet>("HelixFit",fhicl::ParameterSet()))
  {
     produces<TrackSeedCollection>();
//   // tag the data product instance by the direction and particle type found by this fitter
    _iname = _fdir.name() + _tpart.name();
//    produces<StrawHitFlagCollection>(_iname);
  }

  RobustHelixFinder::~RobustHelixFinder(){}

  void RobustHelixFinder::beginJob(){
    // create a histogram of throughput: this is a basic diagnostic that should ALWAYS be on
    //art::ServiceHandle<art::TFileService> tfs;
    _eventid = 0;
  }

  void RobustHelixFinder::beginRun(art::Run& ){}

  void RobustHelixFinder::produce(art::Event& event ) {
    _eventid = event.event();

    // create output
    unique_ptr<TrackSeedCollection> outseeds(new TrackSeedCollection);
    // event printout
    _iev=event.id().event();
    if((_iev%_printfreq)==0)cout<<"RobustHelixFinder: event="<<_iev<<endl;
    // find the data
    if(!findData(event)){
      cout << "RobustHelixFinder: No straw hits found, event="<<_iev << endl;
      return;
    }
    loadTimePeaks(_tpeaks,_tccol);

    // dummy objects
    static HelixDef dummyhdef;
    static HelixFitResult dummyhfit(dummyhdef);
    for(unsigned ipeak=0;ipeak<_tpeaks.size();++ipeak){
      // create track definitions for the helix fit from this initial information 
      HelixDef helixdef(_shcol,_shpcol,_tpeaks[ipeak]._trkptrs,_tpart,_fdir);
      // set some identifiers
      helixdef.setEventId(_eventid);
      helixdef.setTrackId(ipeak);
      // copy this for the other fits
      TrkDef seeddef(helixdef);
      // track fitting objects for this peak
      HelixFitResult helixfit(helixdef);
      // robust helix fit
      if(_hfit.findHelix(helixfit/*,_icepeak==(int)ipeak*/)){
	//findhelix = true;
	// convert the result to standard helix parameters, and initialize the seed definition helix
	HepVector hpar;
	HepVector hparerr;
	_hfit.helixParams(helixfit,hpar,hparerr);
	HepSymMatrix hcov = vT_times_v(hparerr);
	seeddef.setHelix(HelixTraj(hpar,hcov));
	// Filter outliers using this helix
	if (_debug>0) {std::cout <<"RobustHelixFinder::produce - helix params " << hpar << "and errors " << hparerr << endl;}
	//fill seed information
	TrackSeed tmpseed;
	fillTrackSeed(tmpseed, seeddef, _tclusthitH, ipeak, _strawhitsH);
        outseeds->push_back(tmpseed);
      }
    }
    event.put(std::move(outseeds));
  }

  void RobustHelixFinder::endJob(){
    // does this cause the file to close?
    //art::ServiceHandle<art::TFileService> tfs;
  }

  // find the input data objects 
  bool RobustHelixFinder::findData(const art::Event& evt){
    _shcol = 0;
    _shpcol = 0;
    _tccol = 0;

    if(evt.getByLabel(_shLabel,_strawhitsH))
      _shcol = _strawhitsH.product();
    art::Handle<mu2e::StrawHitPositionCollection> shposH;
    if(evt.getByLabel(_shpLabel,shposH))
      _shpcol = shposH.product();

    if (evt.getByLabel(_tpkfLabel,_tclusthitH))
      _tccol = _tclusthitH.product();
// don't require stereo hits: they are only used for diagnostics
    return _shcol != 0 && _shpcol != 0 && _tccol!=0;
  }

}
using mu2e::RobustHelixFinder;
DEFINE_ART_MODULE(RobustHelixFinder);
