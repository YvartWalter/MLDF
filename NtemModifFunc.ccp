// ------------------------------------------------------------------------------------------------------------- //
#ifndef _NON_TEMPLATE_FUNCTION_MODIFIER_

#include "boost/regex.hpp"
#include <boost/algorithm/string.hpp>
#include "ReadWrite.hpp"
#include "AterSol.hpp"

typedef std::vector< double > Dvector;
typedef std::vector< Dvector > Dmatrix; //COL_MAJOR!

int GetPolyFunc(std::string& com, std::vector<double>& coeffs);
bool Process(std::vector<std::string>& com, std::vector<std::string>& labels, std::string& title, Dmatrix& Data);
void PrettyShow(Dmatrix& Data, std::vector<std::string>& labels, std::string& title,
                unsigned int target, unsigned int n_rows);
bool FileOutput(std::string& file_name, std::string&  title, std::vector<std::string>& labels,
                Dmatrix& Data, unsigned int n_rows);
                
// ------------------------------------------------------------------------------------------------------------- //
                
using namespace std;

///
/// \brief Programme principal
///
int sub_main(int argc, char* argv[])
{
  vector< string > com,labels;
  std::string title;
  Dmatrix Data;
  while( argc )
  {
    string arg(argv[--argc]);
    com.push_back( arg );
  }
  
  Process(com, labels, title, Data);

  // Fin du programme 
  cout<<flush;
  cout<<"\E[32m\E[1mTerminated Normaly\n\E[0m"<<endl;
  return EXIT_SUCCESS;
}

bool FileOutput(std::string& file_name, std::string&  title, std::vector<std::string>& labels,
                Dmatrix& Data, unsigned int n_rows)
{
  //TODO exporter un fichier en copiant juste l'entete et en remplaçant les données
  FileWriter<int> FileOut;
  std::vector< unsigned int > Dim(1, n_rows);
  Format::FORMAT_FILES format=Format::TEXPLOT;
  if( FileOut.Replace<double>( file_name, title, Data, labels, Dim, format) )
  {
    cout<<"\E[31m Error in reccord : \E[0m"<<file_name<<endl;
    return false;
  }
  cout<<"\E[34m Final state reccord in \E[0m"<<file_name<<endl;
  return true;
}

///
///
///
void PrettyShow(Dmatrix& Data, std::vector<std::string>& labels, std::string& title, 
                unsigned int target, unsigned int n_rows)
{
  string l("---------------------------------------------\n");
  cout<<l+title+"\n"+l+"\nline"<<endl;
  for(unsigned int i=0;i<Data.size();++i) cout<<labels[i]<<"  ";
  cout<<endl;
  cout<<l<<endl;
  for(unsigned int j=0;j<n_rows;++j)
  {
    for(unsigned int i=0;i<Data.size();++i)
    {
      if(i==0) cout<<"   "<<j<<" "<<flush;
      if(i==target) cout<<"\E[35m";
      cout<<"  "<<Data[i][j];
      if(i==target) cout<<"\E[0m";
      cout<<flush;
    }
    cout<<endl;
  }
 cout<<l<<endl;
}

///
///
///
bool Process(std::vector<std::string>& com, std::vector<std::string>& labels, std::string& title, Dmatrix& Data)
{
  if(com.size()<3) return false;
  string op=com[0], tg=com[1], ac=com[2];
  string newf=com[ com.size()-2 ];
  unsigned int n_head,n_cols,n_rows, target, degree;
  bool f=false;

  /*!
   *  Chargement du fichier source et des entêtes
   */
  if( !LoadSimpleSolution<Dmatrix>( newf ,n_head,n_cols,n_rows,Data,labels, title) ) return false;
  cout<<"From reading Head:"<<n_head<<", Rows:"<<n_rows<<", Cols:"<<n_cols<<endl;
  
  /*!
   * Cible
   */
  for(unsigned int i=0;i<labels.size();++i)
  {
    if(labels[i].find( tg )!=std::string::npos ){  target=i; f=true; break;  }
  }
  if(!f) return false;
  
  /*!
   * Opération sur les données
   */
  Operation<double> Gen( Data[target].data(), Data[target].size() );
  if( ( ac.find("poly")!=std::string::npos ) || ( ac.find("Poly")!=std::string::npos ) )
  {
    Dvector coeffs;
    degree = GetPolyFunc(op, coeffs);
    if(degree>21) return false;
    Gen.Poly(coeffs.data(), coeffs.size());
  }
  
  newf+=string(".export");
  PrettyShow(Data, labels, title, target, n_rows);
  if( FileOutput(newf, title, labels, Data, Data[0].size()) )
  {
     return true;
  }
  else return false;
  return true;
}

///
/// nom de la variable : x
///
//:-0.5:+8.1e3*x:-0.0077x2:
int GetPolyFunc(std::string& com, std::vector<double>& coeffs)
{
  //Degré du polynome
  for(unsigned int i=20;i!=1;--i)
  {
    if(com.find(string("x")+to_string(i) )!=std::string::npos)
    {
      coeffs.resize( i+1 );
      break;
    }
  }
  if(com.find(string("x"))!=std::string::npos){ coeffs.resize( 2 ); }
  else{ coeffs.resize( 1 ); }
  for(unsigned int i=0;i<coeffs.size();++i) coeffs[i]=0.0;
  cout<<"\E[34mDegree "<<coeffs.size()-1<<"\E[0m"<<endl;
  
  // Coefficients
  while(com.size()>2)
  {
    boost::regex filter("\\:(.+?)\\:(.*)");
    boost::smatch what;
    //cout<<com.size()<<" "<<com<<endl;
    //cout<<filter<<endl;
    if( boost::regex_match(com, what, filter) )
    {
      string arg=what[1];
      com=string(":")+what[2];
      std::size_t loc = arg.find(string("x"));
      //cout<<"GET="<<arg;
      if(loc!=std::string::npos)
      {
        if( !isdigit(arg[loc+1]) )
        {
          coeffs[1]+=stod(arg);
          //cout<<"    ++a_1";
        }
        else
        {
          for(unsigned int i=2;i<=coeffs.size();++i)
          {
            if(arg.find( string("x")+to_string(i) )!=std::string::npos)
            {
              coeffs[i]+=stod(arg);
              //cout<<"    ++a_"<<i;
              break;
            }
          }
        }
      }
      else
      {
        coeffs[0]+=stod(arg);
        //cout<<"    ++a_0";
      }
      
      //cout<<"\nREST="<<com<<endl;
    }
  }
  
  //Affichage
  cout<<"\E[34mP(X) = ";
  for(unsigned int i=0;i<coeffs.size();++i) cout<<" +"<<coeffs[i]<<" X**"<<i;
  cout<<"\E[0m"<<endl;
  
  return coeffs.size()-1;
}

#define _NON_TEMPLATE_FUNCTION_MODIFIER_
#endif

