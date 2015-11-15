# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
    . /etc/bashrc
fi

# User specific aliases and functions

alias ls='ls --color=auto'
alias lt="ls -lahtr"
alias pse='ps -ef | grep'
alias cl='clear'
alias rl='root -l'
alias rlb='root -l -b'
alias rlbq='root -l -b -q'
alias psg='ps -ef | grep'
alias reli='readlink -f'
alias cmdlcg='cat ~/.commandLcg'
alias cvg='du -h | grep ^[0-9.]*G'
alias cvm='du -h | grep ^[0-9.]*M'
alias cva='du -h'
alias gc='g++ `$ROOTSYS/bin/root-config --cflags --glibs` -o'
#alias gopho=''

# for the CMSSW !!
#alias vocms='voms-proxy-init --voms cms'
#alias sb='scram build -j4'
#alias pro='cd /afs/cern.ch/work/y/ygo/private/PRODUCTION/CMSSW_5_3_20/src/;. setupCrabEnv.sh;'
#alias ce5='cd /afs/cern.ch/work/y/ygo/private/PRODUCTION/CMSSW_5_3_20/src/; eval `scramv1 ru -sh`; cd -; echo $ROOTSYS ;'
#alias ce7='export SCRAM_ARCH=slc6_amd64_gcc491; cd /afs/cern.ch/work/y/ygo/private/PRODUCTION/CMSSW_7_5_0_pre5/src/; eval `scramv1 ru -sh`; cd -; echo $ROOTSYS ;'
#alias gow='cd /afs/cern.ch/work/y/ygo/private/PhotonAnalysis2015'
#alias gof='cd /afs/cern.ch/user/y/ygo/workspace/public'
#alias pro750='cd ~/PRODUCTION/CMSSW_7_5_0/src/;. setupCrab3Env.sh;'
#alias cmsenv750='export SCRAM_ARCH=slc6_amd64_gcc491;cd /afs/cern.ch/work/y/ygo/private/PRODUCTION/CMSSW_7_5_0/src/; eval `scramv1 runtime -sh`; cd -;'
