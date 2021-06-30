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
void ReadMatrix ( ifstream & inFile, double* matrix, unsigned int nmode )
{
   stringstream ss;
   string line;
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
       // For the full lines, there are always the mode # followed by five entries
       ss >> num >> t1 >> t2 >> t3 >> t4 >> t5;
       matrix[i*5*nmode+j] = t1;
       matrix[(i*5+1)*nmode+j] = t2;
       matrix[(i*5+2)*nmode+j] = t3;
       matrix[(i*5+3)*nmode+j] = t4;
       matrix[(i*5+4)*nmode+j] = t5;
     }
   }
   // Now we take care of the remaining non-full lines
   if ( nmode%5 != 0 )
   {
     getline(inFile,line); // Column labels
     for ( unsigned int i = 0; i < nmode; i++ ) // Loop over rows
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
         matrix[((nmode/5)*5+j)*nmode+i] = t1;
       }
     }
   }
}

void WriteMatrix ( ofstream & outFile, const double* hess, unsigned int nmode )
{
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

void ConvertHess ( ifstream & inFile, ofstream & outFile )
{
  unsigned int nmode;
  string line;
  stringstream ss;
  getline(inFile,line); // This should be 3*N
  ss << line;
  ss >> nmode;
  double hess[nmode*nmode];
  ReadMatrix(inFile,hess,nmode);
  outFile << nmode << '\n';
  WriteMatrix(outFile,hess,nmode);
}

void ConvertNormalModes ( ifstream & inFile, ofstream & outFile )
{
  stringstream ss;
  string line;
  unsigned int nmodes{};
  getline(inFile,line);
  ss << line;
  ss >> nmodes;
  double modes[nmodes*nmodes];
  ReadMatrix(inFile,modes,nmodes);
  outFile << nmodes << " " << nmodes << '\n';
  WriteMatrix(outFile,modes,nmodes);
}

void ConvertAtoms ( ifstream & inFile, ofstream & outFile )
{
  string line;
  stringstream ss;
  unsigned int natom;
  CopyToFlag(inFile,outFile,"$atoms");
  getline(inFile,line);
  ss << line;
  ss >> natom;
  outFile << natom << '\n';
  for ( unsigned int i = 0; i < natom; i++ )
  {
    string el;
    double wt, x, y, z;
    ss.clear(); ss.str("");
    getline(inFile,line);
    ss << line;
    ss >> el >> wt >> x >> y >> z;
    outFile << setw(2) << el;
    outFile << fixed << setw(12) << setprecision(4) << wt;
    outFile << fixed << setw(14) << setprecision(6) << x;
    outFile << fixed << setw(13) << setprecision(6) << y;
    outFile << fixed << setw(13) << setprecision(6) << z << '\n';
  }
}

void ConvertDipole ( ifstream & inFile, ofstream & outFile )
{
  stringstream ss;
  string line;
  unsigned int nmodes;
  getline(inFile,line);
  ss << line;
  ss >> nmodes;
  outFile << nmodes << '\n';
  for ( unsigned int i = 0; i < nmodes; i++ )
  {
    double x, y, z;
    ss.clear(); ss.str("");
    getline(inFile,line);
    ss << line;
    ss >> x >> y >> z;
    outFile << fixed << setw(13) << setprecision(6) << x;
    outFile << fixed << setw(13) << setprecision(6) << y;
    outFile << fixed << setw(13) << setprecision(6) << z << '\n';
  }
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
  // Content up to $hessian should be identical with v3 vs. v4
  CopyToFlag(inFile,outFile,"$hessian");

  ConvertHess(inFile,outFile);

  // Content between $hessian and $normal_modes is identical between v3 and v4
  CopyToFlag(inFile,outFile,"$normal_modes");
  ConvertNormalModes(inFile,outFile);

  ConvertAtoms(inFile,outFile);

  CopyToFlag(inFile,outFile,"$dipole_derivatives");
  ConvertDipole(inFile,outFile);

  CopyToFlag(inFile,outFile,"$end");

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
    exit(2);
  }
  outFile.close();
  inFile.close();
  return 0;
}
