#include "resolution.hpp"
#include <queue>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
//               ICI LES FONCTIONS SONT A IMPLEMENTER                         //
////////////////////////////////////////////////////////////////////////////////
struct less_priority {
	inline bool operator() (const cls_t& lhs, const cls_t& rhs) const{
		if (lhs.size() < rhs.size())
			return (lhs<rhs);
		else
			return (lhs>rhs);
	}
};

lit_t tseitin(formule f, const map<string, var_t> & m, cnf_t & c, var_t & next){
	lit_t retour;
	lit_t p1,p2,l;
	cls_t cl1,cl2,cl3,cl4,cl5;
	switch(f->op){
		case o_variable:
			var_t var;
			var=m.find(*f->nom)->second;
			retour = var2lit(var);
			return retour;
		case o_non:
			p1=tseitin(f->arg,m,c,next);
			retour=neg(p1);
			return retour;
		case o_et:
			p1=tseitin(f->arg1,m,c,next);
			p2=tseitin(f->arg2,m,c,next);
			l = var2lit(next);
			next++;
			cl1.insert(l);cl1.insert(neg(p1));cl1.insert(neg(p2));
			
			cl2.insert(neg(l));cl2.insert(p1);
			
			cl3.insert(neg(l));cl3.insert(p2);
			
			c.push_back(cl1);c.push_back(cl2);c.push_back(cl3);
			
			return l;
		case o_ou:
			p1=tseitin(f->arg1,m,c,next);
			p2=tseitin(f->arg2,m,c,next);
			l = var2lit(next);
			next++;
			cl1.insert(neg(l));cl1.insert(p1);cl1.insert(p2);
			
			cl2.insert(l);cl2.insert(neg(p1));
			
			cl3.insert(l);cl3.insert(neg(p2));
			
			c.push_back(cl1);c.push_back(cl2);c.push_back(cl3);
			return(l);
		case o_equivaut:
			
			p1=tseitin(f->arg1,m,c,next);
			p2=tseitin(f->arg2,m,c,next);
			l = var2lit(next);
			next++;
			cl1.insert(neg(l));cl1.insert(neg(p1));cl1.insert(p2);
			
			cl2.insert(neg(l));cl2.insert(p1);cl2.insert(neg(p2));
			
			cl3.insert(l);cl3.insert(p1);cl3.insert(p2);
			
			cl4.insert(l);cl4.insert(neg(p1));cl4.insert(neg(p1));
			
			c.push_back(cl1);c.push_back(cl2);c.push_back(cl3);
			return(l);
		case o_implique:
			p1=tseitin(f->arg1,m,c,next);
			p2=tseitin(f->arg2,m,c,next);
			l = var2lit(next);
			next++;
			cl1.insert(neg(l));cl1.insert(neg(p1));cl1.insert(p2);
			
			cl2.insert(l);cl2.insert(p1);
			
			cl3.insert(l);cl3.insert(neg(p2));
			
			c.push_back(cl1);c.push_back(cl2);c.push_back(cl3);
			return(l);
	}
	return 0;
}

cnf_t tseitin(formule f){
	cnf_t c;
	lit_t l;
	cls_t cl,cl2;
	map<string,var_t> m;
	var_t next;
	
	numerote(f,m);
	next=m.size();
	l=tseitin(f,m,c,next);
	cl2.insert(l);
	c.push_back(cl2);
	return c;
}

lit_t pivot(const cls_t& cl1, const cls_t& cl2){
	for(cls_t::const_iterator it=cl1.cbegin();it!=cl1.cend();++it){
		for(cls_t::const_iterator it2=cl2.cbegin();it2!=cl2.cend();++it2){
			if((*it%2==0 && *it==*it2-1)||(*it%2==1 && *it==*it2+1))
				return *it;
		}
	}
	return -1;
}
	
cls_t resolvante(const cls_t& cl1, lit_t l, const cls_t& cl2){
	cls_t res;
	l=pivot(cl1,cl2);
	for(cls_t::const_iterator it=cl1.cbegin();it!=cl1.cend();++it){
		if(*it!=l)
			res.insert(*it);
	}
	for(cls_t::const_iterator it=cl2.cbegin();it!=cl2.cend();++it){
		if(*it!=neg(l))
			res.insert(*it);
	}
			
	return res;
}

bool equivaut_top(const cls_t cl){
	cls_t::const_iterator it = cl.cbegin();
	lit_t tmp=*it;
	it++;
	for(;it!=cl.cend();it++){
		if(tmp==neg(*it)){
			return true;
		}
		tmp = *it;
	}
	return false;
}

bool subsume(const cls_t& cl1, const cls_t& cl2){
	unsigned int contenu=0;
	for(cls_t::const_iterator it=cl1.begin();it!=cl1.end();it++){
		for(cls_t::const_iterator it2=cl2.begin();it2!=cl2.end();it2++){
			if(*it==*it2) 
				contenu++;
		}
	}
	if(contenu==cl1.size()) return true;
	else return false;
}

bool subsumeCNF(const cnf_t& c, const cls_t& cl){
	for(cnf_t::const_iterator it=c.cbegin();it!=c.cend();it++){
		if(subsume(cl,*it))
			return true;
	}
	return false;
}

void add_clause(cnf_t & cnf, idx_t & idx_simple, idx_t & idx_multiple, const cls_t & c){  
	cnf.push_back(c);
	lit_t l = *(c.begin());
	unsigned int i = (unsigned int)l;
	if(i >= (idx_simple.size()))
	{
		idx_simple.resize(i + 1);
	}
	idx_simple[i].insert(cnf.size() - 1);
	for(cls_t::const_iterator it = c.begin(); it != c.end(); it++)
	{
		if((*it) >= (idx_multiple.size()))
		{
			idx_multiple.resize((*it) + 1);
		}
		idx_multiple[(*it)].insert(cnf.size() - 1);
	}
}

bool est_subsumee(const cnf_t & cnf, const idx_t & idx_simple, const cls_t & c){
	for(cls_t::const_iterator it = c.begin(); it != c.end(); it++){
		if(idx_simple.size() > (size_t)(*it)){
			if(!idx_simple[*it].empty()){
				for(set<cls_ref_t>::const_iterator it2 = idx_simple[*it].begin(); it2 != idx_simple[*it].end(); it2++){
					if(cnf.at(*it2).size() == 1 || subsume(cnf.at(*it2),c))
						return true;
				}
			}  
		}
	}
	return false;    
}

bool resolution_first(const cnf_t& c){
	queue<cls_t> q;
	cnf_t travail=c;
	cls_t cl,clr;
	
	for(cnf_t::const_iterator it=travail.cbegin();it!=travail.cend();++it){
		q.push(*it);
	}
	travail.clear();
	while(!q.empty()){
		cl=q.front();
		q.pop();
		if(cl.size()==0)
			return true;
		if(!equivaut_top(cl) && !est_dans(cl,travail)){
			for(cnf_t::const_iterator cl2=c.cbegin();cl2!=c.cend();++cl2){
				lit_t pi = pivot(cl,*cl2);
				clr=resolvante(cl,pi,*cl2);
				q.push(clr);
			}
			travail.push_back(cl);
		}
	}
	return false;
}


bool resolution_second(const cnf_t& c){
	priority_queue<cls_t,vector<cls_t>,less_priority> q;
	cnf_t travail=c;
	cls_t cl,clr;
	
	for(cnf_t::const_iterator it=travail.cbegin();it!=travail.cend();++it){
		q.push(*it);
	}
	travail.clear();
	while(!q.empty()){
		cl=q.top();
		q.pop();
		if(cl.size()==0)
			return true;
		if(!equivaut_top(cl) && !subsumeCNF(travail,cl)){
			for(cnf_t::const_iterator cl2=travail.cbegin();cl2!=travail.cend();++cl2){
				lit_t pi = pivot(cl,*cl2);
				clr=resolvante(cl,pi,*cl2);
				q.push(clr);
			}
			travail.push_back(cl);
		}
	}
	return false;
}


bool resolution_third(const cnf_t& c){
	priority_queue<cls_t,vector<cls_t>,less_priority> q;
	int i=0;
	cnf_t travail=c;
	cls_t cl,clr;
	
	for(cnf_t::const_iterator it=travail.cbegin();it!=travail.cend();++it){
		q.push(*it);
	}
	travail.clear();
	while(!q.empty()){
		cl=q.top();
		q.pop();
		if(cl.size()==0)
			return true;
		if(!equivaut_top(cl) && !subsumeCNF(travail,cl)){
			for(cnf_t::const_iterator cl2=travail.cbegin();cl2!=travail.cend();++cl2){
				lit_t pi = pivot(cl,*cl2);
				clr=resolvante(cl,pi,*cl2);
				q.push(clr);
			}
			for(cnf_t::const_iterator cl3=travail.cbegin();cl3!=travail.cend();++cl3){
				i++;
				if(subsume(cl,*cl3)){
					travail.erase(travail.begin()+i);
				}
			}
			travail.push_back(cl);
		}
	}
	return false;
}

//Pas terminée
/*bool resolution_fourth(const cnf_t& c){
	priority_queue<cls_t,vector<cls_t>,less_priority> q;
	cnf_t travail=c;
	cls_t cl,clr;
	idx_t index_s,index_m;
	
	for(cnf_t::const_iterator it=travail.cbegin();it!=travail.cend();++it){
		
		q.push(*it);
	}
	travail.clear();
	while(!q.empty()){
		cl=q.top();
		add_clause(travail,index_s,index_m,cl);
		q.pop();
		if(cl.size()==0)
			return true;
		if(!equivaut_top(cl) && !est_subsumee(travail,index_m,cl)){
			for(cnf_t::const_iterator cl2=travail.cbegin();cl2!=travail.cend();++cl2){
				lit_t pi = pivot(cl,*cl2);
				clr=resolvante(cl,pi,*cl2);
				q.push(clr);
			}
			travail.push_back(cl);
		}
	}
	return false;
}*/
