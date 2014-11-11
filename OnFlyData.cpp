#ifndef _CLASS_MATRIX_

//#define _DEBUG_MATRIX_STORAGE_
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
///
/// \class MatrixId
///
/// Une utilisation du type : data.ID.Grid = data.new_data(Phys.G->xc, Phys.dim_Grid);
///
template< class Tcount>
class MatrixId
{
  public:
    MatrixId(void):Grid(0), Core(0), T(), O(), X(), B(), Pop(),  beta(), tex(),
    Yi(0), Xi(0), Bi(0), I(0), ID(0), Error(0)
    {};
    
    Tcount Grid, Core, ;
    std::vector<Tcount> T, O, X, B,  Pop, beta, tex;
    
    Tcount Yi, Xi, Bi, I;
    Tcount ID;
    Tcount Error;
    
    inline Tcount operator++(void){ return ++ID;};

};

///
/// \class SingleMatrix
///
/// Fourni une classe de storage pour une matrice basée sur la classe std::vector
/// mat[i] = colonne (ou ligne) contenant des vector\<T\>.
/// La taille des matrices s'adapte aux besoins.
///
template<typename T>
class SingleMatrix
{
  friend class ModelMatrix<T>;
  private:
    typedef std::vector<T> Double;  //vecteurs
    unsigned int m_nelem;
    std::vector< Double > m_mat;
    std::string m_name;
    typename std::vector< Double >::iterator m_i;
    unsigned int m_maxrow;
  public:
    //static int ID; //compteur de matrices
    
  /*!
   * \brief Constructeur par défaut
   */
  SingleMatrix(void):
  m_nelem(0), m_mat(), m_name(""), m_i(0), m_maxrow(0)
  { };
  
  /*!
   * \brief Constructeur par copie
   */
  SingleMatrix(const SingleMatrix& C):
  m_nelem(0), m_mat(C.m_mat), m_name(C.m_name), m_i(m_mat.begin()), m_maxrow(C.m_maxrow)
  {
    m_nelem=0;
    for(unsigned int i=0;i<m_mat.size();++i) m_nelem+=m_mat[i].size();
  };
  
  /*!
   * \brief Renvoie le nombre d'élément total (mis à jour lors de chaque modification de l'objet)
   */
  inline unsigned int size(void) const {return m_nelem;};
  
  /*!
   * \brief Connaître le nombre de colonnes
   */
  inline unsigned int cols(void) const {return m_mat.size();};
  
  /*!
   * \brief Connaître le plus grand des nombres de lignes : MAX(m_mat[:].size()), mis à jours lors de chaque modification de l'objet
   */
  inline unsigned int rows(void)const {return m_maxrow;};
  
  /*!
   * \brief Ajouter une colonne (ou ligne) à la matrice, passée par T* 
   */
  unsigned int push_new( T* x, unsigned int s )
  {
    if(s>=m_maxrow) m_maxrow=s;
    m_mat.emplace_back( Double() );
    m_i = m_mat.end()-1;
    for(unsigned int j=0;j<s;++j){ m_i->push_back( x[j]); ++m_nelem;}
    return std::distance(m_mat.begin(), m_i);
  };
  
  /*!
   * \brief ajouter une ligne passée par itérateur
   */
  template <class Iter>
  unsigned int push_new( Iter start, Iter stop )
  {
    m_mat.emplace_back( Double() );
    m_i = m_mat.end()-1;
    while ( start != stop ){ m_i->push_back((*start++)); ++m_nelem;}
    return std::distance(m_mat.begin(), m_i);
  };
  
  /*!
   *  \brief ajouter une ligne passée par vecteur
   */
  unsigned int push_new( std::vector<T>& x )
  {
    if(x.size()>=m_maxrow) m_maxrow=x.size();
    m_mat.emplace_back( x );
    m_nelem+=x.size();
    return std::distance(m_mat.begin(), m_mat.end()-1);
  };
  
  /*!
   * \brief Vider une matrice sauf sa première colonne
   */
  void purge(void)
  {
    Double save;
    save = m_mat[0];
    m_mat.clear();
    m_mat.push_back(save);
    m_nelem=save.size();
  };
  
  /*!
   * \brief Vider une matrice complètement
   */
  void clear(void)
  {
    m_mat.clear();
    m_nelem=0;
  };
  
  /*!
   * \brief Accéder à un élément (i,j) ou i est l'indice de ligne mat[j][i]
   * et j l'indice de colonne mat[j], si l'élément n'existe pas alors renvoie 0
   */
  T at(unsigned int line, unsigned int col)
  {
    if(col>=m_mat.size())
    {
      return static_cast<T>(0);
    }
    if(line>=m_mat[col].size())
    {
      return static_cast<T>(0);
    }
    return m_mat[col][line];
  };
  
  /*!
   * \brief Ecrit une matrice mat[][] complète dans un fichier (passé par un flux)
   */
  SingleMatrix<T>& print_one(std::ofstream& f)
  {
    f<<"# data class : "<<m_name<<std::endl;
    f<<"# cell ";
    for(unsigned int i=0;i<m_mat.size();i++)
    {
      f<<std::setw(15)<<i+1<<" ";
    }
    f<<std::endl;
    for(unsigned int line=0;line<m_maxrow;line++)
    {
      for(unsigned int col=0;col<m_mat.size();col++)
      {
        if(!col)
	{
	  f<<std::setw(6)<<std::right<<line<<" ";
          f<<std::scientific<<std::setw(15)<<at(line,0)<<" ";
	}
	else
	{
          f<<std::scientific<<std::setw(15)<<at(line,col)<<" ";
	}
      }
      f<<std::endl;
    }
    f<<std::endl;
    return *this;
  };
  
  /*!
   * \brief Ecrire le contenu de la matrice sur std::cout
   */
  SingleMatrix<T>& ShowContent(void)
  {
    std::cout<<"# data class : "<<m_name<<" "<<m_mat.size()<<"    "<<m_maxrow<<std::endl;
    std::cout<<"# cell ";
    for(int i=0;i<m_mat.size();i++)
    {
      std::cout<<std::setw(15)<<i<<" ";
    }
    std::cout<<std::endl;
    for(unsigned int line=0;line<m_maxrow;line++)
    {
      for(int col=0;col<m_mat.size();col++)
      {
        if(!col)
	{
	  std::cout<<line<<" ("<<col<<" =>"<<m_mat[col][line]<<" = "<<at(line,0)<<") ";
	}
	else
	{
	  std::cout<<" ("<<col<<" =>"<<m_mat[col][line]<<" = "<<at(line,col)<<") ";
	}
      }
      std::cout<<std::endl;
    }
    std::cout<<std::endl;
    return *this;
  };
  
  /*!
   * \brief Ajoute une nouvelle colonne en sommant toutes les colonnes sauf la première (abscisses)
   * et la positionne en position donnée par location
   */
  void Sum_1(unsigned int location){ Sum(location, 1); };
  
  /*!
   * \brief Ajoute une nouvelle colonne contenant la somme de toutes les colonnes
   * et la positionne en position donnée par location
   */
  void Sum_a(unsigned int location){ Sum(location, 0); };
  
  /*!
   * \brief Ajoute une nouvelle colonne contenant la somme des colonnes de firstcol à la dernière
   * et la positionne à la position location
   */
  void Sum(unsigned int location, unsigned firstcol)
  {
    Double newcol;
    for(unsigned int line=0;line<m_maxrow;line++)
    {
      newcol.push_back(static_cast<T>(0));
      for(unsigned int col=firstcol;col<m_mat.size();col++)
      {
	newcol[line] = newcol[line] + at(line,col);
      }
    }
    typename std::vector< Double >::iterator it;
    if(location>=m_mat.size())
    {
      it=m_mat.end();
    }
    else
    {
      it=m_mat.begin();
      for(int i=0;i<location;++i)
      {
	++it;
      }
    }
    m_mat.insert(it, newcol);
    m_nelem+=newcol.size();
  };
  
}; //template<typename T> class SingleMatrix


//template<typename T> SingleMatrix<T>::ID=0;

///
/// \class ModelMatrix
/// Fournie une classe de gestion d'un vecteur de matrices SingleMatrix\<T\>,
/// permet d'avoir un conteneur unique pour interfacer les différentes matrices
/// de sauvegarde des données. Abscisses et données ne sont pas séparées. Les colonnes peuvent
/// avoir des tailles différentes. Conteneur polyvalent.
///
template<typename T>
class ModelMatrix
{
  private:
    typedef std::vector<T> Double;
    typedef std::vector< SingleMatrix<T> > Data;
    Data m_All;
    unsigned int m_id;
    int auto_ID;
  public:
  //Données
  MatrixId<unsigned int> ID;

  /*!
   * \brief Constructeur par défaut
   */
  ModelMatrix(void):m_All(), m_id(0), auto_ID(0), ID() {};
  
  /*!
   * \brief Constructeur par copie
   */
  ModelMatrix(const ModelMatrix& C):m_All(C.m_All), m_id(C.m_id), auto_ID(C.auto_ID+1), ID(C.ID) {};
  
  /*!
   * \brief Constructeur par copie polymorphe
   */
  template <typename U>  
  ModelMatrix(const ModelMatrix<U>& C):m_All(dynamic_cast<T>(C.m_All)), m_id(C.m_id) {};
  
  /*!
   * \brief Afficher le contenu du conteneur (débug)
   */
  void See(void)
  {
    std::cout<<"------------------------------"<<std::endl;
    for(int i=0;i<m_All.size();++i)
    {
      std::cout<<"Element "<<i<<" named : '"+m_All[i].name+"' rows="<<m_All[i].rows()
      <<" cols="<<m_All[i].cols()<<std::endl;
      std::cout<<"CONTENT :"<<std::endl;
      m_All[i].ShowContent();
    }
    std::cout<<"------------------------------"<<std::endl;
  }
  
  /*!
   * \brief Ajouter une nouvelle matrice au conteneur, cet ajout se fait
   * par l'entrée d'une première colonne qui est l'asbcisse ou simplement la première colonne
   * de données.
   */
  unsigned int new_data(T* X, unsigned int s)
  {
    SingleMatrix<T> newobject;
    newobject.name = "not defined";
    newobject.push_new(X,s);
    m_All.push_back( newobject );
    m_id=m_All.size()-1;
#ifdef _DEBUG_MATRIX_STORAGE_
    std::cout<<"[ModelMatrix] add new id="<<m_id<<" -->"<<m_All.size()
             <<" -- size="<<m_All.back().size()
	     <<"  - rows="<<m_All.back().rows()
	     <<"  - cols="<<m_All.back().cols()
	     <<std::endl;
#endif
    return m_id;
  };
  
  /*!
   * \brief Créer une nouvelle matrice en donnant un nom et en passant une première colonne de données
   */
  unsigned int new_data(T* X, unsigned int s, const std::string& nm)
  {
    SingleMatrix<T> newobject;
    newobject.m_name = nm;
    newobject.push_new(X,s);
    m_All.push_back( newobject );
    m_id=m_All.size()-1;
#ifdef _DEBUG_MATRIX_STORAGE_
    std::cout<<"[ModelMatrix] add new with name(const) '"+m_All[m_id].name+"' id="<<m_id<<" -->"<<m_All.size()
             <<" -- size="<<m_All.back().size()
	     <<"  - rows="<<m_All.back().rows()
	     <<"  - cols="<<m_All.back().cols()
	     <<std::endl;
    for(int i=0;i<s;++i) std::cout<<i<<" "<<X[i]<<std::endl;
    std::cout<<"=>Object content ["<<m_id<<"] is :"<<std::endl;
    m_All[m_id].ShowContent();
    std::cout<<"******"<<std::endl;
#endif
    return m_id;
  };
  
  ///
  /// \brief Créer une nouvelle matrice en donnant un nom et en passant une première colonne de données
  ///
  unsigned int new_data(T* X, unsigned int s, std::string& nm)
  {
    SingleMatrix<T> newobject;
    newobject.name = nm;
    newobject.push_new(X,s);
    m_All.push_back( newobject );
    m_id=m_All.size()-1;
#ifdef _DEBUG_MATRIX_STORAGE_
    std::cout<<"[ModelMatrix] add new with name '"+m_All[m_id].name+"' id="<<m_id<<" -->"<<m_All.size()
             <<" -- size="<<m_All.back().size()
	     <<"  - rows="<<m_All.back().rows()
	     <<"  - cols="<<m_All.back().cols()
	     <<std::endl;
#endif
    return m_id;
  };
  
  ModelMatrix<T>& name_one(unsigned int loc, std::string& nm)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].name=nm;
    return *this;
  };
  
  ///
  /// \brief Nommer une matrice déjà créée
  ///
  ModelMatrix<T>& name_one(unsigned int loc, const std::string& nm)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].name=nm;
#ifdef _DEBUG_MATRIX_STORAGE_
    std::cout<<"Name ["<<loc<<"]"+m_All[loc].name<<std::endl;
#endif
    return *this;
  };
  
  ///
  /// \brief Ajouter une colonne à une matrice déjà contenue
  ///
  ModelMatrix<T>& push_at(unsigned int loc, T* X, unsigned int s)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].push_new(X,s);
#ifdef _DEBUG_MATRIX_STORAGE_
    std::cout<<"Push data of size "<<s<<" to ["<<loc<<"]"+m_All[loc].name<<std::endl;
    for(int i=0;i<s;++i) std::cout<<i<<" "<<X[i]<<std::endl;
    std::cout<<"Object content ["<<loc<<"] is :"<<std::endl;
    m_All[loc].ShowContent();
#endif
    return *this;
  };
  
  ///
  /// \brief Ajouter une matrice de colonnes à un conteneur déjà existant
  ///
  ModelMatrix<T>& push_rowmajor(unsigned int loc, T* X, unsigned int cols, unsigned int lines)
  {
    if(loc>=m_All.size()) return *this;
    for(unsigned int i=0;i<cols;++i)
    {
      m_All[loc].push_new(&X[i*lines],cols);   //FIXME
    }
    return *this;
  }
  ModelMatrix<T>& push_colmajor(unsigned int loc, T* X, unsigned int cols, unsigned int lines)
  {
    if(loc>=m_All.size()) return *this;
    for(unsigned int i=0;i<lines;++i)
    {
      m_All[loc].push_new(&X[i*cols],lines);   //FIXME
    }
    return *this;
  }
  
  ///
  /// \brief Imprimer dans un fichier un seule des matrices contenues (surchargée fichier-flux)
  ///
  /// \param loc : ( unsigned int ) indice de la matrice, permet- d'accéder à la matrice,
  /// la classe MatrixId permet d'accéder à des éléments enregistrés
  /// \param file : ( std::string& ) nom du fichier qui sera remplacé par le contenu
  /// de la matrice sélectionnée.
  ///
  ModelMatrix<T>& print_one(unsigned int loc, std::string& file)
  {
    if(loc>=m_All.size()) return *this;
    std::ofstream f(file);
    m_All[loc].print_one(f);
    f.close();
    return *this;
  };
  /// \param loc : ( unsigned int ) indice de la matrice
  /// \param f : ( std::ofstream& ) flux sur un fichier, les données seont ajoutées au flux
  ///
  ModelMatrix<T>& print_one(unsigned int loc, std::ofstream& f)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].print_one(f);
    return *this;
  };
  
  ///
  /// \brief Purger une matrice stoquée, la première colonne est conservée
  ///
  void purge_one(unsigned int loc)
  {
    if(loc<m_All.size()) m_All[loc].purge();
  };
  
  ///
  /// \brief Vider complètement une matrice
  ///
  void clear_one(unsigned int loc)
  {
    if(loc<m_All.size()) m_All[loc].clear();
  };
  
  ///
  /// \brief Vider completement toutes les matrices
  ///
  void Clear(void)
  {
    for(unsigned int loc=0;loc<m_All.size();m_All[loc++].clear());
  }
  ///
  /// \brief Sommer toutes les colonnes d'un élément et insérer la colonne contenant
  /// la somme au début ou à la fin
  ///
  ModelMatrix& SumColsWithAbscissesFirst(unsigned int loc)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].Sum_1(1);
    return *this;
  };
  
  ModelMatrix& SumColsWithAbscissesLast(unsigned int loc)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].Sum_1(m_All[loc].size());
    return *this;
  };
  
  ModelMatrix& SumColsFirst(unsigned int loc)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].Sum_a(1);
    return *this;
  };
  
  ModelMatrix& SumColsLast(unsigned int loc)
  {
    if(loc>=m_All.size()) return *this;
    m_All[loc].Sum_a(m_All[loc].size());
    return *this;
  };
  
};

typedef ModelMatrix<float> Fdata;
typedef ModelMatrix<double> Ddata;
typedef ModelMatrix<long double> LDdata;
typedef ModelMatrix<int> Idata;
typedef ModelMatrix<int> Idata;

#ifdef _DEBUG_MATRIX_STORAGE_
#undef _DEBUG_MATRIX_STORAGE_
#endif

#define _CLASS_MATRIX_
#endif
