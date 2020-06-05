#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>

using namespace std;

void CopyToHess ( ifstream & inFile, ofstream & outFile )
{
  string line;
  getline(inFile,line);
  while ( line.find("$hessian") == string::npos )
  {
    outFile << line << endl;
    getline(inFile,line);
  }
  outFile << line << endl;
}

void ReadHess ( ifstream & inFile, vector< vector< double > > & hess )
{
   stringstream ss;
   string line;
   unsigned int dim;
   getline(inFile,line); ss << line; ss >> dim;
   for ( unsigned int i = 0; i < dim/5; i++ )
   {
     for ( unsigned int j = 0; j < 5; j++ ) hess.push_back(vector<double>());
     getline(inFile,line);
     for ( unsigned int j = 0; j < dim; j++ )
     {
       unsigned int num;
       double t1, t2, t3, t4, t5;
       ss.clear(); ss.str("");
       getline(inFile,line);
       ss << line;
       ss >> num >> t1 >> t2 >> t3 >> t4 >> t5;
       hess[i*5].push_back(t1);
       hess[i*5+1].push_back(t2);
       hess[i*5+2].push_back(t3);
       hess[i*5+3].push_back(t4);
       hess[i*5+4].push_back(t5);
     }
   }
   if ( dim%5 != 0 )
   {
     getline(inFile,line);
     for ( unsigned int i = 0; i < dim%5; i++ ) hess.push_back(vector<double>());
     for ( unsigned int i = 0; i < dim; i++ )
     {
       getline(inFile,line);
       ss.clear(); ss.str("");
       unsigned int num;
       ss << line;
       ss >> num;
       for ( unsigned int j = 0; j < dim%5; j++ )
       {
         double t1;
         ss >> t1;
         hess[(dim/5)*5+j].push_back(t1);
       }
     }
   }
}

void ReadNormalModes ( ifstream & inFile, vector< vector< double > > & modes )
{
   stringstream ss;
   string line;
   unsigned int dim;
   getline(inFile,line);
   getline(inFile,line); ss << line; ss >> dim;
   for ( unsigned int i = 0; i < dim/5; i++ )
   {
     for ( unsigned int j = 0; j < 5; j++ ) modes.push_back(vector<double>());
     getline(inFile,line);
     for ( unsigned int j = 0; j < dim; j++ )
     {
       unsigned int num;
       double t1, t2, t3, t4, t5;
       ss.clear(); ss.str("");
       getline(inFile,line);
       ss << line;
       ss >> num >> t1 >> t2 >> t3 >> t4 >> t5;
       modes[i*5].push_back(t1);
       modes[i*5+1].push_back(t2);
       modes[i*5+2].push_back(t3);
       modes[i*5+3].push_back(t4);
       modes[i*5+4].push_back(t5);
     }
   }
   if ( dim%5 != 0 )
   {
     getline(inFile,line);
     for ( unsigned int i = 0; i < dim%5; i++ ) modes.push_back(vector<double>());
     for ( unsigned int i = 0; i < dim; i++ )
     {
       getline(inFile,line);
       ss.clear(); ss.str("");
       unsigned int num;
       ss << line;
       ss >> num;
       for ( unsigned int j = 0; j < dim%5; j++ )
       {
         double t1;
         ss >> t1;
         modes[(dim/5)*5+j].push_back(t1);
       }
     }
   }
}

void WriteHess ( ofstream & outFile, const vector< vector< double > > & hess )
{
  outFile << hess[0].size() << endl;
  for ( unsigned int i = 0; i < hess[0].size()/6; i++ )
  {
    outFile << setw(19) << i*6;
    for ( unsigned int j = 0; j < 5; j++ )
    {
      outFile << setw(11) << i*6+j+1;
    }
    outFile << endl;
    for ( unsigned int j = 0; j < hess[0].size(); j++ )
    {
      outFile << setw(7) << j;
      outFile << "    ";
      for ( unsigned int k = 0; k < 6; k++ )
      {
        outFile << setw(11) << fixed << setprecision(6) << hess.at(i*6+k).at(j);
      }
      outFile << endl;
    }
  }
  if ( hess[0].size()%6 !=0 )
  {
    outFile << setw(19) << (hess[0].size()/6)*6;
    for ( unsigned int i = 1; i < hess[0].size()%6; i++ )
    {
      outFile << setw(11) << (hess[0].size()/6)*6+i;
    }
    outFile << endl;
    for ( unsigned int i = 0; i < hess[0].size(); i++ )
    {
      outFile << setw(7) << i;
      outFile << "    ";
      for ( unsigned int j = 0; j < hess[0].size()%6; j++ )
      {
        outFile << setw(11) << fixed << setprecision(6) << hess.at((hess[0].size()/6)*6+j).at(i);
      }
      outFile << endl;
    }
  }
  outFile << endl;
}

void WriteNormalModes ( ofstream & outFile, const vector< vector< double > > & modes )
{
  outFile << "$normal_modes" << endl;
  outFile << modes[0].size() << " " << modes[0].size() << endl;
  for ( unsigned int i = 0; i < modes[0].size()/6; i++ )
  {
    outFile << setw(19) << i*6;
    for ( unsigned int j = 0; j < 5; j++ )
    {
      outFile << setw(11) << i*6+j+1;
    }
    outFile << endl;
    for ( unsigned int j = 0; j < modes[0].size(); j++ )
    {
      outFile << setw(7) << j;
      outFile << "    ";
      for ( unsigned int k = 0; k < 6; k++ )
      {
        outFile << setw(11) << fixed << setprecision(6) << modes.at(i*6+k).at(j);
      }
      outFile << endl;
    }
  }
  if ( modes[0].size()%6 !=0 )
  {
    outFile << setw(19) << (modes[0].size()/6)*6;
    for ( unsigned int i = 1; i < modes[0].size()%6; i++ )
    {
      outFile << setw(11) << (modes[0].size()/6)*6+i;
    }
    outFile << endl;
    for ( unsigned int i = 0; i < modes[0].size(); i++ )
    {
      outFile << setw(7) << i;
      outFile << "    ";
      for ( unsigned int j = 0; j < modes[0].size()%6; j++ )
      {
        outFile << setw(11) << fixed << setprecision(6) << modes.at((modes[0].size()/6)*6+j).at(i);
      }
      outFile << endl;
    }
  }
  outFile << endl;
}

void ConvertHess ( ifstream & inFile, ofstream & outFile )
{
  vector<vector<double > > hess;
  ReadHess(inFile,hess);
  WriteHess(outFile,hess);
}

void CopyEnergies ( ifstream & inFile, ofstream & outFile )
{
  unsigned int numfreq;
  string line;
  stringstream ss;
  getline(inFile,line); getline(inFile,line);
  outFile << line << endl;
  getline(inFile,line);
  ss << line;
  ss >> numfreq;
  outFile << numfreq << endl;
  for ( unsigned int i = 0; i < numfreq; i++ )
  {
    unsigned int fnum;
    double freq;
    getline(inFile,line);
    ss.clear(); ss.str("");
    ss << line;
    ss >> fnum >> freq;
    outFile << setw(5) << fnum << setw(16) << fixed << setprecision(6) << freq << endl;
  }
  getline(inFile,line);
  outFile << endl;
}

void ConvertNormalModes ( ifstream & inFile, ofstream & outFile )
{
  vector<vector<double > > modes;
  ReadNormalModes(inFile,modes);
  WriteNormalModes(outFile,modes);
}

void ReadWriteHessian ( ifstream & inFile, ofstream & outFile )
{
  CopyToHess(inFile,outFile);
  ConvertHess(inFile,outFile);
  CopyEnergies(inFile,outFile);
  ConvertNormalModes(inFile,outFile);
  /*ConvertAtoms(inFile,outFile);
  CopyTemperature(inFile,outFile);
  ConvertDipole(inFile,outFile);
  CopyTheRest(inFile,outFile);*/
}

int main ( int argc, char* argv[] )
{
  if ( argc != 3 ) { cout << "ERROR! INCORRECT OPTIONS! " << endl; exit(1); }
  ifstream inFile;
  ofstream outFile;
  inFile.open(argv[1]);
  outFile.open(argv[2]);
  if ( inFile.good() )
  {
    ReadWriteHessian(inFile,outFile);
  }
  else
  {
    cout << "ERROR! INPUT HESSIAN COULDN'T BE OPENED!" << endl;
    exit(1);
  }
  outFile.close();
  inFile.close();
  return 0;
}
