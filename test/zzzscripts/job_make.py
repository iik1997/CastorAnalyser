#!/usr/bin/env python
#
# ./job_make.py 229479 229489 730p2 1700900 ----------->
#
# ./job_make.py 239028 239754 \'/store/data/Commissioning2015/MinimumBias/RAW/v1/000/239/754/00000/94E8E718-75DB-E411-9D0E-02163E0123CC.root\',\'/store/data/Commissioning2015/MinimumBias/RAW/v1/000/239/754/00000/94E8E718-75DB-E411-9D0E-02163E0123CC.root\' 730p2 test_firstEQ_1
#
# ./job_make.py 239028 243671 \'/store/express/Commissioning2015/ExpressPhysics/FEVT/Express-v1/000/243/671/00000/187F9991-35F4-E411-B86A-02163E01268A.root\' 730p2 1200x4_1600x10
#
# ./job_make.py 239028 TuneMBRp8 \'/store/mc/RunIISpring15DR74/MinBias_TuneMBR_13TeV-pythia8/GEN-SIM-RECO/NoPU0TRawReco_magnetOff_MCRUN2_740TV0-v1/00000/0AB11284-FD0F-E511-87DD-0002C90B7472.root\' 742 magnetOff
#
# ./job_make.py 239028 247324 \'/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/247/324/00000/005C8F6F-1A0D-E511-A77D-02163E0142BC.root\' 742 1200x4_1600x10 ZB1zbp0ls97to291v0a
#
# ./job_make.py 239028 255031 \'\' 742 nocastor ZB1zbp0ls47to49v0i
#
#
# ./job_make.py none TuneMBRp8 \'\' 742 mc magnetOff200kv0k
# ./job_make.py 239028 247324 \'\' 7414 1200x4_1600x10_v0 ZB1zbp0ls97to291v0k
# ./job_make.py 239028 247324 \'\' 7414 1200x4_1600x10_v0 BXQtt7ls97to291v0k
# ./job_make.py 239028 262172 \'\' 754 1200x4_1600x10_v0 FEVTzbv2ls50to160
#

import sys, math, re, subprocess, commands, os

def main():

    usage = 'Usage: %s argpedrun/171704 argrawrun/171702 arglistofrawfiles/root,root argcmssw/386 arghv/HighGain_38T/eq_1st argsuf/ZB1zbp0ls230to241v3' % sys.argv[0]

    try:
        argpedrun = sys.argv[1]
        argledrun = sys.argv[2]
        arglistofrawfiles = sys.argv[3]
        argcmssw  = sys.argv[4]
        arghv     = sys.argv[5]
        argsuf    = sys.argv[6]
    except:
        print usage; sys.exit(1)

    dic = {'argpedrun':argpedrun, 'argledrun':argledrun, 'arglistofrawfiles':arglistofrawfiles, 'argcmssw':argcmssw, 'arghv':arghv, 'argsuf':argsuf}

    f1 = open('./castor_locped_cfg_template.py', 'r')
    filestr = f1.read()

    for ifrom, ito in dic.iteritems():
        filestr = filestr.replace(ifrom,ito)

    f1.close()

    f1name = '../castor_locped_cfg_{0}.py'.format(argpedrun)
    f1 = open(f1name, 'w')
    f1.write(filestr)
    f1.close()
    ###retcode = subprocess.call(["chmod", "+x", f1name])

    f2 = open('./castor_gloraw_cfg_template.py', 'r')
    filestr = f2.read()

    for ifrom, ito in dic.iteritems():
        filestr = filestr.replace(ifrom,ito)

    f2.close()

    f2name = '../castor_gloraw_cfg_{0}{1}.py'.format(argledrun,argsuf)
    f2 = open(f2name, 'w')
    f2.write(filestr)
    f2.close()

    ###

    f3 = open('./job_run_template.sh', 'r')
    filestr = f3.read()

    for ifrom, ito in dic.iteritems():
        filestr = filestr.replace(ifrom,ito)

    f3.close()

    f3name = '../job_run{0}{1}.sh'.format(argledrun,argsuf)
    f3 = open(f3name, 'w')
    f3.write(filestr)
    f3.close()
    make_executable(f3name)

    f4 = open('./0sub_modified_template.sh', 'r')
    filestr = f4.read()

    for ifrom, ito in dic.iteritems():
        filestr = filestr.replace(ifrom,ito)

    f4.close()

    f4name = '../0sub_modified{0}{1}.sh'.format(argledrun,argsuf)
    f4 = open(f4name, 'w')
    f4.write(filestr)
    f4.close()
    make_executable(f4name)

    print "\n\nLook the directory one level above (ls -ltr ..)\n\n"

# http://stackoverflow.com/questions/12791997/how-do-you-do-a-simple-chmod-x-from-within-python
def make_executable(path):
    mode = os.stat(path).st_mode
    mode |= (mode & 0444) >> 2    # copy R bits to X
    os.chmod(path, mode)

if __name__ == '__main__':
    main()
