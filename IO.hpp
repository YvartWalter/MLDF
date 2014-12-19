///
/// \file ReadWrite.hpp
///
/// \include Reader.hpp
/// \include Writter.hpp
///
/// IO module
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

#ifndef _ANNOUNCE_CLASSES_IO_
#define _ANNOUNCE_CLASSES_IO_
#include <cctype>
#include <cstdio>
#include <regex>
template<typename Basic_type, typename Index_type> class VOp_norm2;
template<typename Basic_type> class VPredicate;
template <typename Basic> class FileHeader;
template <typename Basic> class FileReader;
template <typename Basic> class FileWriter;
#endif


#ifndef _ENUM_AND_CLASS_IO_
namespace Format {
   enum FORMAT_FILES {
                       TEXPLOT,
                       TEXPLOT_EXTENDED,
                       MIRO,
                       MAMOJ,
                       RAW,
                       UNKNOWN
                     };
}

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

template<typename Basic_type>
VPredicate<Basic_type>::VPredicate(void):list()
{  };

template<typename Basic_type>
VPredicate<Basic_type>::VPredicate(unsigned int size):list()
{  list.resize(size);  };

template<typename Basic_type>
VPredicate<Basic_type>::~VPredicate()
{  list.clear();  }

template<typename Basic_type>
void VPredicate<Basic_type>::add(Basic_type& value)
{  list.push_back( value );  }

template<typename Basic_type>
void VPredicate<Basic_type>::reset(void)
{  list.clear();  }

template<typename Basic_type>
template <class InputOperator, class UnaryPredicate>
bool VPredicate<Basic_type>::operator()(InputOperator Op, UnaryPredicate Pred)
{  return Pred( Op(list.begin(), list.end()) );  };


#define _ENUM_AND_CLASS_IO_
#endif //_ENUM_AND_CLASS_IO_
//TODO remplacer H0 -> H0+EQ et définir H0 avec +EQ, idem pour H1
static const std::string CO=std::string(",");
static const std::string GM=std::string("\"");
static const std::string EQ=std::string("=");
static const std::string SH=std::string("#");
static const std::string S_2=std::string("  ");
static const std::string H0=std::string("TITLE");              //Titre
static const std::string H1=std::string("VARIABLES");          //labels des colonnes
static const std::string H2=std::string("ZONE");               //information sur l'organisation de la grille
static const std::string H3=std::string("DATASTRUCTURE");      //information sur l'organisation du fichier

#include "Readheader.hpp"
                  
#include "Reader.hpp"

#include "Writter.hpp"


#ifndef _FUNCTION_IO_TEMPL_
///
/// \brief Ecrire les données contenues par colonnes dans un container data passé par itérateur
/// dans un fichier
///
template<class data, class size, class label>
int TexplotOutput(std::string& file_name,
                  std::vector< std::vector<data> >& D,
                  std::vector< std::vector<size> >& S,
                  std::vector<label>& L)
{
  FileWriter<int> Output;
  Format::FORMAT_FILES f=Format::TEXPLOT;
  return Output.Replace<data>(file_name, D, L, S, f);
}
#define _FUNCTION_IO_TEMPL_
#endif
