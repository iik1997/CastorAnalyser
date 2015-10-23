#!/bin/bash

echo "Didn't you forget to re-compile?"
echo "DATA or MC: THIS_IS_MC ??"
echo "DO_RECHITS? GET_BY_LABEL? CM_ENERGY? CASTOR_ETA?"
echo "ecalScaleFactor, hbheScaleFactor, hfScaleFaxtor?"

##cmsRun castor_locped_cfg_argpedrun.py
#
echo " "
pwd
##cp -f argpedrun-peds_fC.txt ../src/peds_fC_run_argpedrun.txt
ls -l ${CMSSW_BASE}/src/peds_fC_run_argpedrun.txt
echo " "
#
cmsRun castor_gloraw_cfg_argledrunargsuf.py

