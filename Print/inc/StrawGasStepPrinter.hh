//
//  Utility class to print StrawGasStep
//
#ifndef Print_inc_StrawGasStepPrinter_hh
#define Print_inc_StrawGasStepPrinter_hh

#include <cstring>
#include <iostream>

#include "Offline/MCDataProducts/inc/StrawGasStep.hh"
#include "Offline/Print/inc/ProductPrinter.hh"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Persistency/Common/Ptr.h"

namespace mu2e {

class StrawGasStepPrinter : public ProductPrinter {
 public:
  StrawGasStepPrinter(const Config& conf) : ProductPrinter(conf) {}

  // all the ways to request a printout
  void Print(art::Event const& event, std::ostream& os = std::cout) override;
  void Print(const art::Handle<StrawGasStepCollection>& handle,
             std::ostream& os = std::cout);
  void Print(const art::ValidHandle<StrawGasStepCollection>& handle,
             std::ostream& os = std::cout);
  void Print(const StrawGasStepCollection& coll, std::ostream& os = std::cout);
  void Print(const art::Ptr<StrawGasStep>& ptr, int ind = -1,
             std::ostream& os = std::cout);
  void Print(const mu2e::StrawGasStep& obj, int ind = -1,
             std::ostream& os = std::cout);

  void PrintHeader(const std::string& tag, std::ostream& os = std::cout);
  void PrintListHeader(std::ostream& os = std::cout);
};

}  // namespace mu2e
#endif
