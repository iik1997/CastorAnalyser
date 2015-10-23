import FWCore.ParameterSet.Config as cms

import os
base_path = os.getenv('CMSSW_BASE', 'empty_defualt_value')

process = cms.Process("LEDcheck")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
###
cmssw_used = '{0}'.format(argcmssw)
glob_tag = 'GR_P_V56' #'74X_dataRun2_Prompt_v1' #'GR_P_V56' #config dataset=/ZeroBias1/Run2015B-PromptReco-v1/RECO
run_num = '{0}'.format(argledrun)
run_ped = '{0}'.format(argpedrun)
post_fix = '{0}'.format('argsuf') #'ZB1zbp0ls230to241v1'
volt_set = '{0}'.format('arghv')
trig_sel = 'HLT_ZeroBias_part0_v1'
process.MessageLogger.destinations = ['{0}{1}_detailedInfo01.txt'.format(run_num,post_fix)]
###

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    arglistofrawfiles 
#'root://cms-xrd-global.cern.ch//store/data/Run2015C/ZeroBias1/RECO/PromptReco-v1/000/255/031/00000/001878BE-444E-E511-A8C0-02163E014761.root'
#'root://cmsxrootd.fnal.gov//store/data/Run2015C/ZeroBias1/RECO/PromptReco-v1/000/254/989/00000/0054182B-114D-E511-B10C-02163E0141E0.root'
#'/store/data/Commissioning2015/MinimumBias/RAW/v1/000/239/754/00000/94E8E718-75DB-E411-9D0E-02163E0123CC.root',
#        #'file:/usr/users/ikatkov/hiforest/CMSSW_4_4_2_patch7/test/skim_conformal_test.root'
#        'file:/storage/5/cbaus/RECO/PbPb_1380_eposLHC_HI44/job_N5_10.root'
    )
#   ,skipEvents = cms.untracked.uint32(200000),
#   ,eventsToProcess = cms.untracked.VEventRange('239754:71:128-239754:71:128')  ####################################################
#   ,lumisToProcess = cms.untracked.VLuminosityBlockRange('239821:274-239821:276')
)
######################################################################################################################################
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring( # child (e.g. GEN-SIM-RECO)
#    '/store/data/Commissioning2015/MinimumBias/RECO/PromptReco-v1/000/245/194/00000/78127995-3301-E511-95CE-02163E01432F.root'
#    ),
#    secondaryFileNames = cms.untracked.vstring(# parent (e.g. GEN-SIM-RAW)
#    '/store/data/Commissioning2015/MinimumBias/RAW/v1/000/245/194/00000/A69709E4-B6FF-E411-8861-02163E01384F.root'
#    )
#   ,eventsToProcess = cms.untracked.VEventRange('239754:71:128-239754:71:128')  ####################################################
#   ,lumisToProcess = cms.untracked.VLuminosityBlockRange('245194:220-245194:220')
#)
######################################################################################################################################


###process.source = cms.Source("HcalTBSource",
###    streams = cms.untracked.vstring('HCAL_Trigger', 
###        'HCAL_DCC690','HCAL_DCC691','HCAL_DCC692', 
###),
    #fileNames = cms.untracked.vstring('/store/caft2/user/campbell/castor_localruns/USC_119814.root') USC_132564.root
    #fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/k/katkov/P5_2011/USC_argledrun.root') #171702 #157405 #156686 #171704
    ###fileNames = cms.untracked.vstring('file:/tmp/katkov/USC_argledrun.root')
###    fileNames = cms.untracked.vstring('/store/group/phys_heavyions/katkov/cas_loc_2013/USC_argledrun.root')
    #fileNames = cms.untracked.vstring('file:/tmp/katkov/USC_141894.root')  #157405
    #fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/k/katkov/P5_2010/USC_138974.root')
###)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200000) ##20000 10000 -1 100000
)

###process.castorDigis = cms.EDProducer("CastorRawToDigi",
###   CastorFirstFED = cms.untracked.int32(690),
###   FilterDataQuality = cms.bool(True),
###   ExceptionEmptyData = cms.untracked.bool(True),
###   InputLabel = cms.InputTag("source"),
###   UnpackCalib = cms.untracked.bool(False),
###   FEDs = cms.untracked.vint32(690,691,692),
###   lastSample = cms.int32(9),
###   firstSample = cms.int32(0)
###) 

process.castorDigis = cms.EDProducer("CastorRawToDigi",
   CastorFirstFED = cms.int32(690),
   FilterDataQuality = cms.bool(True),
   ExceptionEmptyData = cms.untracked.bool(True),
   InputLabel = cms.InputTag("rawDataCollector"), #source #rawDataCollector
   #UnpackCalib = cms.untracked.bool(False),
   # castor technical trigger processor
   UnpackTTP = cms.bool(True),
   FEDs = cms.untracked.vint32(690,691,692),
   lastSample = cms.int32(9),
   firstSample = cms.int32(0),
   CastorCtdc = cms.bool(False),
   UseNominalOrbitMessageTime = cms.bool(True),
   ExpectedOrbitMessageTime = cms.int32(-1)#,
   # Do not complain about missing FEDs
   #ComplainEmptyData = cms.untracked.bool(False),
)

###process.out = cms.OutputModule("PoolOutputModule",
###    fileName = cms.untracked.string('/tmp/katkov/USC_XXXXXX_unpacked.root')
###)
###process.dumpRaw = cms.EDAnalyzer( "DumpFEDRawDataProduct",
###   feds = cms.untracked.vint32( 690,691,692,693 ),
###   dumpPayload = cms.untracked.bool( True )
###)
###process.m = cms.EDAnalyzer("HcalDigiDump")
###process.dump = cms.EDAnalyzer("HcalTBObjectDump")
###process.dumpECA = cms.EDAnalyzer("EventContentAnalyzer")


process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.CastorDbProducer = cms.ESProducer("CastorDbProducer")

#process.es_hardcode = cms.ESSource("CastorHardcodeCalibrations",
#    toGet = cms.untracked.vstring(
#        'ElectronicsMap'
#        'Gains', 
#        'Pedestals',         
#        'PedestalWidths', 
#        'GainWidths', 
#        'QIEShape', 
#        'QIEData', 
#        'ChannelQuality', 
#        'RespCorrs', 
#        'ZSThresholds'
#        )
#)

process.es_ascii = cms.ESSource("CastorTextCalibrations",
   input = cms.VPSet(
#                    cms.PSet(
#       object = cms.string('ElectronicsMap'),
#       file = cms.FileInPath('emap_dcc_nominal_Run121872.txt')
#                            ),
                     cms.PSet(
       object = cms.string('Pedestals'),
       file = cms.FileInPath('peds_fC_run_{0}.txt'.format(run_ped)) #156686
                             ),
         cms.PSet(
             object = cms.string('Gains'),
             file = cms.FileInPath('gain__1200x4_1600x10_led0to38.txt')
         ),
         cms.PSet(
             object = cms.string('ChannelQuality'),
             file = cms.FileInPath('quality__2015.txt')
         ),
         cms.PSet(
             object = cms.string('SaturationCorrs'),
             file = cms.FileInPath('CastorCondSaturationCorrs_Run245194.txt')
         )
                     )
)

# use this when it works
#   connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),

#####process.es_pool = cms.ESSource(
#####   "PoolDBESSource",
#####   process.CondDBSetup,
#####   timetype = cms.string('runnumber'),
#####   #connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierPrep/CMS_COND_30X_HCAL'),
#####   connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),
#####   authenticationMethod = cms.untracked.uint32(0),
#####   toGet = cms.VPSet(
########       cms.PSet(
########           record = cms.string('CastorPedestalsRcd'),
#####           #tag = cms.string('castor_pedestals_v1.0_test')
######           tag = cms.string('castor_pedestals_v1.0')
########         tag = cms.string('castor_pedestals_v2.0')
########           ),
#####       cms.PSet(
#####           record = cms.string('CastorPedestalWidthsRcd'),
#####           #tag = cms.string('castor_pedestalwidths_v1.0_test')
#####           tag = cms.string('castor_pedestalwidths_v1.0')
#####           #tag = cms.string('castor_pedestalwidths_v2.0') //v2.0 does not exist ????
#####           ),
#####       cms.PSet(
#####           record = cms.string('CastorGainsRcd'),
#####           #tag = cms.string('castor_gains_v1.0_test') 
#####           tag = cms.string('castor_gains_v1.0')
#####           #tag = cms.string('castor_gains_v2.0')
#####           ),
#####       cms.PSet(
#####           record = cms.string('CastorGainWidthsRcd'),
#####           #tag = cms.string('castor_gainwidths_v1.0_test') 
#####           tag = cms.string('castor_gainwidths_v1.0')
#####           #tag = cms.string('castor_gainwidths_v2.0')
#####           ),
#####       cms.PSet(
#####           record = cms.string('CastorQIEDataRcd'),
#####           #tag = cms.string('castor_qie_v1.0_test')
#####           tag = cms.string('castor_qie_v1.0')
#####           #tag = cms.string('castor_qie_v2.0')
#####           ),
#####       cms.PSet(
#####           record = cms.string('CastorChannelQualityRcd'),
#####           #tag = cms.string('castor_channelquality_v1.0_test')
#####           tag = cms.string('castor_channelquality_v1.0')
#####           #tag = cms.string('castor_channelquality_v2.0')
#####           ),
#####       cms.PSet(
#####           record = cms.string('CastorElectronicsMapRcd'),
#####           #tag = cms.string('castor_emap_dcc_v1.0_test')
#####           tag = cms.string('castor_emap_dcc_v1.0')
#####           #tag = cms.string('castor_emap_dcc_v2.0')
#####           )
#####   )
#####)

process.es_pool = cms.ESSource(
   "PoolDBESSource",
   process.CondDBSetup,
   timetype = cms.string('runnumber'),
   #connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierPrep/CMS_COND_30X_HCAL'),
   #connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),
   connect = cms.string('frontier://FrontierProd/CMS_COND_HCAL_000'),
   authenticationMethod = cms.untracked.uint32(0),
   toGet = cms.VPSet(
###       cms.PSet(
###           record = cms.string('CastorSaturationCorrsRcd'),
###           tag = cms.string('CastorSaturationCorrs_v3.00_offline')
###           ),
###       cms.PSet(
###           record = cms.string('CastorPedestalsRcd'),
###           tag = cms.string('CastorPedestals_v3.00_offline')
###           ),
       cms.PSet(
           record = cms.string('CastorPedestalWidthsRcd'),
           tag = cms.string('CastorPedestalWidths_v3.00_offline')
           ),
###       cms.PSet(
###           record = cms.string('CastorGainsRcd'),
###           tag = cms.string('CastorGains_v3.00_offline')
###           ),
       cms.PSet(
           record = cms.string('CastorGainWidthsRcd'),
           tag = cms.string('CastorGainWidths_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorQIEDataRcd'),
           tag = cms.string('CastorQIEData_v3.00_offline')
           ),
###       cms.PSet(
###           record = cms.string('CastorChannelQualityRcd'),
###           tag = cms.string('CastorChannelQuality_v3.00_offline')
###           ),
       cms.PSet(
           record = cms.string('CastorElectronicsMapRcd'),
           tag = cms.string('CastorElectronicsMap_v3.00_offline')
           )
   )
)


###process.castorpedestalsanalysis = cms.EDAnalyzer("CastorPedestalsAnalysis",
###    hiSaveFlag  = cms.untracked.bool( False ),
###	verboseflag = cms.untracked.bool( True ),
###        firstTS = cms.untracked.int32(0),
###         lastTS = cms.untracked.int32(9),
###)

#===================
# CASTOR Analyzer
#===================

#process.cAnalyser = cms.EDAnalyzer('CastorAnalyser',
#                                  firstTS = cms.untracked.int32(0),
#                                  lastTS  = cms.untracked.int32(9),
#                                  pedFCpath = cms.untracked.string(base_path),
#                                  pedFCrun  = cms.untracked.int32(229479)
#)
process.cAnalyser = cms.EDAnalyzer('CastorAnalyser',
                                  firstTS = cms.untracked.int32(4),
                                  lastTS  = cms.untracked.int32(5), ###################
                                  showDebug = cms.untracked.bool(True), #True #False
                                  hltPathName = cms.untracked.string("{0}".format(trig_sel)), 
                                  vtxLabel = cms.untracked.InputTag("offlinePrimaryVertices","","RECO"),
                                  trkLabel = cms.untracked.InputTag("generalTracks","","RECO"),
                                  casDigiLabel = cms.untracked.InputTag("castorDigis","","LEDcheck"),
                                  hfRHLabel = cms.untracked.InputTag("hfreco","","LEDcheck"), #"RECO"
                                  hbheRHLabel = cms.untracked.InputTag("hbhereco","","LEDcheck"), #"RECO"
                                  ebRHLabel = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEB","RECO"),
                                  eeRHLabel = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEE","RECO"),
                                  casRHLabel = cms.untracked.InputTag("castorreco","","LEDcheck"), #"RECO" #"LEDcheck"
                                  casTowLabel = cms.untracked.InputTag("CastorTowerReco","","LEDcheck"), #"RECO"
                                  casJetLabel = cms.untracked.InputTag("ak5CastorJets","","LEDcheck"), #"RECO"
                                  pfEcalCluLabel = cms.untracked.InputTag("particleFlowClusterECAL","","RECO"),
                                  pfHcalCluLabel = cms.untracked.InputTag("particleFlowClusterHCAL","","RECO"),
                                  pfHFCluLabel = cms.untracked.InputTag("particleFlowClusterHF","","LEDcheck"),
                                  pfCandLabel = cms.untracked.InputTag("particleFlow","","RECO"),
                                  caloTowerLabel = cms.untracked.InputTag("towerMaker","","LEDcheck"), #"RECO"
                                  genPartLabel = cms.untracked.InputTag("genParticles","","HLT"),
                                  pedFCpath = cms.untracked.string(base_path),
                                  pedFCrun  = cms.untracked.int32(229479)
)

process.TFileService = cms.Service("TFileService",
                                    fileName = cms.string('{0}/test/{1}_raw_{2}_ped_{3}_{4}_{5}.root'.format(base_path,run_num,run_ped,cmssw_used,volt_set,post_fix)) #/tmp/katkov/
                                  )

process.options = cms.untracked.PSet(
 wantSummary = cms.untracked.bool(True)
)
#
###process.load("RecoLocalCalo.CastorReco.CastorSimpleReconstructor_cfi")
process.castorreco = cms.EDProducer("CastorSimpleReconstructor",
                                    correctionPhaseNS = cms.double(0.0),
                                    digiLabel = cms.InputTag("castorDigis"),
                                    samplesToAdd = cms.int32(2),
                                    Subdetector = cms.string('CASTOR'),
                                    firstSample = cms.int32(4),
                                    correctForPhaseContainment = cms.bool(False),
                                    correctForTimeslew = cms.bool(False),
                                    tsFromDB = cms.bool(True), #True #False
                                    setSaturationFlag = cms.bool(True),
                                    maxADCvalue = cms.int32(127),
                                    doSaturationCorr = cms.bool(True) #True #False
)
###process.castorreco.tsFromDB = cms.untracked.bool(False)
process.load('RecoLocalCalo.Castor.Castor_cff') #castor tower and jet reconstruction
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltFilter = process.hltHighLevel.clone()
process.hltFilter.HLTPaths = ["{0}".format(trig_sel)] #["HLT_ZeroBias_v1"]#["HLT_L1Tech_BPTXplus_OR_BPTXminus_v1"]#["HLT_L1CastorMuon_v1"]#HLT_Random_v2 #HLT_ZeroBias_v1 #HLT_L1Tech5_BPTX_PlusOnly_v1
process.hltFilter.andOr = cms.bool(True)
process.load("Configuration.StandardSequences.Reconstruction_cff")
#from Configuration.AlCa.autoCond import autoCond
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_P_V56::All'#autoCond['run2_data']
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '{0}'.format(glob_tag) #'GR_P_V56'
process.es_prefer_cas1 = cms.ESPrefer('PoolDBESSource','es_pool')
process.es_prefer_cas2 = cms.ESPrefer('CastorTextCalibrations','es_ascii')
process.load('Configuration.Geometry.GeometryRecoDB_cff')
#process.load('Configuration.Geometry.GeometryIdeal_cff')
#https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/RecoParticleFlow/PFClusterProducer/python/particleFlowCluster_cff.py
from RecoParticleFlow.PFClusterProducer.particleFlowRecHitHBHE_cfi import *
particleFlowRecHitHBHE.src = cms.InputTag("hbhereco","","LEDcheck") 
from RecoParticleFlow.PFClusterProducer.particleFlowClusterHBHE_cfi import *
particleFlowClusterHBHE.recHitsSource = cms.InputTag("particleFlowRecHitHBHE","","LEDcheck")
from RecoParticleFlow.PFClusterProducer.particleFlowClusterHCAL_cfi import *
particleFlowClusterHCAL.clustersSource = cms.InputTag("particleFlowClusterHBHE","","LEDcheck")
process.PFClustersHCAL = cms.Sequence(particleFlowRecHitHBHE*particleFlowClusterHBHE*particleFlowClusterHCAL)
#
from RecoParticleFlow.PFClusterProducer.particleFlowRecHitHF_cfi import *
particleFlowRecHitHF.src = cms.InputTag("hfreco","","LEDcheck")
from RecoParticleFlow.PFClusterProducer.particleFlowClusterHF_cfi import *
process.PFClustersHF = cms.Sequence(particleFlowRecHitHF*particleFlowClusterHF)
# https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/RecoJets/JetProducers/python/CaloTowerSchemeB_cfi.py
# https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/RecoLocalCalo/CaloTowersCreator/python/calotowermaker_cfi.py
from RecoLocalCalo.CaloTowersCreator.calotowermaker_cfi import *
towerMaker = calotowermaker.clone()
towerMaker.UseHO = False
towerMaker.hfInput = cms.InputTag("hfreco","","LEDcheck")
towerMaker.hbheInput = cms.InputTag("hbhereco","","LEDcheck")
##process.out = cms.OutputModule("PoolOutputModule",
##    outputCommands = cms.untracked.vstring('drop *', 'keep *_*HF_*_*'),
##    fileName = cms.untracked.string('filtered.root'))
#
# # # # # https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2
process.load('RecoMET.METFilters.CSCTightHaloFilter_cfi') #Beam Halo: process.CSCTightHaloFilter
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi') #Hcal HBHE: 
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
   reverseDecision = cms.bool(False)
)
# process.HBHENoiseFilterResultProducer* #produce bools
# process.ApplyBaselineHBHENoiseFilter*  #reject events
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter", #PV filter 
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24),
                                           maxd0 = cms.double(2)
                                           )
process.load('RecoMET.METFilters.eeBadScFilter_cfi') # Bad EE Supercrystal filter: process.eeBadScFilter
#
# # # # # https://github.com/CmsHI/HiForestProduction/blob/master/Appeltel/RpPbAnalysis/python/PAPileUpVertexFilter_cff.py
import Appeltel.RpPbAnalysis.PAPileUpVertexFilter_cfi
pileupVertexFilterCutG = Appeltel.RpPbAnalysis.PAPileUpVertexFilter_cfi.pileupVertexFilter.clone()
process.pileupVertexFilterCutGplusUpsPP = pileupVertexFilterCutG.clone(
    dzCutByNtrk = cms.vdouble(
        999., 999., 1.5, 1.0, 0.8,
        0.6, 0.5, 0.4, 0.3, 0.2,
        0.2, 0.2, 0.2, 0.2, 0.2,
        0.2, 0.0
    ),
    dxyVeto = cms.double(999.)
)
#
# # # # # https://github.com/iik1997/RHAnalyser/blob/master/test/rhanalyser_cfg.py
process.noscraping = cms.EDFilter("FilterOutScraping", #scraping removal
applyfilter = cms.untracked.bool(True),
debugOn = cms.untracked.bool(False),
numtrack = cms.untracked.uint32(10),
thresh = cms.untracked.double(0.25),
src = cms.untracked.InputTag('generalTracks')
)
#
process.primaryVertexFilter1 = cms.EDFilter("VertexSelector", #vertex selection
   src = cms.InputTag('offlinePrimaryVertices'),
   cut = cms.string("!isFake && tracksSize() > 1 && abs(z) <= 15 && position.Rho <= 2"), #("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"), # tracksSize() > 3 for the older cut ##abs(z) <= 15 # ndof > 4
   filter = cms.bool(True),   # if False it won't filter the events, just produce an empty vertex collection. ##True
)
process.justOnePlusVertexFilter = cms.EDFilter( ### in primaryVertexFilter set 'filter' to False to filter here on number of vertices
  "VertexCountFilter",
  src = cms.InputTag("primaryVertexFilter1"),
  minNumber = cms.uint32(1),
  maxNumber = cms.uint32(999)
)
#
#process.p = cms.Path(process.hltFilter*process.PFClustersHF*process.castorDigis*process.castorreco*process.CastorFullReco*process.cAnalyser)
#
process.es_pool_hcal = cms.ESSource("PoolDBESSource",
                               process.CondDBSetup,
                               timetype = cms.string('runnumber'),
                               toGet = cms.VPSet(
    cms.PSet(record = cms.string("HcalRespCorrsRcd"),
    tag = cms.string("HcalRespCorrs_updatedHBHEHFscale_Sep2015")
    )
  ),
  connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
  authenticationMethod = cms.untracked.uint32(0)
)
process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool_hcal" )
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi") #process.hcalDigis
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hbhe_cfi") #process.hbhereco or hbheprereco?
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_ho_cfi")   #process.horeco
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hf_cfi")   #process.hfreco
process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_zdc_cfi")  #process.zdcreco
#
process.p = cms.Path(
                     process.hltFilter*
                     process.noscraping*
                     process.CSCTightHaloFilter*
                     process.HBHENoiseFilterResultProducer*process.ApplyBaselineHBHENoiseFilter*
                     process.eeBadScFilter*
#                     process.primaryVertexFilter*
                     process.primaryVertexFilter1*
                     process.pileupVertexFilterCutGplusUpsPP*
                     process.hcalDigis*
                     process.hbhereco*
                     process.PFClustersHCAL*
                     process.hfreco*
                     process.PFClustersHF*
                     process.towerMaker*
                     process.castorDigis*process.castorreco*process.CastorFullReco*
                     process.cAnalyser)
#process.p = cms.Path(process.hltFilter*process.PFClustersHF*process.castorDigis*process.castorreco*process.CastorFullReco*process.cAnalyser)
#process.p = cms.Path(process.castorDigis*process.cAnalyser)
#
#process.p = cms.Path(process.dumpRaw*process.castorDigis*process.dump*process.m*process.dumpECA)
###process.p = cms.Path(process.castorDigis*process.castorpedestalsanalysis*process.cAnalyser)
###process.p = cms.Path(process.castorDigis*process.cAnalyser)
#process.ep = cms.EndPath(process.out)

