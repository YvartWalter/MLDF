///
/// \file Writer.hpp
///
/*
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef _CLASS_WRITER_

template <typename Basic>
class FileWriter
{
  private:
    std::string header_as_text; //mettre un '/n' à la fin
    std::string footer_as_text;
    
    template<class T> bool GenerateHeader(Format::FORMAT_FILES& f, std::vector<std::string>& variables,
                                          std::vector<unsigned int>& zones, std::string& title,
                                          std::string& cm, char eol, unsigned int prec, std::string& cs,
                                          unsigned int WL);
    
    bool GenerateFooter(void);
    
    bool VARIABLE(std::vector<std::string>& variables, char eol);
    
    bool ZONE(std::vector<unsigned int>& zones, char eol);
    
    ///
    /// cs : column separator
    /// fcb : first col begin, -1 = no
    /// fcy : first colon by : increment
    ///
    template<class T>
    bool ouputs(std::ofstream& file, std::vector< std::vector<T> >& data,
                 std::string& title, std::vector<std::string>& data_name,
                 std::vector<unsigned int>& cols, std::vector<unsigned int>& dimensions,
                 unsigned int& from, unsigned int& by, unsigned int& to,
                 int fcb, int fcy, std::string& cs, std::string& com, char eol, unsigned int prec,
                 Format::FORMAT_FILES& f, bool no_head, bool no_foot);
    
  public:
  
    FileWriter(void):header_as_text(), footer_as_text() { };
    
    FileWriter(const FileWriter<Basic> & FW):header_as_text(), footer_as_text() { };
    
    template<class W>
    FileWriter(const FileWriter<W>& FW):header_as_text(), footer_as_text() { };
    
    virtual ~FileWriter(){ };
    
    template<class T>
    Basic Write(std::string& file_name);
    
    template<class T>
    Basic Replace(std::string& file_name, std::vector< std::vector<T> >& data,
                  std::vector<unsigned int>& dimensions, Format::FORMAT_FILES& f);
    
    template<class T>
    Basic Replace(std::string& file_name, std::string& title, std::vector< std::vector<T> >& data,
                  std::vector<std::string>& data_name, std::vector<unsigned int>& dimensions, Format::FORMAT_FILES& f);
    
    template<class T>
    Basic Replace(std::string& file_name, std::string& title, std::vector< std::vector<T> >& data,
                  std::vector<std::string>& data_name, std::vector<unsigned int>& cols,
                  std::vector<unsigned int>& dimensions, Format::FORMAT_FILES& f);
    
    template<class T>
    Basic Replace(std::string& file_name, std::string& title, std::vector< std::vector<T> >& data,
                  std::vector<std::string>& data_name, std::vector<unsigned int>& cols,
                  std::vector<unsigned int>& dimensions,
                  unsigned int from, unsigned int by, unsigned int to,
                  int firstcolbeg, int firstcolstep, std::string& colsep, std::string& com,
                  char eol, unsigned int prec, Format::FORMAT_FILES& f);
    
    //TODO option append
};


//
// REPLACE : file_name, data, dimensions, format
//
template<typename Basic>
template <class T>
Basic FileWriter<Basic>::Replace(std::string& file_name, std::vector< std::vector<T> >& data,
                                 std::vector<unsigned int>& dimensions, Format::FORMAT_FILES& f)
{
  std::ofstream file( file_name.c_str(), std::ios_base::trunc );
  std::vector<std::string> data_name;
  std::vector<unsigned int> cols;
  unsigned int from=0, by=1, to=0;
  std::string colsep=S_2;//std::string("  ");
  std::string com=SH;//std::string('#');
  std::string title=std::string("untitled");
  for(unsigned int i=0;i<data.size();++i)
  {
    data_name.push_back(std::to_string(i));
    cols.push_back(i);
    if(data[i].size()>=to) to=data[i].size();
  }
  if( !ouputs<T>(file, data, data_name, cols, dimensions, from, by, to,
                 -1, 1, colsep, com, '\n', 6, f, false, false) )
  {
    std::cerr<<"Error writting file "<<file_name<<std::endl;
    return static_cast<Basic>(1);
  }
  std::cerr<<"File "<<file_name<<" created"<<std::endl;
  file.close();
  return static_cast<Basic>(0);
}

//
// REPLACE : file_name, title, data, data_name, dimensions, format
//
template<typename Basic>
template<class T>
Basic FileWriter<Basic>::Replace(std::string& file_name, std::string& title,
                                 std::vector< std::vector<T> >& data,
                                 std::vector<std::string>& data_name,
                                 std::vector<unsigned int>& dimensions, Format::FORMAT_FILES& f)
{
  std::ofstream file( file_name.c_str(), std::ios_base::trunc );
  std::vector<unsigned int> cols;
  unsigned int from=0, by=1, to=0;
  std::string colsep=S_2;//std::string("  ");
  std::string com=SH;//std::string("#");
  for(unsigned int i=0;i<data.size();++i)
  {
    cols.push_back(i);
    if(data[i].size()>=to) to=data[i].size();
  }
  if( !ouputs<T>(file, data, title, data_name, cols, dimensions, from, by, to,
                 -1, 1, colsep, com, '\n', 6, f, false, false) )
  {
    std::cerr<<"Error writting file "<<file_name<<std::endl;
    return static_cast<Basic>(1);
  }
  std::cerr<<"File "<<file_name<<" created"<<std::endl;
  file.close();
  return static_cast<Basic>(0);
}

//
// REPLACE : file_name, title data, data_name, cols, dimensions, format
//
template<typename Basic>
template<class T>
Basic FileWriter<Basic>::Replace(std::string& file_name, std::string& title,
                                 std::vector< std::vector<T> >& data,
                                 std::vector<std::string>& data_name, std::vector<unsigned int>& cols,
                                 std::vector<unsigned int>& dimensions, Format::FORMAT_FILES& f)
{
  std::ofstream file( file_name.c_str(), std::ios_base::trunc );
  unsigned int from=0, by=1, to=0;
  std::string colsep=S_2;//std::string("  ");
  std::string com=SH;//std::string("#");
  for(unsigned int i=0;i<data.size();++i){ if(data[i].size()>=to) to=data[i].size(); }
  if( !ouputs<T>(file, data, title, data_name, cols, dimensions, from, by, to,
                 -1, 1, colsep, com, '\n', 6, f, false, false) )
  {
    std::cerr<<"Error writting file "<<file_name<<std::endl;
    return static_cast<Basic>(1);
  }
  std::cerr<<"File "<<file_name<<" created"<<std::endl;
  file.close();
  return static_cast<Basic>(0);
}

template<typename Basic>
template<class T>
Basic FileWriter<Basic>::Replace(std::string& file_name, std::string& title,
                                 std::vector< std::vector<T> >& data,
                                 std::vector<std::string>& data_name, std::vector<unsigned int>& cols,
                                 std::vector<unsigned int>& dimensions,
                                 unsigned int from, unsigned int by, unsigned int to,
                                 int firstcolbeg, int firstcolstep, std::string& colsep, std::string& com,
                                 char eol, unsigned int prec, Format::FORMAT_FILES& f)
{
  std::ofstream file( file_name.c_str(), std::ios_base::trunc );
  if( !ouputs<T>(file, data, title, data_name, cols, dimensions, from, by, to,
                 firstcolbeg, firstcolstep, colsep, com, eol, prec, f, false, false) )
  {
    std::cerr<<"Error writting file "<<file_name<<std::endl;
    return static_cast<Basic>(1);
  }
  std::cerr<<"File "<<file_name<<" created"<<std::endl;
  file.close();
  return static_cast<Basic>(0);
}

//
//
// INTERNAL
//
//

template<typename Basic>
bool FileWriter<Basic>::VARIABLE(std::vector<std::string>& variables, char eol)
{
  header_as_text+=H1+EQ+GM; //std::string("VARIABLES=\"");
  unsigned int i=0;
  while(i<variables.size()-1)
  {
    header_as_text+=variables[i++]+GM+CO+GM;//std::string("\",\"");
  }
  header_as_text+=variables[i]+std::string(1,eol);
  return true;
}

template<typename Basic>
bool FileWriter<Basic>::ZONE(std::vector<unsigned int>& zones, char eol)
{
  header_as_text+=H2;//std::string("ZONE ");
  unsigned int i=0;
  const char* Z="IJKXYZ\0";
  while(i<zones.size())
  {
    header_as_text+=std::string(1,Z[i])+std::string(1,'=')+std::to_string(zones[i++])+std::string(", ");
  }
  header_as_text+=std::string("F=POINT")+std::string(1,eol);
  return true;
}

template <typename Basic>
template<class T>
bool FileWriter<Basic>::GenerateHeader(Format::FORMAT_FILES& f, std::vector<std::string>& variables,
                                       std::vector<unsigned int>& zones, std::string& title,
                                       std::string& cm, char eol, unsigned int prec, std::string& cs,
                                       unsigned int WL)
{
  header_as_text="";
  unsigned int nl=1, wd=prec+6;
  for(std::vector<unsigned int>::iterator i=zones.begin();i!=zones.end();++i){nl*= *i;}
  if(WL!=nl){std::cerr<<"Write header <"<<title<<"> warning w_line!("<<WL<<")=n_line"<<nl<<std::endl;}
  switch(f)
  {
    //TEXPLOT_EXTENDED:
    //#FUI1333717593
    //#DATASTRUCTURE 7 25
    //VARIABLES="x[m]","v[m/s]","n[1/m3]","t[K]","doppb[m/s]","ne[1/m3]","te[K]"
    //ZONE I= 25 
    case Format::TEXPLOT_EXTENDED:
      header_as_text+=cm+title+std::string(1,eol);
      header_as_text+=cm+H3+std::to_string(variables.size())+std::string(" ")+std::to_string(WL)+std::string(1,eol);
      if( !VARIABLE(variables, eol) ) return false;
      if( !ZONE(zones, eol) ) return false;
      break;
      
    //TEXPLOT:
    //TITLE="Volume data of hospAmix.geo and hospAmix01.gas"
    //VARIABLES="x","y","vx","vy","T","rhoCO","rhoH2O",void
    //ZONE I=61, J=101, F=POINT 
    case Format::TEXPLOT:
      header_as_text+=H0+EQ+GM+title+GM+std::string(1,eol);
      if( !VARIABLE(variables, eol) ) return static_cast<Basic>(1);
      if( !ZONE(zones, eol) ) return false;
      break;
      
    //MIRO:
    //# data class : Xsingle
    //# cell               1               2 
    case Format::MIRO:
      header_as_text+=cm+std::string(" data : ")+title+std::string(1,eol);
      header_as_text+=cm+std::string(" cell")+cs;
      for(unsigned int i=0;i<variables.size();++i)
      {
        int nc=0, cc;
        if(i<10){ nc=1; } else if(i<100){ nc=2;} else if(i<1000){ nc=3;} else if(i<1000){ nc=4;} else{nc=5;}
        cc=nc;
        while(cc<wd){ header_as_text+=std::to_string(' '); }
        header_as_text+=std::to_string(i)+cs;
      }
      header_as_text+=std::string(1,eol);
      
    //MAMOJ:
    //#       1        2        3        4        5 
    //#       x        y       vx       vy    rhoCO
    case Format::MAMOJ:
      header_as_text+=cm;
      for(unsigned int i=0;i<variables.size();++i)
      {
        int nc=0, cc;
        if(i<10){ nc=1; } else if(i<100){ nc=2;} else if(i<1000){ nc=3;} else if(i<1000){ nc=4;} else{nc=5;}
        cc=nc;
        while(cc<wd){ header_as_text+=std::to_string(' '); }
        header_as_text+=std::to_string(i)+cs;
      }
      header_as_text+=std::string(1,eol);
      header_as_text+=cm;
      for(unsigned int i=0;i<variables.size();++i)
      {
        header_as_text+= variables[i].substr( 0, ((wd<variables[i].size())?wd:variables[i].size()-1) )+cs; 
      }
      header_as_text+=std::string(1,eol);
      
    //RAW:
    case Format::RAW:
      header_as_text="";
      break;
      
    default:
      header_as_text="";
      break;
  }
  return true;
}

template <typename Basic>
bool FileWriter<Basic>::GenerateFooter(void)
{
  footer_as_text="";
  return true;
}

template <typename Basic>
template<class T>
bool FileWriter<Basic>::ouputs(std::ofstream& file, std::vector< std::vector<T> >& data,
                               std::string& title, std::vector<std::string>& data_name,
                               std::vector<unsigned int>& cols, std::vector<unsigned int>& dimensions,
                               unsigned int& from, unsigned int& by, unsigned int& to,
                               int fcb, int fcy, std::string& cs, std::string& com, char eol, unsigned int prec,
                               Format::FORMAT_FILES& f, bool no_head, bool no_foot)
{
  std::vector<std::string> variables;
  unsigned int wd=prec+6, l, WL;
  int fc = fcb, NL=0;
  for(std::vector<unsigned int>::iterator c=cols.begin(); c!=cols.end();++c)
  {
    variables.push_back(data_name[*c]);
    if(data.size()< *c) return false;
    if(data[*c].size()>NL) NL=data[*c].size();//nombre total de lignes dans data>=nombre total de lignes à ecrire
  }
  if(from>=NL){ return false; }
  if(to>NL) to=NL;
  WL=0; l=from; while(l<to){l+=by; ++WL;};
  if(!no_head)
  {
    if( !GenerateHeader<T>(f, variables, dimensions, title, com, eol, prec, cs, WL) ) return false;
    file<<header_as_text<<std::flush;
  }
  file<<std::setprecision(prec)<<std::scientific;
  l=from;
  while(l<to)
  {
    std::vector<unsigned int>::iterator c=cols.begin();
    if(fcb>=0)
    {
      if(f==Format::MIRO){ file<<std::setw(6)<<fc<<cs;}
      else{ file<<std::setw(wd)<<static_cast<T>(fc)<<cs;}
      fc+=fcy;
    }
    while(c!=cols.end())
    {
      if(data.size()<= *c) continue;
      if(data[*c].size()>l)
      {
        file<<std::setw(wd)<<data[*c][l]<<cs;
      }
      else
      {
        file<<std::setw(wd)<<static_cast<T>(0)<<cs;
      }
      ++c;
    }
    file<<eol<<std::flush;
    l+=by;
  }
  file<<std::setprecision(6);//default
  if(!no_foot)
  {
    if( !GenerateFooter() ) return false;
    file<<footer_as_text<<std::flush;
  }
  return true;
}


#define _CLASS_WRITER_
#endif

