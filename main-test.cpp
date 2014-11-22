#include <UnitTest++.h>
#include "formule.hpp"
#include "resolution.hpp"
#include "parser.hpp"
#include <iostream>

// Exemple de test des fonctions de passage arbre de syntaxe abstraite <-> string
TEST(lit_ecrit_formule) {
	formule f = lit_formule("p /\\ (q => r \\/ p)");
	string s = formule2string(f);
	formule f2 = lit_formule(s);
	string s2 = formule2string(f2);
	CHECK(s == s2);
	free_all(f);
	free_all(f2);
}

TEST(affclause){

	cls_t cl1,cl2;
	cl1.insert(1);
	cl1.insert(4);
	cl1.insert(7);
	cl1.insert(6);

	cl2.insert(10);
	cl2.insert(40);
	cl2.insert(70);
	cl2.insert(60);
	
	cout<<endl<<" * Affichage clauses: Clause 1: "<<cl1;
	cout<<" Clause 2: "<<cl2<<" *"<<endl;
}
TEST(clauses){
	cls_t cl1,cl2;
	cl1.insert(1);
	cl1.insert(4);
	cl1.insert(7);
	cl1.insert(6);

	cl2.insert(10);
	cl2.insert(40);
	cl2.insert(70);
	cl2.insert(60);
	
	CHECK(cl1 != cl2);
   	CHECK(cl2 != cl1);
	CHECK(cl1 == cl1);
	CHECK(cl2 == cl2);
}

TEST(affcnf){
	cnf_t c;
	cls_t cl1,cl2;
	cl1.insert(1);
	cl1.insert(4);
	cl1.insert(7);
	cl1.insert(6);

	cl2.insert(10);
	cl2.insert(40);
	cl2.insert(70);
	cl2.insert(60);
	
	c.push_back(cl1);
	c.push_back(cl2);

	cout<<endl<<" * Affichage CNF:"<<c<<" *"<<endl;
}
TEST(est_dans_CNF){
	cnf_t c;
	cls_t cl1,cl2;
	cl1.insert(1);
	cl1.insert(4);
	cl1.insert(7);
	cl1.insert(6);
	cl2.insert(10);
	cl2.insert(40);
	cl2.insert(70);
	cl2.insert(60);
	
	c.push_back(cl1);
	c.push_back(cl2);
	
	CHECK(est_dans(cl1,c));
	CHECK(est_dans(cl2,c));
}

TEST(numerote_formules){
	formule f1,f2,f3,f4;
	map<string,var_t> m1,m2,m3;
	f1=variable((string)"p");
	f2=variable((string)"q");
	f3=non(f1);
	f4=ou(f2,f3);
	
	numerote(f1,m1);
	numerote(f3,m2);
	numerote(f4,m3);
	CHECK(m1.size()==1);
	CHECK(m2.size()==1);
	CHECK(m3.size()==2);
}

TEST(variable_literal){
	var_t v1,v2;
	lit_t l1,l2,l3,l4;
	v1=0;
	v2=1;
	l1=var2lit(v1,true);
	l2=var2lit(v2,false);
	l3=neg(l1);
	l4=neg(l2);
	CHECK(l1==0);
	CHECK(l2==3);
	CHECK(l3==1);
	CHECK(l4==2);
}

TEST(tseitin){
	formule f1,f2,F1,F2,F3,F4,F5;
	cnf_t c1,c2,c3,c4,c5,c6;
	map<string,var_t> m1,m2,m3,m4,m5;
	
	var_t next1=(unsigned int) 1;
	var_t next2=(unsigned int) 2;
	
	f1=variable((string) "p");
	f2=variable((string) "q");
	F1=non(f1);
	F2=ou(f1,f2);
	F3=impl(f1,f2);
	F4=equiv(f1,f2);
	F5=et(f1,f2);
	
	numerote(f1,m1);
	numerote(F2,m2);
	numerote(F3,m3);
	numerote(F4,m4);
	numerote(F5,m5);
	tseitin(f1,m1,c1,next1);
	tseitin(F1,m1,c2,next1);
	tseitin(F2,m2,c3,next2);
	tseitin(F3,m3,c4,next2);
	tseitin(F4,m4,c5,next2);
	tseitin(F5,m5,c6,next2);
	
	CHECK(c1.size()==0);
	CHECK(c2.size()==0);
	CHECK(c3.size()==3);
	CHECK(c4.size()==3);
	CHECK(c5.size()==3);
	CHECK(c6.size()==3);
}

TEST(tseitin_cnf){
	formule f1,f2,F1,F2,F3,F4,F5;
	cnf_t c1,c2,c3,c4,c5,c6;
	f1=variable((string) "p");
	f2=variable((string) "q");
	F1=non(f1);
	F2=ou(f1,f2);
	F3=impl(f1,f2);
	F4=equiv(f1,f2);
	F5=et(f1,f2);
	
	c1=tseitin(f1);
	c2=tseitin(F1);
	c3=tseitin(F2);
	c4=tseitin(F3);
	c5=tseitin(F4);
	c6=tseitin(F5);
	CHECK(c1.size()==1);
	CHECK(c2.size()==1);
	CHECK(c3.size()==4);
	CHECK(c4.size()==4);
	CHECK(c5.size()==4);
	CHECK(c6.size()==4);
}

TEST(pivot){
	cls_t cl1,cl2;
	lit_t l;
	cl1.insert(0);cl1.insert(3);cl1.insert(4);
	cl2.insert(1);cl2.insert(8);cl2.insert(10);
	l=pivot(cl1,cl2);
	CHECK(l==0);
}

TEST(resolvante){
	cls_t cl1,cl2,clr,clt;
	lit_t l;
	cl1.insert(0);cl1.insert(3);cl1.insert(4);
	cl2.insert(1);cl2.insert(8);cl2.insert(10);
	l=pivot(cl1,cl2);
	clr=resolvante(cl1,l,cl2);
	clt.insert(3);clt.insert(4);clt.insert(8);clt.insert(10);
	CHECK(clr.size()==4);
	CHECK(clr==clt);
}

TEST(resolution_1er){
	cls_t cl1,cl2;
	cnf_t test,test2;
	cl1.insert(3);
	cl2.insert(2);
	test.push_back(cl1);
	test.push_back(cl2);

	formule f1,f2,f3,f4,f5;
	f1=variable((string)"a");
	f2=variable((string)"b");
	f3=variable((string)"c");
	f4=ou(f1,f2);
	f5=impl(f4,f3);
	test2=tseitin(f5);

	CHECK(resolution_first(test));
	CHECK(!resolution_first(test2));
}

TEST(subsume){
	cls_t cl1,cl2;
	cl1.insert(0);cl1.insert(8);
	cl2.insert(0);cl2.insert(1);cl2.insert(5);cl2.insert(8);cl2.insert(12);
	CHECK(subsume(cl1,cl2));
}

TEST(subsumeCNF){
	cls_t cl1,cl2,cl3;
	cnf_t c;
	cl1.insert(0);cl1.insert(8);
	cl2.insert(0);cl2.insert(1);cl2.insert(5);cl2.insert(8);cl2.insert(12);
	cl3.insert(8);
	c.push_back(cl1);c.push_back(cl2);
	CHECK(subsume(cl1,cl2));
	CHECK(subsumeCNF(c,cl3));
}

TEST(resolution_2eme){
	cls_t cl1,cl2;
	cnf_t test,test2;
	cl1.insert(3);
	cl2.insert(2);
	test.push_back(cl1);
	test.push_back(cl2);

	formule f1,f2,f3,f4,f5;
	f1=variable((string)"a");
	f2=variable((string)"b");
	f3=variable((string)"c");
	f4=ou(f1,f2);
	f5=impl(f4,f3);
	test2=tseitin(f5);

	CHECK(resolution_second(test));
	CHECK(!resolution_second(test2));
}

TEST(resolution_3eme){
	cls_t cl1,cl2;
	cnf_t test,test2;
	cl1.insert(3);
	cl2.insert(2);
	test.push_back(cl1);
	test.push_back(cl2);

	formule f1,f2,f3,f4,f5;
	f1=variable((string)"a");
	f2=variable((string)"b");
	f3=variable((string)"c");
	f4=ou(f1,f2);
	f5=impl(f4,f3);
	test2=tseitin(f5);

	CHECK(resolution_third(test));
	CHECK(!resolution_third(test2));
}

TEST(add_clause){
	cnf_t cnf;
	idx_t idx_simple,idx_multiple;
	cls_t cls1, cls2, cls3, cls4, cls5;
	cls1 = {0};
	cls2 = {2,6};
	cls3 = {1,0,3};
	cls4 = {10,6,4,9};
	cls5 = {0,9,11,12};
	
	/* Insértion et indexation des clauses*/
	add_clause(cnf,idx_simple,idx_multiple,cls1);
	add_clause(cnf,idx_simple,idx_multiple,cls2);
	add_clause(cnf,idx_simple,idx_multiple,cls3);
	add_clause(cnf,idx_simple,idx_multiple,cls4);
	add_clause(cnf,idx_simple,idx_multiple,cls5);
	/* Vérification de la taille de la CNF*/ 
	CHECK(cnf.size() == 5);
	/* On vérifie que la taille de l'index est égale à 10*/
	CHECK(idx_simple.size() == 5);
	/* On vérifie que les indexes n'ayant pas de clauses correspondantes soient vide*/
	CHECK(idx_simple[1].empty());
}

TEST(est_subsumee){
	cnf_t cnf;
	idx_t idx_simple, idx_multiple;
	cls_t cls1, cls2, cls3, cls4, cls5;
	cls_t insert;
	/*Création des clauses*/
	cls1 = {0};
	cls2 = {5,4};
	cls3 = {1,0,3};
	cls4 = {7,3,4,8};
	cls5 = {12,9,10,22};
	/*Insértion et indexation des clauses*/
	add_clause(cnf,idx_simple,idx_multiple,cls1);
	add_clause(cnf,idx_simple,idx_multiple,cls2);
	add_clause(cnf,idx_simple,idx_multiple,cls3);
	add_clause(cnf,idx_simple,idx_multiple,cls4);
	add_clause(cnf,idx_simple,idx_multiple,cls5);
	/*une clause non subsumée*/
	insert = {2,3};
	CHECK(!est_subsumee(cnf,idx_simple,insert));
	insert.clear();
	/*une clause subsumée*/
	insert = {2,5,4,22,12,13,14};
	CHECK(est_subsumee(cnf,idx_simple,insert));
	insert.clear();
	insert = {5,4,1,0,3};
	CHECK(est_subsumee(cnf,idx_simple,insert));
	insert.clear();
}


int main()
{
  return UnitTest::RunAllTests(); // un peu magique: lance tous les tests
}
