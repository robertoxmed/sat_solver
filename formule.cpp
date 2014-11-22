#include "formule.hpp"

// BEGIN Création de formules simples //////////////////////////////////////////
formule variable(string nom) {
	formule res = new formule_s;
	res->op = o_variable;
	res->nom = new string(nom);
	return res;
}

formule variable(char* nom) {
	formule res = new formule_s;
	res->op = o_variable;
	res->nom = new string(nom);
	return res;
}

formule non(formule arg) {
	formule res = new formule_s;
	res->op = o_non;
	res->arg = arg;
	return res;
}

formule et(formule arg1, formule arg2) {
	formule res = new formule_s;
	res->op = o_et;
	res->arg1 = arg1;
	res->arg2 = arg2;
	return res;
}

formule ou(formule arg1, formule arg2) {
  formule res = new formule_s;
  res->op = o_ou;
  res->arg1 = arg1;
  res->arg2 = arg2;
  return res;
}

formule impl(formule arg1, formule arg2) {
  formule res = new formule_s;
  res->op = o_implique;
  res->arg1 = arg1;
  res->arg2 = arg2;
  return res;
}

formule equiv(formule arg1, formule arg2) {
  formule res = new formule_s;
  res->op = o_equivaut;
  res->arg1 = arg1;
  res->arg2 = arg2;
  return res;
}
// END Création de formules simples ////////////////////////////////////////////


// BEGIN Affichage d'une formule ///////////////////////////////////////////////
// Affichage des opérateurs
string operateur2string(operateur op) {
  switch (op) {
  case o_variable:
    return "var";
  case o_non:
    return "~";
  case o_et:
    return "/\\";
  case o_ou:
    return "\\/";
  case o_implique:
    return "=>";
  case o_equivaut:
    return "<=>";
  }
  return string();
}

// Affichage d'une formule par induction sur son arbre de syntaxe
string formule2string(formule f) {
  switch (f->op) {
  case o_variable: 
    return *(f->nom);
  case o_non:
    return "~" + formule2string(f->arg);
  case o_et:
  case o_ou:
  case o_equivaut:
  case o_implique:
    return "("+formule2string(f->arg1)
      +" "+operateur2string(f->op)+" "
      +formule2string(f->arg2)+")";
  }
  return string();
}

// Définition de l'opérateur << pour le type formule
ostream& operator<<(ostream& out, formule f){
  out << formule2string(f);
  return out;
}
// END Affichage d'une formule /////////////////////////////////////////////////


// BEGIN Gestion de la mémoire dynamique des formules //////////////////////////
formule copier(formule f) {
  switch (f->op) {
  case o_variable:
    return variable(* f->nom);
  case o_non:
    return non(copier(f->arg));
  case o_et:
    return et(copier(f->arg1),copier(f->arg2));
  case o_ou:
    return ou(copier(f->arg1),copier(f->arg2));
  case o_implique:
    return impl(copier(f->arg1),copier(f->arg2));
  case o_equivaut:
    return equiv(copier(f->arg1),copier(f->arg2));
  }
  return nullptr;
}

void free_all(formule f) {
  switch (f->op) {
  case o_implique:
  case o_equivaut:
  case o_ou:
  case o_et:
    free_all(f->arg1);
    free_all(f->arg2);
    break;
  case o_non:
    free_all(f->arg);
    break;
  case o_variable:
    delete f->nom;
    break;
  }
  delete f;
}

// END Gestion de la mémoire dynamique des formules ////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//            A PARTIR D'ICI LES FONCTIONS SONT A IMPLEMENTER                 //
////////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& out, const cls_t& cl){
 	out<<"( ";
	for(cls_t::const_iterator it=cl.cbegin();it!=cl.cend();++it)
		out<<*it<<" ";

	out <<" )";
	return out;
}

ostream& operator<<(ostream& out, const cnf_t& c){
  	out<<"( ";
	for(cnf_t::const_iterator it=c.cbegin();it!=c.cend();++it)
		out<<*it<<" ";
	out<<" )";
	return out;
}


bool est_dans(const cls_t& c, const cnf_t& f){
	for(cnf_t::const_iterator it=f.cbegin();it!=f.cend();++it){
		if(c==*it)
			return true;
	}
	return false;
}

void numerote(formule f, map<string,var_t> & corresp){
	switch (f->op) {
		case o_variable:
			corresp[*(f->nom)]=corresp.size()-1;
		break;
		case o_non:
			numerote(f->arg,corresp);
		break;
		case o_et:
		case o_ou:
		case o_equivaut:
		case o_implique:
			numerote(f->arg1,corresp);
			numerote(f->arg2,corresp);
		break;
	}
}

lit_t var2lit(var_t v, bool p){
	lit_t retour;
	if(p==true)
		retour = v*2;
	else
		retour = v*2+1;
	return retour;

}

lit_t neg(lit_t l)
{
	lit_t retour;
	if (l%2==0)
		retour=l+1;
	else
		retour=l-1;
	return retour;
}

