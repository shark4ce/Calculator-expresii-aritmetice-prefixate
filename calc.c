#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "info.h"

TArb constr_frunza(int flag,int* opd,char* opt,TArb parinte)
{
	TArb aux = NULL;
	aux = (TArb)malloc(sizeof(TNod));
	if(!aux) return NULL;
	aux->opt = opt;
	aux->opd = opd;
	aux->parinte = parinte;
	aux->curent = NULL;
	aux->st = NULL;
	aux->dr = NULL;
	aux->flag = flag;
	return aux;
}

void distrugere(TArb r)
{	
	if( r!= NULL )
	{
		if(r->flag == 1)
			free(r->opd);
		if (r->st)
			distrugere(r->st);
		if (r->dr)
			distrugere(r->dr);		
		free(r);
	}
}

int verificare_operatie(char *v)
{
	if (strcmp(v,"+") == 0 || strcmp(v,"-") == 0 || strcmp(v,"*") == 0
	 || strcmp(v,"/") == 0 || strcmp(v,"^") == 0 || strcmp(v,"sqrt") == 0)
		return 1;
	else return 0;
}

TArb introducere_arbore(int flag,int *opd,char* opt,TArb a)
{
	//daca arborele este creem primul element care va fi si radacina acestuia
	if (a == NULL) 
	{
		a = constr_frunza(flag,NULL,opt,a);
		a->curent = a;
		return a;
	}
	//in caz ca pentru sqrt deja avem un operand, ne intoarcem catre parintele nodului cu sqrt
	if(a->curent->opt != NULL && a->curent->st != NULL)
		if(strcmp(a->curent->opt,"sqrt") == 0) a->curent = a->curent->parinte;
	/*daca am suntem intr-un nod cu un operator ce are ambii copii setati, mergem inapoi 
	cu *parinte pentru a determina daca mai avem "locuri libere"*/
	if(a->curent->st != NULL && a->curent->dr != NULL)
	{
		while(a->curent != NULL)
		{
			a->curent = a->curent->parinte;
			if((a->curent->st == NULL) || 
				((a->curent->dr == NULL) && (strcmp(a->curent->opt,"sqrt") != 0))) break;
		}
	}
	/*daca vrem sa introducem un operator atunci curentul devine nodul respectiv,insa daca introducem un numar,
	atunci curentul ramane acelasi, in general a->curent intotdeauna va fi setat pe un nod cu un operator*/
	if(a->curent->st == NULL)
	{
		if(opt != NULL)
		{
			a->curent->st = constr_frunza(flag,NULL,opt,a->curent);
			a->curent = a->curent->st;
		}else{
			a->curent->st = constr_frunza(flag,opd,NULL,a->curent);
		}
	}else if(a->curent->dr == NULL)
	{
		if(opt != NULL)
		{
				a->curent->dr = constr_frunza(flag,NULL,opt,a->curent);
				a->curent = a->curent->dr;
		}else{
			a->curent->dr = constr_frunza(flag,opd,NULL,a->curent);
		}
	}
	return a;
}

TVar* citire_variabile(FILE *file,int *nr_var)
{	
	int i;
	char* elm_Arb = NULL;
	elm_Arb = (char*)malloc(sizeof(char)*300);
	if(!elm_Arb) return NULL;

	/*cu fgets citim cate variabile sunt declarate in fisier,convertam char-ul in int 
	si prin efect lateral il trimitem in alta functie*/
	fgets(elm_Arb,10,file);
	*nr_var = atoi(elm_Arb);

	TVar* vector_var = NULL;
	if(*nr_var != 0)
	{
		vector_var = (TVar*)malloc(sizeof(TVar)*(*nr_var));
		if(!vector_var) return NULL;
		/*creem vectorul de variabile ce va contine atat numele cat si valoarea acestora*/
		for(i = 0; i < *nr_var; i++)
		{
			fgets(elm_Arb,100,file);
			vector_var[i].val_var = (int*)malloc(sizeof(int));
			if (!vector_var[i].val_var) return NULL;
			sscanf(elm_Arb,"%*[^-0123456789]%d",vector_var[i].val_var);
			vector_var[i].nume_var = (char*)malloc(sizeof(char)*10);
			if(!vector_var[i].nume_var) return NULL;
			strcpy(vector_var[i].nume_var,strtok(elm_Arb," "));
		}
		free(elm_Arb);
	}else return NULL;
return vector_var;
}

int efectuare_operatii(TArb a,int *eror,int *eror2)
{
	/* recursiv citim operanzii si in functie de operator
	efectuam o anumita operatie rezultatul careia o intarocem recursiv,
	pana ajungem la final. eror si eror2 sunt 2 flag-uri pe care le setam 
	cu anumite valori un anumite cazuri si le trimiem valoarea prin efect
	lateral*/
	if(!a) return 0;
	if(a->opd != NULL)
		return *a->opd;

	if(a->opt == NULL && a->opd == NULL)
	{
		(*eror2) = -2;
		return 0;
	}

	int v_st = efectuare_operatii(a->st,eror,eror2);
	int v_dr = efectuare_operatii(a->dr,eror,eror2);

	if((strcmp(a->opt,"+") == 0))
	{
		if(a->st != NULL && a->dr != NULL)
			return v_st + v_dr;
		else (*eror) = -1;
	}
	if((strcmp(a->opt,"-") == 0))
	{
		if(a->st != NULL && a->dr != NULL)
			return v_st - v_dr;
		else (*eror) = -1;
	}
	if((strcmp(a->opt,"*") == 0))
	{
		if(a->st != NULL && a->dr != NULL)
			return v_st * v_dr;
		else (*eror) = -1;
	}
	if((strcmp(a->opt,"/") == 0))
	{
		if(a->st != NULL && a->dr != NULL)
		{
			if(v_dr == 0)
			{
				(*eror) = -1;
				if((*eror2) == -2)
					(*eror2) = -1;	
			 	return 0;
			}else return v_st / v_dr;
		}else (*eror) = -1;
	}
	if((strcmp(a->opt,"^") == 0))
	{
		if(a->st != NULL && a->dr != NULL)
			return pow(v_st,v_dr);
		else (*eror) = -1;
	}
	if((strcmp(a->opt,"sqrt") == 0))
	{
		if(v_st < 0)
		{
			(*eror) = -1;
			if((*eror2) == -2)
				(*eror2) = -1;
			return 0;
		}else return sqrt(v_st);
	}
	return 0;
}

void creare_arb(TVar* vector_var,char* vect_elA,int nr_el_vA,char* file)
{
	/*in aceasta functie creeam arborele conform datelor primite din fisier
	,"calculam" valoarea acestuia , dupa il stergem.*/
	FILE *fd;
	fd = fopen(file,"a");

	TArb a = NULL;
	int i = 0;
	char *elm_Arb = NULL;
	elm_Arb = strtok(vect_elA," ");

	char* nume_var_und = NULL;
	int flag = 0;

	while(elm_Arb != NULL)
	{
		if(((elm_Arb[0] >= '0') && (elm_Arb[0] <= '9')) || ((elm_Arb[1] >= '0') && (elm_Arb[1] <= '9')) )
		{
			int *l = (int*)malloc(sizeof(int));
			if(!l) return;

			*l = atoi(elm_Arb);
			a = introducere_arbore(1,l,NULL,a);
		}else if(verificare_operatie(elm_Arb) == 1)
			a = introducere_arbore(flag,NULL,elm_Arb,a);
		else if( vector_var != NULL)
		{
			int z = 0;
			for(i = 0; i < nr_el_vA; i++)
			{
				if(strcmp(vector_var[i].nume_var,elm_Arb) == 0)
				{
					z++;
					break;
				}
			}
			if(z > 0)
				a = introducere_arbore(flag,vector_var[i].val_var,NULL,a);
			else
			{
				nume_var_und = elm_Arb;
				a = introducere_arbore(flag,NULL,NULL,a);
			}
		}		
		elm_Arb = strtok(NULL," ");
	}
	int rez = 0,eror = 0,eror2 = 0;
	rez = efectuare_operatii(a,&eror,&eror2);
	
	if(((eror == -1) && (eror2 == -2)) || ((eror == -1) && (eror2 == 0)))
	{
		fprintf(fd,"Invalid expression\n");
		fseek(fd,1,SEEK_END);
		fclose(fd);
		distrugere(a);
		return;
	}else if(((eror == 0) && (eror2 == -2)) || ((eror == -1) && (eror2 == -1)))
	{
		fprintf(fd,"Variable %s undeclared\n",nume_var_und);	
		fseek(fd,1,SEEK_END);
		fclose(fd);
		distrugere(a);
		return;
	}else{
		fprintf(fd,"%d\n",rez);
		fseek(fd,1,SEEK_END);
		fclose(fd);
	}
	distrugere(a);
	return ;
}

void eliberare_vect(TVar* vector_var, int n)
{
	int i;
	for(i = 0; i < n; i++ )
	{
		free(vector_var[i].nume_var);
		free(vector_var[i].val_var);
	}
	free(vector_var);
}

void calculare(FILE *file,char* file2)
{
	int i,nr_var;
	TVar* vector_var = citire_variabile(file,&nr_var);
	char* vect_elA = NULL;
	vect_elA = (char*)malloc(sizeof(char)*300);
	if(!vect_elA) return ;

	fgets(vect_elA,300,file);
	int nr_arb = atoi(vect_elA);

	for (i = 0; i < nr_arb; i++)
	{
		fgets(vect_elA,300,file);
		vect_elA = strtok(vect_elA,"\n"); //eliminam \n de la sfarsit
		creare_arb(vector_var,vect_elA,nr_var,file2);
	}
	eliberare_vect(vector_var,nr_var);
	free(vect_elA);	
	return;
}

int main(int argc, char **argv)
{
	FILE *fileIN;
	fileIN = fopen(argv[1],"r");
	calculare(fileIN,argv[2]);
	fclose(fileIN);
	return 0;
}