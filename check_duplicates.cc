#include <iostream>
#include <set>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <sstream>
#include <TSystem.h>
#include <vector>
#include <TChain.h>
#include "TSystem.h"
#include <fstream>
#include <algorithm>

using namespace std;


void check_duplicates (const vector<string>& files);

int main(int argc, char* argv[])
{

	if (argc <= 1) {
		cerr <<"Please give 1 arguments: file_list" << endl;
		return -1;
	}

	const char *inputFileList = argv[1];

	ifstream file;
	file.open(inputFileList);
	vector<string> filelist;
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			cout << line << endl;
			filelist.push_back(line);
		}

		check_duplicates(filelist);

	} else {
		cout << "Cannot find " << inputFileList << "!" <<  endl;
	}

	return 0;
}

void check_duplicates (const vector<string>& files)
{

	std::vector<std::string>::const_iterator fileIt;  
 
	TChain *tree = new TChain("treeMaker/tree");

  for (fileIt = files.begin(); fileIt != files.end(); ++fileIt) {
	  TFile* file = new TFile(fileIt->c_str(),"READ");
	  if (file->IsZombie()) {
			std::cout << "Cannot find file:" << *fileIt  << " pl. check." << std::endl;
			return;
  		} else {
			tree->Add(fileIt->c_str());
		}
  }

  unsigned run, ls, evt;

  tree->SetBranchStatus  ("*", 0);
  tree->SetBranchStatus  ("t_EvtRun"  , 1);
  tree->SetBranchStatus  ("t_EvtLS"   , 1);
  tree->SetBranchStatus  ("t_EvtEvent", 1);
  tree->SetBranchAddress ("t_EvtRun"  , &run);
  tree->SetBranchAddress ("t_EvtLS"   , &ls );
  tree->SetBranchAddress ("t_EvtEvent", &evt);

  unsigned event = 0;
  unsigned ENTRIES = (unsigned)tree->GetEntries();
  std::cout << "Entries Found = " << ENTRIES << std::endl;
	
	
  vector<string> found, dups;

  while (tree->GetEvent (event))
  {
	  ++ event;
//	  if ((event % 100000) == 0) std::cout << (int) (event * 100. /tree->GetEntries())<< " % complete." << std::endl; 
     
	
		stringstream str;
		str << run << ":" << ls << ":" << evt;
		found.push_back(str.str());
		cout << str.str() << endl;
		
  	}
	
	//found uplicates

  sort(found.begin(), found.end());

  bool duplicate = false;
  for (unsigned i=0; i < found.size()-1; ++i)
  {
	  string str1 = found.at(i);
	  string str2 = found.at(i+1);
	  if (str1.compare(str2) == 0)
	  {
		  duplicate == true;
		  dups.push_back(str1);
	  }
  }

		
	if (duplicate) 
	{
		cout << "DUPLICATES" << endl;
		for (unsigned i=0; i < dups.size(); ++i)
		{
			cout << dups.at(i) << endl;
		}
	} else {
		cout << "No duplicates" << endl;
	}
  //std::cout << found.size() << "/" << event << std::endl;
  std::cout << found.size() << "/" << event << std::endl;
  std::cout << "Duplicate events = " << dups.size() << " found!" << std::endl;
  return;
  
 // for (std::set<std::pair<unsigned,unsigned> >::const_iterator it = found.begin() ; it != found.end(); ++it) {
 //		std::cout << it->first << ",\t" << it->second << std::endl;
 // }

  //if (dupEvts.size()<1) return;

	//if duplicates are found, run this additional option
	//to create a new ntuple without any duplcates.
  
 /* TTree *oldTree=0;
  Stuple oldStuple;

  tree->SetBranchStatus  ("*", 0);
  PrepStupleTreeForReading(tree, &oldStuple);
*/  //oldStuple.Dump();

  //tree->SetBranchStatus  ("evt_RunNumber", 1);
  //tree->SetBranchAddress ("evt_RunNumber", &(oldStuple.evt_RunNumber));
  //tree->SetBranchStatus  ("evt_EventNumber", 1);
  //tree->SetBranchAddress ("evt_EventNumber", &(oldStuple.evt_EventNumber));


  /*
  std::cout << "ch = " << tree << std::endl;
  std::cout << "N = " << tree->GetEntries() << std::endl;
  

  TTree* newTree=0;
  Stuple newStuple;
  
  std::stringstream newfile;
  newfile << files.at(0) << "_NoDuplicates";
  TFile f(newfile.str().c_str(),"RECREATE");
  newTree = GetStupleTree("Stuple", newStuple);
  assert (newTree != NULL && "new tree is null");

  std::set<std::pair<unsigned,unsigned> >::iterator dupIt;
  
  unsigned iDupCount=0, iOrigCount=0;
  //for (unsigned i=1; i< 50; ++i)
  for (unsigned i=0; i< tree->GetEntries(); ++i)
  {
	  if ((i % 100000) == 0) std::cout << (int) (i * 100. /tree->GetEntries())<< " % complete." << std::endl; 
	  tree->GetEntry(i);
	  //std::cout << oldStuple.evt_RunNumber << ", "<< oldStuple.evt_EventNumber << std::endl;
	  std::pair<unsigned,unsigned> id (oldStuple.evt_RunNumber, oldStuple.evt_EventNumber);

	  dupIt = dupEvts.find(id);
	  if (dupIt == dupEvts.end())
	  {
		  newStuple.Init();
		  newStuple = oldStuple;
		  newTree->Fill();
		  iOrigCount++;
	  } else {
		  dupEvts.erase(dupIt);
		  std::cout << "dulicate found " << dupIt->first << "\t" << dupIt->second << " ------------> dupEvts.size = " << dupEvts.size() << std::endl;
		  iDupCount++;
	  }
  }

  std::cout << "orig / dulicates " << iOrigCount << " / " << iDupCount <<std::endl;
	
  f.Write();
  f.ls();
  f.Close();
  */
  //delete tree;
  //delete oldTree;
  //delete newTree;
};
