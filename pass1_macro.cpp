#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

vector<vector<string>> tokenizer(const string& fname){
  ifstream tokenfile(fname);
  vector<vector<string>> result;

  string line;
  while(getline(tokenfile,line)){
    replace(line.begin(),line.end(),',',' ');

    vector<string> tokens;
    stringstream ss(line);
    string token;
    while(ss >> token) tokens.push_back(token);
    result.push_back(tokens);
  }

  return result;
}

vector<string> mnt;
vector<string> mdt;
vector<string> kpdt;
int kpdtPtr = 1;
int mdtPtr = 1;

void separateMacro(const string& filename) {
  ifstream inputfile(filename);
  ofstream macro("MacroFile.txt");
  ofstream out("OutputFile.txt");

  bool inside = false;
  string line;
  while(getline(inputfile,line)){
    if(line=="MACRO"){
      inside = true;
      macro << line << endl;
    }
    else if(line=="MEND"){
      inside = false;
      macro << line << endl;
    }
    else if(inside){
      macro << line << endl;
    }
    else{
      out << line << endl;
    }
  }

  inputfile.close();
  out.close();
  macro.close();
}

void processMacro(vector<vector<string>>& macro){
  vector<string> pnt;
  //mnt
  string mntEntry = macro[1][0] + " ";
  int ppCount = 0;
  int kpCount = 0;

  for(size_t i = 1;i<macro[1].size();i++){
    if(macro[1][i].find('=') != string::npos){
      kpCount++;
    }
    else ppCount++;
  }

  mntEntry += (to_string(ppCount) + " " + to_string(kpCount) + " " + to_string(kpdtPtr) + " " + to_string(mdtPtr));
  mnt.push_back(mntEntry);

  //kpdt
  for(size_t i = 1;i<macro[1].size();i++){
    if(macro[1][i].find('=') != string::npos){
      size_t pos = macro[1][i].find('=');
      string name = macro[1][i].substr(0,pos);
      string val = macro[1][i].substr(pos+1);

      if(val.empty()) kpdt.push_back(name + " -1");
      else kpdt.push_back(name + " " + val);

      kpdtPtr++;
    }
  }

  //pnt
  for(size_t i=1;i<macro[1].size();i++){
    size_t pos = macro[1][i].find('=');
    if(pos!=string::npos){
      pnt.push_back(macro[1][i].substr(0,pos));
    }
    else pnt.push_back(macro[1][i]);
  }

  //mdt
  for(size_t i = 2;i<macro.size();i++){
    string mdtEntry = "";

    for(string word:macro[i]){
      if(word[0]=='&'){
        mdtEntry += "(P," + to_string(find(pnt.begin(),pnt.end(),word) - pnt.begin()) + ") ";
      }
      else mdtEntry += word + " ";
    }
    mdt.push_back(mdtEntry);
    mdtPtr++;
  }
}

void save(vector<string>& data,const string& fname,const string& title){
  ofstream file(fname);
  cout<<"--- "<<title<<" ---"<<endl;
  for(string line:data){
    file << line << endl;
    cout << line << endl;
  }
  cout<<endl;
  file.close();
}

void processPassOne(){
  separateMacro("tc1.txt");
  vector<vector<string>> instructions = tokenizer("MacroFile.txt");

  for(size_t i=0;i<instructions.size();i++){
    if(instructions[i].size()>0 && instructions[i][0]=="MACRO"){
      vector<vector<string>> macroBlock;

      while(i<instructions.size()){
        if(instructions[i].size()>0 && instructions[i][0]=="MEND"){
          macroBlock.push_back(instructions[i]);
          break;
        }
        else macroBlock.push_back(instructions[i]);
        i++;
      }
      processMacro(macroBlock);
    }
  }

  save(mnt,"mnt1.txt","MNT");
  save(kpdt,"kpdt1.txt","KPDT");
  save(mdt,"mdt1.txt","MDT");
  cout<<"# # # FINISH # # #"<<endl;
}

int main() {processPassOne();}


