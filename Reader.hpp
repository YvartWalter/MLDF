///
/// READER
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
#ifndef _CLASS_READER_
///
/// \class VOperator
///
/// Operations applyed to a vector
/// Applique l'opération norme 2 fois sqrt(Cte) sur 1, 2 ou 3 composantes sélectionnées à
/// la création de l'objet par I0, I1 et I2 d'un vecteur pouvant comporter plus de 3 composantes
/// \todo Des optimisations peuvent être faites ici
///
template<typename Basic_type, typename Index_type>
class VOp_norm2
{
  public:
    ///
    /// \brief Constructeur par défaut (I0=0, size=1), sous vecteur de dimension 1 : scalaire
    ///
    VOp_norm2(Index_type I0, Basic_type Cte);
    
    ///
    /// \brief Constructeur pour un sous vecteur de dimension 2
    ///
    VOp_norm2(Index_type I0, Index_type I1, Basic_type Cte);
    
    ///
    /// \brief Constructeur pour un sous vecteur de dimension 3
    ///
    VOp_norm2(Index_type I0, Index_type I1, Index_type I2, Basic_type Cte);
    virtual ~VOp_norm2();
    
    ///
    /// \brief Renvoie la norme 2 du sous vecteur maximal (I0, I1, I2) de dimension size du
    /// vecteur passé par itérateur
    ///
    template<class InputIterator>
   Basic_type operator()(InputIterator first, InputIterator last);
  private:
    Index_type i0, i1, i2;
    Basic_type cte;
    unsigned int size;
};

//
// Constructeur par defaut
//
template<typename Basic_type, typename Index_type>
VOp_norm2<Basic_type, Index_type>::VOp_norm2(Index_type I0 = static_cast<Index_type>(0), Basic_type Cte = static_cast<Basic_type>(0)):
i0(I0), i1(static_cast<Index_type>(0)), i2(static_cast<Index_type>(0)), cte(Cte), size(1)
{  }

template<typename Basic_type, typename Index_type>
VOp_norm2<Basic_type, Index_type>::VOp_norm2(Index_type I0, Index_type I1, Basic_type Cte):
i0(I0), i1(I1), i2(static_cast<Index_type>(0)), cte(Cte), size(2)
{  }

template<typename Basic_type, typename Index_type>
VOp_norm2<Basic_type, Index_type>::VOp_norm2(Index_type I0, Index_type I1, Index_type I2, Basic_type Cte):
i0(I0), i1(I1), i2(I2), cte(Cte), size(3)
{  }

template<typename Basic_type, typename Index_type>
VOp_norm2<Basic_type, Index_type>::~VOp_norm2()
{  }

template<typename Basic_type, typename Index_type>
template<class InputIterator>
Basic_type VOp_norm2<Basic_type, Index_type>::operator()(InputIterator first, InputIterator last)
{
  Basic_type N = static_cast<Basic_type>(0);
  Index_type i = static_cast<Index_type>(0);
  while(first!=last)
  {
    Basic_type n;
    // TODO remplacer par fonction de norme ici
    if( i==i0 ){ n = *first; N=N+n*n; }
    if(size<=1) if( i==i1 ){ n = *first; N=N+n*n; }
    if(size<=2) if( i==i2 ){ n = *first; N=N+n*n; }
    ++i;
    ++first;
  }
  N=N*cte;
  return static_cast<Basic_type>( sqrt( static_cast<double>(N) ) );
}

///
/// \class VPredicate
///
/// Predicate applyed to a vector
/// Applique le prédicat unaire UnaryPredicate Pred au résultat de l'opération InputOperator Op sur
/// une liste de valeurs de type Basic_type list et retourne le résultat sous forme de booléen
///
template<typename Basic_type>
class VPredicate
{
  private:
    std::vector<Basic_type> list;
  public:
    ///
    /// \brief Constructeur par defaut
    ///
    VPredicate(void);
    
    ///
    /// \brief Constructeur définissant la taille de list
    ///
    VPredicate(unsigned int size);
    virtual ~VPredicate();
    
    ///
    /// \brief Test Pred( Op(list.begin(), list.end()) )
    ///
    template <class InputOperator, class UnaryPredicate>
    bool operator()(InputOperator Op, UnaryPredicate Pred);
    
    ///
    /// \brief Ajouter un élément à list
    /// \param value valeur de type Basic_type à ajouter au vecteur list
    ///
    void add(Basic_type& value);
    
    ///
    /// \brief Vider list
    ///
    void reset(void);
};

//
//
//
template<typename Basic_type>
VPredicate<Basic_type>::VPredicate(void):list()
{  };

//
//
//
template<typename Basic_type>
VPredicate<Basic_type>::VPredicate(unsigned int size):list()
{  list.resize(size);  };

//
//
//
template<typename Basic_type>
VPredicate<Basic_type>::~VPredicate()
{  list.clear();  }

//
//
//
template<typename Basic_type>
void VPredicate<Basic_type>::add(Basic_type& value)
{  list.push_back( value );  }

//
//
//
template<typename Basic_type>
void VPredicate<Basic_type>::reset(void)
{  list.clear();  }

//
//
//
template<typename Basic_type>
template <class InputOperator, class UnaryPredicate>
bool VPredicate<Basic_type>::operator()(InputOperator Op, UnaryPredicate Pred)
{  return Pred( Op(list.begin(), list.end()) );  };

///
/// \class FileReader
///
/// Ensemble de Foncteurs servant à lire les fichiers
/// Les documentation des fonctions dim() et DIM() contiennent les explications détaillées
/// des paramètres dc, ld et Ignore. Les fonctions sont déclarées de la plus simple à la plus
/// complexe. Dans le principe les fonction sans paramètres de lecture appellent
/// les fonctions avec paramètres en leur passant les valeurs par défaut et les fonctions
/// prenant un nom de fichier se chargent de l'ouvrir et de passer le flux à la fonction
/// faisant la lecture à partir du flux.
/// Le travail est toujours effectué par la fonction prenant un flux et les paramètres de lecture.
/// Version sans regex ou split, n'a pas besoin de la boost ni de bibliothèques tierces
///
template <typename Basic>
class FileReader
{
  public: 
  
  ///
  /// \brief Constructeur
  ///
  FileReader(void);
  
  ///
  /// \brief Constructeur par copie
  ///
  FileReader(const FileReader<Basic> & FR);
  
  ///
  /// Polymorphisme
  ///
  template<class F>
  FileReader(const FileReader<F>& FR);
  
  ///
  /// \brief Destructeur
  ///
  virtual ~FileReader();
  
  // ------------------------------------------- DIM ------------------------------------------- //
  ///
  /// \brief Lecture d'un fichier et recherche la balise, va ensuite lire à la ligne
  /// Nargs arguments de type T, \b un \b seul par ligne pour les placer dans le vecteur lines.
  ///
  /// La version dim() utilise les valeurs par défaut des options de lecture :
  /// les commentaires sont cd, '#' par défaut, les fins de lignes sont
  /// ld ou '\n' par défaut et où 'Ignore' lignes sont ignorées après la balise, 0
  /// par défaut
  /// \param data_file : nom du fichier (il est ouvert en lecture et fermé dans la fonction)
  /// \param balise : string contenant la chaine de caractère à rechercher dans le fichier,
  /// la fonction renvoie 1 si la chaine n'est pas trouvée, 0 si la lecture est complète
  /// \param Nargs : nombre d'arguments ( = nombre de lignes ) à lire après avoir trouvé la balise
  /// la ligne est définie par l'option ld qui vaut '\n' ici
  /// \param lines vecteur d'élément de type T pour lesquels l'opérateur >> est défini.
  /// les arguments lus dans le fichier sont placés dans ce vecteur.
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  /// 
  template<class T>
  Basic dim(std::string& data_file, std::string& balise, unsigned int Nargs, std::vector<T>& lines);
  
  ///
  /// \brief Même effet que dim(), mais ici les option sont spécifiée explicitement
  /// \param data_file
  /// \param balise
  /// \param Nargs
  /// \param lines
  /// \param cd char définissant les commentaires, toute ligne commençant par le caractère contenu dans cd
  /// sera ignorée et non comptabilisée.
  /// \param ld char définissant le caractère de fin de ligne pour la lecture des paramètres
  /// \param Ignore Nombre de ligne à totalement ignorer au début de la lecture (permet d'accélérer
  /// le parcours du fichier si on a une estimation de la position de la balise recherchée
  /// \return 0 si la lecture est réussie, 1 si une erreur s'est produite.
  ///
  template<class T>
  Basic dim(std::string& data_file, std::string& balise, unsigned int Nargs, std::vector<T>& lines,
          const char cd, const char ld, unsigned int Ignore);

  ///
  /// \brief Lecture d'un fichier passé par fluxet recherche la balise, va ensuite lire à la ligne
  /// Nargs arguments de type T, \b un \b seul par ligne pour les placer dans le vecteur lines.
  /// \param file : flux sur le fichier ouvert
  /// \param balise
  /// \param Nargs
  /// \param lines
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  /// 
  template<class T>
  Basic dim(std::ifstream& file, std::string& balise, unsigned int Nargs, std::vector<T>& lines);
  
  ///
  /// \brief Même effet que dim(), mais ici les option sont spécifiée explicitement, fichier passé par flux
  /// \param file : flux sur le fichier
  /// \param balise
  /// \param Nargs
  /// \param lines
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \return 0 si la lecture est réussie, 1 si une erreur s'est produite.
  ///
  template<class T>
  Basic dim(std::ifstream& file, std::string& balise, unsigned int Nargs, std::vector<T>& lines,
          const char cd, const char ld, unsigned int Ignore);
  // ------------------------------------------ GET 1 ------------------------------------------ //
  ///
  /// \brief Version la plus simple de get() permettant de lire une unique colonne d'objet de type T
  /// \param data_file : nom du fichier
  /// \param Nlines : nombre de lignes à lire si non nul, ou nombre de lignes lues si nul
  /// \param cols : vecteur contenant la colonne lue, elle est forcément en position 1 dans
  /// le fichier
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T>
  Basic get(std::string& data_file, unsigned int& Nlines, std::vector<T>& cols);
  
  ///
  /// \brief Version la plus simple de get() permettant de redéfinir les options de lecture
  /// \param data_file
  /// \param Nlines
  /// \param cols
  /// \param cd : marqueur de commentaires
  /// \param ld : marqueur de fin de ligne
  /// \param Ignore : nombre de ligne à ignorer dès le début de la lecture
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T>
  Basic get(std::string& data_file, unsigned int& Nlines, std::vector<T>& cols,
          const char cd, const char ld, unsigned int Ignore);
  
  ///
  /// \brief Version de get() simple avec paramètre par défaut et passant un flux
  /// \param file : flux sur le fichier ouvert
  /// \param Nlines
  /// \param cols
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T>
  Basic get(std::ifstream& file, unsigned int& Nlines, std::vector<T>& cols);
  
  ///
  /// \brief Version de get() simple sans paramètre par défaut et passant un flux
  /// \param file : flux sur le fichier ouvert
  /// \param Nlines
  /// \param cols
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T>
  Basic get(std::ifstream& file, unsigned int& Nlines, std::vector<T>& cols,
          const char cd, const char ld, unsigned int Ignore);

  // ------------------------------------------ GET N ------------------------------------------ //
  ///
  /// \brief Lecture d'un fichier avec données en N colonnes du même type.
  /// \param data_file : nom du fichier qui est ouvert en lecture dans la fonction
  /// \param Ncols : nombre de colonnes à lire
  /// \param Nlines : nombre de lignes lues jusqu'à la fin du fichier si la valeur passée de Nline est 0
  /// si la valeur est non nulle alors la fonction s'arrêtera après avoir lue Nlines lignes.
  /// \param cols : vecteur de vecteur (matrice 2D) contient les colonnes lues, chaque élément
  /// du vecteur représente une colonne stoquée sous forme d'un vecteur d'objets T possédant
  /// une définition de l'opérateur >>. Le vecteur cols est effacé avant d'être rempli.
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic get(std::string& data_file, unsigned int Ncols,
          unsigned int& Nlines, std::vector< std::vector<T> >& cols);
  
  ///
  /// \brief Version de get() avec vecteur de vecteur permettant de redéfinir les
  /// option de lecture
  /// \param data_file
  /// \param Ncols
  /// \param Nlines
  /// \param cols
  /// \param cd : caractère définissant les commentaires.
  /// \param ld : caratère délimiteur de lignes.
  /// \param Ignore : nombre de ligne à ignorer au début de la lecture.
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic get(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
          std::vector< std::vector<T> >& cols, const char cd, const char ld, unsigned int Ignore);
  
  ///
  /// \brief Lecture d'un fichier avec données en N colonnes du même type, prenant un flux sur un fichier.
  /// \param file : flux sur un fichier
  /// \param Ncols
  /// \param Nlines
  /// \param cols
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic get(std::ifstream& file, unsigned int Ncols,
          unsigned int& Nlines, std::vector< std::vector<T> >& cols);
  
  ///
  /// \brief Version de get() avec vecteur de vecteur permettant de redéfinir les
  /// option de lecture et prenant un flux sur un fichier
  /// \param file : flux sur un fichier ouvert
  /// \param Ncols
  /// \param Nlines
  /// \param cols
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic get(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
          std::vector< std::vector<T> >& cols, const char cd, const char ld, unsigned int Ignore);

  // ----------------------------------------- SGET N ------------------------------------------ //
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// \param data_file : nom du fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col : liste des colonnes sélectionnées (par défaut cols contient les colonnes du
  /// dichier réordonnées selon Sel_col)
  /// \param cols
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic sget(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
           std::vector< unsigned int >& Sel_col, std::vector< std::vector<T> >& cols);

  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// \param file : flux sur un fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col : liste des colonnes sélectionnées (par défaut cols contient les colonnes du
  /// dichier réordonnées selon Sel_col)
  /// \param cols
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic sget(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, std::vector< std::vector<T> >& cols);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et définition des paramètres par défaut
  /// \param data_file : nom du fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param cols
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered : si true cols contient les colonnes ordonnées dans le même ordre que le fichier
  /// sinon dans l'ordre donné par Sel_col, par défaut vaut false.
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic sget(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, std::vector< std::vector<T> >& cols,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);

  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et définition des paramètres par défaut
  /// \param file : flux sur le fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param cols
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered : si true cols contient les colonnes ordonnées dans le même ordre que le fichier
  /// sinon dans l'ordre donné par Sel_col, par défaut vaut false.
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic sget(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, std::vector< std::vector<T> >& cols,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);
  
  // ----------------------------------------- EXTR N ------------------------------------------- //
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param data_file : nom du fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From : ligne du fichier à partir de laquelle commencer la lecture des lignes
  /// \param By sauter des lignes entre deux lectures, ne peut pas être négatif.
  /// \param cols
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param file : flux sur le fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From : ligne du fichier à partir de laquelle commencer la lecture des lignes
  /// \param By sauter des lignes entre deux lectures, ne peut pas être négatif.
  /// \param cols
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col,  unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param data_file : nom du fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From : ligne du fichier à partir de laquelle commencer la lecture des lignes
  /// \param By sauter des lignes entre deux lectures, ne peut pas être négatif.
  /// \param cols
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);

  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param file : flux sur le fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From : ligne du fichier à partir de laquelle commencer la lecture des lignes
  /// \param By sauter des lignes entre deux lectures, ne peut pas être négatif.
  /// \param cols
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T>
  Basic extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);
  
  // -------------------------------------- EXTR N < LIM----------------------------------------- //
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param data_file : nom du fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param file : flux sur le fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col,  unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// Ne se soucie pas de l'ordre des colonnes sélectionnées par rapport à celui du fichier
  /// \param data_file : nom du fichier
  /// \param Ncols : nombre de colonnes totales dans le fichier
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);

  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// Ne se soucie pas de l'ordre des colonnes sélectionnées par rapport à celui du fichier
  /// \param file : flux sur le fichier
  /// \param Ncols : nombre de colonne totales dans le fichier
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);
  
  // -------------------------------------- EXTR N < LIM----------------------------------------- //
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param data_file : nom du fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param mults : liste de multiplicateur, chacun correspond à une colonne = conversion d'unité
  /// les valeurs lues de la colonne correpondante seront multipliée par la valeur associée doit
  /// être de taille Ncols
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, std::vector<T>& mults,
             UnaryPredicate Pred, LineOperator Op);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// \param file : flux sur le fichier
  /// \param Ncols
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param mults : multiplicateur de colonne
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col,  unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, std::vector<T>& mults,
             UnaryPredicate Pred, LineOperator Op);
  
  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// Ne se soucie pas de l'ordre des colonnes sélectionnées par rapport à celui du fichier
  /// \param data_file : nom du fichier
  /// \param Ncols : nombre de colonnes totales dans le fichier
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param mults : multiplicateur de colonne
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, std::vector<T>& mults,
             UnaryPredicate Pred, LineOperator Op,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);

  ///
  /// \brief Lecture de N colonnes en passant les indices non consécutifs des colonnes à récupérer
  /// et en imposant de commencer la lecture à partir d'une ligne par saut d'un nombre donné de lignes
  /// jusqu'à ce que la condition d'arrêt Pred (Op(ligne lue)) soit vérifiée
  ///
  /// Possède les mêmes défaut que sget() et SGET()
  /// Ne se soucie pas de l'ordre des colonnes sélectionnées par rapport à celui du fichier
  /// \param file : flux sur le fichier
  /// \param Ncols : nombre de colonne totales dans le fichier
  /// \param Nlines
  /// \param Sel_col
  /// \param From
  /// \param By
  /// \param cols
  /// \param mults : multiplicateur de colonne
  /// \param Pred : Prédicat unaire renvoyant un booléen
  /// \param Op : Opérateur prenant un itérateur de début et de fin sur un vecteur, calcule un résultat
  /// scalaire à partir du vecteur, résultat qui est passé à Pred qui commande la sortie de la fonction
  /// si la condition est remplie
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \param file_ordered
  /// \return 0 si la lecture est réuissie, 1 en cas d'erreur.
  ///
  template<class T, class UnaryPredicate, class LineOperator>
  Basic extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
             std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
             std::vector< std::vector<T> >& cols, std::vector<T>& mults,
             UnaryPredicate Pred, LineOperator Op,
             const char cd, const char ld, unsigned int Ignore, const bool file_ordered);
  
  // --------------------------------------- GET N x M ------------------------------------------ //
  ///
  /// \brief Même effet que la fonction get() prenant un vecteur de vecteur mais
  /// avec cette fois ci deux types de colonnes différentes ordonnées par bloc
  /// 
  /// Dabord on lit celles du type T, puis celles du type U à leur droite.
  /// Permet d'ignorer les premières colonnes par exemple.
  /// \param data_file : nom du fichier
  /// \param Ncols_T : nombre de colonne d'objets T
  /// \param Ncols_U : nombre de colonnes d'objets U, le nombre de colonnes totales du fichier est
  /// Ncols_T + Ncols_U.
  /// \param Nlines : nombre de ligne à lire si non nul ou nombre de lignes lues si nul.
  /// \param cols_T : vecteur contenant les Ncols_T colonnes de type U à lire.
  /// \param cols_U : vecteur contenant les Ncols_T colonnes de type U à lire.
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T, class U>
  Basic get(std::string& data_file, unsigned int Ncols_T, unsigned int Ncols_U, unsigned int& Nlines,
            std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U);
  
  ///
  /// \brief Version de get() prenant deux types d'objets U et T permettant de redéfinir
  /// les options de lecture.
  /// \param data_file
  /// \param Ncols_T
  /// \param Ncols_U
  /// \param Nlines
  /// \param cols_T
  /// \param cols_U
  /// \param cd : marqueur de commentaires
  /// \param ld : marqueur de fin de ligne
  /// \param Ignore : nombre de lignes à ignorer en commençant la lecture
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T, class U>
  Basic get(std::string& data_file, unsigned int Ncols_T, unsigned int Ncols_U, unsigned int& Nlines,
            std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U,
            const char cd, const char ld, unsigned int Ignore);

  ///
  /// \brief Version de get() prenant deux types d'objets U et T et un flux sur un fichier ouvert.
  /// \param file
  /// \param Ncols_T
  /// \param Ncols_U
  /// \param Nlines
  /// \param cols_T
  /// \param cols_U
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T, class U>
  Basic get(std::ifstream& file, unsigned int Ncols_T, unsigned int Ncols_U, unsigned int& Nlines,
            std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U);
  
  ///
  /// \brief Version de get() prenant deux types d'objets U et T permettant de redéfinir
  /// les options de lecture et un flux sur un fichier ouvert.
  /// \param file
  /// \param Ncols_T
  /// \param Ncols_U
  /// \param Nlines
  /// \param cols_T
  /// \param cols_U
  /// \param cd
  /// \param ld
  /// \param Ignore
  /// \return 1 si la lecture a échouée, 0 si la lecture a réussie.
  ///
  template<class T, class U>
  Basic get(std::ifstream& file, unsigned int Ncols_T, unsigned int Ncols_U, unsigned int& Nlines,
            std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U,
            const char cd, const char ld, unsigned int Ignore);
  
};
  //
  //
  //
  template <typename Basic>
  FileReader<Basic>::FileReader(void){  }
  
  //
  //
  //
  template <typename Basic>
  FileReader<Basic>::~FileReader(void){  }
  
  template <typename Basic>
  template<class F>
  FileReader<Basic>::FileReader(const FileReader<F>& FR){ }
  // --------------------------------------------------------------------------------------- //
  //                                      DIM                                                //
  // --------------------------------------------------------------------------------------- //
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::dim(std::string& data_file, std::string& balise, unsigned int Nargs,
                             std::vector<T>& lines)
  {
    return dim<T>(data_file, balise, Nargs, lines, '#', '\n', 0);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::dim(std::ifstream& file, std::string& balise, unsigned int Nargs,
                             std::vector<T>& lines)
  {
    return dim<T>(file, balise, Nargs, lines, '#', '\n', 0);
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::dim(std::string& data_file, std::string& balise,
                             unsigned int Nargs, std::vector<T>& lines,
                             const char cd, const char ld, unsigned int Ignore)
  {
    Basic status = static_cast<Basic>(0);
    std::ifstream file( data_file.c_str() );
    if( file.fail() )
    {
      std::cerr<<"can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = dim<T>(file, balise, Nargs, lines, cd, ld, Ignore);
    file.close();
    return status;
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::dim(std::ifstream& file, std::string& balise, unsigned int Nargs,
                             std::vector<T>& lines,
                             const char cd, const char ld, unsigned int Ignore)
  {
    unsigned int Nlines;
    bool found;
    std::string line;
    lines.clear();
    Nlines = 0;
    while( std::getline( file, line, ld ))
    {
      if(line.find(balise)<line.size())
      {
        found=true;
        break;
      }
      found=false;
    }
    if(found)
    {
      while( std::getline( file, line, ld ))
      {
        T r;
        if( Ignore>0 )
        {
          Ignore--;
          continue;
        }
        char c = line[0];
        if( c==cd )
        {
          continue;
        }
        std::istringstream getsub( line );
        getsub>>r;
        Nlines++;
        lines.push_back(r);
        if(Nlines>=Nargs){ break; }
      }
      file.close();
      return static_cast<Basic>(0);
    }
    std::cerr<<"can't find : '"<<balise<<"'"<<std::endl;
    return static_cast<Basic>(1);
  }

  // --------------------------------------------------------------------------------------- //
  //                                   GET N                                                 //
  // --------------------------------------------------------------------------------------- //
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
		             std::vector< std::vector<T> >& cols)
  {
    return get<T>(data_file,Ncols,Nlines,cols,'#','\n',0);
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                             std::vector< std::vector<T> >& cols)
  {
    return get<T>(file,Ncols,Nlines,cols,'#','\n',0);
  }

  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
		             std::vector< std::vector<T> >& cols,
                             const char cd, const char ld, unsigned int Ignore)
  {
    Basic status = static_cast<Basic>(0);
    std::ifstream file( data_file.c_str() );
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = get<T>(file,Ncols,Nlines,cols,cd,ld,Ignore);
    file.close();
    return status;
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                             std::vector< std::vector<T> >& cols,
                             const char cd, const char ld, unsigned int Ignore)
  {
    unsigned int maxline = Nlines;
    std::string line;
    if(Ncols<1) return static_cast<Basic>(1);
    for(unsigned int i=0;i<cols.size();++i) cols[i].clear(); cols.clear();
    cols.resize(Ncols);
    Nlines = 0;
    while( std::getline( file, line, ld ))
    {
      T r;
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      std::istringstream getsub( line );
      for(unsigned int i=0;i<Ncols;++i)
      {
        getsub>>r;
        cols[i].push_back(r);
      }
      Nlines++;
      if(maxline) if(Nlines>=maxline) break;
    }
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }
  
  // --------------------------------------------------------------------------------------- //
  //                                 GET N x M                                               //
  // --------------------------------------------------------------------------------------- //
  //
  //
  //
  template <typename Basic>
  template<class T, class U>
  Basic FileReader<Basic>::get(std::string& data_file, unsigned int Ncols_T, unsigned int Ncols_U,
                             unsigned int& Nlines,
		             std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U)
  {
    return get<T>(data_file,Ncols_T,Ncols_U,Nlines,cols_T,cols_U,'#','\n',0);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T, class U>
  Basic FileReader<Basic>::get(std::ifstream& file, unsigned int Ncols_T, unsigned int Ncols_U,
                             unsigned int& Nlines,
                             std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U)
  {
    return get<T>(file,Ncols_T,Ncols_U,Nlines,cols_T,cols_U,'#','\n',0);
  }
  //
  //
  //
  template <typename Basic>
  template<class T, class U>
  Basic FileReader<Basic>::get(std::string& data_file, unsigned int Ncols_T, unsigned int Ncols_U,
                             unsigned int& Nlines,
		             std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U,
		             const char cd, const char ld, unsigned int Ignore)
  {
    std::ifstream file( data_file.c_str() );
    Basic status = static_cast<Basic>(0);
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = get<T>(file,Ncols_T,Ncols_U,Nlines,cols_T,cols_U,cd,ld,Ignore);
    file.close();
    return status;
  }
 
  //
  //
  //
  template <typename Basic>
  template<class T, class U>
  Basic FileReader<Basic>::get(std::ifstream& file, unsigned int Ncols_T, unsigned int Ncols_U,
                             unsigned int& Nlines,
                             std::vector< std::vector<T> >& cols_T, std::vector< std::vector<U> >& cols_U,
                             const char cd, const char ld, unsigned int Ignore)
  {
    unsigned int maxline = Nlines;
    std::string line;
    if(Ncols_T<1) return static_cast<Basic>(1);
    if(Ncols_U<1) return static_cast<Basic>(1);
    for(int i=0;i<cols_T.size();++i) cols_T[i].clear(); cols_T.clear();
    cols_T.resize(Ncols_T);
    for(int i=0;i<cols_U.size();++i) cols_U[i].clear(); cols_U.clear();
    cols_U.resize(Ncols_U);
    Nlines = 0;
    while( std::getline( file, line, ld ))
    {
      T r;
      U s;
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      std::istringstream getsub( line );
      for(int i=0;i<Ncols_T;++i)
      {
        getsub>>r;
        cols_T[i].push_back(r);
      }
      for(int i=0;i<Ncols_U;++i)
      {
        getsub>>s;
        cols_U[i].push_back(s);
      }
      Nlines++;
      if(maxline) if(Nlines>=maxline) break;
    }
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }
 
  // --------------------------------------------------------------------------------------- //
  //                                   GET 1                                                 //
  // --------------------------------------------------------------------------------------- //
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::string& data_file, unsigned int& Nlines, std::vector<T>& cols)
  {
    return get<T>(data_file,Nlines,cols,'#','\n',0);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::string& data_file, unsigned int& Nlines, std::vector<T>& cols,
                             const char cd, const char ld, unsigned int Ignore)
  {
    std::ifstream file( data_file.c_str() );
    Basic status = static_cast<Basic>(0);
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = get<T>(file,Nlines,cols,cd,ld,Ignore);
    file.close();
    return status;
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::ifstream& file, unsigned int& Nlines, std::vector<T>& cols)
  {
    return get<T>(file,Nlines,cols,'#','\n',0);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::get(std::ifstream& file, unsigned int& Nlines, std::vector<T>& cols,
                             const char cd, const char ld, unsigned int Ignore)
  {
    unsigned int maxline = Nlines;
    std::string line;
    cols.clear();
    Nlines = 0;
    while( std::getline( file, line, ld ))
    {
      T r;
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      std::istringstream getsub( line );
      getsub>>r;
      cols.push_back(r);
      Nlines++;
      if(maxline) if(Nlines>=maxline) break;
    }
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }
  
  // --------------------------------------------------------------------------------------- //
  //                                  SGET N                                                 //
  // --------------------------------------------------------------------------------------- //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::sget(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,
                              std::vector< std::vector<T> >& cols)
  {
    return sget<T>(data_file,Ncols,Nlines,Sel_col,cols,'#','\n',0, false);
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::sget(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,
                              std::vector< std::vector<T> >& cols)
  {
    return sget<T>(file,Ncols,Nlines,Sel_col,cols,'#','\n',0, false);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::sget(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,
                              std::vector< std::vector<T> >& cols,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    Basic status = static_cast<Basic>(0);
    std::ifstream file( data_file.c_str() );
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = sget<T>(file,Ncols,Nlines,Sel_col,cols,cd,ld,Ignore, file_ordered);
    file.close();
    return status;
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::sget(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,
                              std::vector< std::vector<T> >& cols,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    unsigned int maxline = Nlines;
    std::string line;
    if(Ncols<1) return static_cast<Basic>(1);
    if(Sel_col.size() != Ncols) return static_cast<Basic>(1);
    for(unsigned int i=0;i<cols.size();++i) cols[i].clear(); cols.clear();
    cols.resize(Ncols);
    Nlines = 0;
    while( std::getline( file, line, ld ))
    {
      T r;
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      std::istringstream getsub( line );
      if( file_ordered )
      {
        // Cette méthode range les colonnes dans l'ordre du fichier : +rapide
        for(unsigned int i=0;i<Ncols;++i)
        {
          getsub>>r;
          if( find( Sel_col.begin(), Sel_col.end(), i)!=Sel_col.end() ){ cols[i].push_back(r); }
        }
      }
      else
      {
        // Cette méthode range les colonnes dans l'ordre de Sel_col : +lent
        for(unsigned int j=0;j<Sel_col.size();++j)
        {
          for(unsigned int i=0;i<Ncols;++i)
          {
            getsub>>r;
            if( i==Sel_col[j] ){ cols[i].push_back(r); }
          }
        }
      }
      Nlines++;
      if(maxline) if(Nlines>=maxline) break;
    }
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }

  // --------------------------------------------------------------------------------------- //
  //                                  EXTR N                                                 //
  // --------------------------------------------------------------------------------------- //
  //
  // From = coimmencer à la ligne i
  // By = lire toutes les lignes
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                                std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                                std::vector< std::vector<T> >& cols)
  {
    return extr<T>(data_file,Ncols,Nlines,Sel_col,From, By, cols,'#','\n',0, false);
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,  unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols)
  {
    return extr<T>(file,Ncols,Nlines,Sel_col,From, By, cols,'#','\n',0, false);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    Basic status = static_cast<Basic>(0);
    std::ifstream file( data_file.c_str() );
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = extr<T>(file,Ncols,Nlines,Sel_col,From, By, cols,cd,ld,Ignore, file_ordered);
    file.close();
    return status;
  }

  //
  //
  //
  template <typename Basic>
  template<class T>
  Basic FileReader<Basic>::extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    unsigned int maxline = Nlines, cline = 0;
    std::string line;
    if(Ncols<1) return static_cast<Basic>(1);
    if(Sel_col.size() <1) return static_cast<Basic>(1);
    for(unsigned int i=0;i<cols.size();++i) cols[i].clear(); cols.clear();
    cols.resize(Sel_col.size());
    Nlines = 0; cline=0;
    while( std::getline( file, line, ld ))
    {
      T r;
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      if( (cline-From)<0 ){cline++; continue; } //Commence à lire quand cline = From (cline compté à partir de 0)
      if( (cline-From)%By != 0 ){cline++; continue; }
      std::istringstream getsub( line );
      if( file_ordered )
      {
        // Cette méthode range les colonnes dans l'ordre du fichier : +rapide
        for(unsigned int i=0;i<Ncols;++i)
        {
          getsub>>r;
          if( find( Sel_col.begin(), Sel_col.end(), i)!=Sel_col.end() ){ cols[i].push_back(r); }
        }
      }
      else
      {
        // Cette méthode range les colonnes dans l'ordre de Sel_col : +lent
        for(unsigned int j=0;j<Sel_col.size();++j)
        {
          for(unsigned int i=0;i<Ncols;++i)
          {
            getsub>>r;
            if( i==Sel_col[j] ){ cols[j].push_back(r); }
          }
          getsub.clear();
          getsub.seekg(0, getsub.beg);
        }
      }
      Nlines++; cline++;
      if(maxline) if(Nlines>=maxline) break;
    }
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }

  // --------------------------------------------------------------------------------------- //
  //                                EXTR N < LIM                                             //
  // --------------------------------------------------------------------------------------- //
  //
  // From = coimmencer à la ligne i
  // By = lire toutes les lignes
  // Stop if Predicate(cond) is true -> le predicat prend un vecteur en entrée !
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                                std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                                std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op)
  {
    return extr<T>(data_file,Ncols,Nlines,Sel_col,From,By,cols,Pred,Op,'#','\n',0,false);
  }

  //
  //
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,  unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op)
  {
    return extr<T>(file,Ncols,Nlines,Sel_col,From,By,cols,Pred,Op,'#','\n',0,false);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    Basic status = static_cast<Basic>(0);
    std::ifstream file( data_file.c_str() );
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = extr<T>(file,Ncols,Nlines,Sel_col,From, By, cols,Pred,Op,cd,ld,Ignore, file_ordered);
    file.close();
    return status;
  }

  //
  //
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols, UnaryPredicate Pred, LineOperator Op,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    unsigned int maxline = Nlines, cline = 0;
    bool tostop=false;
    std::string line;
    if(Ncols<1) return static_cast<Basic>(1);
    if(Sel_col.size() < 1) return static_cast<Basic>(1);
    VPredicate<T> StopPoint;
    for(unsigned int i=0;i<cols.size();++i) cols[i].clear(); cols.clear();
    cols.resize(Sel_col.size());
    Nlines = 0; cline=0;
    while( std::getline( file, line, ld ))
    {
      T r;
      StopPoint.reset();
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      if( (cline-From)<0 ){cline++; continue; } //Commence à lire quand cline = From (cline compté à partir de 0)
      if( (cline-From)%By != 0 ){cline++; continue; }
      std::istringstream getsub( line );
      if( file_ordered )
      {
        // Cette méthode range les colonnes dans l'ordre du fichier : +rapide
        for(unsigned int i=0;i<Ncols;++i)
        {
          getsub>>r;
          if( find( Sel_col.begin(), Sel_col.end(), i)!=Sel_col.end() )
          {
            cols[i].push_back(r); 
            StopPoint.add(r);
          }
          if( StopPoint(Op, Pred)  ) tostop=true;
        }
        if(tostop) return static_cast<Basic>(0);
      }
      else
      {
        // Cette méthode range les colonnes dans l'ordre de Sel_col : +lent
        for(unsigned int j=0;j<Sel_col.size();++j)
        {
          for(unsigned int i=0;i<Ncols;++i)
          {
            getsub>>r;
            if( i==Sel_col[j] )
            {
              cols[j].push_back(r);
              StopPoint.add(r);
            }
          }
          if( StopPoint(Op, Pred) ) tostop=true;
          getsub.clear();
          getsub.seekg(0, getsub.beg);
        }
        if(tostop) return static_cast<Basic>(0);
      }
      Nlines++; cline++;
      if(maxline) if(Nlines>=maxline) break;
    } //end while
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }
  
  // --------------------------------------------------------------------------------------- //
  //                                CEXTR N < LIM                                             //
  // --------------------------------------------------------------------------------------- //
  //
  // From = coimmencer à la ligne i
  // By = lire toutes les lignes
  // Stop if Predicate(cond) is true -> le predicat prend un vecteur en entrée !
  // mults : multiplicateur de chaque colonne
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                                std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                                std::vector< std::vector<T> >& cols, std::vector<T>& mults,
                                UnaryPredicate Pred, LineOperator Op)
  {
    return extr<T>(data_file,Ncols,Nlines,Sel_col,From,By,cols,mults,Pred,Op,'#','\n',0,false);
  }

  //
  //
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col,  unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols, std::vector<T>& mults,
                              UnaryPredicate Pred, LineOperator Op)
  {
    return extr<T>(file,Ncols,Nlines,Sel_col,From,By,cols,mults,Pred,Op,'#','\n',0,false);
  }
  
  //
  //
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::string& data_file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols, std::vector<T>& mults,
                              UnaryPredicate Pred, LineOperator Op,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    Basic status = static_cast<Basic>(0);
    std::ifstream file( data_file.c_str() );
    if( file.fail() )
    {
      std::cerr<<"(reader) can't open file "<<data_file.c_str()<<std::endl;
      return static_cast<Basic>(1);
    }
    status = extr<T>(file,Ncols,Nlines,Sel_col,From, By, cols,mults,Pred,Op,cd,ld,Ignore, file_ordered);
    file.close();
    return status;
  }

  //
  //
  //
  template <typename Basic>
  template<class T, class UnaryPredicate, class LineOperator>
  Basic FileReader<Basic>::extr(std::ifstream& file, unsigned int Ncols, unsigned int& Nlines,
                              std::vector< unsigned int >& Sel_col, unsigned int& From, unsigned int& By,
                              std::vector< std::vector<T> >& cols, std::vector<T>& mults,
                              UnaryPredicate Pred, LineOperator Op,
                              const char cd, const char ld, unsigned int Ignore, const bool file_ordered)
  {
    unsigned int maxline = Nlines, cline = 0;
    bool tostop=false;
    std::string line;
    if(Ncols<1) return static_cast<Basic>(1);
    if(mults.size()!=Ncols) return static_cast<Basic>(1);
    if(Sel_col.size() < 1) return static_cast<Basic>(1);
    VPredicate<T> StopPoint;
    for(unsigned int i=0;i<cols.size();++i) cols[i].clear(); cols.clear();
    cols.resize(Sel_col.size());
    Nlines = 0; cline=0;
    while( std::getline( file, line, ld ))
    {
      T r;
      StopPoint.reset();
      if( Ignore>0 )
      {
        Ignore--;
        continue;
      }
      char c = line[0];
      if( c==cd ) continue;
      if( (cline-From)<0 ){cline++; continue; } //Commence à lire quand cline = From (cline compté à partir de 0)
      if( (cline-From)%By != 0 ){cline++; continue; }
      std::istringstream getsub( line );
      if( file_ordered )
      {
        // Cette méthode range les colonnes dans l'ordre du fichier : +rapide
        for(unsigned int i=0;i<Ncols;++i)
        {
          getsub>>r;
          if( find( Sel_col.begin(), Sel_col.end(), i)!=Sel_col.end() )
          {
            cols[i].push_back(r * mults[i]); 
            StopPoint.add(r);
          }
          if( StopPoint(Op, Pred)  ) tostop=true;
        }
        if(tostop) return static_cast<Basic>(0);
      }
      else
      {
        // Cette méthode range les colonnes dans l'ordre de Sel_col : +lent
        for(unsigned int j=0;j<Sel_col.size();++j)
        {
          for(unsigned int i=0;i<Ncols;++i)
          {
            getsub>>r;
            if( i==Sel_col[j] )
            {
              cols[j].push_back(r * mults[i]);
              StopPoint.add(r);
            }
          }
          if( StopPoint(Op, Pred) ) tostop=true;
          getsub.clear();
          getsub.seekg(0, getsub.beg);
        }
        if(tostop) return static_cast<Basic>(0);
      }
      Nlines++; cline++;
      if(maxline) if(Nlines>=maxline) break;
    } //end while
    if(maxline) if(Nlines<maxline) return static_cast<Basic>(1);
    return static_cast<Basic>(0);
  }
#define _CLASS_READER_
#endif
