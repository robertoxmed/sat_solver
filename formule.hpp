#ifndef Formule
#define Formule
#include <iostream>
// string est la classe des chaînes de caractères en C++ que l'on privilégie aux char * du C.
#include <string>
// set est la classe des ensembles de la STL du C++. set garantit l'unicité et
// le tri lors du parcours : le nombre et l'ordre des insertions ne compte pas.
// L'accès direct avec [] n'est pas possible : l'utilisation des itérateurs est obligatoire.
#include <set>
// vector est la classe des tableaux dynamiques de la STL du C++. vector ne 
// garantit PAS l'unicité et le tri lors du parcours : le nombre et l'ordre
// des insertions compte. L'accès direct avec [] est possible.
#include <vector>
// map est la classe des tableaux associatifs de la STL du C++.
#include <map>

using namespace std;

// Identificateurs pour les connecteurs de la logique propositionnelle : 
// ce sont les symboles associés aux nœuds de l'arbre de syntaxe des formules
enum operateur {
  o_variable,
  o_non,
  o_et,
  o_ou,
  o_implique,
  o_equivaut
};

// Structure des nœuds de l'arbre de syntaxe abstraite des formules
struct formule_s {
  // A chaque noeud est associé un opérateur
  operateur op;
  union {
    // Pour le cas o_variable, on associe une chaîne pour identifier la proposition
    string * nom;
    // Pour le cas du connecteur unaire o_non, on associe un seul sous-arbre arg.
    formule_s * arg;
    // Pour les cas des connecteurs binaires (o_et, o_ou, o_implique et o_equivaut),
    // on associe deux sous-arbres : le fils gauche arg1 et le fils droit arg2.
    struct {
      formule_s * arg1;
      formule_s * arg2;
    };
  };
};

typedef struct formule_s * formule;

// Création de formules simples ////////////////////////////////////////////////
formule variable(string nom);
formule variable(char* nom);
formule non(formule arg);
formule et(formule arg1, formule arg2);
formule ou(formule arg1, formule arg2);
formule impl(formule arg1, formule arg2);
formule equiv(formule arg1, formule arg2);
// Affichage d'une formule par induction sur son arbre de syntaxe //////////////
string  formule2string(formule f);
ostream& operator<<(ostream& out, formule f);
// Gestion de la mémoire dynamique des formules ////////////////////////////////
formule copier(formule f);
void free_all(formule f);

// Type des variables
typedef unsigned int  var_t;
// Type des littéraux 
typedef int           lit_t;
// Type des clauses, définies comme des ensembles de littéraux
typedef set<lit_t>    cls_t;
// Type des formules normales conjonctives (CNF), définies comme des tableaux
// dynamiques (vector en C++) de clauses
typedef vector<cls_t> cnf_t;

//Définition des nouveaux types pour la résolution
//Index simple et double
typedef cnf_t::size_type cls_ref_t;

typedef vector< set<cls_ref_t> > idx_t;

////////////////////////////////////////////////////////////////////////////////
//        A PARTIR D'ICI LES FONCTIONS SONT SIMPLEMENT DÉCLARÉES ET           //
//               DOIVENT ÊTRE IMPLÉMENTÉES DANS FORMULE.CPP                   //
////////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& out, const cls_t& cl);

ostream& operator<<(ostream& out, const cnf_t& c);

bool est_dans(const cls_t& cl, const cnf_t& c);

void numerote(formule f, map<string,var_t> & corresp);

lit_t var2lit(var_t v, bool p = true);

lit_t neg(lit_t l);

#endif
