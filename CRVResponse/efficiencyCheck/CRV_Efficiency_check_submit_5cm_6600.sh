i=0
layerOffset=42
moduleGap=5
#  for moduleGap in {2..5}
#  do

     dz=$((822+$moduleGap))

#    for layerOffset in {0..62..2}
#    do
      for photonYield in {2500,3000,3500,4000,4500,5000,5500,6000,6500}  # 20,24,27,31,35,39,42,46,50 PE/SiPM for 5cm wide / 5.6m long counter
      do

        ((i++));

        scintTolerance="0"
        backgroundFileName="/mu2e/app/users/ehrlich/work_08302015/Offline/background_T1.root"
        backgroundFileName=""

        genconfigfile=CRVResponse/efficiencyCheck/submit/genconfig_5cm_6600'_'$i.txt
        echo "#include \"CRVResponse/efficiencyCheck/genconfig_5cm_6600.txt\"" >| $genconfigfile
        echo "double cosmicDYB.dz = $dz;" >> $genconfigfile

        geomfile=CRVResponse/efficiencyCheck/submit/geom_5cm_6600'_'$i.txt
        echo "#include \"CRVResponse/efficiencyCheck/geom_5cm_6600.txt\"" >| $geomfile
        echo "double crs.gapBetweenModules = $moduleGap;" >> $geomfile
        echo "double crs.layerOffset       = $layerOffset;" >> $geomfile

#### for adjacentPulseTimeDifference = 5ns

        fclfile=CRVResponse/efficiencyCheck/submit/CRV_Efficiency_check_5cm_6600'_'$i.fcl
        echo "#include \"CRVResponse/efficiencyCheck/CRV_Efficiency_check_5cm_6600.fcl\"" >| $fclfile
        echo "services.user.GeometryService.inputFile                 : \"$geomfile\"" >> $fclfile
        echo "physics.producers.generate.inputfile                    : \"$genconfigfile\"" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.scintillationYield  : $photonYield" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.scintillationYieldTolerance  : $scintTolerance" >> $fclfile
        echo "physics.producers.CrvSiPMResponses.ThermalProb          : 0" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.backgroundSampleFileName  : \"$backgroundFileName\"" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.countersInBackgroundSample: 384" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.backgroundSampleFactor    : 2" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.maxBackgroundTimeShift    : 500" >> $fclfile

        mu2eart --setup=./setup.sh --fcl=$fclfile --njobs=50 --events-per-job=10000 --jobname=CRV_efficiency5cm_top6600_moduleGap$moduleGap'_'layerOffset$layerOffset'_'photonYield$photonYield --outstage=/pnfs/mu2e/scratch/outstage

#### for adjacentPulseTimeDifference = 10ns

        fclfile=CRVResponse/efficiencyCheck/submit/CRV_Efficiency_check_5cm10_6600'_'$i.fcl
        echo "#include \"CRVResponse/efficiencyCheck/CRV_Efficiency_check_5cm10_6600.fcl\"" >| $fclfile
        echo "services.user.GeometryService.inputFile                 : \"$geomfile\"" >> $fclfile
        echo "physics.producers.generate.inputfile                    : \"$genconfigfile\"" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.scintillationYield  : $photonYield" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.scintillationYieldTolerance  : $scintTolerance" >> $fclfile
        echo "physics.producers.CrvSiPMResponses.ThermalProb          : 0" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.backgroundSampleFileName  : \"$backgroundFileName\"" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.countersInBackgroundSample: 384" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.backgroundSampleFactor    : 2" >> $fclfile
        echo "physics.producers.CrvPhotonArrivals.maxBackgroundTimeShift    : 500" >> $fclfile

        mu2eart --setup=./setup.sh --fcl=$fclfile --njobs=50 --events-per-job=10000 --jobname=CRV_efficiency5cm10_top6600_moduleGap$moduleGap'_'layerOffset$layerOffset'_'photonYield$photonYield --outstage=/pnfs/mu2e/scratch/outstage

      done
#    done
#  done