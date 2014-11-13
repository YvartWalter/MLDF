
#include "Physical_constants.hpp"

template<typename Basic_type, typename Integr_type>
class SolutionFileAnalyser
{
  public:
    
    SolutionFileAnalyser(void);
    SolutionFileAnalyser(char c, char m, unsigned int i);
    SolutionFileAnalyser(const SolutionFileAnalyser& c);
    ~SolutionFileAnalyser();
    
    template<class FileStream>
    Integr_type AngleCoverage( FileStream& f, Basic_type* a_min, Basic_type* a_max,
                               Basic_type* r_max, Basic_type* r_min,
                               Integr_type c1, Integr_type c2, Integr_type c3)
    {
      return GetA<FileStream>(f, a_min, a_max, r_min, r_max,
                              static_cast<int>(c1), static_cast<int>(c2), static_cast<int>(c3));
    };
  
    template<class FileStream>
    Integr_type AngleCoverage( FileStream& f, Basic_type* a_min, Basic_type* a_max,
                               Basic_type* r_max, Basic_type* r_min,
                               Integr_type c1, Integr_type c2)
    {
      return GetA<FileStream>(f, a_min, a_max, r_min, r_max,
                              static_cast<int>(c1), static_cast<int>(c2), -1);
    };
  
    template<class FileStream>
    Integr_type AngleCoverage( FileStream& f, Basic_type* a_min, Basic_type* a_max,
                               Basic_type* r_max, Basic_type* r_min,
                               Integr_type c1)
    {
      *a_min = static_cast<Basic_type>(0);
      *a_max = static_cast<Basic_type>(0);
      return GetA<FileStream>(f, a_min, a_max, r_min, r_max, static_cast<int>(c1), -1, -1);
    };
  private:
    template<class FileStream>
    Integr_type GetA(  FileStream& f,
                       Basic_type* a_min, Basic_type* a_max,
                       Basic_type* r_max, Basic_type* r_min,
                       int c1, int c2, int c3);
    int maximum(int c1, int c2, int c3){return (c1>c2)?((c1>c3)?c1:c3):((c2>c3)?c2:c3);}
    char nline, comment;
    unsigned int ignore;
};


template<typename Basic_type, typename Integr_type>
SolutionFileAnalyser<Basic_type,Integr_type>::SolutionFileAnalyser(void):
nline('\n'), comment('#'), ignore(0)
{ }

template<typename Basic_type, typename Integr_type>
SolutionFileAnalyser<Basic_type,Integr_type>::SolutionFileAnalyser(char c, char m, unsigned int i):
nline(c), comment(m), ignore(i)
{ }

template<typename Basic_type, typename Integr_type>
SolutionFileAnalyser<Basic_type,Integr_type>::SolutionFileAnalyser(const SolutionFileAnalyser& c):
nline(c.nline), comment(c.comment), ignore(c.ignore)
{ }

template<typename Basic_type, typename Integr_type>
SolutionFileAnalyser<Basic_type,Integr_type>::~SolutionFileAnalyser()
{ }
 
///
/// \brief Regarde un fichier solution et donne le Rmin, Rmax, angle_min et anglemax (entre 0:2Pi)
///
/// ci<0 => on n'utilise pas cette colonne, les -1 toujours à la fin, c1 forcément > 0
///
template<typename Basic_type, typename Integr_type>
template<class FileStream>
Integr_type SolutionFileAnalyser<Basic_type,Integr_type>::GetA( FileStream& f,
                                                                 Basic_type* a_min, Basic_type* a_max,
                                                                 Basic_type* r_max, Basic_type* r_min,
                                                                 int c1, int c2, int c3)
{
  std::streampos start = f.tellg();
  std::string l;
  unsigned int j,i=ignore, C=maximum(c1,c2,c3), D=1;
  unsigned int counted=0;
  Basic_type r, x[3], nul=static_cast<Basic_type>(0), sina, cosa, A, d;
  Basic_type pi=static_cast<Basic_type>(constants::PI);
  if(c2>0) D++;
  if(c3>0) D++;
  *a_min=pi+pi;
  *a_max=static_cast<Basic_type>(0);
  while( std::getline( f, l, nline ))
  {
    if( i>0 ){ i--; continue; }
    char c=l[0];
    if( c==comment ) continue;
    std::istringstream getsub( l );
    j=0;
    x[0]=nul; x[1]=nul; x[2]=nul;
    for(int c=0;c<C;++c)
    {
      getsub>>r;
      x[j++] = r;
    }
    if(D==3) //3D
    {
      Basic_type a = x[1]*x[1]+x[2]*x[2];
      d = a+x[3]*x[3]; //hypothénuse
      d = static_cast<Basic_type>(sqrt( static_cast<double>(d)));
      a = static_cast<Basic_type>(sqrt( static_cast<double>(a)));
      sina = x[3]/d;
      cosa = a/d;
      if(counted==0){ *r_min = d; }
    }
    else if(D==2)//2D
    {
      d = x[1]*x[1]+x[0]*x[0];
      d = static_cast<Basic_type>(sqrt( static_cast<double>(d)));
      sina = x[1]/d;
      cosa = x[0]/d;
    }
    else
    {
      d=static_cast<Basic_type>( fabs( static_cast<double>(x[0]) ) );
    }
    //angle entre 0 et 2 PI
    if(D>1)
    {
      A = acos( static_cast<double>(cosa));
      if( sina<0 )
      {
        A=pi+pi-A;
      }
      if(A>= *a_max) *a_max=A;
      if(A<= *a_min) *a_min=A;
    }
    if(d<= *r_min) *r_min=d;
    if(d>= *r_max) *r_max=d;
    counted++;
  }
  //Appeler ici le rembobineur
  f.clear();
  f.seekg (start);
  return counted;
}

typedef SolutionFileAnalyser<double, unsigned int> dbui_SolFA;
typedef SolutionFileAnalyser<double, int> dbI_SolFA;
