#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>

using namespace std;

/*
This procedure copies the contents of the input hessian file into the
version 3 hessian file up to the termination flag
*/
void CopyToFlag ( ifstream & inFile, ofstream & outFile, string flag )
{
  string line;
  getline(inFile,line);
  while ( line.find(flag) == string::npos &&
          ! inFile.eof() )
  {
    outFile << line << '\n';
    getline(inFile,line);
  }
  outFile << line << '\n';
  if ( inFile.eof() )
  {
    cout << flag << " not found! Exiting\n";
    exit(2);
  }
}

/*
This procedure reads the hessian from version 4 into an array
We start in the input buffer after the line containing the number of
atoms * 3 ... 3N
These are line 0 1 2 3 4 5 \n 3N Hessian Rows \n 6 7 ...
*/
void ReadHess ( ifstream & inFile, double* hess, unsigned int nmode )
{
   stringstream ss;
   string line;
   unsigned int index{0};
   for ( unsigned int i = 0; i < nmode/5; i++ ) // First we loop over "full" lines
   {
     getline(inFile,line); // This will be column label
     for ( unsigned int j = 0; j < nmode; j++ )
     {
       unsigned int num;
       double t1, t2, t3, t4, t5;
       ss.clear(); ss.str("");
       getline(inFile,line);
       ss << line;
       ss >> num >> t1 >> t2 >> t3 >> t4 >> t5;
       hess[i*5*nmode+j] = t1;
       hess[(i*5+1)*nmode+j] = t2;
       hess[(i*5+2)*nmode+j] = t3;
       hess[(i*5+3)*nmode+j] = t4;
       hess[(i*5+4)*nmode+j] = t5;
     }
   }
   if ( nmode%5 != 0 )
   {
     getline(inFile,line);
     for ( unsigned int i = 0; i < nmode; i++ )
     {
       getline(inFile,line);
       ss.clear(); ss.str("");
       unsigned int num;
       ss << line;
       ss >> num;
       for ( unsigned int j = 0; j < nmode%5; j++ )
       {
         double t1;
         ss >> t1;
         hess[((nmode/5)*5+j)*nmode+i] = t1;
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

void WriteHess ( ofstream & outFile, const double* hess, unsigned int nmode )
{
  outFile << nmode << '\n';
  for ( unsigned int i = 0; i < nmode/6; i++ )
  {
    outFile << setw(19) << i*6;
    for ( unsigned int j = 0; j < 5; j++ )
    {
      outFile << setw(11) << i*6+j+1;
    }
    outFile << '\n';
    for ( unsigned int j = 0; j < nmode; j++ )
    {
      outFile << setw(7) << j;
      outFile << "    ";
      for ( unsigned int k = 0; k < 6; k++ )
      {
        outFile << setw(11) << fixed << setprecision(6) << hess[(i*6+k)*nmode+j];
      }
      outFile << '\n';
    }
  }
  if ( nmode%6 !=0 )
  {
    outFile << setw(19) << (nmode/6)*6;
    for ( unsigned int i = 1; i < nmode%6; i++ )
    {
      outFile << setw(11) << (nmode/6)*6+i;
    }
    outFile << '\n';
    for ( unsigned int i = 0; i < nmode; i++ )
    {
      outFile << setw(7) << i;
      outFile << "    ";
      for ( unsigned int j = 0; j < nmode%6; j++ )
      {
        outFile << setw(11) << fixed << setprecision(6) << hess[((nmode/6)*6+j)*nmode+i];
      }
      outFile << '\n';
    }
  }
  outFile << '\n';
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
  unsigned int nmode;
  string line;
  stringstream ss;
  getline(inFile,line); // This should be 3*N
  ss << line;
  ss >> nmode;
  double hess[nmode*nmode];
  ReadHess(inFile,hess,nmode);
  WriteHess(outFile,hess,nmode);
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

void ConvertAtoms ( ifstream & inFile, ofstream & outFile )
{
  string line;
  stringstream ss;
  unsigned int natom;
  getline(inFile,line);
  for ( unsigned int i = 0; i < 4; i++ )
  {
    getline(inFile,line);
    outFile << line << endl;
  }
  getline(inFile,line);
  ss << line;
  ss >> natom;
  outFile << natom << endl;
  for ( unsigned int i = 0; i < natom; i++ )
  {
    string el;
    double wt, x, y, z;
    ss.clear(); ss.str("");
    getline(inFile,line);
    ss << line;
    ss >> el >> wt >> x >> y >> z;
    outFile << setw(2) << el;
    outFile << setw(13) << setprecision(5) << wt;
    outFile << setw(14) << setprecision(6) << x;
    outFile << setw(13) << setprecision(6) << y;
    outFile << setw(13) << setprecision(6) << z << endl;
  }
  outFile << endl;
  getline(inFile,line);
}

void CopyTemperature ( ifstream & inFile, ofstream & outFile )
{
  string line;
  for ( unsigned int i = 0; i < 3; i++ )
  {
    getline(inFile,line);
    outFile << line << endl;
  }
}

void ConvertDipole ( ifstream & inFile, ofstream & outFile )
{
  stringstream ss;
  string line;
  unsigned int num;
  for ( unsigned int i = 0; i < 3; i++ ) getline(inFile,line);
  getline(inFile,line);
  outFile << line << endl;
  getline(inFile,line);
  ss << line;
  ss >> num;
  outFile << num << endl;
  for ( unsigned int i = 0; i < num; i++ )
  {
    double x, y, z;
    ss.clear(); ss.str("");
    getline(inFile,line);
    ss << line;
    ss >> x >> y >> z;
    outFile << setw(13) << setprecision(6) << x;
    outFile << setw(13) << setprecision(6) << y;
    outFile << setw(13) << setprecision(6) << z << endl;
  }
  outFile << endl;
  getline(inFile,line);
}

void CopyTheRest ( ifstream & inFile, ofstream & outFile )
{
  string line;
  stringstream ss;
  getline(inFile,line);
  while ( line.find("$end") == string::npos )
  {
    outFile << line << endl;
    getline(inFile,line);
  }
  outFile << line << endl;
}

void ReadWriteHessian ( ifstream & inFile, ofstream & outFile )
{
  CopyToFlag(inFile,outFile,"$hessian");
  ConvertHess(inFile,outFile);
  /*
  CopyToFlag(inFile,outFile,"$normal_modes");
  ConvertNormalModes(inFile,outFile);
  ConvertAtoms(inFile,outFile);
  CopyTemperature(inFile,outFile);
  ConvertDipole(inFile,outFile);
  CopyToFlag(inFile,outFile,"$end");
  */
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
