///
/// HEADER
///

///
/// Namespace
///
// namespace alr {
  
  ///
  /// \class Operation 
  /// modification des données pour un seul vecteur Y
  ///
  template<typename T>
  class Operation
  {
  private:
    T* data;
    unsigned int Len;
    
    virtual int see(unsigned int& from, unsigned int& to)
    {
      if( (data==0) || (Len==0) )
      {
        std::cerr<<"No operation applicatble"<<std::endl;
        return -1;
      }
      if(from>=Len) return -1;
      if(to>=Len) to = Len-1;
      return 0;
    };
    
  public:
    Operation(void):data(0), Len(0) { };
    Operation(T* d, unsigned int s): data(d), Len(s) { };
    Operation(const Operation<T>& s): data(s.data), Len(s.Len) { };
    virtual ~Operation(){};
    
   
    ///
    /// \brief Ajout d'une constante : A+a
    ///
    int Add(T& a, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=from;
      while(u<to) { data[u]+=a; u+=by; }
      return u;
    };
    
    ///
    /// \brief Multiplication par une constante A*a
    ///
    int Mul(T& a, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=from;
      while(u<to) { data[u]*=a; u+=by; }
      return u;
    };
    
    ///
    /// \brief Ajouter une fonction de l'indice A+ f(k)
    ///
    template<class FunctionOperator>
    int Add(FunctionOperator& F, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=from;
      while(u<to) { data[u]+=F(u); u+=by; }
      return u;
    };
    
    ///
    /// \brief Multiplication par une fonction de l'indice A+ f(k)
    ///
    template<class FunctionOperator>
    int Mult(FunctionOperator& F, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=from;
      while(u<to) { data[u]*=F(u); u+=by;}
      return u;
    };
    
    ///
    /// \brief Retourne le polynome de A, la polynome est passé par ses coeffs, le coeffs de degré
    /// le plus faible en premier, sur tout l'intervalle de définition ou plus petit
    ///
    int Poly(T* ai, unsigned int ncoef, unsigned int from=0, unsigned int to=0)
    {
      if( see(from, to) ){return -1;}
      if(to==0) to=Len;
      int u=from;
      while(u<to)
      {
        T v=data[u], V=0.0;
        for(unsigned int i=0;i<ncoef;++i)
        {
          T U=1.0;
          unsigned int j=1;
          while(j<=i){ U*=v; ++j; }
          V+=U*ai[i];
        }
        data[u]=V;
        ++u;
      }
      return u;
    };

    ///
    /// \brief Décalage circulaire des indices de k A[k_0 mod N]->A[k_0+k mod N]
    /// dans la limite de l'intervalle [from;to[
    ///
    int circ(unsigned int k, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=0, U=to-from;
      while(u<U) { data[from+u]=data[from+((u+k)%U)]; u+=by; }
      return u;
    };
    
    ///
    /// \brief Décalage circulaire d'indices sur l'intervalle [from:to[
    /// dont les valeurs pevent explorer tout l'intervalle de définition
    ///
    int Circ(unsigned int k, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=from;
      while(u<to) { data[u]=data[((u+k)%to)]; u+=by; }
      return u;
    };
    
    ///
    /// \brief Décalage d'indices u->u+k sur l'intervalle [from:to[
    ///
    int shift(unsigned int k, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      int u=0, U=to-from, i=0;
      while(u<U)
      {
        i=u+k;
        data[from+u]= data[ from+((i<U)?i:U-1) ];
        u+=by;
      }
      return u;
    };
    
    ///
    /// \brief Décallage d'indice u->u+k sur l'intervalle [from:to[
    /// dont les valeurs pevent explorer tout l'intervalle de définition
    ///
    int Shift(unsigned int k, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(to==0) to=Len;
      if(by==0) return 0;
      int u=from, U=0;
      while(u<to) { U=u+k;data[u]=data[((U<Len)?U:Len-1)]; u+=by; }
      return u;
    };
    
    ///
    /// \brief Opération non linéaire A+a*(k-k_n)/(k_n-k_0)/b
    ///
    int NL_bnd(T& a, unsigned int from=0, unsigned int to=0)
    {
      if( see(from, to) ){return -1;}
      if(to==0) to=Len;
      int u=from;
      T dv=static_cast<T>(to-from); dv=((dv<1e-6)?1e-6*a:dv*a);
      while(u<to)
      {
        data[u]+= static_cast<T>(u-to)/dv;
        ++u;
      }
      return u;
    };
  
    ///
    /// \brief Opération non linéaire
    ///
    int NL_mid(T& a, unsigned int mid, unsigned int from=0, unsigned int to=0)
    {
      if( see(from, to) ){return -1;}
      if(to==0) to=Len;
      if((mid>=to)||(mid<from)){mid=(unsigned int)((double)(to+from)/2.0);}
      int u=from;
      T dv=static_cast<T>(to-mid); dv=((dv<1e-6)?1e-6*a:dv*a);
      while(u<to)
      {
        data[u]+= fabs(static_cast<T>(u-mid))/dv;
        ++u;
      }
      return u;
    };
    
    ///
    /// \brief Ajouter une composante aléatoire (uniforme)
    ///
    int RAdd_uniform(T& a, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      std::mt19937_64 gen( std::random_device() );
      std::uniform_real_distribution<T> dist_U();
      int u=from;
      while(u<to) { data[u]+=a*dist_U(gen); u+=by;}
      return u;
    };
    
    ///
    /// \brief Multiplier par une composante aléatoire (uniforme)
    ///
    int RMult_uniform(T& a, unsigned int from=0, unsigned int to=0, int by=1)
    {
      if( see(from, to) ){return -1;}
      if(by==0) return 0;
      if(to==0) to=Len;
      std::mt19937_64 gen( std::random_device() );
      std::uniform_real_distribution<T> dist_U();
      int u=from;
      while(u<to) { data[u]*=a*dist_U(gen); u+=by;}
      return u;
    };
  
};

template<class BasedVectorMatrix, typename BaseType>
bool TestBalanced(BasedVectorMatrix& Data,
                  unsigned int base, unsigned int values,
                  BaseType a
                 )
{
  Operation<BaseType> Gen( Data[base].data(), Data[base].size() );
  return Gen.Mul(a);
};



///
/// \brief Récupère les données (Data) et les entêtes (Label) pour toutes les colonnes disponibles
/// renvoie aussi le nombre de lignes de données, d'entête et le nombre de colonnes
///
template<class BasedVectorMatrix>
bool LoadSimpleSolution(const std::string& file_name,
                        unsigned int& n_head, unsigned int& n_cols, unsigned int& n_rows,
                        BasedVectorMatrix& Data,
                        std::vector< std::string >& labels )
{
  int status=0;
  std::ifstream file( file_name.c_str() );
  n_cols=0;
  n_head=0;
  n_rows=0;
  if( file.fail() )
  {
    std::cerr<<"\E[31mCan't open file "<<file_name<<"\E[0m"<<std::endl;
    return true;
  }
  FileHeader<int> Head;
  FileReader<int> Read;
  std::string title_line;
  std::vector< unsigned int > colrow, dimensions, Sel_col;
  std::vector< double > col_mult;
  unsigned int Nlines=0, From=0, By=1;
  
  labels.clear();
  status = Head.Header(file, title_line, colrow, dimensions, labels, Format::TEXPLOT);
  if( colrow.size()!=3 ) status=0;
  if( !status )
  {
    std::cerr<<"\E[31mError reading header "<<file_name<<"\E[0m"<<std::endl;
    return false;
  }
  n_cols = colrow[0];
  n_rows = colrow[1];
  n_head = colrow[2];
  for(unsigned int i=0;i<n_head;++i)
  {
    std::string d;
    std::getline(file, d);
  }
  for(unsigned int i=0;i<n_cols;++i)
  {
    col_mult.push_back( 1.00e0 );
    Sel_col.push_back(i);
  }
  Data.clear();
  status = Read.extr<double>( file, labels.size(), Nlines, Sel_col, From, By,
                              Data, col_mult, 
                              std::bind2nd(std::greater_equal<double>(),1.0),
                              VOp_norm2<double, unsigned int>(Sel_col[0],1.0e-30) );
  if( !Nlines || ( Data[0].size() != n_rows ) || ( Data.size()!=n_cols ))
  {
    std::cerr<<"\E[31mError reading data "<<file_name<<" "<<Nlines<<" "<<Data[0].size()<<" "<<n_rows<<" "<<n_cols<<"\E[0m"<<std::endl;
    return false;
  }
  std::cout<<"\E[34mRead "<<Nlines<<"/"<<n_rows<<" data lines\E[0m"<<std::endl;
  file.close();
  return true;
};



// }; //end namespace
