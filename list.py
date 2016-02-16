f = open('list.txt','w')
for x in xrange(100,200):
    fname = "/afs/cern.ch/user/y/ygo/eos/cms/store/group/phys_heavyions/jisun/ForestRun2015/PbPbMB/MB1_crab3_DCSONLYjson_Track_AK4CaloJet_D0_tkpt0p9eta1p5/HIMinimumBias1/crab_PbPb_HIMinimumBias1_ForestAOD_Track_AK4CaloJet_D0_tkpt0p9eta1p5_01152016/160114_224020/0000/HiForestAOD_%d.root" % x
    f.write("%s\n" % fname)
f.close()
