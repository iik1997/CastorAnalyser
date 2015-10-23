import FWCore.ParameterSet.Config as cms

process = cms.Process("LEDcheck")

import os
base_path = os.getenv('CMSSW_BASE', 'empty_defualt_value')

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
###
process.MessageLogger.destinations = ['{0}_detailedInfo00.txt'.format(argpedrun)]
###

process.source = cms.Source("HcalTBSource",
    streams = cms.untracked.vstring('HCAL_Trigger', 
        'HCAL_DCC690','HCAL_DCC691','HCAL_DCC692', 
),
    #fileNames = cms.untracked.vstring('/store/caft2/user/campbell/castor_localruns/USC_119814.root') USC_132564.root
    #fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/k/katkov/P5_2011/USC_argpedrun.root') #171702 #157405 #156686 #171704
    ###fileNames = cms.untracked.vstring('file:/tmp/katkov/USC_argpedrun.root')
    fileNames = cms.untracked.vstring('/store/group/phys_heavyions/katkov/cas_loc_2013/USC_argpedrun.root')
    #fileNames = cms.untracked.vstring('file:/tmp/katkov/USC_141894.root')  #157405
    #fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/k/katkov/P5_2010/USC_138974.root')
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) ##20000 10000 -1 100000
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
   InputLabel = cms.InputTag("source"), #source #rawDataCollector
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

# take one set of calibrations from txt and the rest from db;
# here: both emap and pedestals (as well as all the rest) - from db

###process.es_hardcode = cms.ESSource("CastorHardcodeCalibrations",
###    toGet = cms.untracked.vstring(
###        'ElectronicsMap'
#        'Gains', 
###        'Pedestals',         
#        'PedestalWidths', 
#        'GainWidths', 
#        'QIEShape', 
#        'QIEData', 
#        'ChannelQuality', 
#        'RespCorrs', 
#        'ZSThresholds'
###        )
###)

###process.es_ascii = cms.ESSource("CastorTextCalibrations",
###   input = cms.VPSet(
###                    cms.PSet(
###       object = cms.string('ElectronicsMap'),
###       file = cms.FileInPath('emap_dcc_nominal_Run121872.txt')
###                            ),
###                     cms.PSet(
###       object = cms.string('Pedestals'),
###       file = cms.FileInPath('peds_fC_Run156686.txt')
###                             )                      
###                     )
###)

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
#####       cms.PSet(
#####           record = cms.string('CastorPedestalsRcd'),
#####           #tag = cms.string('castor_pedestals_v1.0_test')
#####           tag = cms.string('castor_pedestals_v1.0')
#####           #tag = cms.string('castor_pedestals_v2.0')
#####           ),
#####       cms.PSet(
#####           record = cms.string('CastorPedestalWidthsRcd'),
#####           #tag = cms.string('castor_pedestalwidths_v1.0_test')
#####           tag = cms.string('castor_pedestalwidths_v1.0')
#####           #tag = cms.string('castor_pedestalwidths_v2.0')
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
       cms.PSet(
           record = cms.string('CastorSaturationCorrsRcd'),
           tag = cms.string('CastorSaturationCorrs_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorPedestalsRcd'),
           tag = cms.string('CastorPedestals_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorPedestalWidthsRcd'),
           tag = cms.string('CastorPedestalWidths_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorGainsRcd'),
           tag = cms.string('CastorGains_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorGainWidthsRcd'),
           tag = cms.string('CastorGainWidths_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorQIEDataRcd'),
           tag = cms.string('CastorQIEData_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorChannelQualityRcd'),
           tag = cms.string('CastorChannelQuality_v3.00_offline')
           ),
       cms.PSet(
           record = cms.string('CastorElectronicsMapRcd'),
           tag = cms.string('CastorElectronicsMap_v3.00_offline')
           )
   )
)


process.castorpedestalsanalysis = cms.EDAnalyzer("CastorPedestalsAnalysis",
    hiSaveFlag  = cms.untracked.bool( False ),
    verboseflag = cms.untracked.bool( True ),
        firstTS = cms.untracked.int32(0),
         lastTS = cms.untracked.int32(9),
         castorDigiCollectionTag = cms.InputTag('castorDigis')
)

#===================
# CASTOR Analyzer
#===================

process.cAnalyser = cms.EDAnalyzer('CastorAnalyser',
                                  firstTS = cms.untracked.int32(0),
                                  lastTS  = cms.untracked.int32(9),
                                  pedFCpath = cms.untracked.string(base_path),
                                  pedFCrun  = cms.untracked.int32(229479)
)

process.TFileService = cms.Service("TFileService",
                                    fileName = cms.string('{0}/test/argpedrun_ped_argpedrun_ped_argcmssw_arghv.root'.format(base_path)) #/tmp/katkov/
                                  )

process.options = cms.untracked.PSet(
 wantSummary = cms.untracked.bool(True)
)
#
process.load('Configuration.Geometry.GeometryIdeal_cff')
#process.p = cms.Path(process.dumpRaw*process.castorDigis*process.dump*process.m*process.dumpECA)
process.p = cms.Path(process.castorDigis*process.castorpedestalsanalysis*process.cAnalyser)
###process.p = cms.Path(process.castorDigis*process.cAnalyser)
#process.ep = cms.EndPath(process.out)

