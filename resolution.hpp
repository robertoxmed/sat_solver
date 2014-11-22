#ifndef Resolution
#define Resolution
#include "formule.hpp"


////////////////////////////////////////////////////////////////////////////////
//              ICI LES FONCTIONS SONT SIMPLEMENT DÉCLARÉES ET                //
//               DOIVENT ÊTRE IMPLÉMENTÉES DANS RESOLUTION.CPP                //
////////////////////////////////////////////////////////////////////////////////

lit_t tseitin(formule f, const map<string, var_t> & m, cnf_t & c, var_t & next);

cnf_t tseitin(formule f);

lit_t pivot(const cls_t& cl1, const cls_t& lc2);

cls_t resolvante(const cls_t& cl1, lit_t l, const cls_t& cl2);

bool subsume(const cls_t& cl1, const cls_t& cl2);

bool subsumeCNF(const cnf_t& c, const cls_t& cl);

void add_clause(cnf_t & cnf, idx_t & idx_simple, idx_t & idx_multiple, const cls_t & c);

bool est_subsumee(const cnf_t & cnf, const idx_t & idx_simple, const cls_t & c);

bool resolution_first(const cnf_t& c);

bool resolution_second(const cnf_t& c);

bool resolution_third(const cnf_t& c);


#endif
