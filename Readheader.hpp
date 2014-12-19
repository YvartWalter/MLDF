///
/// \file Readheader.hpp
///

//#include<regex>
//#include <ctype.h>

///
/// \class FileHeader
///
/// Lire les header des fichiers
///
template <typename Basic>
class FileHeader
{
  private:
    
    template<typename ReturnType>
    ReturnType GetDim(ReturnType loc, std::string& got);
    
  public:
    FileHeader(void){ };
    
    FileHeader(const FileHeader<Basic>& FH){ };
    
    template<class H>
    FileHeader(const FileHeader<H>& FH){ };
    
    virtual ~FileHeader(){ };
    
    Basic Header(std::string& file_name, std::string& title, std::vector<unsigned int>& datastructure,
                 std::vector<unsigned int>& dimensions, std::vector<std::string>& labels,
                 Format::FORMAT_FILES fm);
    
    Basic Header(std::string& file_name, std::string& title, std::vector<unsigned int>& datastructure,
                 std::vector<unsigned int>& dimensions, std::vector<std::string>& labels,
                 Format::FORMAT_FILES& fm, const char ld,  const char cm );
    
    Basic Header(std::ifstream& file, std::string& title, std::vector<unsigned int>& datastructure,
                 std::vector<unsigned int>& dimensions, std::vector<std::string>& labels,
                 Format::FORMAT_FILES fm);
    
    Basic Header(std::ifstream& file, std::string& title, std::vector<unsigned int>& datastructure,
                 std::vector<unsigned int>& dimensions, std::vector<std::string>& labels,
                 Format::FORMAT_FILES& fm, const char ld,  const char cm );
    
    bool HASHTAG(std::string& line, std::vector<std::string>& variables);
    
    bool ZONE(std::string& line, std::vector<unsigned int>& dimensions);
    
    bool VARIABLES(std::string& line, std::vector<std::string>& labels);
    
    bool DATASTRUCTURE(std::string& line, std::vector<unsigned int>& datastructure);
    
    bool TITLE(std::string& line, std::string& title);

};

template <typename Basic>
Basic FileHeader<Basic>::Header(std::string& file_name, std::string& title,
                                std::vector<unsigned int>& datastructure,
                                std::vector<unsigned int>& dimensions,
                                std::vector<std::string>& labels,
                                Format::FORMAT_FILES fm)
{
  return Header(file_name, title, datastructure, dimensions, labels, fm, '\n', '#');
}

template <typename Basic>
Basic FileHeader<Basic>::Header(std::string& file_name, std::string& title,
                                std::vector<unsigned int>& datastructure,
                                std::vector<unsigned int>& dimensions,
                                std::vector<std::string>& labels,
                                Format::FORMAT_FILES& fm=Format::UNKNOWN,
                                const char ld='\n',  const char cm='#'  )
{
  std::ifstream file( file_name.c_str() );
  if( file.fail() )
  {
    std::cerr<<"can't open file "<<file_name.c_str()<<std::endl;
    return static_cast<Basic>(0);
  }
  return Header(file, title, datastructure, dimensions, labels,Format::UNKNOWN, '\n', '#' );
}

template <typename Basic>
Basic FileHeader<Basic>::Header(std::ifstream& file, std::string& title,
                                std::vector<unsigned int>& datastructure,
                                std::vector<unsigned int>& dimensions,
                                std::vector<std::string>& labels,
                                Format::FORMAT_FILES fm)
{
  return Header(file, title, datastructure, dimensions, labels,fm, '\n', '#' );
}

///
/// Repositionne le stream à son point d'entrée
///
template <typename Basic>
Basic FileHeader<Basic>::Header(std::ifstream& file, std::string& title,
                                std::vector<unsigned int>& datastructure,
                                std::vector<unsigned int>& dimensions,
                                std::vector<std::string>& labels,
                                Format::FORMAT_FILES& fm=Format::UNKNOWN,
                                const char ld='\n',  const char cm='#'  )
{
  file.clear();
  file.seekg (0, file.beg);
  std::string line;
  bool ds=false, ti=false, va=false, zn=false, cc=false, quith=false;
  switch(fm)
  {
    case Format::TEXPLOT_EXTENDED:
      ti=true;
      title="AutoTEXPLOT";
      ds=false; va=false; zn=false; cc=false; break;
    case Format::TEXPLOT:
      ti=false; va=false; zn=false; ds=true; cc=false;  break;
    case Format::MAMOJ:
    case Format::MIRO:
      title="AutoSELF";
      ti=true; va=true; zn=true; ds=true; cc=true;  break;
    case Format::RAW:
    case Format::UNKNOWN:
    default:
      ds=false; ti=false;  va=false;  zn=false;  cc=true;  break;
  }
  unsigned int nline=0, nhead=0;
  while( std::getline(file, line, ld) )
  {
    bool lh=false;
    if( !ti ) //titre
    {
      if( (line.find(H0)!=std::string::npos) && !ti )
      {
        lh = true;
        ti = TITLE(line, title);
      }
    }
    if( !ds ) //Nombre de lignes et de colonnes
    {
      if( (line.find(H3)!=std::string::npos) )
      {
        lh = true;
        ds = DATASTRUCTURE(line, datastructure);
        if( !ds ) return static_cast<Basic>(0);
      }
    }
    if( !va ) //liste des variables
    {
      if( (line.find(H1)!=std::string::npos) && !va )
      {
        lh = true;
        va = VARIABLES(line, labels);
        if( !va ) return static_cast<Basic>(0);
      }
    }
    if( !zn ) //dimensions
    {
      if( (line.find(H2)!=std::string::npos) && ! zn )
      {
        lh = true;
        zn = ZONE(line, dimensions);
        if( !zn ) return static_cast<Basic>(0);
      }
    }
    if( line[0]==cm )
    {
      lh=true;
      if( cc ) //try to catch header behind #
      {
        va = HASHTAG(line, labels);
      }
    }
    //Compte les lignes contenant des données
    if( !lh )
    {
      unsigned int j=0;
      quith=true;
      while( isspace(line[j]) ){ j++; }
      if( isdigit(line[j])||(line[j]=='-')||(line[j]=='+')||(line[j]=='.') ){  ++nline;  }
    }
    else
    {
      if(!quith){++nhead;}
    }
  }//end while
  //Contrat rempli
  if( fm==Format::TEXPLOT)
  {
    if( !dimensions.size() ){ return static_cast<Basic>(0); }
    if( !labels.size() ){ return static_cast<Basic>(0); }
  }
  if( !nline ) return static_cast<Basic>(0);       
  if( ds && !datastructure.size())
  {
    datastructure.push_back( labels.size() ); //nombre de colonnes
    datastructure.push_back( nline ); //nombre de lignes
  }
  datastructure.push_back( nhead ); //nombre de ligne d'entête
  return static_cast<Basic>(datastructure.size()+dimensions.size()+labels.size());
}

template<typename Basic>
bool FileHeader<Basic>::HASHTAG(std::string& line, std::vector<std::string>& variables)
{
  unsigned int j=0;
  //#FUI1333717593
  //
  //#DATASTRUCTURE 7 25                                    --> ok
  //
  //#       1        2        3        4        5 
  //#       x        y       vx       vy    rhoCO
  //
  //# data class : Xsingle
  //# cell               1               2 
  //
  if( (line.find(H3)!=std::string::npos) ) return false;
  if(variables.size() ) return false;
  std::string word="";
  int len=0;
  while( j<line.size() )
  {
    char C=line[j++];
    if( isspace( C ) )
    {
      variables.push_back( word );
      word=""; len=0;
      continue;
    }
    if(isdigit( C ) || (C=='.') || (C==',') || (C=='-')|| (C=='+') ) ++len;
    word+=C;
  }
  if( variables.size() < 2 )
  {
    variables.clear();
    return false;
  }
  return (variables.size()>0);
}

template <typename Basic>
bool FileHeader<Basic>::ZONE(std::string& line, std::vector<unsigned int>& dimensions)
{
  std::regex e ("(.*"+H2+")(.+)");
  std::smatch sm;
  std::string got;
  if(std::regex_match (line,sm,e))
  {
    got = sm[2];
    std::size_t loc = got.find("I");
    if( loc!=std::string::npos )
    {
      dimensions.push_back( GetDim<unsigned int>(loc+1, got) );
    }
    loc = got.find("J");
    if( loc!=std::string::npos )
    {
      if( dimensions.size()<1) return false;
      dimensions.push_back( GetDim<unsigned int>(loc+1, got) );
    }
    loc = got.find("K");
    if( loc!=std::string::npos )
    {
      if( dimensions.size()<2) return false;
      dimensions.push_back( GetDim<unsigned int>(loc+1, got) );
    }
    return true;
  }
  return false;
}

template<typename Basic>
template<typename ReturnType>
ReturnType FileHeader<Basic>::GetDim(ReturnType loc, std::string& got)
{
  std::string sgot;
  while(loc<got.size())
  {
    if( isdigit(got[loc]) )
    {
      sgot=got.substr(loc,got.size()-loc-1);
      int dim=atoi(sgot.c_str());
      return static_cast<ReturnType>(dim);
    }
    ++loc;
  }
  return static_cast<ReturnType>(0);
}

template <typename Basic>
bool FileHeader<Basic>::VARIABLES(std::string& line, std::vector<std::string>& labels)
{
  std::regex e ("(.*"+H1+EQ+")(.+)");//#DATASTRUCTURE 7 25
  std::smatch sm;
  std::string got, var="";
  if(std::regex_match (line,sm,e))
  {
    got = sm[2];
    labels.clear();
    for(unsigned int i=1;i<got.size();++i)
    {
      if((got[i]==' ')||(got[i]=='"')||(got[i]=='=')||(got[i]=='\r')) continue;
      if(got[i]==',')
      {
        labels.push_back(var);
        var="";
        continue;
      }
      var+=got[i];
    }
    labels.push_back(var);
    return true;
  }
  return false;
}

template <typename Basic>
bool FileHeader<Basic>::DATASTRUCTURE(std::string& line, std::vector<unsigned int>& datastructure)
{
  std::regex e ("(.*"+H3+")(.+)");//#DATASTRUCTURE 7 25
  std::smatch sm;
  std::string got, digit="";
  if(std::regex_match (line,sm,e))
  {
    datastructure.clear();
    got=sm[2]; 
    int value;
    unsigned int k=0;
    for(unsigned int j=1;j<got.size();++j)
    {
      if(isdigit(got[j]))
      {
        if(k==(j-1))
        {
          digit+=got[j];
        }
        else
        {
          value=atoi(digit.c_str());
          datastructure.push_back(value);
          digit=got[j];
        }
        k=j;
      }
      else
      {
        if(k==(j-1)){   }
      }
    }
    value=atoi(digit.c_str());
    datastructure.push_back(value);
    return true;
  }
  return false;
}

//regex is not fully implemented in libstdc++
template <typename Basic>
bool FileHeader<Basic>::TITLE(std::string& line, std::string& title)
{
  std::regex e ("(.*"+H0+EQ+")(.+)"); //TITLE="Volume data of hospAmix.geo and hospAmix01.gas"
  std::smatch sm;
  std::string got;
  title="No_Title";
  if(std::regex_match (line,sm,e))
  {
    got = sm[2];
    title="";
    for(unsigned int i=1;i<got.size();++i)
    {
      if((got[i]==' ')||(got[i]=='"')||(got[i]=='=')||(got[i]=='\r')) continue;
      title+=got[i];
    }
  }
  return true;
}

