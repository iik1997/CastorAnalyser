// -*- C++ -*-
//
// Package:    CastorAnalyser
// Class:      CastorAnalyser
// 
/**\class CastorAnalyser CastorAnalyser.cc Analysis/CastorAnalyser/src/CastorAnalyser.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Igor Katkov,32 4-B20,+41227676487,
//         Created:  Fri Jun 25 22:58:05 CEST 2010
// $Id$
//
//

//////////////***********************/////////// 
#define THIS_IS_MC 
#define DO_RECHITS
/////////#define GET_BY_LABEL
#define SW_75X
/////////Check also ecalScaleFactor etc
#define CM_ENERGY 5020 //13000.0 //2760.0 
#define CASTOR_ETA -5.9

// system include files
#include <memory>

#include <iostream>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// genparticles
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/JetReco/interface/GenJetCollection.h"

//Geometry
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

//Vertex
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
//Tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//HCAL Dataformats
//
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
//
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
//
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
//
#include "DataFormats/CastorReco/interface/CastorTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
//
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
//
#include "DataFormats/DetId/interface/DetId.h"
#include "CondFormats/CastorObjects/interface/CastorPedestals.h"
#include "CondFormats/CastorObjects/interface/CastorPedestalWidths.h"
#include "CondFormats/CastorObjects/interface/CastorQIECoder.h"
#include "CondFormats/CastorObjects/interface/CastorQIEData.h"
#include "CondFormats/CastorObjects/interface/CastorQIEShape.h"
#include "CondFormats/CastorObjects/interface/CastorElectronicsMap.h"
#include "CondFormats/CastorObjects/interface/AllObjects.h"

#include "CalibFormats/CastorObjects/interface/CastorDbRecord.h"
#include "CalibFormats/CastorObjects/interface/CastorDbService.h"
#include "CalibFormats/CastorObjects/interface/CastorCalibrations.h"
#include "CalibFormats/CastorObjects/interface/CastorCalibrationWidths.h"

//  #include "CondTools/Hcal/interface/HcalDbOnline.h"

#include "CalibCalorimetry/CastorCalib/interface/CastorDbASCIIIO.h"
// #include "CalibCalorimetry/CastorCalib/interface/CastorCondXML.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"

//trigger
#ifndef SW_75X
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#else
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h" 
#endif
//VVV
#include "FWCore/Common/interface/TriggerNames.h"
//^^^
#include "DataFormats/Common/interface/TriggerResults.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"

#include "boost/tuple/tuple.hpp"

//
// class declaration
//
//

namespace ForwardRecord {
  const uint nbEtaBins = 30; //20; //16 //////////////////////30//34
  const double Eta_Bin_Edges[nbEtaBins+1] = {
//    -10.0,
//
//    -8.3,  
//
    -6.6,   //IMPORTANT: 1st bin is castor bin! last bin is forced to be equivalent to 1st one!
//0 //-5.5,   //EtaMin = -5.5 if castor is not fully enabled in CMSSW MC
    -5.19, 
//1
    -4.89, 
//2
    -4.54, 
//3
    -4.19, 
//4
    -3.84, 
//5
    -3.49,
//6
    -3.14,
//7
    -2.65,
//8
    -2.17,
//9
    -1.74,
//10
    -1.39, 
//11
    -1.04,
//12
    -0.70,
//13
    -0.35,
//14
    0.00,
//15
    0.35, 
//16
    0.70, 
//17
    1.04, 
//18
    1.39, 
//19
    1.74, 
//20
    2.17,
//21
    2.65, 
//22
    3.14, 
//23
    3.49, 
//24
    3.84, 
//25
    4.19, 
//26
    4.54, 
//27
    4.89, 
//28
    5.19,
//29//5.5,
    6.6//,
//
//    8.3,
// 
//    10.0
  };

  const uint nbEtaBinsExt = 7;
  const double Eta_Bin_EdgesExt[nbEtaBinsExt+1] = {
     7.0,
     7.5,
     8.0,
     8.5,
     9.0,
     9.5,
     10.0,
     10.5
  };

  static const unsigned int CSectors = 16;
  static const unsigned int CModules = 14;
  static const double absCasEscaleFactor = 0.020;
#ifdef THIS_IS_MC
  static const double ecalScaleFactor = 1.0;
  static const double hbheScaleFactor = 1.0;
  static const double hfScaleFactor = 0.8952;  //0.9 //1.0/1.117~0.8952
#else
  static const double ecalScaleFactor = 1.0;
  static const double hbheScaleFactor = 1.0; //E-over-p ~ 0.8 and correction 1/0.8~1.25; but did not it change to E-over-p ~ 0.88 that is 1.14 ? //1.14
  static const double hfScaleFactor = 1.0; //1.15 !!!!!
#endif
} //end namespace ForwardRecord

namespace castor {
  //E-map swap: s5m10<->s5m12, s6m10<->s6m12, s7m10<->s7m12
  //using for Katerina's value's (some correction values seem too big (for me big is all >3), some are zeros)
  const bool channelQuality[ForwardRecord::CSectors][ForwardRecord::CModules] =             // sector
    //mod 1   2     3     4     5      6    7     8     9    10     11    12   13    14
    {{true ,true ,true ,false,true ,true ,false,true ,true ,true ,true ,true ,true ,true }, // 1
     {true ,true ,true ,true ,false,true ,false,true ,true ,false,true ,true ,true ,true }, // 2 //----------->5TeV data: s2m5!!! <----------------<<<
     {true ,true ,true ,true ,true ,true ,false,false,false,true ,false,true ,true ,true }, // 3 //s3m9 ? 
     {true ,true ,true ,true ,true ,true ,false,false,false,true ,false,true ,false,true }, // 4
     {true ,false,true ,true ,true ,true ,false,false,false,false,true ,false,true ,true }, // 5
     {true ,true ,true ,true ,true ,true ,false,false,false,false,true ,false,true ,true }, // 6 //s6m9 ?
     {true ,true ,true ,true ,false,true ,false,true ,true ,false,false,false,false,false}, // 7 //s7m10-14katerina?
     {true ,true ,true ,true ,true ,true ,false,false,true ,false,false,false,false,false}, // 8 //s8m10-14katerina?
     {true ,true ,true ,true ,true ,true ,false,true ,true ,true ,true ,true ,false,true }, // 9 //s9m13?
     {true ,true ,true ,true ,true ,true ,false,true ,true ,true ,true ,true ,true ,true }, // 10
     {true ,true ,true ,true ,true ,true ,false,false,true ,true ,false,true ,true ,true }, // 11 //s11m11katerina?(was also there before)
     {true ,true ,true ,true ,true ,true ,false,false,true ,true ,false,true ,true ,true }, // 12 //s12m11katerina?(was also there before)
     {true ,true ,true ,true ,true ,false,false,false,false,true ,false,true ,false,true }, // 13 //s13m11katerina?(was also there before) //m9s13-SNP//m13s13-SNP
     {true ,true ,true ,true ,true ,true ,false,false,true ,true ,false,true ,true ,false}, // 14 //s14m11katerina?(was also there before)
     {true ,true ,true ,true ,true ,true ,false,false,true ,false,true ,true ,true ,true }, // 15
     {true ,true ,true ,true ,true ,false,false,false,true ,true ,true ,true ,true ,true }};// 16 //m8s16-strange-noise-peak(SNP),closer look needed

  // Katerina's values using halo muon data (w/o TOTEM), already scaled by s9m4 (what about factor 2 EM vs HAD ?)
  const double channelGainQE[ForwardRecord::CSectors][ForwardRecord::CModules] =                                                                                // sector
    //mod 1          2          3          4           5          6          7         8          9          10         11          12        13           14
    {{  0.7510,    0.8700,    2.7370,    0.0000,    0.3630,    0.6430,    0.0000,    0.3100,    0.2120,    0.2740,    0.3030,    0.1690,    0.2650,    0.1550}, //1
     {  0.6190,    0.6160,    1.8130,    0.8690,    0.1820,    0.6280,    0.0000,    0.5070,    0.1680,    0.2910,    0.3380,    0.1460,    0.2490,    0.1250}, //2
     {  1.0700,    0.6510,    1.4250,    0.7660,    0.3040,    0.1930,    8.2170,   13.2900,    0.4650,    0.2350,    0.0000,    0.2950,    0.3430,    0.3510}, //3
     {  0.5310,    0.3300,    0.8910,    0.8260,    0.1170,    0.3300,    0.0000,    0.0000,    0.0000,    0.6390,    0.0000,    0.3170,    0.0000,    0.4580}, //4
     {  0.6120,    0.0000,    1.3410,    0.7020,    0.1560,    0.5690,    0.8360,    0.0000,    0.0000,    0.5230,    0.2360,    0.3290,    0.3990,    0.3420}, //5
     {  1.3130,    0.4870,    1.4000,    0.6320,    0.1990,    0.7950,    1.2090,    0.0000,    0.5100,    0.7060,    0.2330,    0.2800,    0.4830,    0.4410}, //6
     {  0.4160,    0.2820,    1.0430,    0.3130,    0.1140,    0.0860,  250.6690,    0.1950,    0.4200,    6.9160,    3.4790,    1.5110,    4.8590,    3.5340}, //7
     {  0.3420,    0.2950,    1.1980,    1.4030,    0.2130,    1.0730,    0.0000,    0.2060,    0.6350,   27.2690,    9.4210,    3.3400,    3.4880,    1.0100}, //8
     {  0.3030,    0.8460,    1.4120,    1.0000,    0.2180,    0.8830,    0.0000,    0.1320,    0.1950,    0.2490,    0.2250,    0.2270,    0.2990,    0.2780}, //9
     {  0.9040,    1.4030,    2.6580,    1.1900,    0.2350,    1.5570,    0.0000,    0.3160,    0.1990,    0.3100,    0.1790,    0.2510,    0.2510,    0.2520}, //10
     {  1.0160,    0.9930,    1.6950,    0.8870,    0.2850,    0.6230,    0.0000,   10.0790,    0.3730,    0.2440,    9.6350,    0.5240,    0.6990,    0.3790}, //11
     {  1.1690,    1.1300,    2.1400,    1.3920,    0.2630,    1.2470,    0.0000,    0.0000,    0.5670,    0.3030,   99.3510,    0.3510,    0.1980,    0.3560}, //12
     {  0.9160,    1.2700,    1.6430,    0.8070,    0.2310,    2.3020,    0.0000,    0.0000,    0.3230,    0.2910,    0.0000,    0.3430,    0.1280,    0.3080}, //13
     {  0.6010,    0.9840,    2.1400,    0.8210,    0.1770,    1.0970,    0.0000,    0.0000,    0.2030,    0.2920,   16.6350,    0.3020,    0.3510,    0.3680}, //14
     {  0.7590,    1.3650,    2.9620,    1.1740,    0.3800,    2.3370,    0.0000,  517.2540,    0.2690,    0.0000,    0.1940,    0.2740,    0.2800,    0.4100}, //15
     {  0.7420,    0.9720,    2.4600,    0.9240,    0.2200,    0.1630,    3.9070,    0.1970,    0.2700,    0.2580,    0.1510,    0.1340,    0.2790,    0.2620}};//16

  //LED data, r238434(B=3.8T) to r238342(B=0T), HV ~ 1800V, average amplitude w/o ZS
  const double corr0Tto38T[ForwardRecord::CSectors][ForwardRecord::CModules] =
    //mod 1          2              3          4           5               6          7               8          9          10              11          12        13           14
    {{     1.48512,     1.39079,     1.23936,     1.20485,     1.53704,     0.53953,     0.00404,     0.73703,     0.99343,     0.97474,     1.07208,     1.00473,     1.21792,     1.38684}, // s 1
     {     1.34932,     1.31645,     1.16597,     1.07258,     1.10938,     0.91177,     0.00558,     0.06076,     0.94764,     0.92360,     0.96920,     1.16183,     1.18510,     1.23530}, // s 2
     {     1.19602,     1.46404,     1.26956,     1.27067,     1.55640,     0.28252,     0.00494,    -0.00000,     0.04254,     0.97351,     1.00799,     1.01488,     1.17762,     1.33190}, // s 3
     {     1.28421,     1.54017,     1.14003,     1.21478,     1.62988,     0.52778,     0.00217,    -0.00000,    -0.00000,     0.85758,     1.01701,     1.11001,     1.15505,     1.45989}, // s 4
     {     1.08401,     1.27912,     1.15338,     1.02110,     1.48480,     0.18463,     0.04877,    -0.00000,     0.00035,     0.93851,     0.91677,     1.22820,     1.04381,     1.25080}, // s 5
     {     1.09123,     1.19290,     1.02762,     1.15109,     1.01973,     0.27162,     0.01023,     0.00001,     0.22811,     0.95087,     1.02469,     0.94455,     1.14685,     1.19158}, // s 6
     {     1.35555,     1.28333,     0.80874,     1.08100,     0.98072,     0.39716,     0.00095,     0.29955,     1.01768,     0.94803,     0.88673,     1.02905,     1.02306,     1.40891}, // s 7
     {     1.40568,     1.28872,     1.01763,     1.12056,     1.12694,     0.05847,     0.00022,     0.90227,     1.01943,     1.14345,     1.04596,     0.96388,     1.15198,     1.29044}, // s 8
     {     1.06251,     1.14400,     1.01962,     1.03241,     1.02967,     0.18843,     0.00002,     0.80946,     0.87329,     0.88913,     0.86814,     1.13983,     1.05866,     1.04589}, // s 9
     {     1.01657,     1.16994,     1.03485,     1.06841,     1.12475,     0.21338,     0.00008,     0.37391,     0.92616,     0.95543,     0.95621,     1.01440,     0.86022,     1.37397}, // s10
     {     0.98176,     0.98911,     0.99062,     1.03866,     1.21427,     0.10842,     0.00007,     0.05552,     0.71987,     0.87018,     0.89760,     0.95953,     1.19473,     1.22346}, // s11
     {     0.98305,     0.99199,     0.99141,     1.08809,     1.13973,     0.14923,     0.00018,     0.00005,     0.36019,     0.86800,     0.93984,     1.06136,     1.08128,     1.07615}, // s12
     {     0.99641,     0.99703,     0.71680,     1.00467,     1.24617,     0.03526,     0.00227,    -0.00000,     0.36686,     0.68199,     0.98805,     1.09305,     1.19635,     1.33507}, // s13
     {     1.00974,     1.00226,     0.98903,     1.03647,     1.27846,     0.39654,     0.00116,     0.12479,     0.66424,     1.14252,     1.01872,     1.12014,     1.26828,     0.77948}, // s14
     {     1.17599,     1.08516,     1.01189,    -0.00000,     1.36638,     0.07736,    -0.00000,     0.00148,     0.95000,     0.86826,     0.97357,     1.04934,     1.09820,     1.07579}, // s15
     {     1.05973,     1.13987,     1.05313,     0.83706,     1.31051,     0.33701,     0.00358,     0.74754,     0.97755,     0.87077,     1.01832,     1.10647,     1.15042,     1.08966}};// s16

  //https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_7_4_X/RecoLocalCalo/CastorReco/src/CastorSimpleRecAlgo.cc
  // timeshift implementation
  //
  static const float wpksamp0_hf = 0.500635;
  static const float scale_hf    = 0.999301;
  static const int   num_bins_hf = 100;

  static const float actual_ns_hf[num_bins_hf] = {
    0.00000, // 0.000-0.010
    0.03750, // 0.010-0.020
    0.07250, // 0.020-0.030
    0.10750, // 0.030-0.040
    0.14500, // 0.040-0.050
    0.18000, // 0.050-0.060
    0.21500, // 0.060-0.070
    0.25000, // 0.070-0.080
    0.28500, // 0.080-0.090
    0.32000, // 0.090-0.100
    0.35500, // 0.100-0.110
    0.39000, // 0.110-0.120
    0.42500, // 0.120-0.130
    0.46000, // 0.130-0.140
    0.49500, // 0.140-0.150
    0.53000, // 0.150-0.160
    0.56500, // 0.160-0.170
    0.60000, // 0.170-0.180
    0.63500, // 0.180-0.190
    0.67000, // 0.190-0.200
    0.70750, // 0.200-0.210
    0.74250, // 0.210-0.220
    0.78000, // 0.220-0.230
    0.81500, // 0.230-0.240
    0.85250, // 0.240-0.250
    0.89000, // 0.250-0.260
    0.92750, // 0.260-0.270
    0.96500, // 0.270-0.280
    1.00250, // 0.280-0.290
    1.04250, // 0.290-0.300
    1.08250, // 0.300-0.310
    1.12250, // 0.310-0.320
    1.16250, // 0.320-0.330
    1.20500, // 0.330-0.340
    1.24500, // 0.340-0.350
    1.29000, // 0.350-0.360
    1.33250, // 0.360-0.370
    1.38000, // 0.370-0.380
    1.42500, // 0.380-0.390
    1.47500, // 0.390-0.400
    1.52500, // 0.400-0.410
    1.57750, // 0.410-0.420
    1.63250, // 0.420-0.430
    1.69000, // 0.430-0.440
    1.75250, // 0.440-0.450
    1.82000, // 0.450-0.460
    1.89250, // 0.460-0.470
    1.97500, // 0.470-0.480
    2.07250, // 0.480-0.490
    2.20000, // 0.490-0.500
    19.13000, // 0.500-0.510
    21.08750, // 0.510-0.520
    21.57750, // 0.520-0.530
    21.89000, // 0.530-0.540
    22.12250, // 0.540-0.550
    22.31000, // 0.550-0.560
    22.47000, // 0.560-0.570
    22.61000, // 0.570-0.580
    22.73250, // 0.580-0.590
    22.84500, // 0.590-0.600
    22.94750, // 0.600-0.610
    23.04250, // 0.610-0.620
    23.13250, // 0.620-0.630
    23.21500, // 0.630-0.640
    23.29250, // 0.640-0.650
    23.36750, // 0.650-0.660
    23.43750, // 0.660-0.670
    23.50500, // 0.670-0.680
    23.57000, // 0.680-0.690
    23.63250, // 0.690-0.700
    23.69250, // 0.700-0.710
    23.75000, // 0.710-0.720
    23.80500, // 0.720-0.730
    23.86000, // 0.730-0.740
    23.91250, // 0.740-0.750
    23.96500, // 0.750-0.760
    24.01500, // 0.760-0.770
    24.06500, // 0.770-0.780
    24.11250, // 0.780-0.790
    24.16000, // 0.790-0.800
    24.20500, // 0.800-0.810
    24.25000, // 0.810-0.820
    24.29500, // 0.820-0.830
    24.33750, // 0.830-0.840
    24.38000, // 0.840-0.850
    24.42250, // 0.850-0.860
    24.46500, // 0.860-0.870
    24.50500, // 0.870-0.880
    24.54500, // 0.880-0.890
    24.58500, // 0.890-0.900
    24.62500, // 0.900-0.910
    24.66500, // 0.910-0.920
    24.70250, // 0.920-0.930
    24.74000, // 0.930-0.940
    24.77750, // 0.940-0.950
    24.81500, // 0.950-0.960
    24.85250, // 0.960-0.970
    24.89000, // 0.970-0.980
    24.92750, // 0.980-0.990
    24.96250, // 0.990-1.000
  };

  struct pObject {
    long double Energy;
    long double Px;
    long double Py;
    long double Pz;
    double Eta;
    double Rap;
  };

} //end namespace castor

struct standaloneclass { //http://en.cppreference.com/w/cpp/algorithm/sort
  bool operator()(castor::pObject a1, castor::pObject a2) { return a1.Rap < a2.Rap; } //{ return a1.Eta < a2.Eta; } //must sort from negative to positive rapidities
} customLess;


class CastorAnalyser : public edm::EDAnalyzer {
public:
  explicit CastorAnalyser(const edm::ParameterSet&);
  ~CastorAnalyser();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void SetPedestals(int runNo, std::string pathString);

  int nfCBins_;
  float fCBinEdges_[290]; //129| /////////////////////////////////145
  float GetNonInvConv2fC(int iQIE);
  //float GetInvConv2fC(int iQIE);
  void SetNonInvfCBins();
  //void SetInvfCBins();
  float GetfCBinWidth(float signal);

  float timeshift_ns_hf(float wpksamp);

  // ----------member data ---------------------------

private:

  struct NewBunch
  {
    HcalCastorDetId detid;
    bool usedflag;
    double tsCapId[10];
    double tsAdc[10];
    double tsfC[10];
  };

  struct CastorMonVariables
  {
    // event variables
    uint runNb ;
    uint evtNb ;
    uint bxNb ;
    uint orbitNb ;
    uint lumiSectNb ;
    //
    uint vtxNb;
    uint vtxNbNotFake;
    uint vtxTrkNb;
    uint vtxIsFake0;
    double vtxRho;
    double vtxZ;
    double vtxZerr;
    double vtxY;
    double vtxX;
    //
    uint trkNb;
    double trkLeadingPt;
    double trkLeadingEta;
    double trkLeadingPhi;
    uint trkLeadingNbValidPixelHits;
    uint trkLeadingNbValidStripHits;
    uint trkHighNb;
    double trkEtaMax;
    double trkEtaMin;
    //
    uint casDigiPresamples; //???// int digi.presamples() //???//
    uint casDigiSize;
    double casEnergy;
    uint casNb;
    double casEnergyRH;
    double casLeadingRHE;
    double casLeadingRHT;
    //
    uint hfNb;
    uint hfmNb;
    uint hfmNb4;
    uint hfpNb4;
    uint hfmNb5;
    uint hfpNb5;
    uint hfmNb6;
    uint hfpNb6;
    double hfmE;
    double hfmE4all1;
    double hfmE4long1;
    double hfmE4short1;
    double hfmE4all2;
    double hfmE4long2;
    double hfmE4short2;
    double hfmLeadingE;
    double hfmLeadingT;
    double hfpE;
    uint hbheNb;
    uint hbhemNb;
    double hbhemE;
    uint ebNb;
    uint ebmNb;
    double ebmE;
    uint eeNb;
    uint eemNb;
    double eemE;
    //
    uint casTowNb;
    double casTowE;
    double casTowEt;
    uint casJetNb;
    double casLeadingJetE;
    //
    uint pfCluEcalNb;
    double pfLeadingCluEcalE;
    uint pfCluHcalNb;
    double pfLeadingCluHcalE;
    uint pfCluHFNb;
    double pfLeadingCluHFE;
    double pfCluEtaMax;
    double pfCluEtaMin;
    //
    uint pfCandNb;
    double pfLeadingCandE;
    double pfCandEtaMax;
    double pfCandEtaMin;
    //
    uint pfJetNb;
    double pfLeadingJetE;
    //
    uint caloTowerNb;
    uint caloTowerBSCmNb;
    uint caloTowerBSCpNb;
    uint caloTowerBSC1mNb;
    uint caloTowerBSC1pNb;
    uint caloTowerHFmNb;
    uint caloTowerHFmNb4;
    uint caloTowerHFpNb4;
    uint caloTowerHFmNb5;
    uint caloTowerHFpNb5;
    uint caloTowerHFmNb6;
    uint caloTowerHFpNb6;
    double caloLeadingTowerHFE;
    int caloLeadingTowerieta;
    int caloLeadingToweriphi;
    double caloTowerEtaMax;
    double caloTowerEtaMin;
    double log10xiSDdet;
    double log10xiDDdet;
    double deltaRapMaxdet;
    //
    uint nbGenPart;
    uint nbStat1GenPart;
    uint nbStat1ChPt250Eta2p5;
    uint nbStat1ChEtap1p94p9;
    uint nbStat1ChEtam3p51p5;
    uint nbStat1GenPartInBSCm;
    uint nbStat1GenPartInBSCp;
    uint nbStat1GenPartInBSC1m;
    uint nbStat1GenPartInBSC1p;
    double log10xiSD;
    double log10xiDD;
    double genPartCmEnergy;
    double genPartEtaMax;
    double genPartEtaMin;
    double genPartDeltaRapMax;
    //
    uint nbChan;
    double digiChargeSum[224];
    double digiTime[224];
    double digiTime1[224];
    int digiSector[224];
    int digiModule[224];
    int digiCapIdTs0[224];
    int digiAboveNoise[224];
    int digiSaturated[224];
    int digiMarkedBad[224];
    double digiPulseFC[224][10];
    double digiPulseFCped[224][10];
    //
    uint nbEtaIntervals;
    uint nbEtaIntervalsExt;
    double energyInEtaIntervals[ForwardRecord::nbEtaBins]; //PFClu
    double energyInEtaIntervals1[ForwardRecord::nbEtaBins];//CaloTow
    double energyInEtaIntervals2[ForwardRecord::nbEtaBins];//RH
    double energyInEtaIntervals3[ForwardRecord::nbEtaBins];//GenParticles
    double energyInEtaIntervals4[ForwardRecord::nbEtaBins];//PFCand
    double energyInEtaIntervals5[ForwardRecord::nbEtaBins];//PFCandw/cuts
    double energyInEtaIntervals6[ForwardRecord::nbEtaBins];//PFCandtyp4
    double energyInEtaIntervals7[ForwardRecord::nbEtaBins];//PFCandtyp5
    double energyInEtaIntervals8[ForwardRecord::nbEtaBins];//PFCandtyp6
    double energyInEtaIntervals9[ForwardRecord::nbEtaBins];//PFCandtyp7
    double etInEtaIntervals[ForwardRecord::nbEtaBins];     //PFClu
    double etInEtaIntervals1[ForwardRecord::nbEtaBins];    //CaloTow
    double etInEtaIntervals2[ForwardRecord::nbEtaBins];    //RH
    double etInEtaIntervals3[ForwardRecord::nbEtaBins];    //GenParticles
    double etInEtaIntervals33[ForwardRecord::nbEtaBins];   //GenParticles/ATLASthresholds
    double etInEtaIntervals333[ForwardRecord::nbEtaBinsExt];   //GenParticles/extended bins
    double etInEtaIntervals4[ForwardRecord::nbEtaBins];    //PFCand
    double etInEtaIntervals5[ForwardRecord::nbEtaBins];    //PFCandw/cuts
    //
    double etInEtaIntervals10[ForwardRecord::nbEtaBins];   //CaloTowerSomeExcluded
  } ;

  bool _ShowDebug;
  std::string _hltPathName;
  edm::InputTag _vtxSrc; 
  edm::InputTag _casDigiSrc;
  edm::InputTag _hfRHSrc;
  edm::InputTag _hbheRHSrc;
  edm::InputTag _ebRHSrc;
  edm::InputTag _eeRHSrc;
  edm::InputTag _casRHSrc;
  edm::InputTag _casTowSrc;
  edm::InputTag _casJetSrc;
  edm::InputTag _pfCluEcalSrc;
  edm::InputTag _pfCluHcalSrc;
  edm::InputTag _pfCluHFSrc;
  edm::InputTag _caloTowerSrc;
  edm::InputTag _trkSrc;
  edm::InputTag _GenPartSrc;
  edm::InputTag _pfCandSrc;
  edm::InputTag _trgResSrc;
  edm::InputTag _pfJetSrc;
  std::string _pedFilePath;
  int _pedFileRun;

#ifndef SW_75X
  HLTConfigProvider hltConfig_;
#else
  HLTPrescaleProvider hltPrescaleProvider_;
#endif
  int triggerBit_;


#ifndef GET_BY_LABEL
  //tokens
  typedef boost::tuple<
    //edm::EDGetTokenT<trigger::TriggerEventWithRefs>,
    //edm::EDGetTokenT<reco::GenParticleCollection>,
    //edm::EDGetTokenT<reco::MuonCollection> 
    //edm::EDGetTokenT<edm::TriggerResults>,
    //edm::EDGetTokenT<CastorRecHitCollection> 
    edm::EDGetTokenT<CastorDigiCollection>,
    edm::EDGetTokenT<HFRecHitCollection>,
    edm::EDGetTokenT<HBHERecHitCollection>,
    edm::EDGetTokenT<EBRecHitCollection>,
    edm::EDGetTokenT<CastorRecHitCollection>,
    edm::EDGetTokenT<reco::CastorTowerCollection>,
    edm::EDGetTokenT<reco::BasicJetCollection>,
    edm::EDGetTokenT<reco::PFClusterCollection>,
    edm::EDGetTokenT<reco::PFClusterCollection>,
    edm::EDGetTokenT<reco::PFClusterCollection>
    > TokenTuple; //this tuple is FULL (cannot be bigger than 10 items)
  TokenTuple myTokens_;
  typedef boost::tuple<
    edm::EDGetTokenT<CaloTowerCollection>,
    edm::EDGetTokenT<EERecHitCollection>,
    edm::EDGetTokenT<reco::VertexCollection>,
    edm::EDGetTokenT<reco::TrackCollection>,
    edm::EDGetTokenT<reco::GenParticleCollection>,
    edm::EDGetTokenT<reco::PFCandidateCollection>,
    edm::EDGetTokenT<edm::TriggerResults>,
    edm::EDGetTokenT<reco::PFJetCollection> 
    > TokenTupleExt;
  TokenTupleExt myTokensExt_;
#endif

private:
  const CaloGeometry* geo_;
  double protonPDGMass_;
  double cmEnergyDet_;
  
  int firstTS_;
  int lastTS_;
  bool firstTime_;
  bool firstTimeAnalyse_;
  std::vector<NewBunch> Bunches_; //Container for data, 1 per channel
  int iEvt_;
  double noUseValue_;
  uint numBunches_;

  //TFile *file_;
  TTree * tree_;

  CastorMonVariables treeVariables_;
  float pedestalValue_[224][4];
  float pedestalWidth_[224][4];

  std::map< std::string, TH2D* > histos2D_;
  std::map< std::string, TH1D* > histos1D_;
  std::map< std::string, TProfile* > histosProfile_;  

  ////TH1 *h_runnb_;
  //TH1 *h_pulsets_;
  //TH1 *h_pulsets_sect_[16];
  //TH1 *h_charge_[224];
  //TH2 *h_energyzs_map_;
  //TH2 *h_energysqs_map_;
  //TH2 *h_energysum_map_;
  //TH2 *h_occup_map_;
  //TH2 *h_chmean_map_;
  //TH2 *h_chmeanene_map_;
  //TH2 *h_chmeanene_bcor_map_;
  //TH2 *h_chrms_map_;
  //
  //TH2 *h_ch_vs_ts_;
  //TH1 *h_hit_time_;
  //TH2 *h_hittime_mean_map_;
  //TH2 *h_hittime_rms_map_;
  //TH1 *h_hit_time_ch_[224];
  //TProfile *h_casene_vs_bx_;
  //TProfile *h_casene_vs_bx_sel_;
  //TProfile *h_casene_vs_iphi_;
  //TH1 *h_goodch_vs_iphi_;
  //TProfile *h_casene_vs_iz_;
  //TH1 *h_goodch_vs_iz_;

  //TH2 *h_hfme_vs_case_;
  //TH2 *h_hbheme_vs_case_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CastorAnalyser::CastorAnalyser(const edm::ParameterSet& iConfig) :
  _ShowDebug ( iConfig.getUntrackedParameter<bool>("showDebug",true) ),
  _hltPathName ( iConfig.getUntrackedParameter<std::string>("hltPathName","HLT_ZeroBias_part0_v1") ),
  _vtxSrc ( iConfig.getUntrackedParameter<edm::InputTag>("vtxLabel",edm::InputTag("offlinePrimaryVertices","","RECO")) ),
  _casDigiSrc ( iConfig.getUntrackedParameter<edm::InputTag>("casDigiLabel",edm::InputTag("castorDigis","","LEDcheck")) ),
  _hfRHSrc ( iConfig.getUntrackedParameter<edm::InputTag>("hfRHLabel",edm::InputTag("hfreco","","RECO"))  ), 
  _hbheRHSrc ( iConfig.getUntrackedParameter<edm::InputTag>("hbheRHLabel",edm::InputTag("hbhereco","","RECO"))  ), 
  _ebRHSrc ( iConfig.getUntrackedParameter<edm::InputTag>("ebRHLabel",edm::InputTag("ecalRecHit","EcalRecHitsEB","RECO"))  ),  //https://github.com/cms-sw/cmssw/blob/2b75137e278b50fc967f95929388d430ef64710b/Validation/EcalRecHits/src/EcalBarrelRecHitsValidation.cc
  //_ebRHSrc ( iConfig.getUntrackedParameter<edm::InputTag>("ebRHLabel",edm::InputTag("reducedEcalRecHitsEB","","RECO"))  ),
  _eeRHSrc ( iConfig.getUntrackedParameter<edm::InputTag>("eeRHLabel",edm::InputTag("ecalRecHit","EcalRecHitsEE","RECO"))  ), //"ecalRecHit" "EcalRecHitsEE" "RECO"
  _casRHSrc ( iConfig.getUntrackedParameter<edm::InputTag>("casRHLabel",edm::InputTag("castorreco","","RECO")) ),
  _casTowSrc ( iConfig.getUntrackedParameter<edm::InputTag>("casTowLabel",edm::InputTag("CastorTowerReco","","RECO")) ),
  _casJetSrc ( iConfig.getUntrackedParameter<edm::InputTag>("casJetLabel",edm::InputTag("ak5CastorJets","","RECO")) ),
  _pfCluEcalSrc ( iConfig.getUntrackedParameter<edm::InputTag>("pfEcalCluLabel",edm::InputTag("particleFlowClusterECAL","","RECO")) ),
  _pfCluHcalSrc ( iConfig.getUntrackedParameter<edm::InputTag>("pfHcalCluLabel",edm::InputTag("particleFlowClusterHCAL","","RECO")) ),
  //Type                         Module                    Label       Process      
  //vector<reco::PFCluster>      "particleFlowClusterHF"   ""          "LEDcheck"   
  //vector<reco::PFRecHit>       "particleFlowRecHitHF"    ""          "LEDcheck"   
  //vector<reco::PFRecHit>       "particleFlowRecHitHF"    "Cleaned"   "LEDcheck"
  _pfCluHFSrc ( iConfig.getUntrackedParameter<edm::InputTag>("pfHFCluLabel",edm::InputTag("particleFlowClusterHF","","LEDcheck")) ),
  _caloTowerSrc ( iConfig.getUntrackedParameter<edm::InputTag>("caloTowerLabel",edm::InputTag("towerMaker","","RECO")) ),
  _trkSrc ( iConfig.getUntrackedParameter<edm::InputTag>("trkLabel",edm::InputTag("generalTracks","","RECO")) ),
  _GenPartSrc ( iConfig.getUntrackedParameter<edm::InputTag>("genPartLabel",edm::InputTag("genParticles","","HLT")) ),
  //vector<reco::PFCandidate>             "particleFlow"              ""                "RECO"
  _pfCandSrc  ( iConfig.getUntrackedParameter<edm::InputTag>("pfCandLabel",edm::InputTag("particleFlow","","RECO")) ),
  _trgResSrc  ( iConfig.getUntrackedParameter<edm::InputTag>("trgResLabel",edm::InputTag("TriggerResults","","HLT")) ),
  //vector<reco::PFJet>                   "ak4PFJets"                 ""                "RECO"
  _pfJetSrc   ( iConfig.getUntrackedParameter<edm::InputTag>("pfJetLabel" ,edm::InputTag("ak4PFJets","","RECO")) ),
  _pedFilePath ( iConfig.getUntrackedParameter<std::string>("pedFCpath","/afs/cern.ch/work/k/katkov/hiforest/LEDnewB/CMSSW_7_3_0_pre2") ),
  _pedFileRun ( iConfig.getUntrackedParameter<int>("pedFCrun",239027) ) //229479
#ifdef SW_75X
  ,hltPrescaleProvider_(iConfig, consumesCollector(), *this) 
#endif
{

  //now do what ever initialization is needed
  //

#ifndef GET_BY_LABEL
  edm::EDGetTokenT<CastorDigiCollection> tok_input0;
  tok_input0 = consumes<CastorDigiCollection>( _casDigiSrc );

  edm::EDGetTokenT<HFRecHitCollection> tok_input1;
  tok_input1 = consumes<HFRecHitCollection>( _hfRHSrc );

  edm::EDGetTokenT<HBHERecHitCollection> tok_input2;
  tok_input2 = consumes<HBHERecHitCollection>( _hbheRHSrc );

  edm::EDGetTokenT<EBRecHitCollection> tok_input3;
  tok_input3 = consumes<EBRecHitCollection>( _ebRHSrc ); //_ebRHSrc ); //edm::InputTag("reducedEcalRecHitsEB","","RECO")

  edm::EDGetTokenT<EERecHitCollection> tok_input3a;
  tok_input3a = consumes<EERecHitCollection>( _eeRHSrc );

  edm::EDGetTokenT<CastorRecHitCollection> tok_input4;
  tok_input4 = consumes<CastorRecHitCollection>( _casRHSrc );

  edm::EDGetTokenT<reco::CastorTowerCollection> tok_input5;
  tok_input5 = consumes<reco::CastorTowerCollection>( _casTowSrc );  //edm::InputTag("CastorTowerReco","","RECO")

  edm::EDGetTokenT<reco::BasicJetCollection> tok_input6;
  tok_input6 = consumes<reco::BasicJetCollection>( _casJetSrc ); //edm::InputTag("ak5CastorJets","","RECO")

  edm::EDGetTokenT<reco::PFClusterCollection> tok_input7;
  tok_input7 = consumes<reco::PFClusterCollection>( _pfCluEcalSrc );
  edm::EDGetTokenT<reco::PFClusterCollection> tok_input8;
  tok_input8 = consumes<reco::PFClusterCollection>( _pfCluHcalSrc );
  edm::EDGetTokenT<reco::PFClusterCollection> tok_input9;
  tok_input9 = consumes<reco::PFClusterCollection>( _pfCluHFSrc );

  edm::EDGetTokenT<CaloTowerCollection> tok_input10;
  tok_input10 = consumes<CaloTowerCollection>( _caloTowerSrc );

  edm::EDGetTokenT<reco::VertexCollection> tok_input11;
  tok_input11 = consumes<reco::VertexCollection>( _vtxSrc ); 

  edm::EDGetTokenT<reco::TrackCollection> tok_input12;
  tok_input12 = consumes<reco::TrackCollection>( _trkSrc );

  edm::EDGetTokenT<reco::GenParticleCollection> tok_input13;
  tok_input13 = consumes<reco::GenParticleCollection>( _GenPartSrc );

  edm::EDGetTokenT<reco::PFCandidateCollection> tok_input14;
  tok_input14 = consumes<reco::PFCandidateCollection>( _pfCandSrc );

  edm::EDGetTokenT<edm::TriggerResults> tok_input15;
  tok_input15 = consumes<edm::TriggerResults>( _trgResSrc );

  edm::EDGetTokenT<reco::PFJetCollection> tok_input16;
  tok_input16 = consumes<reco::PFJetCollection>( _pfJetSrc );

  TokenTuple myTuple(tok_input0,
                     tok_input1,tok_input2,
		     tok_input3,
		     tok_input4,tok_input5,tok_input6,tok_input7,tok_input8,tok_input9);

  myTokens_ = myTuple;

  TokenTupleExt myTupleExt(tok_input10,
                           tok_input3a,
                           tok_input11,tok_input12,tok_input13,tok_input14,tok_input15,tok_input16);

  myTokensExt_ = myTupleExt;
#endif

  // std::cout << "!!!Check!!! that /tmp/katkov/*.root EXIST!!! or /castor/cern.ch/user/k/katkov/P5_2010/*.root..."  << std::endl;
  // For now pedestals from a text file are effectively not used (even if SetPedestals is still called); one needs to run CastorPedestalsAnalysis
  // and parse_peds.py to produce the txt file in the right format
  // edm::LogVerbatim("CastorAnalyser") << "!!!Check!!! prepared *_pedall with python? SetPedestals argument?" << std::endl; //LogInfo

  iEvt_=0;

  noUseValue_ = -999.0;

  protonPDGMass_ = 9.38272046E-01;
  cmEnergyDet_ = 13000.0;
#ifdef CM_ENERGY
  cmEnergyDet_ = CM_ENERGY;
#endif

  firstTS_ = iConfig.getUntrackedParameter<int>("firstTS", 0);
  lastTS_ = iConfig.getUntrackedParameter<int>("lastTS", 9);

  SetPedestals(_pedFileRun,_pedFilePath);
  //SetPedestals(156686,"python"); //132564 136201 132623 138974; 156686 - after totem tests, B = 0T //"test"

  firstTime_ = true;
  firstTimeAnalyse_ = true;

  //file_ = new TFile("CastorMonTree.root","RECREATE"); ///tmp/katkov/
  //file_->cd() ;

  /* 
     tree_ = new TTree( "CastorMon","CastorMon" );
  */

  // call TFileService and book histograms

  edm::Service<TFileService> fs;

  tree_ = fs->make<TTree>("CastorMon","CastorMon");
  tree_->Branch("runNb",&treeVariables_.runNb,"runNb/i"); //
  tree_->Branch("evtNb",&treeVariables_.evtNb,"evtNb/i"); //
  tree_->Branch("bxNb",&treeVariables_.bxNb,"bxNb/i"); //
  tree_->Branch("orbitNb",&treeVariables_.orbitNb,"orbitNb/i"); //
  tree_->Branch("lumiSectNb",&treeVariables_.lumiSectNb,"lumiSectNb/i");

  tree_->Branch("vtxNb",&treeVariables_.vtxNb,"vtxNb/i");
  tree_->Branch("vtxNbNotFake",&treeVariables_.vtxNbNotFake,"vtxNbNotFake/i");
  tree_->Branch("vtxTrkNb",&treeVariables_.vtxTrkNb,"vtxTrkNb/i");
  tree_->Branch("vtxIsFake0",&treeVariables_.vtxIsFake0,"vtxIsFake0/i");
  tree_->Branch("vtxRho",&treeVariables_.vtxRho,"vtxRho/D");
  tree_->Branch("vtxZ",&treeVariables_.vtxZ,"vtxZ/D");
  tree_->Branch("vtxZerr",&treeVariables_.vtxZerr,"vtxZerr/D");
  tree_->Branch("vtxY",&treeVariables_.vtxY,"vtxY/D");
  tree_->Branch("vtxX",&treeVariables_.vtxX,"vtxX/D");  

  tree_->Branch("trkNb",&treeVariables_.trkNb,"trkNb/i");
  tree_->Branch("trkLeadingPt",&treeVariables_.trkLeadingPt,"trkLeadingPt/D");
  tree_->Branch("trkLeadingEta",&treeVariables_.trkLeadingEta,"trkLeadingEta/D");
  tree_->Branch("trkLeadingPhi",&treeVariables_.trkLeadingPhi,"trkLeadingPhi/D");
  tree_->Branch("trkLeadingNbValidPixelHits",&treeVariables_.trkLeadingNbValidPixelHits,"trkLeadingNbValidPixelHits/i");
  tree_->Branch("trkLeadingNbValidStripHits",&treeVariables_.trkLeadingNbValidStripHits,"trkLeadingNbValidStripHits/i");
  tree_->Branch("trkHighNb",&treeVariables_.trkHighNb,"trkHighNb/i");
  tree_->Branch("trkEtaMax",&treeVariables_.trkEtaMax,"trkEtaMax/D");
  tree_->Branch("trkEtaMin",&treeVariables_.trkEtaMin,"trkEtaMin/D");

  tree_->Branch("casDigiPresamples",&treeVariables_.casDigiPresamples,"casDigiPresamples/i");
  tree_->Branch("casDigiSize",&treeVariables_.casDigiSize,"casDigiSize/i");
  tree_->Branch("casEnergy",&treeVariables_.casEnergy,"casEnergy/D"); //total calibrated energy in channels not marked as bad
  tree_->Branch("casNb",&treeVariables_.casNb,"casNb/i");
  tree_->Branch("casEnergyRH",&treeVariables_.casEnergyRH,"casEnergyRH/D"); //total calibrated energy in channels not marked as bad using RecHits
  tree_->Branch("casLeadingRHE",&treeVariables_.casLeadingRHE,"casLeadingRHE/D"); 
  tree_->Branch("casLeadingRHT",&treeVariables_.casLeadingRHT,"casLeadingRHT/D");
  tree_->Branch("hfNb",&treeVariables_.hfNb,"hfNb/i");
  tree_->Branch("hfmNb",&treeVariables_.hfmNb,"hfmNb/i");
  tree_->Branch("hfmNb4",&treeVariables_.hfmNb4,"hfmNb4/i");
  tree_->Branch("hfpNb4",&treeVariables_.hfpNb4,"hfpNb4/i");
  tree_->Branch("hfmNb5",&treeVariables_.hfmNb5,"hfmNb5/i");
  tree_->Branch("hfpNb5",&treeVariables_.hfpNb5,"hfpNb5/i");
  tree_->Branch("hfmNb6",&treeVariables_.hfmNb6,"hfmNb6/i");
  tree_->Branch("hfpNb6",&treeVariables_.hfpNb6,"hfpNb6/i");
  tree_->Branch("hfmE",&treeVariables_.hfmE,"hfmE/D");
  tree_->Branch("hfmE4all1",&treeVariables_.hfmE4all1,"hfmE4all1/D");
  tree_->Branch("hfmE4long1",&treeVariables_.hfmE4long1,"hfmE4long1/D");
  tree_->Branch("hfmE4short1",&treeVariables_.hfmE4short1,"hfmE4short1/D");
  tree_->Branch("hfmE4all2",&treeVariables_.hfmE4all2,"hfmE4all2/D");
  tree_->Branch("hfmE4long2",&treeVariables_.hfmE4long2,"hfmE4long2/D");
  tree_->Branch("hfmE4short2",&treeVariables_.hfmE4short2,"hfmE4short2/D");
  tree_->Branch("hfmLeadingE",&treeVariables_.hfmLeadingE,"hfmLeadingE/D");
  tree_->Branch("hfmLeadingT",&treeVariables_.hfmLeadingT,"hfmLeadingT/D");
  tree_->Branch("hfpE",&treeVariables_.hfpE,"hfpE/D");
  tree_->Branch("hbheNb",&treeVariables_.hbheNb,"hbheNb/i");
  tree_->Branch("hbhemNb",&treeVariables_.hbhemNb,"hbhemNb/i");
  tree_->Branch("hbhemE",&treeVariables_.hbhemE,"hbhemE/D");
  tree_->Branch("ebNb",&treeVariables_.ebNb,"ebNb/i");
  tree_->Branch("ebmNb",&treeVariables_.ebmNb,"ebmNb/i");
  tree_->Branch("ebmE",&treeVariables_.ebmE,"ebmE/D");  
  tree_->Branch("eeNb",&treeVariables_.eeNb,"eeNb/i");
  tree_->Branch("eemNb",&treeVariables_.eemNb,"eemNb/i");
  tree_->Branch("eemE",&treeVariables_.eemE,"eemE/D");
  tree_->Branch("casTowNb",&treeVariables_.casTowNb,"casTowNb/i");
  tree_->Branch("casTowE",&treeVariables_.casTowE,"casTowE/D");
  tree_->Branch("casTowEt",&treeVariables_.casTowEt,"casTowEt/D");
  tree_->Branch("casJetNb",&treeVariables_.casJetNb,"casJetNb/i");
  tree_->Branch("casLeadingJetE",&treeVariables_.casLeadingJetE,"casLeadingJetE/D");
  tree_->Branch("pfCluEcalNb",&treeVariables_.pfCluEcalNb,"pfCluEcalNb/i");
  tree_->Branch("pfLeadingCluEcalE",&treeVariables_.pfLeadingCluEcalE,"pfLeadingCluEcalE/D");
  tree_->Branch("pfCluHcalNb",&treeVariables_.pfCluHcalNb,"pfCluHcalNb/i");
  tree_->Branch("pfLeadingCluHcalE",&treeVariables_.pfLeadingCluHcalE,"pfLeadingCluHcalE/D");
  tree_->Branch("pfCluHFNb",&treeVariables_.pfCluHFNb,"pfCluHFNb/i");
  tree_->Branch("pfLeadingCluHFE",&treeVariables_.pfLeadingCluHFE,"pfLeadingCluHFE/D");
  tree_->Branch("pfCluEtaMax",&treeVariables_.pfCluEtaMax,"pfCluEtaMax/D");
  tree_->Branch("pfCluEtaMin",&treeVariables_.pfCluEtaMin,"pfCluEtaMin/D"); 
  tree_->Branch("pfCandNb",&treeVariables_.pfCandNb,"pfCandNb/i");
  tree_->Branch("pfLeadingCandE",&treeVariables_.pfLeadingCandE,"pfLeadingCandE/D");
  tree_->Branch("pfCandEtaMax",&treeVariables_.pfCandEtaMax,"pfCandEtaMax/D");
  tree_->Branch("pfCandEtaMin",&treeVariables_.pfCandEtaMin,"pfCandEtaMin/D");
  tree_->Branch("pfJetNb",&treeVariables_.pfJetNb,"pfJetNb/i");
  tree_->Branch("pfLeadingJetE",&treeVariables_.pfLeadingJetE,"pfLeadingJetE/D");
  tree_->Branch("caloTowerNb",&treeVariables_.caloTowerNb,"caloTowerNb/i");
  tree_->Branch("caloTowerBSCmNb",&treeVariables_.caloTowerBSCmNb,"caloTowerBSCmNb/i");
  tree_->Branch("caloTowerBSCpNb",&treeVariables_.caloTowerBSCpNb,"caloTowerBSCpNb/i");
  tree_->Branch("caloTowerBSC1mNb",&treeVariables_.caloTowerBSC1mNb,"caloTowerBSC1mNb/i");
  tree_->Branch("caloTowerBSC1pNb",&treeVariables_.caloTowerBSC1pNb,"caloTowerBSC1pNb/i");
  tree_->Branch("caloTowerHFmNb",&treeVariables_.caloTowerHFmNb,"caloTowerHFmNb/i");
  tree_->Branch("caloTowerHFmNb4",&treeVariables_.caloTowerHFmNb4,"caloTowerHFmNb4/i");
  tree_->Branch("caloTowerHFpNb4",&treeVariables_.caloTowerHFpNb4,"caloTowerHFpNb4/i");
  tree_->Branch("caloTowerHFmNb5",&treeVariables_.caloTowerHFmNb5,"caloTowerHFmNb5/i");
  tree_->Branch("caloTowerHFpNb5",&treeVariables_.caloTowerHFpNb5,"caloTowerHFpNb5/i");
  tree_->Branch("caloTowerHFmNb6",&treeVariables_.caloTowerHFmNb6,"caloTowerHFmNb6/i");
  tree_->Branch("caloTowerHFpNb6",&treeVariables_.caloTowerHFpNb6,"caloTowerHFpNb6/i");
  tree_->Branch("caloLeadingTowerHFE",&treeVariables_.caloLeadingTowerHFE,"caloLeadingTowerHFE/D");
  tree_->Branch("caloLeadingTowerieta",&treeVariables_.caloLeadingTowerieta,"caloLeadingTowerieta/I");
  tree_->Branch("caloLeadingToweriphi",&treeVariables_.caloLeadingToweriphi,"caloLeadingToweriphi/i");
  tree_->Branch("caloTowerEtaMax",&treeVariables_.caloTowerEtaMax,"caloTowerEtaMax/D");
  tree_->Branch("caloTowerEtaMin",&treeVariables_.caloTowerEtaMin,"caloTowerEtaMin/D");
  tree_->Branch("log10xiSDdet",&treeVariables_.log10xiSDdet,"log10xiSDdet/D");
  tree_->Branch("log10xiDDdet",&treeVariables_.log10xiDDdet,"log10xiDDdet/D"); 
  tree_->Branch("deltaRapMaxdet",&treeVariables_.deltaRapMaxdet,"deltaRapMaxdet/D");

  tree_->Branch("nbChan",&treeVariables_.nbChan,"nbChan/i");
  tree_->Branch("digiChargeSum",treeVariables_.digiChargeSum,"digiChargeSum[nbChan]/D");
  tree_->Branch("digiTime",treeVariables_.digiTime,"digiTime[nbChan]/D");
  tree_->Branch("digiTime1",treeVariables_.digiTime1,"digiTime1[nbChan]/D");
  tree_->Branch("digiSector",treeVariables_.digiSector,"digiSector[nbChan]/I");
  tree_->Branch("digiModule",treeVariables_.digiModule,"digiModule[nbChan]/I");
  tree_->Branch("digiCapIdTs0",treeVariables_.digiCapIdTs0,"digiCapIdTs0[nbChan]/I");
  tree_->Branch("digiAboveNoise",treeVariables_.digiAboveNoise,"digiAboveNoise[nbChan]/I");
  tree_->Branch("digiSaturated",treeVariables_.digiSaturated,"digiSaturated[nbChan]/I");
  tree_->Branch("digiMarkedBad",treeVariables_.digiMarkedBad,"digiMarkedBad[nbChan]/I");
  tree_->Branch("digiPulseFC",treeVariables_.digiPulseFC,"digiPulseFC[nbChan][10]/D");
  tree_->Branch("digiPulseFCped",treeVariables_.digiPulseFCped,"digiPulseFCped[nbChan][10]/D");

  tree_->Branch("nbGenPart",&treeVariables_.nbGenPart,"nbGenPart/i"); 
  tree_->Branch("nbStat1GenPart",&treeVariables_.nbStat1GenPart,"nbStat1GenPart/i");
  tree_->Branch("nbStat1ChPt250Eta2p5",&treeVariables_.nbStat1ChPt250Eta2p5,"nbStat1ChPt250Eta2p5/i");//nbStat1ChEtap1p94p9//nbStat1ChEtam3p51p5
  tree_->Branch("nbStat1ChEtap1p94p9",&treeVariables_.nbStat1ChEtap1p94p9,"nbStat1ChEtap1p94p9/i");
  tree_->Branch("nbStat1ChEtam3p51p5",&treeVariables_.nbStat1ChEtam3p51p5,"nbStat1ChEtam3p51p5/i");
  tree_->Branch("nbStat1GenPartInBSCm",&treeVariables_.nbStat1GenPartInBSCm,"nbStat1GenPartInBSCm/i");
  tree_->Branch("nbStat1GenPartInBSCp",&treeVariables_.nbStat1GenPartInBSCp,"nbStat1GenPartInBSCp/i");
  tree_->Branch("nbStat1GenPartInBSC1m",&treeVariables_.nbStat1GenPartInBSC1m,"nbStat1GenPartInBSC1m/i");
  tree_->Branch("nbStat1GenPartInBSC1p",&treeVariables_.nbStat1GenPartInBSC1p,"nbStat1GenPartInBSC1p/i");
  // https://github.com/hvanhaev/CommonFSQFramework/blob/c5b76548f54058ed55665e9d14d2559797373329/Core/src/EventIdData.cc
  // /afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_5_3_8_HI_patch2/src/cbaus/hiforest_analysis/Analysis.C --> "objects in BSC-region"
  tree_->Branch("log10xiSD",&treeVariables_.log10xiSD,"log10xiSD/D");
  tree_->Branch("log10xiDD",&treeVariables_.log10xiDD,"log10xiDD/D");
  tree_->Branch("genPartCmEnergy",&treeVariables_.genPartCmEnergy,"genPartCmEnergy/D");
  tree_->Branch("genPartEtaMax",&treeVariables_.genPartEtaMax,"genPartEtaMax/D");
  tree_->Branch("genPartEtaMin",&treeVariables_.genPartEtaMin,"genPartEtaMin/D");
  tree_->Branch("genPartDeltaRapMax",&treeVariables_.genPartDeltaRapMax,"genPartDeltaRapMax/D");

  tree_->Branch("nbEtaIntervals",&treeVariables_.nbEtaIntervals,"nbEtaIntervals/i");
  tree_->Branch("nbEtaIntervalsExt",&treeVariables_.nbEtaIntervalsExt,"nbEtaIntervalsExt/i");
  tree_->Branch("energyInEtaIntervals",treeVariables_.energyInEtaIntervals,"energyInEtaIntervals[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals1",treeVariables_.energyInEtaIntervals1,"energyInEtaIntervals1[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals2",treeVariables_.energyInEtaIntervals2,"energyInEtaIntervals2[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals3",treeVariables_.energyInEtaIntervals3,"energyInEtaIntervals3[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals4",treeVariables_.energyInEtaIntervals4,"energyInEtaIntervals4[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals5",treeVariables_.energyInEtaIntervals5,"energyInEtaIntervals5[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals6",treeVariables_.energyInEtaIntervals6,"energyInEtaIntervals6[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals7",treeVariables_.energyInEtaIntervals7,"energyInEtaIntervals7[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals8",treeVariables_.energyInEtaIntervals8,"energyInEtaIntervals8[nbEtaIntervals]/D");
  tree_->Branch("energyInEtaIntervals9",treeVariables_.energyInEtaIntervals9,"energyInEtaIntervals9[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals",treeVariables_.etInEtaIntervals,"etInEtaIntervals[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals1",treeVariables_.etInEtaIntervals1,"etInEtaIntervals1[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals2",treeVariables_.etInEtaIntervals2,"etInEtaIntervals2[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals3",treeVariables_.etInEtaIntervals3,"etInEtaIntervals3[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals33",treeVariables_.etInEtaIntervals33,"etInEtaIntervals33[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals333",treeVariables_.etInEtaIntervals333,"etInEtaIntervals333[nbEtaIntervalsExt]/D");
  tree_->Branch("etInEtaIntervals4",treeVariables_.etInEtaIntervals4,"etInEtaIntervals4[nbEtaIntervals]/D");
  tree_->Branch("etInEtaIntervals5",treeVariables_.etInEtaIntervals5,"etInEtaIntervals5[nbEtaIntervals]/D");
  //
  tree_->Branch("etInEtaIntervals10",treeVariables_.etInEtaIntervals10,"etInEtaIntervals10[nbEtaIntervals]/D");
  
  histosProfile_["energy_vs_eta_reco"] = fs->make<TProfile>("energy_vs_eta_reco","Detector-level <Energy> (or <Et>) vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges); //fs_
  histosProfile_["energy_vs_eta_reco"]->Sumw2();
  histosProfile_["energy_vs_eta_reco"]->SetXTitle("#eta");
  histosProfile_["energy_vs_eta_reco"]->SetYTitle("<E(t)>/#Delta#eta");
  histosProfile_["energy_vs_eta_reco1"] = fs->make<TProfile>("energy_vs_eta_reco1","Detector-level <Energy> (or <Et>) vs #eta",ForwardRecord::nbEtaBins,ForwardRecord::Eta_Bin_Edges); //fs_
  histosProfile_["energy_vs_eta_reco1"]->Sumw2();
  histosProfile_["energy_vs_eta_reco1"]->SetXTitle("#eta");
  histosProfile_["energy_vs_eta_reco1"]->SetYTitle("<E(t)>/#Delta#eta");
  histosProfile_["energy_vs_eta_reco2"] = fs->make<TProfile>("energy_vs_eta_reco2","Detector-level <Energy> (or <Et>) vs #eta",ForwardRecord::nbEtaBinsExt,ForwardRecord::Eta_Bin_EdgesExt); //fs_
  histosProfile_["energy_vs_eta_reco2"]->Sumw2();
  histosProfile_["energy_vs_eta_reco2"]->SetXTitle("#eta");
  histosProfile_["energy_vs_eta_reco2"]->SetYTitle("<E(t)>/#Delta#eta");

  for (uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) {
    std::string hi_tag = Form("pfclu_single_ieta%d",ibin); 
    std::string hi_title = Form("Distribution of single PFClusters in eta bin %d %g : %g",ibin, ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_title.c_str(),124000,-1000.0,30000.0);
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Energy (GeV)");
    histos1D_[hi_tag]->SetYTitle("Entries");
    histos1D_[hi_tag]->SetLineWidth(2);
    hi_tag = Form("pfcand1_single_ieta%d",ibin);
    hi_title = Form("Distribution of single PFCandidates1hch in eta bin %d %g : %g",ibin, ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_title.c_str(),124000,-1000.0,30000.0);
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Energy (GeV)");
    histos1D_[hi_tag]->SetYTitle("Entries");
    histos1D_[hi_tag]->SetLineWidth(2);
    hi_tag = Form("pfcand4_single_ieta%d",ibin);
    hi_title = Form("Distribution of single PFCandidates4pho in eta bin %d %g : %g",ibin, ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_title.c_str(),124000,-1000.0,30000.0);
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Energy (GeV)");
    histos1D_[hi_tag]->SetYTitle("Entries");
    histos1D_[hi_tag]->SetLineWidth(2);
    hi_tag = Form("pfcand5_single_ieta%d",ibin);
    hi_title = Form("Distribution of single PFCandidates5h0 in eta bin %d %g : %g",ibin, ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_title.c_str(),124000,-1000.0,30000.0);
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Energy (GeV)");
    histos1D_[hi_tag]->SetYTitle("Entries");
    histos1D_[hi_tag]->SetLineWidth(2);
    hi_tag = Form("pfcand6_single_ieta%d",ibin);
    hi_title = Form("Distribution of single PFCandidates6hHF in eta bin %d %g : %g",ibin, ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_title.c_str(),124000,-1000.0,30000.0);
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Energy (GeV)");
    histos1D_[hi_tag]->SetYTitle("Entries");
    histos1D_[hi_tag]->SetLineWidth(2);
    hi_tag = Form("pfcand7_single_ieta%d",ibin);
    hi_title = Form("Distribution of single PFCandidates7phoHF in eta bin %d %g : %g",ibin, ForwardRecord::Eta_Bin_Edges[ibin],ForwardRecord::Eta_Bin_Edges[ibin+1]);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_title.c_str(),124000,-1000.0,30000.0);
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Energy (GeV)");
    histos1D_[hi_tag]->SetYTitle("Entries");
    histos1D_[hi_tag]->SetLineWidth(2);
  }

  histos2D_["tow_ene_map"] = fs->make<TH2D>("tow_ene_map","ieta-iphi map of tower energies",199,-99.5,99.5,100,-0.5,99.5); //fs_
  histos2D_["tow_ene_map"]->SetXTitle("i#eta");
  histos2D_["tow_ene_map"]->SetYTitle("i#phi");
  histos2D_["tow_occup_map"]=fs->make<TH2D>("tow_occup_map","Number of towers as an ieta-iphi map",199,-99.5,99.5,100,-0.5,99.5); //fs_
  histos2D_["tow_occup_map"]->SetXTitle("i#eta");
  histos2D_["tow_occup_map"]->SetYTitle("i#phi");

  histos2D_["tow_leadingoccup_map"]=fs->make<TH2D>("tow_leadingoccup_map","Number of towers with leading energy as an ieta-iphi map",199,-99.5,99.5,100,-0.5,99.5); //fs_
  histos2D_["tow_leadingoccup_map"]->SetXTitle("i#eta");
  histos2D_["tow_leadingoccup_map"]->SetYTitle("i#phi"); 

  histos2D_["tow_leadingoccup_map1"]=fs->make<TH2D>("tow_leadingoccup_map1","Number of towers with leading energy as an eta-phi map",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["tow_leadingoccup_map1"]->SetXTitle("#eta");
  histos2D_["tow_leadingoccup_map1"]->SetYTitle("#phi");

  histos2D_["tow_ene_map1"] = fs->make<TH2D>("tow_ene_map1","eta-phi map of tower energies",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["tow_ene_map1"]->SetXTitle("i#eta");
  histos2D_["tow_ene_map1"]->SetYTitle("i#phi");
  histos2D_["tow_occup_map1"]=fs->make<TH2D>("tow_occup_map1","Number of towers as an eta-phi map",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["tow_occup_map1"]->SetXTitle("#eta");
  histos2D_["tow_occup_map1"]->SetYTitle("#phi");

  histos2D_["pfclu_ene_map"] = fs->make<TH2D>("plclu_ene_map","eta-phi map of pfcluster energies",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["pfclu_ene_map"]->SetXTitle("#eta");
  histos2D_["pfclu_ene_map"]->SetYTitle("#phi");
  histos2D_["pfclu_occup_map"]=fs->make<TH2D>("pfclu_occup_map","Number of pfclusters above threshold as an eta-phi map",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["pfclu_occup_map"]->SetXTitle("#eta");
  histos2D_["pfclu_occup_map"]->SetYTitle("#phi");

  histos2D_["pfcan_ene_map"] = fs->make<TH2D>("pfcan_ene_map","eta-phi map of pfcandidate energies",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["pfcan_ene_map"]->SetXTitle("#eta");
  histos2D_["pfcan_ene_map"]->SetYTitle("#phi");
  histos2D_["pfcan_occup_map"]=fs->make<TH2D>("pfcan_occup_map","Number of pfcandidates as an eta-phi map",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["pfcan_occup_map"]->SetXTitle("#eta");
  histos2D_["pfcan_occup_map"]->SetYTitle("#phi");

  histos2D_["pfcan_ene_map1"] = fs->make<TH2D>("pfcan_ene_map1","eta-phi map of pfcandidate energies above thresholds",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["pfcan_ene_map1"]->SetXTitle("#eta");
  histos2D_["pfcan_ene_map1"]->SetYTitle("#phi");
  histos2D_["pfcan_occup_map1"]=fs->make<TH2D>("pfcan_occup_map1","Number of pfcandidates above thresholds as an eta-phi map",208,-5.2,5.2, 288,-6.283,6.283); //fs_
  histos2D_["pfcan_occup_map1"]->SetXTitle("#eta");
  histos2D_["pfcan_occup_map1"]->SetYTitle("#phi");

  histos1D_["trigger_info"] = fs->make<TH1D>("trigger_info","Monitor trigger accept rate",1,0.5,1.5);
  histos1D_["trigger_info"]->SetXTitle("Trigger name");
  histos1D_["trigger_info"]->SetYTitle("Event fraction");

  //char hname[1024];

  histos1D_["h_runnb"] = fs->make<TH1D>( "runnb"  , "Run Numbers", 300000,  -0.5, 299999.5 ); //fs->make<TH1F>( "runnb"  , "Run Numbers", 300000,  -0.5, 299999.5 ); //new TH1F
  histos1D_["h_runnb"]->Sumw2();
  histos1D_["h_runnb"]->SetXTitle("Run number");
  histos1D_["h_runnb"]->SetYTitle("Event fraction per run");

  for (int iii = 0; iii < 16; iii++) {
    //sprintf(hname,"pulsets_sect_%d",iii);
    std::string hi_tag = Form("pulsets_sect_%d",iii);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_tag.c_str(),140,-0.5,139.5); //h_pulsets_sect_[iii]
    //h_pulsets_sect_[iii] = fs->make<TH1D>(Form("pulsets_sect_%d",iii) , Form("pulsets_sect_%d",iii), 140,  -0.5, 139.5 );
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Time slices");
    histos1D_[hi_tag]->SetYTitle("<fC> in slice");    
    histos1D_[hi_tag]->SetDrawOption("hist");
    histos1D_[hi_tag]->SetLineWidth(2);
    histos1D_[hi_tag]->GetXaxis()->SetNdivisions(520);
  }

  histos2D_["h_energyzs_map"] = fs->make<TH2D>( "energyzs_map", "Map of Energy Deposits above noise",14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_energyzs_map"]->Sumw2();
  histos2D_["h_energyzs_map"]->SetXTitle("module (iz)");
  histos2D_["h_energyzs_map"]->SetYTitle("sector (iphi)");

  histos2D_["h_energysqs_map"] = fs->make<TH2D>( "energysqs_map", "Map of Sum of Squares of Energy",14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_energysqs_map"]->Sumw2();
  histos2D_["h_energysqs_map"]->SetXTitle("module (iz)");
  histos2D_["h_energysqs_map"]->SetYTitle("sector (iphi)");

  histos2D_["h_energysum_map"] = fs->make<TH2D>( "energysum_map", "Map of Energy Sum",14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_energysum_map"]->Sumw2();
  histos2D_["h_energysum_map"]->SetXTitle("module (iz)");
  histos2D_["h_energysum_map"]->SetYTitle("sector (iphi)");
  
  histos2D_["h_occup_map"] = fs->make<TH2D>( "occup_map",  "Fraction of Hits with Energy above noise", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_occup_map"]->Sumw2();
  histos2D_["h_occup_map"]->SetXTitle("module (iz)");
  histos2D_["h_occup_map"]->SetYTitle("sector (iphi)");
 
  histos2D_["h_chmean_map"] = fs->make<TH2D>( "chmean_map",  "Map of mean values of charge_mXsY", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_chmean_map"]->Sumw2();
  histos2D_["h_chmean_map"]->SetXTitle("module (iz)");
  histos2D_["h_chmean_map"]->SetYTitle("sector (iphi)");

  histos2D_["h_chrms_map"] = fs->make<TH2D>( "chrms_map",  "Map of RMS values of charge_mXsY", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_chrms_map"]->Sumw2();
  histos2D_["h_chrms_map"]->SetXTitle("module (iz)");
  histos2D_["h_chrms_map"]->SetYTitle("sector (iphi)");

  histos2D_["h_chmeanene_map"] = fs->make<TH2D>( "chmeanene_map",  "Map of mean values of calibrated charge_mXsY", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_chmeanene_map"]->Sumw2();
  histos2D_["h_chmeanene_map"]->SetXTitle("module (iz)");
  histos2D_["h_chmeanene_map"]->SetYTitle("sector (iphi)");

  histos2D_["h_chmeanene_bcor_map"] = fs->make<TH2D>( "chmeanene_bcor_map",  "Map of mean values of calibrated (corr to 0T) charge_mXsY", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_chmeanene_bcor_map"]->Sumw2();
  histos2D_["h_chmeanene_bcor_map"]->SetXTitle("module (iz)");
  histos2D_["h_chmeanene_bcor_map"]->SetYTitle("sector (iphi)");
  
  CastorAnalyser::SetNonInvfCBins();   //set non-equidistant binning reflecting QIE->fC conversion
  for (int ic = 0; ic < 224; ic++) {
    //sprintf( hname,"charge_m%ds%d", (ic % 14) + 1, static_cast<int>(ic/14) + 1 );
    std::string hi_tag = Form("charge_m%ds%d", (ic % 14) + 1, static_cast<int>(ic/14) + 1);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_tag.c_str(),nfCBins_,fCBinEdges_);
    //h_charge_[ic] = fs->make<TH1D>(hnam.c_str(),hnam.c_str(),nfCBins_,fCBinEdges_); //h_charge_[ic]
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle(Form("TS%d-%d amplitude (fC)",firstTS_,lastTS_)); //TS4-5 amplitude (fC) 5-8
    histos1D_[hi_tag]->SetYTitle("Events in bin");  //Event fraction in bin
    histos1D_[hi_tag]->SetLineWidth(2);
    histos1D_[hi_tag]->SetLineColor(2);
    histos1D_[hi_tag]->SetMarkerStyle(24);
    histos1D_[hi_tag]->SetMarkerSize(0.7);
    histos1D_[hi_tag]->SetMarkerColor(2);
    /*
      h_charge_v_[ic]->GetXaxis()->SetLabelSize(0.1);    
      h_charge_v_[ic]->GetXaxis()->SetTitleSize(0.09);  
      h_charge_v_[ic]->GetXaxis()->SetTitleOffset(1.2);
      h_charge_v_[ic]->GetXaxis()->SetRangeUser(-50.0,300.0);
      h_charge_v_[ic]->GetYaxis()->SetLabelSize(0.1);
      h_charge_v_[ic]->GetYaxis()->SetTitleSize(0.10);
      h_charge_v_[ic]->GetYaxis()->SetTitleOffset(0.50);
    */
  }

  Float_t tsbins[11] = {-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5};
  Int_t ybinn = nfCBins_;
  Float_t *ybinx = fCBinEdges_;
  Int_t xbinn = 10;
  Float_t *xbinx = tsbins;
  histos2D_["h_ch_vs_ts"] = fs->make<TH2D>("ch_vs_ts","Max charge vs ts-no. where this max is found",xbinn,xbinx,ybinn,ybinx);
  histos2D_["h_ch_vs_ts"]->Sumw2();
  histos2D_["h_ch_vs_ts"]->SetXTitle("25-ns time slice");
  histos2D_["h_ch_vs_ts"]->SetYTitle("amplitude (fC)");

  histos1D_["h_hit_time"] = fs->make<TH1D>( "hit_time"  , "Pulse time estimate for good channels above noise", 4000,  -2000.0, 2000.0 );
  histos1D_["h_hit_time"]->Sumw2();
  histos1D_["h_hit_time"]->SetXTitle("Time (ns)");
  histos1D_["h_hit_time"]->SetYTitle("Event fraction per time bin");
  histos2D_["h_hittime_mean_map"] = fs->make<TH2D>( "hittime_mean_map",  "Map of mean values of hit_time_mXsY", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_hittime_mean_map"]->Sumw2();
  histos2D_["h_hittime_mean_map"]->SetXTitle("module (iz)");
  histos2D_["h_hittime_mean_map"]->SetYTitle("sector (iphi)");
  histos2D_["h_hittime_rms_map"] = fs->make<TH2D>( "hittime_rms_map",  "Map of RMS values of hit_time_mXsY", 14,0.5,14.5,16,0.5,16.5);
  histos2D_["h_hittime_rms_map"]->Sumw2();
  histos2D_["h_hittime_rms_map"]->SetXTitle("module (iz)");
  histos2D_["h_hittime_rms_map"]->SetYTitle("sector (iphi)");
  for (int ic = 0; ic < 224; ic++) {
    //sprintf( hname,"hit_time_m%ds%d", (ic % 14) + 1, static_cast<int>(ic/14) + 1 );
    std::string hi_tag = Form("hit_time_m%ds%d", (ic % 14) + 1, static_cast<int>(ic/14) + 1);
    histos1D_[hi_tag] = fs->make<TH1D>(hi_tag.c_str(),hi_tag.c_str(),400,  -200.0, 200.0 );
    //h_hit_time_ch_[ic] = fs->make<TH1F>(hname,hname,400,  -200.0, 200.0 ); //THIS WAY YOU TRUNCATE -999.0 WHICH HAPPENS IN FACT
    histos1D_[hi_tag]->Sumw2();
    histos1D_[hi_tag]->SetXTitle("Pulse time estimate"); 
    histos1D_[hi_tag]->SetYTitle("Events in bin");  //Event fraction in bin
    histos1D_[hi_tag]->SetLineWidth(2);
    histos1D_[hi_tag]->SetLineColor(2);
    histos1D_[hi_tag]->SetMarkerStyle(24);
    histos1D_[hi_tag]->SetMarkerSize(0.7);
    histos1D_[hi_tag]->SetMarkerColor(2);
  }

  histosProfile_["h_casene_vs_bx"] = fs->make<TProfile>("casene_vs_bx","Total calibrated energy in channels not marked as bad vs BX",3564,-0.5,3564-0.5);
  histosProfile_["h_casene_vs_bx"]->Sumw2();
  histosProfile_["h_casene_vs_bx"]->SetXTitle("BX no.");
  histosProfile_["h_casene_vs_bx"]->SetYTitle("<E> (GeV)");

  histosProfile_["h_casene_vs_bx_sel"] = fs->make<TProfile>("casene_vs_bx_sel","Tot calib energy in chs not marked as bad vs BX (1chgood>noise,nosatur)",3564,-0.5,3564-0.5);
  histosProfile_["h_casene_vs_bx_sel"]->Sumw2();
  histosProfile_["h_casene_vs_bx_sel"]->SetXTitle("BX no.");
  histosProfile_["h_casene_vs_bx_sel"]->SetYTitle("<E> (GeV)");

  histosProfile_["h_casene_vs_iphi"] = fs->make<TProfile>("casene_vs_iphi","Phi profile of calibrated castor energy excluding bad channels",16,0.5,16.5);
  histosProfile_["h_casene_vs_iphi"]->Sumw2();
  histosProfile_["h_casene_vs_iphi"]->SetXTitle("sector (iphi)");
  histosProfile_["h_casene_vs_iphi"]->SetYTitle("<E> (GeV)");
  histosProfile_["h_casenerh_vs_iphi"] = fs->make<TProfile>("casenerh_vs_iphi","Phi profile of castor RH energy",16,0.5,16.5);
  histosProfile_["h_casenerh_vs_iphi"]->Sumw2();
  histosProfile_["h_casenerh_vs_iphi"]->SetXTitle("sector (iphi)");
  histosProfile_["h_casenerh_vs_iphi"]->SetYTitle("<E> (GeV)");
  histos1D_["h_goodch_vs_iphi"] = fs->make<TH1D>("goodch_vs_iphi","Number of channels not marked as bad vs iphi",16,0.5,16.5);
  histos1D_["h_goodch_vs_iphi"]->Sumw2();
  histos1D_["h_goodch_vs_iphi"]->SetXTitle("sector (iphi)");
  histos1D_["h_goodch_vs_iphi"]->SetYTitle("# of good channels");

  histosProfile_["h_casene_vs_iz"] = fs->make<TProfile>("casene_vs_iz","Longitudinal profile of calibrated castor energy excluding bad channels",14,0.5,14.5);
  histosProfile_["h_casene_vs_iz"]->Sumw2();
  histosProfile_["h_casene_vs_iz"]->SetXTitle("module (iz)");
  histosProfile_["h_casene_vs_iz"]->SetYTitle("<E> (GeV)");
  histosProfile_["h_casenerh_vs_iz"] = fs->make<TProfile>("casenerh_vs_iz","Longitudinal profile of castor RH energy",14,0.5,14.5);
  histosProfile_["h_casenerh_vs_iz"]->Sumw2();
  histosProfile_["h_casenerh_vs_iz"]->SetXTitle("module (iz)");
  histosProfile_["h_casenerh_vs_iz"]->SetYTitle("<E> (GeV)");
  histos1D_["h_goodch_vs_iz"] = fs->make<TH1D>("goodch_vs_iz","Number of channels not marked as bad vs iz",14,0.5,14.5);
  histos1D_["h_goodch_vs_iz"]->Sumw2();
  histos1D_["h_goodch_vs_iz"]->SetXTitle("module (iz)");
  histos1D_["h_goodch_vs_iz"]->SetYTitle("# of good channels");

  histos2D_["h_hfme_vs_case"] = fs->make<TH2D>("hfme_vs_case", "HFM RecHit Energy vs tot. calibrated castor energy in channels not marked as bad", 5500,-110000.0,550000.0, 5500,-110000.0,550000.0);
  histos2D_["h_hfme_vs_case"]->SetYTitle("E[HFM] (GeV)");
  histos2D_["h_hfme_vs_case"]->SetXTitle("E[CAS] (GeV)");

  histos2D_["h_hbheme_vs_case"] = fs->make<TH2D>("hbheme_vs_case", "HBHE-minus_side RecHit Energy vs tot. calibrated castor energy in channels not marked as bad", 5500,-110000.0,550000.0, 5500,-110000.0,550000.0);
  histos2D_["h_hbheme_vs_case"]->SetYTitle("E[HBHEminus] (GeV)");
  histos2D_["h_hbheme_vs_case"]->SetXTitle("E[CAS] (GeV)");
	
}


CastorAnalyser::~CastorAnalyser()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

  bool isData = true;
#ifdef THIS_IS_MC
  isData = false;
#endif 
 
  if (iEvt_ != 0) { 
    histos1D_["h_runnb"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    histos1D_["h_goodch_vs_iphi"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    histos1D_["h_goodch_vs_iz"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    //
    histos1D_["trigger_info"]->Scale( 1.0 / static_cast<double>(iEvt_) );
  }

  for(int i1 = 0; i1 < 16; i1++) {
    
    if (iEvt_ != 0) { 
      std::string hi_tag = Form("pulsets_sect_%d",i1);
      histos1D_[hi_tag]->Scale( 1.0 / static_cast<double>(iEvt_) ); //h_pulsets_sect_[i1]
    }

  }  
  
  for(int i1 = 0; i1 < 224; i1++){

    int mod = (i1 % 14) + 1;
    int sec = static_cast<int>(i1/14) + 1;
    std::string hi_tag_h_charge = Form("charge_m%ds%d", mod, sec);
    std::string hi_tag_h_hit_time_ch = Form("hit_time_m%ds%d", mod, sec);
    if (iEvt_ != 0) histos1D_[hi_tag_h_charge]->Scale( 1.0 / static_cast<double>(iEvt_) ); //h_charge_[i1]
    if (iEvt_ != 0) histos1D_[hi_tag_h_hit_time_ch]->Scale( 1.0 / static_cast<double>(iEvt_) ); //h_hit_time_ch_[i1]
    //mod = (ic % 14) + 1; sec = static_cast<int>(ic/14) + 1:
    double mean_of_charge = histos1D_[hi_tag_h_charge]->GetMean();
    double rms_of_charge  = histos1D_[hi_tag_h_charge]->GetRMS(); 
    double mean_of_time = histos1D_[hi_tag_h_hit_time_ch]->GetMean();
    double rms_of_time  = histos1D_[hi_tag_h_hit_time_ch]->GetRMS();
    histos2D_["h_chmean_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, mean_of_charge); //h_charge_[i1] //->GetMean()
    if (castor::channelQuality[sec-1][mod-1]==true) {
      double corrFactor = 0.015; //ForwardRecord::absCasEscaleFactor;
      if (isData) corrFactor = castor::channelGainQE[sec-1][mod-1] * ForwardRecord::absCasEscaleFactor;
      histos2D_["h_chmeanene_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, corrFactor * mean_of_charge); //h_charge_[i1]
      histos2D_["h_chmeanene_bcor_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, /*castor::corr0Tto38T[sec-1][mod-1]* */corrFactor * mean_of_charge); //h_charge_[i1]
    } else {
      histos2D_["h_chmeanene_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, 1e-5);
      histos2D_["h_chmeanene_bcor_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, 1e-5);
    }
    histos2D_["h_chrms_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, rms_of_charge); //h_charge_[i1] //->GetRMS()
    histos2D_["h_hittime_mean_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, mean_of_time); //h_hit_time_ch_[i1] //->GetMean()
    histos2D_["h_hittime_rms_map"]->SetBinContent((i1 % 14) + 1,static_cast<int>(i1/14) + 1, rms_of_time); //h_hit_time_ch_[i1] //->GetRMS()

  }

  //normalise "map of energy deposits according to map of hits (occupancy map)"
  
  if (iEvt_ != 0) { 
    histos2D_["h_energyzs_map"]->Divide(histos2D_["h_occup_map"]);
    histos2D_["h_occup_map"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    //For the time being do not divide by occupancies for tow_ene_map, pfclu_ene_map, pfcan_ene_map...
    //on the other hand we are not going to merge files... so let us divide 
    histos2D_["tow_ene_map"]->Divide(histos2D_["tow_occup_map"]);
    histos2D_["tow_occup_map"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    histos2D_["tow_ene_map1"]->Divide(histos2D_["tow_occup_map1"]);
    histos2D_["tow_occup_map1"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    histos2D_["pfclu_ene_map"]->Divide(histos2D_["pfclu_occup_map"]);
    histos2D_["pfclu_occup_map"]->Scale( 1.0 / static_cast<double>(iEvt_) );     
    histos2D_["pfcan_ene_map"]->Divide(histos2D_["pfcan_occup_map"]);
    histos2D_["pfcan_occup_map"]->Scale( 1.0 / static_cast<double>(iEvt_) );
    histos2D_["pfcan_ene_map1"]->Divide(histos2D_["pfcan_occup_map1"]);
    histos2D_["pfcan_occup_map1"]->Scale( 1.0 / static_cast<double>(iEvt_) );
  } 
    

  if (iEvt_ != 0 && numBunches_ != 0) {
    //  h_pulsebx_->Scale( 1.0 / static_cast<double>(iEvt_) / numBunches_ ); 
    histos1D_["h_hit_time"]->Scale( 1.0 / static_cast<double>(iEvt_) / numBunches_ );
    //  h_hit_time1_->Scale( 1.0 / static_cast<double>(iEvt_) / numBunches_ );
    //  h_pulsets_->Scale( 1.0 / static_cast<double>(iEvt_) / numBunches_ );
  }

  //file_->cd();
  /*
    tree_->Write();
  */
  /*
    h_runnb_->Write();
    h_energyzs_map_->Write();
    h_energysqs_map_->Write();
    h_energysum_map_->Write();
    h_occup_map_->Write();
    for (int iii = 0; iii < 16; iii++) {
    h_pulsets_sect_[iii]->Write();
    }
    for (int ic = 0; ic < 224; ic++) {
    h_charge_[ic]->Write(); 
    }
    h_chmean_map_->Write();
    h_chrms_map_->Write();
  */
  //file_->Close();

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
CastorAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  bool isData = true;
#ifdef THIS_IS_MC
  isData = false;
#endif

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  treeVariables_.runNb = iEvent.id().run() ;
  treeVariables_.evtNb = iEvent.id().event() ;
  treeVariables_.bxNb = iEvent.bunchCrossing() ;
  treeVariables_.orbitNb = iEvent.orbitNumber() ;
  treeVariables_.lumiSectNb = iEvent.luminosityBlock() ;

  treeVariables_.vtxNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.vtxNbNotFake = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.vtxTrkNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.vtxIsFake0 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.vtxRho = noUseValue_;
  treeVariables_.vtxZ = noUseValue_;
  treeVariables_.vtxZerr = noUseValue_;
  treeVariables_.vtxY = noUseValue_;
  treeVariables_.vtxX = noUseValue_;

  treeVariables_.trkNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.trkLeadingPt = noUseValue_;
  treeVariables_.trkLeadingEta = noUseValue_;
  treeVariables_.trkLeadingPhi = noUseValue_;
  treeVariables_.trkLeadingNbValidPixelHits = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.trkLeadingNbValidStripHits = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.trkHighNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.trkEtaMax = noUseValue_;
  treeVariables_.trkEtaMin = noUseValue_;

  treeVariables_.casDigiPresamples = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.casDigiSize = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.casEnergy = noUseValue_;
  treeVariables_.casNb = static_cast<uint>(-noUseValue_);
  treeVariables_.casEnergyRH = noUseValue_;
  treeVariables_.casLeadingRHE = noUseValue_;
  treeVariables_.casLeadingRHT = noUseValue_;

  treeVariables_.hfNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfmNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfmNb4 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfpNb4 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfmNb5 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfpNb5 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfmNb6 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfpNb6 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hfmE = noUseValue_;
  treeVariables_.hfmE4all1 = noUseValue_;
  treeVariables_.hfmE4long1 = noUseValue_;
  treeVariables_.hfmE4short1 = noUseValue_;
  treeVariables_.hfmE4all2 = noUseValue_;
  treeVariables_.hfmE4long2 = noUseValue_;
  treeVariables_.hfmE4short2 = noUseValue_;
  treeVariables_.hfmLeadingE = noUseValue_;
  treeVariables_.hfmLeadingT = noUseValue_;
  treeVariables_.hfpE = noUseValue_;

  treeVariables_.hbheNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hbhemNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.hbhemE = noUseValue_;

  treeVariables_.ebNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.ebmNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.ebmE = noUseValue_;

  treeVariables_.eeNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.eemNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.eemE = noUseValue_;

  treeVariables_.casTowNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.casTowE = noUseValue_;
  treeVariables_.casTowEt = noUseValue_;

  treeVariables_.casJetNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.casLeadingJetE = noUseValue_;

  treeVariables_.pfCluEcalNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.pfLeadingCluEcalE = noUseValue_;
  treeVariables_.pfCluHcalNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.pfLeadingCluHcalE = noUseValue_;
  treeVariables_.pfCluHFNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.pfLeadingCluHFE = noUseValue_;
  treeVariables_.pfCluEtaMax = noUseValue_;
  treeVariables_.pfCluEtaMin = noUseValue_;

  treeVariables_.pfCandNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.pfLeadingCandE = noUseValue_;
  treeVariables_.pfCandEtaMax = noUseValue_;
  treeVariables_.pfCandEtaMin = noUseValue_;
  treeVariables_.pfJetNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.pfLeadingJetE = noUseValue_;

  treeVariables_.caloTowerNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerBSCmNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerBSCpNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerBSC1mNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerBSC1pNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFmNb = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFmNb4 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFpNb4 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFmNb5 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFpNb5 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFmNb6 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloTowerHFpNb6 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.caloLeadingTowerHFE = noUseValue_;
  treeVariables_.caloLeadingTowerieta = static_cast<int>(noUseValue_);
  treeVariables_.caloLeadingToweriphi = static_cast<int>(noUseValue_);
  treeVariables_.caloTowerEtaMax = noUseValue_;
  treeVariables_.caloTowerEtaMin = noUseValue_;
  treeVariables_.log10xiSDdet = noUseValue_;
  treeVariables_.log10xiDDdet = noUseValue_;
  treeVariables_.deltaRapMaxdet = noUseValue_;

  treeVariables_.nbGenPart = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1GenPart = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1ChPt250Eta2p5 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1ChEtap1p94p9 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1ChEtam3p51p5 = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1GenPartInBSCm = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1GenPartInBSCp = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1GenPartInBSC1m = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.nbStat1GenPartInBSC1p = static_cast<uint>(noUseValue_*noUseValue_);
  treeVariables_.log10xiSD = noUseValue_;
  treeVariables_.log10xiDD = noUseValue_;
  treeVariables_.genPartCmEnergy = noUseValue_;
  treeVariables_.genPartEtaMax = noUseValue_;
  treeVariables_.genPartEtaMin = noUseValue_;
  treeVariables_.genPartDeltaRapMax = noUseValue_;

  treeVariables_.nbChan = 0; ////Effectively if nbChan == 0 then event was not selected for some reason

  treeVariables_.nbEtaIntervals = 0;
  treeVariables_.nbEtaIntervalsExt = 0;

  // INIT your treeVariables_.XXX here ??? !!!

  ///////////////////////////////////////////////////////////////if(Event.bunchCrossing() != || iEvent.bunchCrossing() != ) return;
  //

  std::vector<double> etaBinEnergies(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies1(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies2(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies3(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies4(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies5(ForwardRecord::nbEtaBins,0.0);
//
  std::vector<double> etaBinEnergies6(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies7(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies8(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEnergies9(ForwardRecord::nbEtaBins,0.0);
//
  std::vector<double> etaBinEts(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts1(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts2(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts3(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts33(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts333(ForwardRecord::nbEtaBinsExt,0.0);
  std::vector<double> etaBinEts4(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts5(ForwardRecord::nbEtaBins,0.0);
  std::vector<double> etaBinEts10(ForwardRecord::nbEtaBins,0.0);

  histos1D_["h_runnb"]->Fill( static_cast<double>(iEvent.id().run()) );

  bool isAccessibleVtxInformation = false;
  uint vtxNo = 0;
  uint vtxNoNotFake = 0;
  uint vtxIsFake0 = 1;
  uint vtxTrkNo = 0;
  double vtxRho = 0.0;
  double vtxZ = 0.0;
  double vtxZerr = 0.0;
  double vtxY = 0.0;
  double vtxX = 0.0;

  bool isAccessibleTrkInformation = false;
  uint trkNo = 0;
  uint trkHighNo = 0;
  double trkLeadingPt = noUseValue_;
  double trkLeadingEta = noUseValue_;
  double trkLeadingPhi = noUseValue_;
  uint trkLeadingNoValidPixelHits = static_cast<uint>(noUseValue_*noUseValue_);
  uint trkLeadingNoValidStripHits = static_cast<uint>(noUseValue_*noUseValue_);
  double trkEtaMax = -20.0;
  double trkEtaMin =  20.0;

  bool isAccessibleHFRHInformation = false;
  uint hfNo = 0;
  uint hfmNo = 0;
  uint hfmNo4 = 0;
  uint hfpNo4 = 0;
  uint hfmNo5 = 0;
  uint hfpNo5 = 0;
  uint hfmNo6 = 0;
  uint hfpNo6 = 0;
  double hfmE = 0.0;
  double hfmE4all1 = 0.0;
  double hfmE4long1 = 0.0;
  double hfmE4short1 = 0.0;
  double hfmE4all2 = 0.0;
  double hfmE4long2 = 0.0;
  double hfmE4short2 = 0.0;
  double hfmLeadingE = noUseValue_;
  double hfmLeadingT = noUseValue_; 
  double hfpE = 0.0;

  bool isAccessibleHBHERHInformation = false;
  uint hbheNo = 0;
  uint hbhemNo = 0;
  double hbhemE = 0.0;

  bool isAccessibleEBRHInformation = false;
  uint ebNo = 0;
  uint ebmNo = 0;
  double ebmE = 0.0;

  bool isAccessibleEERHInformation = false;
  uint eeNo = 0;
  uint eemNo = 0;
  double eemE = 0.0;

  bool isAccessibleCasRHInformation = false;
  uint casRHNo = 0;
  double casRHE = 0.0;
  double casLeadRHE = noUseValue_;
  double casLeadRHT = noUseValue_;

  bool isAccessibleCasTowInformation = false;
  uint casTowNo = 0;
  double casTowE = 0.0;
  double casTowEt = 0.0;

  bool isAccessibleCasJetInformation = false;
  uint casJetNo = 0;
  double casLeadingJetE = noUseValue_;

  bool isAccessiblePFCluEcalInformation = false;
  uint pfCluEcalNo = 0;
  double pfLeadingCluEcalE = noUseValue_;

  bool isAccessiblePFCluHcalInformation = false;
  uint pfCluHcalNo = 0;
  double pfLeadingCluHcalE = noUseValue_;

  bool isAccessiblePFCluHFInformation = false;
  uint pfCluHFNo = 0;
  double pfLeadingCluHFE = noUseValue_;
  double pfCluEtaMax = -20.0;
  double pfCluEtaMin =  20.0;

  bool isAccessiblePFCandInformation = false;
  uint pfCandNo = 0;
  double pfLeadingCandE = noUseValue_;
  double pfCandEtaMax = -20.0;
  double pfCandEtaMin =  20.0;

  bool isAccessiblePFJetInformation = false;
  uint pfJetNo = 0;
  double pfLeadingJetE = noUseValue_;

  bool isPFJet20Found = false; 
  bool isCastorJet3000Found = false;

  bool isAccessibleCaloTowerInformation = false;
  uint caloTowerNo = 0;
  uint caloTowerBSCmNo = 0;
  uint caloTowerBSCpNo = 0;
  uint caloTowerBSC1mNo = 0;
  uint caloTowerBSC1pNo = 0;
  uint caloTowerHFmNo = 0;
  uint caloTowerHFmNo4 = 0;
  uint caloTowerHFpNo4 = 0;
  uint caloTowerHFmNo5 = 0;
  uint caloTowerHFpNo5 = 0;
  uint caloTowerHFmNo6 = 0;
  uint caloTowerHFpNo6 = 0;
  double caloLeadingTowerHFE = noUseValue_;
  double caloLeadingTowerE = noUseValue_;
  int caloLeadingTowerieta = static_cast<int>(noUseValue_);
  int caloLeadingToweriphi = static_cast<int>(noUseValue_);
  double caloLeadingTowereta = noUseValue_;
  double caloLeadingTowerphi = noUseValue_;
  double caloTowerEtaMax = -20.0;
  double caloTowerEtaMin =  20.0;
  double xiSDdet = 0.0;
  double xiDDdet = 0.0;
  double deltaRapMaxdet = 0;

  bool isAccessibleGenPartInformation = false;
  uint noGenPart = 0;
  uint noStat1GenPart = 0;
  uint noStat1ChPt250Eta2p5 = 0;
  uint noStat1ChEtap1p94p9 = 0;
  uint noStat1ChEtam3p51p5 = 0;
  uint noStat1GenPartInBSCm = 0;
  uint noStat1GenPartInBSCp = 0;
  uint noStat1GenPartInBSC1m = 0;
  uint noStat1GenPartInBSC1p = 0;
  double xiSD = 0.0;
  double xiDD = 0.0;
  double genPartCmEnergy = 0.0;
  double genPartEtaMax = -1000.0;
  double genPartEtaMin =  1000.0;
  double genPartDeltaRapMax = 0.0;

/*
%MSG-i GetByLabelWithoutRegistration:  CastorAnalyser:cAnalyser  04-Oct-2015 23:43:10 CEST Run: 1 Event: 10721
::getByLabel without corresponding call to consumes or mayConsumes for this module.
  type: L1GlobalTriggerReadoutRecord
  module label: gtDigis
  product instance name: ''
  process name: 'RECO'
*/
  edm::Handle<edm::TriggerResults> triggerResults_h;
  try{ 
#ifdef GET_BY_LABEL
    iEvent.getByLabel(_trgResSrc, triggerResults_h); 
#else
    iEvent.getByToken(myTokensExt_.get<6>(), triggerResults_h);
#endif
// https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/HLTrigger/HLTcore/plugins/HLTEventAnalyzerAOD.cc
// https://github.com/cms-sw/cmssw/blob/CMSSW_7_5_X/HLTrigger/HLTcore/plugins/HLTEventAnalyzerAOD.cc
    const edm::TriggerResults *triggerResults = triggerResults_h.failedToGet()? 0 : &*triggerResults_h;
    if (triggerResults) {
      bool changedConfig = false;
      bool configInitFailed = false;
#ifndef SW_75X
      if (!hltConfig_.init(iEvent.getRun(), iSetup, "HLT", changedConfig)) {
#else
      if (!hltPrescaleProvider_.init(iEvent.getRun(), iSetup, "HLT", changedConfig)) {
#endif
	edm::LogVerbatim(" hltConfig ") << "Initialization of HLTConfigPrescaleProvider failed!!" << std::endl;      
	configInitFailed = true;
      }
      if (changedConfig && !configInitFailed){
        triggerBit_ = -1;
#ifndef SW_75X
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        hltConfig_.dump("ProcessName");
        hltConfig_.dump("GlobalTag");
        hltConfig_.dump("TableName");
        hltConfig_.dump("Streams");
        hltConfig_.dump("Datasets");
        hltConfig_.dump("PrescaleTable");
        hltConfig_.dump("ProcessPSet");
        //
	if (_ShowDebug) edm::LogVerbatim(" hltConfig ") << " The HLTcurrentMenu is " << hltConfig_.tableName() << std::endl;
	for (size_t j1 = 0; j1 < hltConfig_.triggerNames().size(); j1++) {
	  std::pair<int,int> psValueCombo =  hltConfig_.prescaleValues(iEvent, iSetup, hltConfig_.triggerNames()[j1]); //hltPath
	  if (TString(hltConfig_.triggerNames()[j1]).Contains(_hltPathName)) { 
	    triggerBit_ = j1; ///////////////////////////////////////////prescaleValuesInDetail (but very recent releases)
	    histos1D_["trigger_info"]->GetXaxis()->SetBinLabel(1,hltConfig_.triggerNames()[j1].c_str());
	    //histos1D_["trigger_info"]->Fill(1,triggerResults->accept(j1));
	  }
	  if (_ShowDebug) edm::LogVerbatim(" hltConfig ") << " Name: " << hltConfig_.triggerNames()[j1] << " accept: " << triggerResults->accept(j1) << " , L1 pre-scale: " << psValueCombo.first << " , HLT prescale: " << psValueCombo.second << std::endl;
	}
	if (triggerBit_ == -1) { 
	  edm::LogWarning(" hltConfig ") << "Reference HLT path " << _hltPathName << " not found" << std::endl;
	} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
        //if (_ShowDebug) edm::LogVerbatim(" hltConfig ") << " The HLTcurrentMenu is " << hltPrescaleProvider_.tableName() << std::endl;
        HLTConfigProvider const& hltConfig = hltPrescaleProvider_.hltConfigProvider();
        const std::string triggerName = _hltPathName;
        hltConfig.dump("Triggers"); // Dumps onto standard output !
        hltConfig.dump("ProcessName");
        hltConfig.dump("GlobalTag");
        hltConfig.dump("TableName");
        hltConfig.dump("Streams");
        hltConfig.dump("Datasets");
        hltConfig.dump("PrescaleTable");
        hltConfig.dump("ProcessPSet");         
        //
        const unsigned int nsize(hltConfig.size());
        //for (unsigned int j1=0; j1!=nsize; ++j1) { /*pseudoCodeAnalyzeTrigger(iEvent,iSetup,hltConfig.triggerName(j1));*/ if ( hltConfig.triggerName(j1).Contains(triggerName) ) {triggerBit_ = j1;}  }
        const unsigned int triggerIndex(hltConfig.triggerIndex(triggerName)); 
        if (triggerIndex>=nsize || triggerResults->size()!=nsize || triggerIndex!=iEvent.triggerNames(*triggerResults).triggerIndex(triggerName)) {
          edm::LogVerbatim(" hltConfig ") << " Event trigger contents inconsistence " << std::endl;
          std::cout << " triggerIndex: " << triggerIndex << " nsize: " << nsize << std::endl;
          std::cout << " triggerResults->size(): " << triggerResults->size() << " nsize: " << nsize << std::endl;
          std::cout << " triggerIndex: " << triggerIndex << " iEvent.triggerNames(*triggerResults).triggerIndex(triggerName): " << iEvent.triggerNames(*triggerResults).triggerIndex(triggerName) << std::endl;
        }
        else {
          triggerBit_ = triggerIndex;
          histos1D_["trigger_info"]->GetXaxis()->SetBinLabel(1,hltConfig.triggerName(triggerIndex).c_str());
          const std::pair<int,int> prescales(hltPrescaleProvider_.prescaleValues(iEvent,iSetup,triggerName)); 
          if (_ShowDebug) edm::LogVerbatim(" hltConfig ") << " Path " << triggerName << " [" << triggerIndex << "] "
                          << std::endl 
                          << " prescales L1T,HLT: " << prescales.first << "," << prescales.second
                          << std::endl;
          const std::pair<std::vector<std::pair<std::string,int> >,int> prescalesInDetail(hltPrescaleProvider_.prescaleValuesInDetail(iEvent,iSetup,triggerName));
          std::ostringstream message;
          for (unsigned int i1=0; i1<prescalesInDetail.first.size(); ++i1) {
            message << " " << i1 << ":" << prescalesInDetail.first[i1].first << "/" << prescalesInDetail.first[i1].second;
          }
          if (_ShowDebug) edm::LogVerbatim(" hltConfig ") << " Path " << triggerName << " [" << triggerIndex << "] "
                          << std::endl
                          << " prescales L1T: " << prescalesInDetail.first.size() <<  message.str()
                          << std::endl
                          << " prescale  HLT: " << prescalesInDetail.second
                          << std::endl;          
        }
#endif
      }
      if (triggerBit_ != -1) histos1D_["trigger_info"]->Fill(1,triggerResults->accept(triggerBit_));
    }
  }
  catch(...) { edm::LogVerbatim(" TriggerResults ") << " Cannot get TriggerResults " << std::endl; }

#ifdef DO_RECHITS 
  bool accessMoreThanJustCastor = true;
#else
  bool accessMoreThanJustCastor = false;
#endif
  std::vector<castor::pObject> pRapOrdObjectsDet;
  //
  if (accessMoreThanJustCastor) {
    if (firstTimeAnalyse_) {
      edm::ESHandle<CaloGeometry> pG;
      try { iSetup.get<CaloGeometryRecord>().get(pG); }
      catch(...) { edm::LogWarning(" GEOMETRY ") << " Cannot get CaloGeometryRecord " << std::endl; }
      if (pG.isValid() && !pG.failedToGet()) { geo_ = pG.product();
      } else { geo_= 0; } //NULL; }
      if (!geo_) {
	edm::LogError(" GEOMETRY ") << " Cannot proceed without CaloGeometryRecord " << std::endl;
	exit(1);
      }
    }
    edm::Handle<reco::VertexCollection> vertices_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_vtxSrc, vertices_h);
#else
      iEvent.getByToken(myTokensExt_.get<2>(), vertices_h);
#endif
      const reco::VertexCollection *vertices = vertices_h.failedToGet()? 0 : &*vertices_h;
      if (vertices) {
	isAccessibleVtxInformation = true;
	vtxNo = vertices->size();
	if (_ShowDebug) edm::LogVerbatim("Vtx") << " VtxCollection size: " << vertices->size() << std::endl;
	int firstNotFake = -1;
	for (unsigned int i1=0; i1<vertices->size(); i1++) { 
	  if (!vertices->at(i1).isFake()) { 
	    vtxNoNotFake++; 
	    if(firstNotFake < 0) {firstNotFake = i1;} 
	  } 
	}
	if (firstNotFake >= 0) {
	  const reco::Vertex&  pv = vertices->at(firstNotFake);
	  if (firstNotFake == 0) vtxIsFake0 = 0;
	  //double ndof = pv.normalizedChi2();
	  //double zpos = std::abs(pv.z()); //fabs()
	  double rho = sqrt(pv.x()*pv.x()+pv.y()*pv.y()); 
	  vtxRho = rho;
	  //if(ndof > 4.0 && zpos <= 24.0 && rho <= 2.0) vtxNo++;
	  //if(i==0 && ndof > 4.0 && rho <= 2.0) 
	  vtxZ = pv.z();
	  vtxZerr = pv.zError();
	  vtxY = pv.y();
	  vtxX = pv.x();
	  //if(i==0 && ndof > 4.0 && rho <= 2.0 && zpos <= 24.0) 
	  vtxTrkNo = pv.tracksSize();
	}       
      }
    }//try vertices 
    catch(...) { //try vertices 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get vertices" << std::endl; //LogError //std::cout //LogInfo
    }
    edm::Handle<reco::TrackCollection> tracks_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_trkSrc, tracks_h);
#else
      iEvent.getByToken(myTokensExt_.get<3>(),tracks_h); 
#endif
      const reco::TrackCollection *tracks = tracks_h.failedToGet()? 0 : &*tracks_h; 
      if (tracks) {
        isAccessibleTrkInformation = true;
        trkNo = tracks->size(); 
        if (_ShowDebug) edm::LogVerbatim("Trk") << " TrkCollection size: " << tracks->size() << std::endl;
        for (unsigned int i1=0; i1<tracks->size(); i1++) {
	  const reco::Track& trackData = (*tracks)[i1];
	  bool highPurity = trackData.quality(reco::Track::highPurity);
	  double eta = trackData.eta(); 
          double pt =  trackData.pt();
          double phi = trackData.phi();
          if (pt > trkLeadingPt) { trkLeadingPt = pt;  trkLeadingEta = eta; trkLeadingPhi = phi; trkLeadingNoValidStripHits = trackData.hitPattern().numberOfValidStripHits(); trkLeadingNoValidPixelHits = trackData.hitPattern().numberOfValidPixelHits(); }
	  //https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/DPGAnalysis/Skims/src/FilterOutScraping.cc
	  //fraction = (float)numhighpurity/(float)tkColl->size();if(tkColl->size()>numtrack&&fraction>thr)accept;
	  if (highPurity) { trkHighNo++; } //trackData.pt()>1;trackData.eta();trackData.phi();
	  if (eta > trkEtaMax) { trkEtaMax = eta; }
          if (eta < trkEtaMin) { trkEtaMin = eta; }
        }
      }
    }//try tracks
    catch(...) { //try tracks 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get tracks" << std::endl; //LogError //std::cout //LogInfo
    }
    edm::Handle < HFRecHitCollection > hfrechits_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_hfRHSrc, hfrechits_h);
#else
      iEvent.getByToken(myTokens_.get<1>(),hfrechits_h);
#endif
      const HFRecHitCollection *hfrechits = hfrechits_h.failedToGet()? 0 : &*hfrechits_h;
      if (hfrechits) {
        isAccessibleHFRHInformation = true;
        hfNo = hfrechits->size();
        if (_ShowDebug) edm::LogVerbatim("HFRecHits") << " HFRecHitCollection size: " << hfrechits->size() << std::endl; 
        for(size_t i1 = 0; i1 < hfrechits->size(); ++i1){
	  const HFRecHit & hfrh = (*hfrechits)[i1];
	  HcalDetId hfid = hfrh.id();
          double energy = ForwardRecord::hfScaleFactor * hfrh.energy();
          double ti     =                                hfrh.time();
	  double eta = geo_->getPosition(hfid).eta();
	  if (std::abs(hfid.ieta()) == 29 || std::abs(hfid.ieta()) == 41) { continue; } //exclude 1st/last towers of HF from all considerations (one starts counting from 1?) //fabs
	  if (hfid.ieta() < 0)  {
	    hfmNo++;
	    hfmE += energy;
            if (energy > hfmLeadingE) { hfmLeadingE = energy; hfmLeadingT = ti; }
	    // https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/RecoLocalCalo/HcalRecAlgos/src/HcalHFStatusBitFromRecHits.cc
	    // https://github.com/cms-sw/cmssw/blob/2b75137e278b50fc967f95929388d430ef64710b/RecoCaloTools/Selectors/test/TestCaloSelectors.cc (geometry, et, detid)
	    if (energy > 4.0) { 
	      hfmNo4++; 
	      if (eta >= -4.0) {
		hfmE4all1 += energy; 
		if (hfid.depth()==1) {hfmE4long1 += energy;}; 
		if (hfid.depth()==2) {hfmE4short1 += energy;}; 
	      }
	      if (eta < -4.0) {
		hfmE4all2 += energy;
		if (hfid.depth()==1) {hfmE4long2 += energy;};
		if (hfid.depth()==2) {hfmE4short2 += energy;};
	      }
	    }
	    if (energy > 5.0) hfmNo5++;
	    if (energy > 6.0) hfmNo6++;
	  }
	  if (hfid.ieta() > 0)  {
	    hfpE += energy;
	    if (energy > 4.0) hfpNo4++;
	    if (energy > 5.0) hfpNo5++;
	    if (energy > 6.0) hfpNo6++;
	  }
	  const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
	  if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {etaBinEnergies2[ibin-1] += energy; etaBinEts2[ibin-1] += energy/cosh(eta);} //no cut on rechits!
        }
      }
    }//try hfrechits 
    catch(...) { //try hfrechits
      edm::LogVerbatim("CastorAnalyser") << " Cannot get hfrechits " << std::endl; //LogError //std::cout //LogInfo
    }  
    edm::Handle < HBHERecHitCollection > hbherechits_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_hbheRHSrc, hbherechits_h);
#else
      iEvent.getByToken(myTokens_.get<2>(),hbherechits_h);
#endif
      const HBHERecHitCollection *hbherechits = hbherechits_h.failedToGet()? 0 : &*hbherechits_h;
      if (hbherechits) {
        isAccessibleHBHERHInformation = true;
        hbheNo = hbherechits->size();
        if (_ShowDebug) edm::LogVerbatim("HBHERecHits") << " HBHERecHitCollection size: " << hbherechits->size() << std::endl;
        for(size_t i1 = 0; i1 < hbherechits->size(); ++i1){
	  const HBHERecHit & hbherh = (*hbherechits)[i1];
	  HcalDetId hbheid = hbherh.id();
          double energy = ForwardRecord::hbheScaleFactor * hbherh.energy();
          double eta = geo_->getPosition(hbheid).eta();
	  if (hbheid.ieta() < 0) {
	    hbhemNo++;
	    hbhemE += energy; 
	  } 
	  const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
	  if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {etaBinEnergies2[ibin-1] += energy; etaBinEts2[ibin-1] += energy/cosh(eta);} //no cut on rechits!
        }
      }
    }//try hbherechits
    catch(...) { //try hbherechits
      edm::LogVerbatim("CastorAnalyser") << " Cannot get hbherechits " << std::endl;
    }
    edm::Handle < EBRecHitCollection > ebrechits_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_ebRHSrc, ebrechits_h);
#else
      iEvent.getByToken(myTokens_.get<3>(),ebrechits_h); // ----> ???this does not work (" Cannot get ebrechits " see in the log)! why?
#endif
      //iEvent.getByLabel( edm::InputTag("ecalRecHit","EcalRecHitsEB","RECO"),ebrechits_h);
      const EBRecHitCollection *ebrechits = ebrechits_h.failedToGet()? 0 : &*ebrechits_h;
      if (ebrechits) {
	isAccessibleEBRHInformation = true;   
	ebNo = ebrechits->size();
	if (_ShowDebug) edm::LogVerbatim("EBRecHits") << " EBRecHitCollection size: " << ebrechits->size() << std::endl;
	for(size_t i1 = 0; i1 < ebrechits->size(); ++i1){
          const EcalRecHit & ebrh = (*ebrechits)[i1];
          EBDetId ebid = ebrh.id();
          double energy = ForwardRecord::ecalScaleFactor * ebrh.energy();
          double eta = geo_->getPosition(ebid).eta();
          if (ebid.ieta() < 0) {
	    ebmNo++;
	    ebmE += energy;             
          }
          const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {etaBinEnergies2[ibin-1] += energy; etaBinEts2[ibin-1] += energy/cosh(eta);} //no cut on rechits!
	} 
      }
    } //try ebrechits
    catch(...) { //try ebrechits
      edm::LogVerbatim("CastorAnalyser") << " Cannot get ebrechits " << std::endl;
    }
    edm::Handle < EERecHitCollection > eerechits_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_eeRHSrc, eerechits_h);
#else
      iEvent.getByToken(myTokensExt_.get<1>(),eerechits_h); 
#endif
      const EERecHitCollection *eerechits = eerechits_h.failedToGet()? 0 : &*eerechits_h;
      if (eerechits) {
	isAccessibleEERHInformation = true;
	eeNo = eerechits->size();
	if (_ShowDebug) edm::LogVerbatim("EERecHits") << " EERecHitCollection size: " << eerechits->size() << std::endl;
	for(size_t i1 = 0; i1 < eerechits->size(); ++i1){
          const EcalRecHit & eerh = (*eerechits)[i1];
          EEDetId eeid = eerh.id();
          double energy = ForwardRecord::ecalScaleFactor * eerh.energy();
          double eta = geo_->getPosition(eeid).eta();
          if (eeid.zside() < 0) {
	    eemNo++;
	    eemE += energy;
          }
          const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {etaBinEnergies2[ibin-1] += energy; etaBinEts2[ibin-1] += energy/cosh(eta);} //no cut on rechits!
	}
      }
    } //try eerechits
    catch(...) { //try eerechits
      edm::LogVerbatim("CastorAnalyser") << " Cannot get eerechits " << std::endl;
    }
    edm::Handle<CastorRecHitCollection> casrechits_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_casRHSrc, casrechits_h);
#else
      iEvent.getByToken(myTokens_.get<4>(),casrechits_h);
#endif
      const CastorRecHitCollection *casrechits = casrechits_h.failedToGet()? 0 : &*casrechits_h;
      if (casrechits) {
	isAccessibleCasRHInformation = true;
	casRHNo = casrechits->size(); 
	if (_ShowDebug) edm::LogVerbatim("CastorRecHits") << " CastorRecHitCollection size: " << casrechits->size() << std::endl;
	for(size_t i1 = 0; i1 < casrechits->size(); ++i1){
          const CastorRecHit casrh = (*casrechits)[i1];
          HcalCastorDetId casid = casrh.id();
          int sec = casid.sector();
          int mod = casid.module();
          if (castor::channelQuality[sec-1][mod-1]==true) {
	    double corrFactor = 1.0; //0.015; MC should already include this factor for RecHits //ForwardRecord::absCasEscaleFactor;
	    if (isData) corrFactor = /*castor::corr0Tto38T[sec-1][mod-1]* */castor::channelGainQE[sec-1][mod-1] * ForwardRecord::absCasEscaleFactor; //-> is relevant in case of prompt-reco'ed RecHits; now you try to re-reco RecHits yourself and so specify the conditions via python file pooldbsource
	    //PRINT sec,mod,corr for all channels
	    if (firstTimeAnalyse_ && _ShowDebug) {edm::LogVerbatim("FTA") << "FTA" << sec << "," << mod << "," << corrFactor << std::endl;}
	    double energy = corrFactor*casrh.energy();
            double ti     =            casrh.time();
	    casRHE += energy;
            if (energy > casLeadRHE) { casLeadRHE = energy; casLeadRHT = ti; }
            histosProfile_["h_casenerh_vs_iz"]->Fill( static_cast<double>(mod) , energy );
            histosProfile_["h_casenerh_vs_iphi"]->Fill( static_cast<double>(sec) , energy );
          } else {
            if (firstTimeAnalyse_ && _ShowDebug) edm::LogVerbatim("FTA") << "FTA " << sec << "," << mod << "," << "NoNum" << std::endl;
          }
	}
      }
    } //try castorhits
    catch(...) { //try castorhits
      edm::LogVerbatim("CastorAnalyser") << " Cannot get castorhits " << std::endl;
    }
    edm::Handle<reco::CastorTowerCollection> castowers_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_casTowSrc, castowers_h);
#else
      iEvent.getByToken(myTokens_.get<5>(),castowers_h);
#endif
      const reco::CastorTowerCollection *castowers = castowers_h.failedToGet()? 0 : &*castowers_h; 
      if (castowers) {
	isAccessibleCasTowInformation = true;
	casTowNo = castowers->size();
	if (_ShowDebug) edm::LogVerbatim("CastorTowers") << " CastorTowerCollection size: " << castowers->size() << std::endl;
	//https://github.com/cms-sw/cmssw/blob/e9d56d349206311983789ab1a4c374761519425d/RecoHI/HiEvtPlaneAlgos/src/EvtPlaneProducer.cc
	for(std::vector<reco::CastorTower>::const_iterator j1 = castowers->begin(); j1 != castowers->end(); j1++) {
          double energy = (j1->energy());
          double et     = (j1->et());
          double eta = (j1->eta());
          double phi = (j1->phi());
          //
          double pz     = et*sinh(eta);
          double px     = et*cos(phi);
          double py     = et*sin(phi);
          //
          casTowE += energy;
          casTowEt += et;
          if (eta > caloTowerEtaMax) {caloTowerEtaMax = eta;} //global calotower eta-max, including also castor towers
          if (eta < caloTowerEtaMin) {caloTowerEtaMin = eta;}
          if (_ShowDebug && firstTimeAnalyse_ && et > 1.e-6) {edm::LogVerbatim("FTA") << "FTA " << " casTowE = :" << energy << " casTowEt = :" << et << " casTowE / casTowEt = : " << energy/et << " tower eta: " << eta << std::endl;}
          //xiSD at detector level
          castor::pObject obj;
          obj.Energy = energy;
          obj.Pz     = pz;
          obj.Px     = px;
          obj.Py     = py;
          obj.Eta    = eta;
          obj.Rap    = eta; //0.5*log( (obj.Energy+obj.Pz) / (obj.Energy-obj.Pz) )
          pRapOrdObjectsDet.push_back(obj);          
	}
	//for(size_t i1 = 0; i1 < castowers->size(); ++i1){
	//   const reco::CastorTower castow = (*castowers)[i1]; 
	//   casTowE += castow.energy();
	//}
      } //check of calotower-handler
    }
    catch(...) { //try castortowers
      edm::LogVerbatim("CastorAnalyser") << " Cannot get castortowers " << std::endl;
    }
    edm::Handle<reco::BasicJetCollection> casjets_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_casJetSrc, casjets_h);
#else
      iEvent.getByToken(myTokens_.get<6>(),casjets_h);
#endif
      const reco::BasicJetCollection *casjets = casjets_h.failedToGet()? 0 : &*casjets_h;
      if (casjets) {
	isAccessibleCasJetInformation = true;
	casJetNo = casjets->size();
	if (_ShowDebug) edm::LogVerbatim("CastorJets") << " CastorJetCollection size: " << casjets->size() << std::endl;
	//https://github.com/UAEDF/UATree/blob/master/UABaseTree/src/GetCastorJet.cc
	for (std::vector<reco::BasicJet>::const_iterator j1 = casjets->begin(); j1 != casjets->end(); j1++) {
          if(j1->energy() > casLeadingJetE) casLeadingJetE = j1->energy();
          if(j1->energy() > 3000.0) isCastorJet3000Found = true; //https://indico.cern.ch/event/438563/session/2/attachments/1139174/1631238/JetTriggerEff.pdf
	}
      }
    }
    catch(...) { //try castorjets
      edm::LogVerbatim("CastorAnalyser") << " Cannot get castorjets " << std::endl;
    }
    //
    edm::Handle<reco::PFClusterCollection> pfcluecal_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_pfCluEcalSrc, pfcluecal_h);
#else
      iEvent.getByToken(myTokens_.get<7>(),pfcluecal_h);
#endif
      const reco::PFClusterCollection *pfcluecal = pfcluecal_h.failedToGet()? 0 : &*pfcluecal_h;
      if (pfcluecal) {
	isAccessiblePFCluEcalInformation = true;
	pfCluEcalNo = pfcluecal->size();
	if (_ShowDebug) edm::LogVerbatim("pfEcalClusters") << " PFClusterCollection size in ECAL: " << pfcluecal->size() << std::endl;
	for (reco::PFClusterCollection::const_iterator j1 = pfcluecal->begin(); j1 != pfcluecal->end(); ++j1) {
	  double energy = ForwardRecord::ecalScaleFactor * j1->energy();
	  double eta    = j1->eta();
	  double phi    = j1->phi();
	  if (energy > pfLeadingCluEcalE) pfLeadingCluEcalE = energy;
	  //if(energy > 1.0) { //above threshold //3.0
	  histos2D_["pfclu_ene_map"]->Fill(eta,phi,energy);
	  histos2D_["pfclu_occup_map"]->Fill(eta,phi);
	  //}
	  const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
	  if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {
            etaBinEnergies[ibin-1] += energy; etaBinEts[ibin-1] += energy/cosh(eta);
            std::string hi_tag = Form("pfclu_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);            
          }
          if (eta > pfCluEtaMax) {pfCluEtaMax = eta;}
          if (eta < pfCluEtaMin) {pfCluEtaMin = eta;}
	}
      }
    }
    catch(...) { //try pfclusters in ecal 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get pfclusters in ecal " << std::endl;
    }
    edm::Handle<reco::PFClusterCollection> pfcluhcal_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_pfCluHcalSrc, pfcluhcal_h);
#else
      iEvent.getByToken(myTokens_.get<8>(),pfcluhcal_h);
#endif
      const reco::PFClusterCollection *pfcluhcal = pfcluhcal_h.failedToGet()? 0 : &*pfcluhcal_h;
      if (pfcluhcal) {
	isAccessiblePFCluHcalInformation = true;
	pfCluHcalNo = pfcluhcal->size();
	if (_ShowDebug) edm::LogVerbatim("pfHcalClusters") << " PFClusterCollection size in HCAL: " << pfcluhcal->size() << std::endl;
	for (reco::PFClusterCollection::const_iterator j1 = pfcluhcal->begin(); j1 != pfcluhcal->end(); ++j1) {
	  double energy = ForwardRecord::hbheScaleFactor * j1->energy();
	  double eta = j1->eta();
	  double phi = j1->phi();
	  if(energy > pfLeadingCluHcalE) pfLeadingCluHcalE = energy;
	  //if(energy > 1.0) { //above threshold //3.0
	  histos2D_["pfclu_ene_map"]->Fill(eta,phi,energy);
	  histos2D_["pfclu_occup_map"]->Fill(eta,phi);
	  //}
	  const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
	  if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {
            etaBinEnergies[ibin-1] += energy; etaBinEts[ibin-1] += energy/cosh(eta);
            std::string hi_tag = Form("pfclu_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          if (eta > pfCluEtaMax) {pfCluEtaMax = eta;}
          if (eta < pfCluEtaMin) {pfCluEtaMin = eta;}
	}
      }
    }
    catch(...) { //try pfclusters in hcal 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get pfclusters in hcal " << std::endl;
    }
    edm::Handle<reco::PFClusterCollection> pfcluhf_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_pfCluHFSrc, pfcluhf_h);
#else
      iEvent.getByToken(myTokens_.get<9>(),pfcluhf_h);
#endif
      const reco::PFClusterCollection *pfcluhf = pfcluhf_h.failedToGet()? 0 : &*pfcluhf_h;
      if (pfcluhf) {
	isAccessiblePFCluHFInformation = true;
	pfCluHFNo = pfcluhf->size();
	if (_ShowDebug) edm::LogVerbatim("pfHFClusters") << " PFClusterCollection size in HF: " << pfcluhf->size() << std::endl;
	for (reco::PFClusterCollection::const_iterator j1 = pfcluhf->begin(); j1 != pfcluhf->end(); ++j1) {
	  //should exclude 1st/last towers of HF from all considerations BUT DO NOT KNOW HOW TO DO IT HERE!
	  double energy = ForwardRecord::hfScaleFactor * j1->energy();
	  double eta = j1->eta();
	  double phi = j1->phi();        
	  if(energy > pfLeadingCluHFE) pfLeadingCluHFE = energy;
	  //if(energy > 1.0) { //above threshold //3.0
	  histos2D_["pfclu_ene_map"]->Fill(eta,phi,energy);
	  histos2D_["pfclu_occup_map"]->Fill(eta,phi);
	  //}
	  const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
	  if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {
            etaBinEnergies[ibin-1] += energy; etaBinEts[ibin-1] += energy/cosh(eta);
            std::string hi_tag = Form("pfclu_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          if (eta > pfCluEtaMax) {pfCluEtaMax = eta;}
          if (eta < pfCluEtaMin) {pfCluEtaMin = eta;}
	}
      }
    }
    catch(...) { //try pfclusters in hf 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get pfclusters in hf " << std::endl;
    }
    //
    edm::Handle<reco::PFCandidateCollection> pfcand_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_pfCandSrc, pfcand_h);
#else
      iEvent.getByToken(myTokensExt_.get<5>(),pfcand_h);
#endif
      const reco::PFCandidateCollection *pfcand = pfcand_h.failedToGet()? 0 : &*pfcand_h;
      if (pfcand) {
	isAccessiblePFCandInformation = true;
	pfCandNo = pfcand->size();
	if (_ShowDebug) edm::LogVerbatim("pfCandidates") << " PFCandidateCollection size: " << pfcand->size() << std::endl;
	for(reco::PFCandidateCollection::const_iterator j1 = pfcand->begin(); j1 != pfcand->end(); ++j1) {
          double energy = j1->energy(); //for now do not introduce any energy scaling like ForwardRecord::hbheScaleFactor: do not know how to do it in a uniform way
          /*double et = j1->et();*/
          double eta = j1->eta();
          double phi = j1->phi();
	  int id = j1->particleId();
	  bool add_particle = false; //true;/*false;*/
	  //X=0,           // undefined
	  //h=1,           // charged hadron
	  //e=2,           // electron 
	  //mu=3,          // muon 
	  //gamma=4,       // photon
	  //h0=5,          // neutral hadron
	  //h_HF=6,        // HF tower identified as a hadron
	  //egamma_HF=7    // HF tower identified as an EM particle		
	  if(id != 4 && id != 5 && id != 6 && id != 7) add_particle = true;
	  if(std::abs(eta) > 0.0 && std::abs(eta) < 1.4 && id == 4 && energy > 0.4) add_particle = true;
	  if(std::abs(eta) > 0.0 && std::abs(eta) < 1.4 && id == 5 && energy > 2.0) add_particle = true;
	  if(std::abs(eta) > 1.4 && std::abs(eta) < 3.2 && id == 4 && energy > 1.8) add_particle = true;
	  if(std::abs(eta) > 1.4 && std::abs(eta) < 3.2 && id == 5 && energy > 2.9) add_particle = true;
          /*
	  if(std::abs(eta) > 3.2 && std::abs(eta) < 5.0 && id == 6 && energy > 4.0) add_particle = true;
	  if(std::abs(eta) > 3.2 && std::abs(eta) < 5.0 && id == 7 && energy > 4.0) add_particle = true;
          */
          if(std::abs(eta) > 2.8 && std::abs(eta) < 4.2 && id == 6 && energy > 2.9) add_particle = true; //3.2
          if(std::abs(eta) > 2.8 && std::abs(eta) < 4.2 && id == 7 && energy > 2.9) add_particle = true; //3.2
          if(std::abs(eta) > 4.2 && std::abs(eta) < 5.0 && id == 6 && energy > 4.0) add_particle = true;
          if(std::abs(eta) > 4.2 && std::abs(eta) < 5.0 && id == 7 && energy > 4.0) add_particle = true;
          if(energy > pfLeadingCandE) pfLeadingCandE = energy;
          const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {etaBinEnergies4[ibin-1] += energy; etaBinEts4[ibin-1] += energy/cosh(eta);/*et*/}
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && id == 1) {
            std::string hi_tag = Form("pfcand1_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && id == 4) {
            etaBinEnergies6[ibin-1] += energy;
            std::string hi_tag = Form("pfcand4_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && id == 5) {
            etaBinEnergies7[ibin-1] += energy;
            std::string hi_tag = Form("pfcand5_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && id == 6) {
            etaBinEnergies8[ibin-1] += energy;
            std::string hi_tag = Form("pfcand6_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins && id == 7) {
            etaBinEnergies9[ibin-1] += energy;
            std::string hi_tag = Form("pfcand7_single_ieta%d",ibin);
            histos1D_[hi_tag]->Fill(energy);
          }
          histos2D_["pfcan_ene_map"]->Fill(eta,phi,energy);
          histos2D_["pfcan_occup_map"]->Fill(eta,phi);
          if(add_particle) { //above threshold //energy > 3.0
            histos2D_["pfcan_ene_map1"]->Fill(eta,phi,energy);
            histos2D_["pfcan_occup_map1"]->Fill(eta,phi);
	    if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {etaBinEnergies5[ibin-1] += energy; etaBinEts5[ibin-1] += energy/cosh(eta);/*et*/}
            if (eta > pfCandEtaMax) {pfCandEtaMax = eta;}
            if (eta < pfCandEtaMin) {pfCandEtaMin = eta;}
          }
	}
      }
    }
    catch(...) { //try pfcandidates 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get pfcandidates " << std::endl;
    }  
    //
    edm::Handle<reco::PFJetCollection> pfjet_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_pfJetSrc, pfjet_h);
#else
      iEvent.getByToken(myTokensExt_.get<7>(),pfjet_h);
#endif
      const reco::PFJetCollection *pfjet = pfjet_h.failedToGet()? 0 : &*pfjet_h;
      if (pfjet) {
	isAccessiblePFJetInformation = true; 
	pfJetNo = pfjet->size();
	if (_ShowDebug) edm::LogVerbatim("pfJets") << " PFJetCollection size: " << pfjet->size() << std::endl;
	for(reco::PFJetCollection::const_iterator j1 = pfjet->begin(); j1 != pfjet->end(); ++j1) {
	  // https://github.com/UAEDF/UATree/blob/master/UABaseTree/src/GetRecoPFJet.cc
          if(j1->energy() > pfLeadingJetE) pfLeadingJetE = j1->energy();
          if(j1->et() > 20.0) isPFJet20Found = true;
	}
      }
    }
    catch(...) { //try pfjets
      edm::LogVerbatim("CastorAnalyser") << " Cannot get pfjets " << std::endl;
    }
    //
    edm::Handle<CaloTowerCollection> towers_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_caloTowerSrc, towers_h);
#else
      iEvent.getByToken(myTokensExt_.get<0>(),towers_h);
#endif
      const CaloTowerCollection *towers = towers_h.failedToGet()? 0 : &*towers_h;
      if (towers) { 
	isAccessibleCaloTowerInformation = true;
	caloTowerNo = towers->size();
	if (_ShowDebug) edm::LogVerbatim("caloTowers") << " caloTowerCollection size: " << towers->size() << std::endl;
	for(unsigned int i1 = 0; i1 < towers->size(); ++i1){
	  const CaloTower & hit= (*towers)[i1];
          double eta = hit.eta();
//          if (std::abs(hit.ieta()) == 29 || std::abs(hit.ieta()) == 41) { if (_ShowDebug && firstTimeAnalyse_) {edm::LogVerbatim("FTA") << "FTA " << " ieta = :" << hit.ieta() << " eta = :" << eta << std::endl;}; continue; } //exclude 1st/last towers of HF from all considerations (one starts counting from 1?) //fabs
          double emEnergy  = hit.emEnergy();
          double hadEnergy = hit.hadEnergy();
          bool hasEB = false;
          bool hasEE = false;
          bool hasHB = false;
          bool hasHE = false;
          bool hasHF = false;
          for(size_t iconst = 0; iconst < hit.constituentsSize(); iconst++){ //-- loop over CaloTower constituents
            DetId detId = hit.constituent(iconst);
            if(detId.det()==DetId::Ecal){
              EcalSubdetector ecalSubDet = (EcalSubdetector)detId.subdetId();
	      if(ecalSubDet == EcalBarrel) hasEB = true;
              else if(ecalSubDet == EcalEndcap) hasEE = true;
            }
            else if(detId.det()==DetId::Hcal){
	      HcalDetId hcalDetId(detId);
	      if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
	      else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
	      else if(hcalDetId.subdet()==HcalForward) hasHF = true;
            } 
          }
          if(hasEB || hasEE) { emEnergy = ForwardRecord::ecalScaleFactor * emEnergy; }
          if(hasHB || hasHE) 
            { hadEnergy = ForwardRecord::hbheScaleFactor * hadEnergy; }
          else if (hasHF) 
            { hadEnergy = ForwardRecord::hfScaleFactor * hadEnergy; }
          double energy = emEnergy + hadEnergy; //hit.energy()
          double et = energy/cosh(eta);
          double phi = hit.phi();
          double pz     = et*sinh(eta);
          double px     = et*cos(phi);
          double py     = et*sin(phi);
	  /*
	    FTA  ieta = :-29 eta = :-2.87445
	    FTA  ieta = :-29 eta = :-2.87446
	    FTA  ieta = :29 eta = :2.87445
	    FTA  ieta = :29 eta = :2.87445
	    FTA  ieta = :29 eta = :2.83262
	    FTA  ieta = :-29 eta = :-2.90256
	    FTA  ieta = :-29 eta = :-2.89352
	    FTA  ieta = :-41 eta = :-5.03999
	    FTA  ieta = :41 eta = :5.04
	  */
          histos2D_["tow_ene_map"]->Fill(static_cast<double>(hit.ieta()),static_cast<double>(hit.iphi()),energy);
          histos2D_["tow_occup_map"]->Fill(static_cast<double>(hit.ieta()),static_cast<double>(hit.iphi()));
          histos2D_["tow_ene_map1"]->Fill(eta,phi,energy);
          histos2D_["tow_occup_map1"]->Fill(eta,phi);
	  if (std::abs(hit.ieta()) == 29 || std::abs(hit.ieta()) == 41) { if (_ShowDebug && firstTimeAnalyse_) {edm::LogVerbatim("FTA") << "FTA " << " ieta = :" << hit.ieta() << " eta = :" << eta << std::endl;}; continue; } //exclude 1st/last towers of HF from all considerations (one starts counting from 1?) //fabs
          if (energy > caloLeadingTowerE) { caloLeadingTowerE = energy; caloLeadingTowerieta = hit.ieta(); caloLeadingToweriphi = hit.iphi(); caloLeadingTowereta = eta; caloLeadingTowerphi = phi; if (caloLeadingTowerieta > 50 && _ShowDebug) edm::LogVerbatim("FTA") << "FTA " << " (strange) leading ieta = :" << caloLeadingTowerieta << std::endl; }
	  if ((hit.ieta() > 29 || hit.ieta() < -29) && eta < 0.0)  { //HFminus //ieta=29,depth=3->HBHE
	    if (energy > caloLeadingTowerHFE) caloLeadingTowerHFE = energy;
	    caloTowerHFmNo++;
	    //caloTowerHFmE += hit.energy();
	    if (energy > 4.0) caloTowerHFmNo4++;
	    if (energy > 5.0) caloTowerHFmNo5++;
	    if (energy > 6.0) caloTowerHFmNo6++;
	  }
	  if ((hit.ieta() > 29 || hit.ieta() < -29) && eta > 0.0)  { //HFplus //ieta=29,depth=3->HBHE
	    if (energy > caloLeadingTowerHFE) caloLeadingTowerHFE = energy;
	    //caloTowerHFpE += hit.energy();
	    if (energy > 4.0) caloTowerHFpNo4++;
	    if (energy > 5.0) caloTowerHFpNo5++;
	    if (energy > 6.0) caloTowerHFpNo6++;
	  }
	  if (eta>3.23 && eta<4.65 && energy>4.0) { caloTowerBSCpNo++; } //need BSC in the second range of eta's!!!!!!!! still need to fill the trigger histogram //4.0
	  if (eta>-4.65 && eta<-3.23 && energy>4.0) { caloTowerBSCmNo++; } //4.0
	  if (eta>3.9 && eta<4.4 && energy>4.0) { caloTowerBSC1pNo++; }  //4.0
	  if (eta>-4.4 && eta<-3.9 && energy>4.0) { caloTowerBSC1mNo++; } //4.0
	  //if(energy > 1.0) { //above threshold
	    //histos2D_["tow_ene_map"]->Fill(static_cast<double>(hit.ieta()),static_cast<double>(hit.iphi()),energy);
	    //histos2D_["tow_occup_map"]->Fill(static_cast<double>(hit.ieta()),static_cast<double>(hit.iphi()));
            //histos2D_["tow_ene_map1"]->Fill(eta,phi,energy);
            //histos2D_["tow_occup_map1"]->Fill(eta,phi);
	    //if((hit.ieta() > 29 || hit.ieta() < -29) && std::abs(hit.eta())<4.0) { //HF eta<4 //hit.et() //fabs
	    //  runningSampleEnergySum_[4] += hit.et();
	    //  (runningSamplesNo_[4])++;
	    //}
	    //if((hit.ieta() > 29 || hit.ieta() < -29) && std::abs(hit.eta())>4.0) { //HF eta>4 //hit.et() //fabs
	    //  runningSampleEnergySum_[5] += hit.et();
	    //  (runningSamplesNo_[5])++;
	    //}
	  //}	
	  const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( eta );     
	  if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins) { etaBinEnergies1[ibin-1] += energy;  etaBinEts1[ibin-1] += et; /*hit.et();*/ } //no energy thresholds for a tower to be summed
          if(ibin>=1 && ibin<=ForwardRecord::nbEtaBins &&
             (hit.iphi() != 60) && (std::abs(hit.ieta()) != 30) && (hit.ieta() != 21 || hit.iphi() != 5)) { etaBinEts10[ibin-1] += et; } 
          if (eta > caloTowerEtaMax) {caloTowerEtaMax = eta;}
          if (eta < caloTowerEtaMin) {caloTowerEtaMin = eta;}
          //xiSD at detector level
          castor::pObject obj;
          obj.Energy = energy;
          obj.Pz     = pz;
          obj.Px     = px;
          obj.Py     = py;
          obj.Eta    = eta;
          obj.Rap    = eta; //0.5*log( (obj.Energy+obj.Pz) / (obj.Energy-obj.Pz) )
          pRapOrdObjectsDet.push_back(obj);
	}
        if (pRapOrdObjectsDet.size() > 1) std::sort(pRapOrdObjectsDet.begin(), pRapOrdObjectsDet.end(), customLess); //depends on CastorTowerCollection!!//assume sorting from neg. to pos. rap.
        int deltaymax_pos = -1;
        //double deltaRapMaxdet = 0;
        for (uint ipart=0; ipart < pRapOrdObjectsDet.size()-1; ipart++) {
          double deltay = pRapOrdObjectsDet[ipart+1].Rap - pRapOrdObjectsDet[ipart].Rap;
          if (deltay > deltaRapMaxdet) { deltaRapMaxdet = deltay; deltaymax_pos = ipart; }
        }
        double detPositiveEdge = 5.2;
        double detNegativeEdge = -5.2;
        if (isAccessibleCasTowInformation) detNegativeEdge = -6.6; 
        double deltayneg = 0.0;
        double deltaypos = 0.0;
        if (pRapOrdObjectsDet.size() > 0) {
           deltayneg = pRapOrdObjectsDet[0].Eta-detNegativeEdge;
           deltaypos = detPositiveEdge-pRapOrdObjectsDet[pRapOrdObjectsDet.size()-1].Eta;
        }
        if (deltayneg > deltaRapMaxdet) {deltaRapMaxdet = deltayneg; deltaymax_pos = -1; }
        if (deltaypos > deltaRapMaxdet) {deltaRapMaxdet = deltaypos; deltaymax_pos = pRapOrdObjectsDet.size()-1;}
        double xix = 100.0;
        double xiy = 100.0;
        long double XEtot = 0.0;
        long double XPxtot = 0.0;
        long double XPytot = 0.0;
        long double XPztot = 0.0;
        long double YEtot = 0.0;
        long double YPxtot = 0.0;
        long double YPytot = 0.0;
        long double YPztot = 0.0;
        for (int ipart=0;ipart<=deltaymax_pos;ipart++) {
          XEtot  += pRapOrdObjectsDet[ipart].Energy;
          XPxtot += pRapOrdObjectsDet[ipart].Px;
          XPytot += pRapOrdObjectsDet[ipart].Py;
          XPztot += pRapOrdObjectsDet[ipart].Pz;
        }
        long double Mx2 = -1.0;
        Mx2 = XEtot*XEtot - XPxtot*XPxtot - XPytot*XPytot - XPztot*XPztot;
        for (uint ipart=deltaymax_pos+1;ipart<pRapOrdObjectsDet.size();ipart++) {
          YEtot  += pRapOrdObjectsDet[ipart].Energy;
          YPxtot += pRapOrdObjectsDet[ipart].Px; 
          YPytot += pRapOrdObjectsDet[ipart].Py;
          YPztot += pRapOrdObjectsDet[ipart].Pz;
        }
        long double My2 = -1.0;
        My2 = YEtot*YEtot - YPxtot*YPxtot - YPytot*YPytot - YPztot*YPztot;
        if (Mx2 < 0.0) Mx2 = 0.0; // set Mx2, My2 to zero if negative (i.e. the X,Y system is a photon)
        if (My2 < 0.0) My2 = 0.0; 
        if (cmEnergyDet_ > 1e-6) xix = Mx2/(cmEnergyDet_*cmEnergyDet_);
        if (cmEnergyDet_ > 1e-6) xiy = My2/(cmEnergyDet_*cmEnergyDet_);
        xiSDdet = std::max(xix,xiy);
        if (cmEnergyDet_ > 1e-6) xiDDdet = xix*xiy*cmEnergyDet_*cmEnergyDet_/(protonPDGMass_*protonPDGMass_);
      } //check of towers-handler
    }
    catch(...) { //try calotowers 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get calotowers " << std::endl;
    }
    /////////////////////#endif
  } //DO_RECHITS --> accessMoreThanJustCastor

  std::vector<castor::pObject> pRapOrdObjects;
  if(!isData){
    edm::Handle<reco::GenParticleCollection> genParticles_h;
    try{
#ifdef GET_BY_LABEL
      iEvent.getByLabel(_GenPartSrc, genParticles_h);
#else
      iEvent.getByToken(myTokensExt_.get<4>(),genParticles_h);
#endif
      const reco::GenParticleCollection *genParticles = genParticles_h.failedToGet()? 0 : &*genParticles_h; 
      if(genParticles) {
	isAccessibleGenPartInformation = true;
	noGenPart = genParticles->size();
	if (_ShowDebug) edm::LogVerbatim("genParticles") << " GenParticleCollection size: " << genParticles->size() << std::endl;
	int bpcnt = 0;
	for(uint igen=0; igen < genParticles->size(); igen++) {
	  const reco::GenParticle gen = genParticles->at(igen);   
	  if (gen.px() < 1e-6 && gen.py() < 1e-6 && std::abs(gen.pz()) > 0.0 && gen.pdgId() == 2212) {
            bpcnt++;
            if (bpcnt < 3) { genPartCmEnergy += std::abs(gen.pz()); } // add pz of beam particles to get cmenergy, only add two
	  }
	  int status = gen.status();
	  int id = gen.pdgId();
	  if (status != 1) continue; //interested in "final state particles" only
	  if (std::abs(id) == 12 || //muon + neutrinos //muons in case of particle flow???
	      std::abs(id) == 14 ||
	      std::abs(id) == 16 ||
	      std::abs(id) == 13) continue;
	  noStat1GenPart++;
	  castor::pObject obj;
	  obj.Energy = gen.energy();
	  obj.Pz     = gen.pz();
	  obj.Px     = gen.px();
	  obj.Py     = gen.py();
	  obj.Eta    = gen.eta();
	  obj.Rap    = 0.5*log( (obj.Energy+obj.Pz) / (obj.Energy-obj.Pz) ); //https://root.cern.ch/root/htmldoc/rootdoc/html/_t_lorentz_vector_8cxx_source.html
	  bool addParticle = false; 
	  if (obj.Energy > 0.0)  addParticle = true;//want a real threshold here rather than 0?
	  if (addParticle) { 
            pRapOrdObjects.push_back(obj);
            if (std::abs(obj.Eta)<2.5 && (gen.charge()>0 || gen.charge()<0) && gen.pt()>0.25) noStat1ChPt250Eta2p5++;
            if (obj.Eta > 1.9 && obj.Eta < 4.9 && (gen.charge()>0 || gen.charge()<0)) noStat1ChEtap1p94p9++;
            if (obj.Eta > -3.5 && obj.Eta < -1.5 && (gen.charge()>0 || gen.charge()<0)) noStat1ChEtam3p51p5++;
            if (obj.Eta > 3.23 && obj.Eta < 4.65 && (gen.charge()>0 || gen.charge()<0)) noStat1GenPartInBSCp++; //FWD-10-011: BSC=3.9<|eta|<4.4 
            if (obj.Eta > -4.65 && obj.Eta < -3.23 && (gen.charge()>0 || gen.charge()<0)) noStat1GenPartInBSCm++; //FSQ-12-005: BSC=3.23<|eta|<4.65
            if (obj.Eta > 3.9 && obj.Eta < 4.4 && (gen.charge()>0 || gen.charge()<0)) noStat1GenPartInBSC1p++;
            if (obj.Eta > -4.4 && obj.Eta < -3.9 && (gen.charge()>0 || gen.charge()<0)) noStat1GenPartInBSC1m++;
            const uint ibin = histosProfile_["energy_vs_eta_reco"]->FindBin( obj.Eta );
            if (ibin>=1 && ibin<=ForwardRecord::nbEtaBins) {
              etaBinEnergies3[ibin-1] += obj.Energy; etaBinEts3[ibin-1] += obj.Energy/cosh(obj.Eta);/*or better ~sqrt(px2+py2+m2) ???*/ 
              if ((gen.charge()>0 || gen.charge()<0) && gen.p()>0.500) etaBinEts33[ibin-1] += obj.Energy/cosh(obj.Eta);
              if ( gen.charge() == 0                 && gen.p()>0.200) etaBinEts33[ibin-1] += obj.Energy/cosh(obj.Eta);
            } 
            if (ForwardRecord::nbEtaBinsExt > 0 && obj.Eta > ForwardRecord::Eta_Bin_EdgesExt[0]) { 
              const uint ibin2 = histosProfile_["energy_vs_eta_reco2"]->FindBin( obj.Eta );
              if (ibin2>=1 && ibin2<=ForwardRecord::nbEtaBinsExt) {
                etaBinEts333[ibin2-1] += obj.Energy/cosh(obj.Eta);
              }
            }
	  }
	} 
	if (pRapOrdObjects.size() > 1) std::sort(pRapOrdObjects.begin(), pRapOrdObjects.end(), customLess); 
	if (pRapOrdObjects.size() > 0) {
	  genPartEtaMin = pRapOrdObjects[0].Eta;
	  genPartEtaMax = pRapOrdObjects[pRapOrdObjects.size()-1].Eta;
	}
	//double deltaymax = 0;
	int deltaymax_pos = -1;
	for (uint ipart=0; ipart < pRapOrdObjects.size()-1; ipart++) {
	  double deltay = pRapOrdObjects[ipart+1].Rap - pRapOrdObjects[ipart].Rap;
	  if (deltay > genPartDeltaRapMax) { genPartDeltaRapMax = deltay; deltaymax_pos = ipart; }
	}
	double xix = 100.0;
	double xiy = 100.0;
	long double XEtot = 0.0;
	long double XPxtot = 0.0;
	long double XPytot = 0.0;
	long double XPztot = 0.0;
	long double YEtot = 0.0;
	long double YPxtot = 0.0;
	long double YPytot = 0.0;
	long double YPztot = 0.0;      
	for (int ipart=0;ipart<=deltaymax_pos;ipart++) {
       	  XEtot  += pRapOrdObjects[ipart].Energy;
	  XPxtot += pRapOrdObjects[ipart].Px;
	  XPytot += pRapOrdObjects[ipart].Py;
	  XPztot += pRapOrdObjects[ipart].Pz;
	}
	long double Mx2 = -1.0;
	Mx2 = XEtot*XEtot - XPxtot*XPxtot - XPytot*XPytot - XPztot*XPztot;
	for (uint ipart=deltaymax_pos+1;ipart<pRapOrdObjects.size();ipart++) {
    	  YEtot  += pRapOrdObjects[ipart].Energy;
	  YPxtot += pRapOrdObjects[ipart].Px;
	  YPytot += pRapOrdObjects[ipart].Py;
	  YPztot += pRapOrdObjects[ipart].Pz;
	}
	long double My2 = -1.0;
	My2 = YEtot*YEtot - YPxtot*YPxtot - YPytot*YPytot - YPztot*YPztot;
	if (Mx2 < 0.0) Mx2 = 0.0; // set Mx2, My2 to zero if negative (i.e. the X,Y system is a photon)
	if (My2 < 0.0) My2 = 0.0;
	if (genPartCmEnergy > 1e-6) xix = Mx2/(genPartCmEnergy*genPartCmEnergy);
	if (genPartCmEnergy > 1e-6) xiy = My2/(genPartCmEnergy*genPartCmEnergy); 
	xiSD = std::max(xix,xiy);
	if (genPartCmEnergy > 1e-6) xiDD = xix*xiy*genPartCmEnergy*genPartCmEnergy/(protonPDGMass_*protonPDGMass_);
      } //end of if-statement checking genParticle-Handle is non-empty
    }
    catch(...) { //try genparticles 
      edm::LogVerbatim("CastorAnalyser") << " Cannot get genparticles " << std::endl;
    }
  }

  bool isAccessibleCasDigiInformation = false;
  bool isSaturationInThisEvent = false;
  bool isGoodDigiAboveNoiseInThisEvent = false; 
  double casCalibratedEnergy = 0;

  numBunches_ = 0;
  edm::Handle<CastorDigiCollection> castorDigis;
  try{

    //iEvent.getByType(castorDigis);
    //iEvent.getByLabel("castorDigis",castorDigis); 
#ifdef GET_BY_LABEL
    iEvent.getByLabel(_casDigiSrc, castorDigis);
#else
    iEvent.getByToken(myTokens_.get<0>(),castorDigis);
#endif

    edm::ESHandle<CastorDbService> conditions;
    iSetup.get<CastorDbRecord>().get(conditions);

    const CastorQIEShape* shape = conditions->getCastorShape();

    if(firstTime_)
      {

	ESHandle<CastorElectronicsMap> refEMap;
	iSetup.get<CastorElectronicsMapRcd>().get(refEMap);
	const CastorElectronicsMap* myRefEMap = refEMap.product();
	std::vector<HcalGenericDetId> listEMap = myRefEMap->allPrecisionId();

	for (std::vector<HcalGenericDetId>::const_iterator it = listEMap.begin(); it != listEMap.end(); it++)
	  {
	    HcalGenericDetId mygenid(it->rawId());
	    if(mygenid.isHcalCastorDetId())
	      {
		NewBunch a;
		HcalCastorDetId chanid(mygenid.rawId());
		a.detid = chanid;
		a.usedflag = false;
		std::string type;
		type = "CASTOR";
		for(int i = 0; i != 10; i++)
		  {
		    a.tsCapId[i] = 0;
		    a.tsAdc[i] = 0;
		    a.tsfC[i] = 0.0;
		  }
		Bunches_.push_back(a);
	      }
	  }

	firstTime_ = false;

      } // if(firstTime_)

    std::vector<NewBunch>::iterator bunch_it;
   
    ////////////////////////////////////treeVariables_.nbChan = 0;
 
    //
    int numBunches = 0;
    //
    int firstTS = firstTS_;
    int lastTS  = lastTS_;
    //
    //bool firstTimeHere = true;
    //
    for(CastorDigiCollection::const_iterator j = castorDigis->begin(); j != castorDigis->end(); j++)
      {
	const CastorDataFrame digi = (const CastorDataFrame)(*j);

        //
        isAccessibleCasDigiInformation = true;
	//
	//if ( lastTS+1 > digi.size() ) lastTS = digi.size()-1;
	//
	for(bunch_it = Bunches_.begin(); bunch_it != Bunches_.end(); bunch_it++)
	  if(bunch_it->detid.rawId() == digi.id().rawId()) break;
	bunch_it->usedflag = true;
	//
	numBunches++;
	//
	//Skip noisy channels present in 2009 beam data:
	//if ( (bunch_it->detid.sector() == 16 && bunch_it->detid.module() == 6)  ||
	//     (bunch_it->detid.sector() ==  3 && bunch_it->detid.module() == 8)  ||
	//     (bunch_it->detid.sector() ==  8 && bunch_it->detid.module() == 8) ) continue;
	//
        int secNb = bunch_it->detid.sector();
        int modNb = bunch_it->detid.module();
        //
        const CastorCalibrations& calibrations=conditions->getCastorCalibrations(digi.id().rawId());
        const CastorCalibrationWidths& calibrationWidths=conditions->getCastorCalibrationWidths(digi.id().rawId());
        //
        double tsSum = 0.0;
        double tsSumZS = noUseValue_;
	//int capId0 = digi.sample(0).capid(); //check of CapId cycles???
	if ( lastTS+1 > digi.size() ) lastTS = digi.size()-1;	
	//for(int ts = firstTS; ts != lastTS+1; ts++)
        double chMax = -9999.0;
        int tsMax = 0;
        bool digiSaturated = false;
        //
        treeVariables_.casDigiPresamples = digi.presamples(); //ntuple stores eventually the values for the last digi
        treeVariables_.casDigiSize = digi.size();
        //
        for(int ts = 0; ts != digi.size(); ts++)
	  {
	    if (ts < 10) {
	      //
	      //if (firstTimeHere) {
	      //	int itemp = (iEvent.bunchCrossing()+ts-1-digi.presamples());
	      //	if ( itemp < 0 ) itemp += 3563;
	      //treeVariables_.BXinTS[ts] =( itemp % 3563 ) + 1;
	      //	firstTimeHere = false;
	      //     }
	      //
	      const CastorQIECoder* coder = conditions->getCastorCoder(digi.id().rawId());
	      bunch_it->tsCapId[ts] = digi.sample(ts).capid();
	      bunch_it->tsAdc[ts] = digi.sample(ts).adc();
              if ( digi.sample(ts).adc() >= 127 ) {
		digiSaturated = true;
		isSaturationInThisEvent = true;
              }
	      double charge1 = coder->charge(*shape, digi.sample(ts).adc(), digi.sample(ts).capid()) -
		calibrations.pedestal(digi.sample(ts).capid());
	      //text file read by hand: //pedestalValue_[14*(secNb-1)+(modNb-1)][(ts%4+capId0)%4] ; //subtract pedestal here
	      bunch_it->tsfC[ts] = charge1;
              if (ts >= firstTS && ts <= lastTS) {
                tsSum += charge1;
                //if (charge1 > 3.0 * pedestalWidth_[14*(secNb-1)+(modNb-1)][(ts%4+capId0)%4]) { //above 3-pedestal-widths
		if ( charge1 > 3.0 * sqrt(calibrationWidths.pedestal(digi.sample(ts).capid())) ) {
                  if (tsSumZS < noUseValue_ + 1.0) tsSumZS = 0.0;  //sqrt(calibrationWidths.pedestal(digi.sample(ts).capid()))
                  tsSumZS += charge1;
                }
              }
              treeVariables_.digiPulseFC[14*(secNb-1)+(modNb-1)][ts] = charge1; //pedestal subtracted; indices???
              treeVariables_.digiPulseFCped[14*(secNb-1)+(modNb-1)][ts] = calibrations.pedestal(digi.sample(ts).capid());
              std::string hi_tag_pulsets_sect = Form("pulsets_sect_%d",bunch_it->detid.sector()-1);
              histos1D_[hi_tag_pulsets_sect]->Fill( static_cast<double>(10*(bunch_it->detid.module()-1)+ts), bunch_it->tsfC[ts] ); //h_pulsets_sect_[bunch_it->detid.sector()-1]
              //
              //h_ch_vs_ts_->Fill( static_cast<double>(ts),bunch_it->tsfC[ts] );
              if( bunch_it->tsfC[ts] > chMax ) {
                chMax = bunch_it->tsfC[ts]; 
                tsMax = ts;
              }
	      //
	      //treeVariables_.digiChargeSum += charge1; //fC, no pedestal subtraction
	      //if (bunch_it->detid.module() > 11 && bunch_it->detid.module() < 15 ) {
	      //	treeVariables_.digiChargeSum121314 += charge1;
	      //}
              
	    } //if (ts < 10)
	  } //ts for-loop
        if (castor::channelQuality[secNb-1][modNb-1]==true) { 
          double corrFactor = 0.015; //ForwardRecord::absCasEscaleFactor;
          if (isData) corrFactor = /*castor::corr0Tto38T[secNb-1][modNb-1]* */castor::channelGainQE[secNb-1][modNb-1] * ForwardRecord::absCasEscaleFactor;
          if (firstTimeAnalyse_ && _ShowDebug) edm::LogVerbatim("FTA") << "FTA" << secNb << "," << modNb << "," << corrFactor << std::endl; 
          casCalibratedEnergy += corrFactor * tsSum; 
          histosProfile_["h_casene_vs_iphi"]->Fill( static_cast<double>(secNb) , corrFactor * tsSum ); 
          histos1D_["h_goodch_vs_iphi"]->Fill( static_cast<double>(secNb));
          histosProfile_["h_casene_vs_iz"]->Fill( static_cast<double>(modNb) , corrFactor * tsSum );
          histos1D_["h_goodch_vs_iz"]->Fill( static_cast<double>(modNb));
        } else {
          if (firstTimeAnalyse_ && _ShowDebug) edm::LogVerbatim("FTA") << "FTA " << secNb << "," << modNb << "," << "NoNum" << std::endl;
          if (firstTimeAnalyse_ && _ShowDebug)  { 
            if(bunch_it->detid.module()<3) edm::LogVerbatim("FTA") << "FTA " << " " <<  bunch_it->detid.zside() << " " << bunch_it->detid.sector() << " " << bunch_it->detid.module() << " CASTOR_EM 1" << std::hex << std::uppercase << bunch_it->detid.rawId() << std::dec << std::endl; 
            if(bunch_it->detid.module()>2) edm::LogVerbatim("FTA") << "FTA " << " " <<  bunch_it->detid.zside() << " " << bunch_it->detid.sector() << " " << bunch_it->detid.module() << " CASTOR_HAD 1 " << std::hex << std::uppercase << bunch_it->detid.rawId() << std::dec << std::endl;
          }
        }
        histos2D_["h_ch_vs_ts"]->Fill( static_cast<double>(tsMax), chMax );
        double rh_time_ns = -noUseValue_*noUseValue_;
        double rh_time_ns_corr = -noUseValue_*noUseValue_;
        double ampmax  = bunch_it->tsfC[tsMax];
        double ampprev = 0.0; //if max is at the edge than set prev/next to zero
        double ampnext  = 0.0;
        if (tsMax > 0) {
	  ampprev = bunch_it->tsfC[tsMax-1];
        }
	if (tsMax < (digi.size()-1)) {
	  ampnext = bunch_it->tsfC[tsMax+1];
	}
	if (ampprev + ampmax + ampnext > 0.0) {
	  double wpksamp = (ampmax + 2.0 * ampnext) / (ampprev + ampmax + ampnext);
          rh_time_ns = ( tsMax - digi.presamples() + wpksamp)*25.0;
          rh_time_ns_corr = ( tsMax - digi.presamples())*25.0 + timeshift_ns_hf(wpksamp);
        }
        if (tsSumZS > noUseValue_ + 1.0 && castor::channelQuality[secNb-1][modNb-1]) { //if at least one time slice above noise and good quality
          histos1D_["h_hit_time"]->Fill(rh_time_ns); //check and exclude saturation as well?
        }
        if (14*(secNb-1)+(modNb-1) >= 0 && 14*(secNb-1)+(modNb-1) < 224) {
	  treeVariables_.digiChargeSum[14*(secNb-1)+(modNb-1)] = tsSum; 
          treeVariables_.digiTime[14*(secNb-1)+(modNb-1)] = rh_time_ns;
          treeVariables_.digiTime1[14*(secNb-1)+(modNb-1)] = rh_time_ns_corr;
	  treeVariables_.digiSector[14*(secNb-1)+(modNb-1)] = secNb;
	  treeVariables_.digiModule[14*(secNb-1)+(modNb-1)] = modNb;
          treeVariables_.digiCapIdTs0[14*(secNb-1)+(modNb-1)] = digi.sample(0).capid();
          treeVariables_.digiAboveNoise[14*(secNb-1)+(modNb-1)] = 0;
          if (tsSumZS > noUseValue_) { 
	    treeVariables_.digiAboveNoise[14*(secNb-1)+(modNb-1)] = 1;
	    if(castor::channelQuality[secNb-1][modNb-1]==true) isGoodDigiAboveNoiseInThisEvent = true;
          }
          treeVariables_.digiSaturated[14*(secNb-1)+(modNb-1)] = 0;
          if (digiSaturated==true) treeVariables_.digiSaturated[14*(secNb-1)+(modNb-1)] = 1;
          treeVariables_.digiMarkedBad[14*(secNb-1)+(modNb-1)] = 0;
          if (castor::channelQuality[secNb-1][modNb-1]==false) treeVariables_.digiMarkedBad[14*(secNb-1)+(modNb-1)] = 1;
          //////
          std::string hi_tag_h_charge = Form("charge_m%ds%d", modNb, secNb);
          histos1D_[hi_tag_h_charge]->Fill(tsSum,1.0/GetfCBinWidth(tsSum)); //h_charge_[14*(secNb-1)+(modNb-1)]
          std::string hi_tag_h_hit_time_ch = Form("hit_time_m%ds%d", modNb, secNb);
          histos1D_[hi_tag_h_hit_time_ch]->Fill(rh_time_ns); //h_hit_time_ch_[14*(secNb-1)+(modNb-1)]
          //////
	}
	if ( 
	    bunch_it->detid.sector() > 0 && bunch_it->detid.sector() < 17 &&
	    bunch_it->detid.module() > 0 && bunch_it->detid.module() < 15){
	  if (tsSumZS > noUseValue_ + 1.0) {
	    histos2D_["h_energyzs_map"]->Fill(static_cast<double>(bunch_it->detid.module()),
					      static_cast<double>(bunch_it->detid.sector()),tsSumZS);
	    histos2D_["h_occup_map"]->Fill(static_cast<double>(bunch_it->detid.module()),
					   static_cast<double>(bunch_it->detid.sector()));
	  }
	  histos2D_["h_energysum_map"]->Fill(static_cast<double>(bunch_it->detid.module()),
					     static_cast<double>(bunch_it->detid.sector()),tsSum);
	  histos2D_["h_energysqs_map"]->Fill(static_cast<double>(bunch_it->detid.module()),
					     static_cast<double>(bunch_it->detid.sector()),tsSum*tsSum);
        }
      } //digi for-loop
    treeVariables_.nbChan = numBunches;
    if (_ShowDebug) edm::LogVerbatim("CastorDigis") << " CastorDigi size: " << numBunches << std::endl;
    numBunches_ = numBunches;
    treeVariables_.casEnergy = casCalibratedEnergy;
    histosProfile_["h_casene_vs_bx"]->Fill(iEvent.bunchCrossing(),casCalibratedEnergy);
    if (isSaturationInThisEvent == false && isGoodDigiAboveNoiseInThisEvent == true) histosProfile_["h_casene_vs_bx_sel"]->Fill(iEvent.bunchCrossing(),casCalibratedEnergy);

  } //try castordigis
  catch(...) { //try castordigis

    edm::LogVerbatim("CastorAnalyser") << " Cannot get Castor digis" << std::endl; //LogError //std::cout //LogInfo

  }
 
  if (isAccessibleVtxInformation == true) {
    treeVariables_.vtxNb = vtxNo;
    treeVariables_.vtxNbNotFake = vtxNoNotFake;
    treeVariables_.vtxTrkNb = vtxTrkNo;
    treeVariables_.vtxIsFake0 = vtxIsFake0;
    treeVariables_.vtxRho = vtxRho;
    treeVariables_.vtxZ = vtxZ;
    treeVariables_.vtxZerr = vtxZerr;
    treeVariables_.vtxY = vtxY;
    treeVariables_.vtxX = vtxX;
  }

  if (isAccessibleTrkInformation == true) {
    treeVariables_.trkNb = trkNo;
    treeVariables_.trkLeadingPt = trkLeadingPt;
    treeVariables_.trkLeadingEta = trkLeadingEta;
    treeVariables_.trkLeadingPhi = trkLeadingPhi;
    treeVariables_.trkLeadingNbValidPixelHits = trkLeadingNoValidPixelHits;
    treeVariables_.trkLeadingNbValidStripHits = trkLeadingNoValidStripHits; 
    treeVariables_.trkHighNb = trkHighNo;
    treeVariables_.trkEtaMax = trkEtaMax;
    treeVariables_.trkEtaMin = trkEtaMin;
  }

  if (isAccessibleHFRHInformation == true) {
    if (isAccessibleCasDigiInformation == true) histos2D_["h_hfme_vs_case"]->Fill(casCalibratedEnergy,hfmE); 
    treeVariables_.hfNb = hfNo;
    treeVariables_.hfmNb = hfmNo;
    treeVariables_.hfmNb4 = hfmNo4; //TODO: add also hfmNo5,6,7 and alike to do cuts on them as in RHAnalyser
    treeVariables_.hfpNb4 = hfpNo4;
    treeVariables_.hfmNb5 = hfmNo5; 
    treeVariables_.hfpNb5 = hfpNo5;
    treeVariables_.hfmNb6 = hfmNo6; 
    treeVariables_.hfpNb6 = hfpNo6;
    treeVariables_.hfmE = hfmE;
    treeVariables_.hfmE4all1 = hfmE4all1;     
    treeVariables_.hfmE4long1 = hfmE4long1;
    treeVariables_.hfmE4short1 = hfmE4short1;
    treeVariables_.hfmE4all2 = hfmE4all2;
    treeVariables_.hfmE4long2 = hfmE4long2;
    treeVariables_.hfmE4short2 = hfmE4short2;
    treeVariables_.hfmLeadingE = hfmLeadingE;
    treeVariables_.hfmLeadingT = hfmLeadingT;
    treeVariables_.hfpE = hfpE;
  }

  if (isAccessibleHBHERHInformation == true) {
    if (isAccessibleCasDigiInformation == true) histos2D_["h_hbheme_vs_case"]->Fill(casCalibratedEnergy,hbhemE);
    treeVariables_.hbheNb = hbheNo;
    treeVariables_.hbhemNb = hbhemNo;
    treeVariables_.hbhemE = hbhemE;
  }

  if (isAccessibleEBRHInformation == true) {
    treeVariables_.ebNb = ebNo;
    treeVariables_.ebmNb = ebmNo;
    treeVariables_.ebmE = ebmE;
  }

  if (isAccessibleEERHInformation == true) {
    treeVariables_.eeNb = eeNo;
    treeVariables_.eemNb = eemNo;
    treeVariables_.eemE = eemE;
  }

  if (isAccessibleCasRHInformation == true) {
    treeVariables_.casNb = casRHNo;
    treeVariables_.casEnergyRH = casRHE;
    treeVariables_.casLeadingRHE = casLeadRHE;
    treeVariables_.casLeadingRHT = casLeadRHT;
  }

  if (isAccessibleCasTowInformation == true) {
    treeVariables_.casTowNb = casTowNo;
    treeVariables_.casTowE = casTowE;
    treeVariables_.casTowEt = casTowEt;
  }

  if (isAccessibleCasJetInformation == true) {
    treeVariables_.casJetNb = casJetNo;
    treeVariables_.casLeadingJetE = casLeadingJetE;
  }

  if (isAccessiblePFCluEcalInformation == true) {
    treeVariables_.pfCluEcalNb = pfCluEcalNo;
    treeVariables_.pfLeadingCluEcalE = pfLeadingCluEcalE;
  }

  if (isAccessiblePFCluHcalInformation == true) {
    treeVariables_.pfCluHcalNb = pfCluHcalNo;
    treeVariables_.pfLeadingCluHcalE = pfLeadingCluHcalE;
  }

  if (isAccessiblePFCluHFInformation == true) {
    treeVariables_.pfCluHFNb = pfCluHFNo;
    treeVariables_.pfLeadingCluHFE = pfLeadingCluHFE;
    treeVariables_.pfCluEtaMax = pfCluEtaMax;
    treeVariables_.pfCluEtaMin = pfCluEtaMin;
  }

  if (isAccessiblePFCandInformation == true) {
    treeVariables_.pfCandNb = pfCandNo;
    treeVariables_.pfLeadingCandE = pfLeadingCandE;
    treeVariables_.pfCandEtaMax = pfCandEtaMax;
    treeVariables_.pfCandEtaMin = pfCandEtaMin;
  }

  if (isAccessiblePFJetInformation == true) {
    treeVariables_.pfJetNb = pfJetNo;
    treeVariables_.pfLeadingJetE = pfLeadingJetE;
  }

  if (isAccessibleCaloTowerInformation == true) { 
    treeVariables_.caloTowerNb = caloTowerNo;
    treeVariables_.caloTowerBSCmNb = caloTowerBSCmNo;
    treeVariables_.caloTowerBSCpNb = caloTowerBSCpNo;
    treeVariables_.caloTowerBSC1mNb = caloTowerBSC1mNo;
    treeVariables_.caloTowerBSC1pNb = caloTowerBSC1pNo;
    treeVariables_.caloTowerHFmNb = caloTowerHFmNo;
    treeVariables_.caloTowerHFmNb4 = caloTowerHFmNo4;
    treeVariables_.caloTowerHFpNb4 = caloTowerHFpNo4;
    treeVariables_.caloTowerHFmNb5 = caloTowerHFmNo5;
    treeVariables_.caloTowerHFpNb5 = caloTowerHFpNo5;
    treeVariables_.caloTowerHFmNb6 = caloTowerHFmNo6;
    treeVariables_.caloTowerHFpNb6 = caloTowerHFpNo6;
    treeVariables_.caloLeadingTowerHFE = caloLeadingTowerHFE;
    treeVariables_.caloLeadingTowerieta = caloLeadingTowerieta;
    treeVariables_.caloLeadingToweriphi = caloLeadingToweriphi;
    histos2D_["tow_leadingoccup_map"]->Fill(static_cast<double>(caloLeadingTowerieta),static_cast<double>(caloLeadingToweriphi));
    histos2D_["tow_leadingoccup_map1"]->Fill(caloLeadingTowereta,caloLeadingTowerphi);
    treeVariables_.caloTowerEtaMax = caloTowerEtaMax;
    treeVariables_.caloTowerEtaMin = caloTowerEtaMin;
    if (xiSDdet > 0.0) { treeVariables_.log10xiSDdet = log10(xiSDdet); }
    if (xiDDdet > 0.0) { treeVariables_.log10xiDDdet = log10(xiDDdet); }
    treeVariables_.deltaRapMaxdet = deltaRapMaxdet;
  }

  if (isAccessibleGenPartInformation == true) {
    treeVariables_.nbGenPart = noGenPart;
    treeVariables_.nbStat1GenPart = noStat1GenPart;
    treeVariables_.nbStat1ChPt250Eta2p5 = noStat1ChPt250Eta2p5;
    treeVariables_.nbStat1ChEtap1p94p9 = noStat1ChEtap1p94p9;
    treeVariables_.nbStat1ChEtam3p51p5 = noStat1ChEtam3p51p5;
    treeVariables_.nbStat1GenPartInBSCm = noStat1GenPartInBSCm;
    treeVariables_.nbStat1GenPartInBSCp = noStat1GenPartInBSCp;
    treeVariables_.nbStat1GenPartInBSC1m = noStat1GenPartInBSC1m;
    treeVariables_.nbStat1GenPartInBSC1p = noStat1GenPartInBSC1p;
    // assume init with noUseValue_ already done;
    if (xiSD > 0.0) { treeVariables_.log10xiSD = log10(xiSD); }
    // assume init with noUseValue_ already done;
    if (xiDD > 0.0) { treeVariables_.log10xiDD = log10(xiDD); }
    treeVariables_.genPartCmEnergy = genPartCmEnergy;
    treeVariables_.genPartEtaMax = genPartEtaMax;
    treeVariables_.genPartEtaMin = genPartEtaMin;
    treeVariables_.genPartDeltaRapMax = genPartDeltaRapMax;
  }

  treeVariables_.nbEtaIntervals = ForwardRecord::nbEtaBins;
  treeVariables_.nbEtaIntervalsExt = ForwardRecord::nbEtaBinsExt;
  if (isAccessibleCasDigiInformation){ 
    etaBinEnergies[0] = casCalibratedEnergy; etaBinEts[0] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    etaBinEnergies1[0] = casCalibratedEnergy; etaBinEts1[0] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    etaBinEnergies2[0] = casCalibratedEnergy; etaBinEts2[0] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    //for genParticles hopefully there is no need to fill the 1st and last bins "by hand"
    etaBinEnergies4[0] = casCalibratedEnergy; etaBinEts4[0] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    //
    etaBinEts10[0] = casCalibratedEnergy/cosh(CASTOR_ETA);
    //
    etaBinEnergies[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy; etaBinEts[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    etaBinEnergies1[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy; etaBinEts1[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    etaBinEnergies2[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy; etaBinEts2[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    //for genParticles hopefully there is no need to fill the 1st and last bins "by hand"
    etaBinEnergies4[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy; etaBinEts4[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy/cosh(CASTOR_ETA); //5.9
    //
    etaBinEts10[ForwardRecord::nbEtaBins-1] = casCalibratedEnergy/cosh(CASTOR_ETA);
  }
  if (!isAccessibleCasDigiInformation && isAccessibleCasRHInformation) { 
    etaBinEnergies[0] = casRHE; etaBinEts[0] = casRHE/cosh(CASTOR_ETA); //5.9
    etaBinEnergies1[0] = casRHE; etaBinEts1[0] = casRHE/cosh(CASTOR_ETA); //5.9
    etaBinEnergies2[0] = casRHE; etaBinEts2[0] = casRHE/cosh(CASTOR_ETA); //5.9
    //for genParticles hopefully there is no need to fill the 1st and last bins "by hand"
    etaBinEnergies4[0] = casRHE; etaBinEts4[0] = casRHE/cosh(CASTOR_ETA); //5.9
    //
    etaBinEts10[0] = casRHE/cosh(CASTOR_ETA);
    //
    etaBinEnergies[ForwardRecord::nbEtaBins-1] = casRHE; etaBinEts[ForwardRecord::nbEtaBins-1] = casRHE/cosh(CASTOR_ETA); //5.9
    etaBinEnergies1[ForwardRecord::nbEtaBins-1] = casRHE; etaBinEts1[ForwardRecord::nbEtaBins-1] = casRHE/cosh(CASTOR_ETA); //5.9
    etaBinEnergies2[ForwardRecord::nbEtaBins-1] = casRHE; etaBinEts2[ForwardRecord::nbEtaBins-1] = casRHE/cosh(CASTOR_ETA); //5.9
    //for genParticles hopefully there is no need to fill the 1st and last bins "by hand"
    etaBinEnergies4[ForwardRecord::nbEtaBins-1] = casRHE; etaBinEts4[ForwardRecord::nbEtaBins-1] = casRHE/cosh(CASTOR_ETA); //5.9
    //
    etaBinEts10[ForwardRecord::nbEtaBins-1] = casRHE/cosh(CASTOR_ETA);
  }
  if (isAccessibleCasTowInformation) {
    etaBinEnergies5[0] = casTowE; etaBinEts5[0] = casTowEt;
    etaBinEnergies5[ForwardRecord::nbEtaBins-1] = casTowE; etaBinEts5[ForwardRecord::nbEtaBins-1] = casTowEt;//casRHE/cosh(5.9); //unlike other "etaBinEnergies" use here castorTowers (i.e some thresholds)
  } 
  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBins; ibin++) {
    treeVariables_.energyInEtaIntervals[ibin] = etaBinEnergies[ibin];
    treeVariables_.energyInEtaIntervals1[ibin] = etaBinEnergies1[ibin];
    treeVariables_.energyInEtaIntervals2[ibin] = etaBinEnergies2[ibin];
    treeVariables_.energyInEtaIntervals3[ibin] = etaBinEnergies3[ibin];
    treeVariables_.energyInEtaIntervals4[ibin] = etaBinEnergies4[ibin];
    treeVariables_.energyInEtaIntervals5[ibin] = etaBinEnergies5[ibin];
//
    treeVariables_.energyInEtaIntervals6[ibin] = etaBinEnergies6[ibin];
    treeVariables_.energyInEtaIntervals7[ibin] = etaBinEnergies7[ibin];
    treeVariables_.energyInEtaIntervals8[ibin] = etaBinEnergies8[ibin];
    treeVariables_.energyInEtaIntervals9[ibin] = etaBinEnergies9[ibin];
//
    treeVariables_.etInEtaIntervals[ibin] = etaBinEts[ibin]; 
    treeVariables_.etInEtaIntervals10[ibin] = etaBinEts10[ibin];
//
    double width = std::abs(ForwardRecord::Eta_Bin_Edges[ibin+1]-ForwardRecord::Eta_Bin_Edges[ibin]);
    double bincenter = (ForwardRecord::Eta_Bin_Edges[ibin+1]+ForwardRecord::Eta_Bin_Edges[ibin])/2.0;
    if (width > 1e-6) { histosProfile_["energy_vs_eta_reco"]->Fill(bincenter,etaBinEts[ibin]/width); histosProfile_["energy_vs_eta_reco1"]->Fill(bincenter,etaBinEts10[ibin]/width); }
//
    treeVariables_.etInEtaIntervals1[ibin] = etaBinEts1[ibin];
    treeVariables_.etInEtaIntervals2[ibin] = etaBinEts2[ibin];
    treeVariables_.etInEtaIntervals3[ibin] = etaBinEts3[ibin];
    treeVariables_.etInEtaIntervals33[ibin] = etaBinEts33[ibin]; 
    treeVariables_.etInEtaIntervals4[ibin] = etaBinEts4[ibin];
    treeVariables_.etInEtaIntervals5[ibin] = etaBinEts5[ibin];
  }
  for(uint ibin = 0; ibin < ForwardRecord::nbEtaBinsExt; ibin++) {
    treeVariables_.etInEtaIntervals333[ibin] = etaBinEts333[ibin];
    double width = std::abs(ForwardRecord::Eta_Bin_EdgesExt[ibin+1]-ForwardRecord::Eta_Bin_EdgesExt[ibin]);
    double bincenter = (ForwardRecord::Eta_Bin_EdgesExt[ibin+1]+ForwardRecord::Eta_Bin_EdgesExt[ibin])/2.0;
    if (width > 1e-6) { histosProfile_["energy_vs_eta_reco2"]->Fill(bincenter,etaBinEts333[ibin]/width); }
  }

  tree_->Fill();

  iEvt_++;
  firstTimeAnalyse_ = false;

  if ( _ShowDebug ) {
    if(isPFJet20Found == true && isCastorJet3000Found == true) {
      edm::LogVerbatim("FTA") << "FTA " << " CasJet&&PFJet -- run :" << iEvent.id().run() << " event: " << iEvent.id().event() << " ls: " << iEvent.luminosityBlock() << std::endl;  
    }
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
CastorAnalyser::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CastorAnalyser::endJob() {
}

void CastorAnalyser::SetPedestals(int runNo, std::string pathString)
{
  char pedFileName[200];
  sprintf(pedFileName,"%s/src/CalibCalorimetry/CastorCalib/test/%d_pedallfC.txt",pathString.c_str(),runNo);
  //sprintf(pedFileName,"/afs/cern.ch/user/k/katkov/scratch0/lhc_beams/CMSSW_3_5_8/%s/%d_pedall.txt",pathString.c_str(),runNo);
  //sprintf(pedFileName,"/usr/users/ikatkov/CMSSW_3_8_6/%s/%d_pedall.txt",pathString.c_str(),runNo);

  for (int ich=0; ich<224; ich++){
    for (int icap=0; icap<4; icap++){
      pedestalValue_[ich][icap]=0.0;
      pedestalWidth_[ich][icap]=0.0;
    }
  }

  std::ifstream in0;
  in0.open(pedFileName);
  int chId=-1;
  float pedVal[9]={-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
  while (1) {
    in0 >> pedVal[0] >> pedVal[1] >> pedVal[2] >> pedVal[3] >> pedVal[4] >> pedVal[5] >> pedVal[6] >> pedVal[7] >> pedVal[8];
    if ( !in0.good() ) {
      edm::LogVerbatim("CastorAnalyser") << std::endl << "!!!Failing to read array pedestal!!!" << std::endl << std::endl;  //std::cout //LogInfo
      //now trying also to read conditions in a normal way but from a text file
      break;
    }
    edm::LogVerbatim("CastorAnalyser") <<"!!!"<< pedVal[0] <<" "<< pedVal[2] <<" "<< pedVal[4] <<" "<< pedVal[6] << std::endl; //std::cout //LogInfo
    chId = static_cast<int>(pedVal[0]);
    if ( chId >=0 && chId < 224 ) {
      for (int ic = 0; ic < 4; ic++) {
        pedestalValue_[chId][ic] = pedVal[ic+1];
        pedestalWidth_[chId][ic] = pedVal[ic+5];
      }
    }
  };
  in0.close();

}

float CastorAnalyser::GetNonInvConv2fC(int iQIE){

  /*  
      float //DCCData::gl_noninv_conversionTable[128] = { //HTBDAQ_data -> src/DCCData.cc
      conversionTable[128] = {
      -1.3f,   1.3f,   3.9f,   6.5f,   9.1f,  11.7f,  14.3f,  16.9f,
      19.5f,  22.1f,  24.7f,  27.3f,  29.9f,  32.5f,  35.1f,  39.0f,
      44.2f,  49.4f,  54.6f,  59.8f,  65.0f,  70.2f,  76.7f,  84.5f,
      92.3f, 100.1f, 109.2f, 119.6f, 130.0f, 141.7f, 154.7f, 167.7f,
      154.7f, 167.7f, 180.7f, 193.7f, 206.7f, 219.7f, 232.7f, 245.7f,
      258.7f, 271.7f, 284.7f, 297.7f, 310.7f, 323.7f, 336.7f, 356.2f,
      382.2f, 408.2f, 434.2f, 460.2f, 486.2f, 512.2f, 544.7f, 583.7f,
      622.7f, 661.7f, 707.2f, 759.2f, 811.2f, 869.7f, 934.7f, 999.7f,
      934.7f, 999.7f,1064.7f,1129.7f,1194.7f,1259.7f,1324.7f,1389.7f,
      1454.7f,1519.7f,1584.7f,1649.7f,1714.7f,1779.7f,1844.7f,1942.2f,
      2072.2f,2202.2f,2332.2f,2462.2f,2592.2f,2722.2f,2884.7f,3079.7f,
      3274.7f,3469.7f,3697.2f,3957.2f,4217.2f,4509.7f,4834.7f,5159.7f,
      4834.7f,5159.7f,5484.7f,5809.7f,6134.7f,6459.7f,6784.7f,7109.7f,
      7434.7f,7759.7f,8084.7f,8409.7f,8734.7f,9059.7f,9384.7f,9872.2f,
      10522.2f,11172.2f,11822.2f,12472.2f,13122.2f,13772.2f,14584.7f,15559.7f,
      16534.7f,17509.7f,18647.2f,19947.2f,21247.2f,22709.7f,24334.7f,25959.7f};
  */

  float conversionTable[128] = {
    -1.35135f,
    1.35135f,
    4.05405f,
    6.75676f,
    9.45946f,
    12.1622f,
    14.8649f,
    17.5676f,
    20.2703f,
    22.973f,
    25.6757f,
    28.3784f,
    31.0811f,
    33.7838f,
    36.4865f,
    40.5405f,
    45.9459f,
    51.3514f,
    56.7568f,
    62.1622f,
    67.5676f,
    72.973f,
    79.7297f,
    87.8378f,
    95.9459f,
    104.054f,
    113.514f,
    124.324f,
    135.135f,
    147.297f,
    160.811f,
    174.324f,
    160.811f,
    174.324f,
    187.838f,
    201.351f,
    214.865f,
    228.378f,
    241.892f,
    255.405f,
    268.919f,
    282.432f,
    295.946f,
    309.459f,
    322.973f,
    336.486f,
    350.0f,
    370.27f,
    397.297f,
    424.324f,
    451.351f,
    478.378f,
    505.405f,
    532.432f,
    566.216f,
    606.757f,
    647.297f,
    687.838f,
    735.135f,
    789.189f,
    843.243f,
    904.054f,
    971.622f,
    1039.19f,
    971.622f,
    1039.19f,
    1106.76f,
    1174.32f,
    1241.89f,
    1309.46f,
    1377.03f,
    1444.59f,
    1512.16f,
    1579.73f,
    1647.3f,
    1714.86f,
    1782.43f,
    1850.0f,
    1917.57f,
    2018.92f,
    2154.05f,
    2289.19f,
    2424.32f,
    2559.46f,
    2694.59f,
    2829.73f,
    2998.65f,
    3201.35f,
    3404.05f,
    3606.76f,
    3843.24f,
    4113.51f,
    4383.78f,
    4687.84f,
    5025.68f,
    5363.51f,
    5025.68f,
    5363.51f,
    5701.35f,
    6039.19f,
    6377.03f,
    6714.86f,
    7052.7f,
    7390.54f,
    7728.38f,
    8066.22f,
    8404.05f,
    8741.89f,
    9079.73f,
    9417.57f,
    9755.41f,
    10262.2f,
    10937.8f,
    11613.5f,
    12289.2f,
    12964.9f,
    13640.5f,
    14316.2f,
    15160.8f,
    16174.3f,
    17187.8f,
    18201.4f,
    19383.8f,
    20735.1f,
    22086.5f,
    23606.8f,
    25295.9f,
    26985.1f
  };

  if ( iQIE >=0 && iQIE < 128) {
    return conversionTable[iQIE];
  } else {
    return -1;
  }

}

void CastorAnalyser::SetNonInvfCBins() {

  int nShift = 12;

  fCBinEdges_[0+nShift]=-2.6;
  int nedge = 2;
  fCBinEdges_[1+nShift]=2.0*GetNonInvConv2fC(0)-fCBinEdges_[0+nShift]; //GetNonInvConv2fC defines bin middles
  float lastbinmid = GetNonInvConv2fC(0);
  for (int i = 2; i <= 128; i++){
    if ( GetNonInvConv2fC(i-1) <= lastbinmid ) continue;
    fCBinEdges_[nedge+nShift]=2.0*GetNonInvConv2fC(i-1)-fCBinEdges_[nedge-1+nShift];
    if (i > 2 && nShift + 2 - i >= 0) fCBinEdges_[nShift + 2 - i] = - fCBinEdges_[nedge+nShift];
    lastbinmid = GetNonInvConv2fC(i-1);
    edm::LogVerbatim("CastorAnalyser") << i <<" nedge: "<< nedge <<" binmid: "<< GetNonInvConv2fC(i-1) <<" binedge: "<< fCBinEdges_[nedge+nShift] << std::endl; //std::cout //LogInfo
    nedge++;      
  };
  nfCBins_ = (nedge-1)+nShift;

  //add more bins of the same width at the end
  int nBinsEnd = 289 - nfCBins_;                                            ////////////////////////////////////144
  if (nfCBins_ + nBinsEnd  <= 289) {                                        ////////////////////////////////////144
    for (int i1 = 1; i1 <= nBinsEnd; i1++) {
      fCBinEdges_[nfCBins_ + i1] = fCBinEdges_[nfCBins_ + i1 - 1] + 
        ( fCBinEdges_[nfCBins_] - fCBinEdges_[nfCBins_-1] );
        
    }
    nfCBins_ = nfCBins_ + nBinsEnd;
  }

}

float CastorAnalyser::GetfCBinWidth(float signal) {

  for ( int i = 1; i < nfCBins_+1; i++ ) {
      
    if (  fCBinEdges_[i] > signal ) {
      float width = TMath::Abs(fCBinEdges_[i]-fCBinEdges_[i-1]); //Positive width!!!
      return width;
    }
      
  }    

  return -1.0;

}

//https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_7_4_X/RecoLocalCalo/CastorReco/src/CastorSimpleRecAlgo.cc
float CastorAnalyser::timeshift_ns_hf(float wpksamp) {
  float flx = (castor::num_bins_hf*(wpksamp - castor::wpksamp0_hf)/castor::scale_hf);
  int index = (int)flx;
  float yval;
  
  if      (index <    0)        return castor::actual_ns_hf[0];
  else if (index >= castor::num_bins_hf-1) return castor::actual_ns_hf[castor::num_bins_hf-1];

  // else interpolate:
  float y1       = castor::actual_ns_hf[index];
  float y2       = castor::actual_ns_hf[index+1];

  // float delta_x  = 1/(float)castor::num_bins_hf;
  // yval = y1 + (y2-y1)*(flx-(float)index)/delta_x;

  yval = y1 + (y2-y1)*(flx-(float)index);
  return yval;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CastorAnalyser);
