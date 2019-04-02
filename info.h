#ifndef ARB
#define ARB

typedef struct 
{
	char* nume_var;
	int* val_var;
}TVar;

typedef struct nod
{
	int *opd;
	char* opt;
	int flag;
	struct nod *st,*dr,*parinte,*curent;
}TNod,*TArb;

#endif
