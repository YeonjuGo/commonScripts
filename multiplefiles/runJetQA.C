#include "anaJetQA.C"

std::vector<std::string> CollectFiles(TString list) {

  std::vector<std::string> urls;

  ifstream inputFile;
  inputFile.open(list.Data());
  TString line;
  while( inputFile>>line) {
    urls.push_back(line.Data());
  }

  return urls;

}

void runJetQA(TString list = "output.list", TString jetTreeName = "akPu3PFJetAnalyzer10", Int_t startEntry = 0, Int_t maxEvents = -1,  size_t startFile = 0, size_t nfiles = 0, Int_t jobNumber = 0, Int_t etaBin = 0) {

  std::vector<std::string> urls = CollectFiles(list);
  
  //Run analysis
  anaJetQA(urls, jetTreeName, startEntry, maxEvents,startFile,nfiles,jobNumber,etaBin);

}


