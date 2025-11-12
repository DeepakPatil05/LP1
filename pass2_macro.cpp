#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<string> load(const string& fname){
  vector<string> result;
  ifstream file(fname);

  string line;
  while(getline(file,line)){
    if(!line.empty()) result.push_back(line);
  }
  return result;
}

vector<string> tokenize(const string& line){
  vector<string> tokens;
  stringstream ss(line);
  string token;
  while(ss >> token) tokens.push_back(token);
  return tokens;
}

void getMNTEntry(vector<string>& mnt,const string& macroName,int& pp,int& kp,int& kpdtptr,int& mdtptr){
  for(string entry:mnt){
    stringstream ss(entry);
    string name;
    ss >> name >> pp >> kp >> kpdtptr >> mdtptr;
    if(name==macroName) return;
  }
}

void expandMacro(const string& macroName,vector<string>& call,vector<string>& mnt,vector<string> mdt,const vector<string> &kpdt,ofstream& outfile){
  int pp,kp,mdtPtr,kpdtPtr;
  getMNTEntry(mnt,macroName,pp,kp,kpdtPtr,mdtPtr);

  vector<string> params(pp+kp);

  for(int i=0;i<pp && i+1<call.size();i++){
    params[i] = call[i+1];
  };

  for(int i=0;i<kp;i++){
    stringstream ss(kpdt[kpdtPtr+i-1]);
    string key,value;
    ss >> key >> value;
    params[pp+i] = value;
  }

  for(int i=pp;i<call.size()-1 && (i-pp)<kp;i++){
    params[i] = call[i+1];
  }

  int i = mdtPtr-1;
  while(i<mdt.size() && mdt[i]!="MEND"){
    string line = mdt[i];

    for(int p=0;p<params.size();p++){
      string placeholder = "(P,"+to_string(p)+")";
      if(line.find(placeholder) != string::npos){
        line.replace(line.find(placeholder),placeholder.length(),params[p]);
      }
    }

    outfile << line << endl;
    i++;
  }
}

void performPass2(){
  vector<string> mnt = load("mnt1.txt");
  vector<string> mdt = load("mdt1.txt");
  vector<string> kpdt = load("kpdt1.txt");
  vector<string> input = load("OutputFile.txt");

  ofstream out("Expanded.txt");

  for(string line:input){
    vector<string> tokens = tokenize(line);
    if(tokens.empty()) continue;

    string word = tokens[0];
    bool isMacro = false;

    for(string str:mnt){
      if(str.find(word) != string::npos){
        isMacro = true;
        expandMacro(word,tokens,mnt,mdt,kpdt,out);
        break;
      }
    }
     if(!isMacro){
      out << line << endl;
     } 
  }
}

int main(){
  performPass2();
  cout<<"Pass 2 Completed";
}
