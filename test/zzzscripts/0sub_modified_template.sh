#!/usr/bin/env bash
#/afs/cern.ch/user/k/katkov/scratch0/lhc_beams/starlight_hepforge_out_convert/zsubmit/ralf_sub2root.sh
#/afs/cern.ch/user/k/katkov/afswork/hiforest/CMSSW_5_3_8_HI_patch2/src/cbaus/hiforest_analysis/000BATCH

CPATH=/afs/cern.ch/work/k/katkov/hiforest/Tmp/CMSSW_7_4_2/src
CWORK=/afs/cern.ch/work/k/katkov/hiforest/Tmp/CMSSW_7_4_2/test

RUNSCRI=job_runargledrunargsuf.sh


CFGFILE1=castor_locped_cfg_argpedrun.py
#castor_locped_cfg_239028.py

CFGFILE2=castor_gloraw_cfg_argledrunargsuf.py

OUTFILE1=argledrunargsuf_detailedInfo01.txt

OUTFILE2=argledrun_raw_argpedrun_ped_argcmssw_arghv_argsuf.root

cd ${CPATH}
export SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`
uname -a
#go back to pool area:
cd -

#xrootd
export X509_USER_PROXY=/afs/cern.ch/user/k/katkov/x509up_u27806
echo "X509_USER_PROXY: ${X509_USER_PROXY}"

ENDPATH=job`date --rfc-3339=seconds | sed 's|\ ||g'`
mkdir ${CWORK}/${ENDPATH}
echo "\n ${ENDPATH} \n" >> ${CWORK}/000BATCH

cp ${CWORK}/${RUNSCRI} ${CWORK}/${CFGFILE1} ${CWORK}/${CFGFILE2} .
pwd
echo ./${RUNSCRI} 
sleep 5
./${RUNSCRI}
ls -ltr
sleep 5
cp ${CWORK}/${RUNSCRI} ${CWORK}/${CFGFILE1} ${CWORK}/${CFGFILE2} ${CWORK}/${ENDPATH}
mv ./${OUTFILE1} ${CWORK}/${OUTFILE2} ${CWORK}/${ENDPATH}
#echo /afs/cern.ch/project/eos/installation/0.2.5/bin/eos.select cp ${OUTPUTDIR}/${OUTPUT}.root /eos/cms/store/group/phys_heavyions/katkov/starlight/starlight_pythia_pPb/${OUTPUT}.root
#rm -f ${OUTPUTDIR}/${OUTPUT}.root
