# Calculator-expresii-aritmetice-prefixate
Am implementat un program care calculeaza valoarea unei expresii aritmetice date in forma prefixata(poloneaza)

Programul primeste ca input un fisier in care sunt definitie unele variabile si unele expresii arimtetice.
Model de input: test7.in

-----Pentru rulare:


    make
    ./calc test7.in a.out
    
In a.out se va genera rezultatele.    

In acest program am realizat un calculator care, initial, evalueaza o expresie matematica primita in forma prefixata folosind arbori de expresie care ne vor scuti de necesitatea folosirii parantezelor , in care nodurile sunt asociate operatorilor iar frunzele operanzilor.

O expresie poate fi reprezentată prin următoarele reguli:
expr ::= variabilă | valoare
expr ::= expr + expr | expr * expr | expr / expr| sqrt(expr) | expr ^ expr


----Detalii despre implementare---

1.In headerul info.h avem declarate 2 structuri care ne va ajuta
in realizarea programului.Prima denumita TVar contine campurile cu un 
char* nume_var in care vom salva numele variabilelor declarate
in fisierul input, precum si int* val_var in care vol salva 
valoarea acestor variabile. Structura TNod/*TArb va reprezenta
structura unui nod a arborelui pe care-l vom construi, in care
avem opd - operandul, opt - operatorul, flag - un flag ce ne
va ajuta la distrugerea acestui arbore, si st,dt,parinte,curent 
- ne vor ajuta sa gestionam cu introducerea in arbore.

2.Functia constr_frunza va construi o celula pentru arbore si va 
seta elementele necesare conform conditiilor de input.

3.Functia verificare_operatie, verifica daca in string-ul dat ca
parametru a fost stocat din fisierul input un operator.

4.Functia introducere_arbore apeleaza functia constr_frunza 
pentru a crea un nod nou si il aranjeaza in arbore pe pozitia 
potrivita.In general tratam 2 cazuri:
-atunci cand nodul nou creat va contine in sine un operator,prin 
urmare, urmatoarele 2(sau 1 element in cazul "sqrt") sigur vor 
fi copii acestuia.Astfel,am creat o variabila numita "curent" ce 
face parte din structura *TArb, care ne va indica arborele in 
care ne aflam,astfel ca sa putem introduce noul nod mai usor.De 
asemenea, de fiecare data il actualizam.
-atunci cand nodul nou creat va contine in sine un operand,asfel 
vom sti sigur ca nodul respectiv nu va mai avea nici un copil,
va fi o "frunza"
In momentul in care ne aflam intr-o "frunza" si mai avem 
elemente de introdus in arborele nostru, parcurgem arborele 
inapoi cu ajutorul "a->parinte" in care avem salvata adresa 
nodului-parinte a nodului curent in care ne aflam.In momentul un 
care am gasit un nod potrivit, inseram noul nod,daca nu, 
inseamna ca expresia e gresita.De asemenea, mai setam o 
conditie, in caz ca opertorul este sqrt si deja are un copil,
acesta nu mai poate avea copii.

5.Functia citire_variabile citeste din fisierul input primit ca 
parametru, variabilele si creaza un vector de tip TVar pentru a 
le stoca in el.Mai intaii citim cate variabilele vom avea, dupa 
cu o functie ciclica for, alocam memorie pentru numele 
variabilei precum si valoarea acesteia.

6.Functia efectuarea_operatii e o functie recursiva care 
analizeaza arborele si conform setarii acestuia, efectueaza 
operatiile aritmetice necesare.eror si eror2 reprezinta 2 
flag-ri de eroare cu ajutorul careia stabilim erorile 
respective.Acestea sunt setate in functie de anumite cazuri,cand 
o variabila nu e declarata, cand expresia e gresita etc, dupa 
care valoarea este trimisa prin efect lateral, iar functia data 
returneaza valoarea finala.

7.Functia creare_arb, in linii generale, creeaza un arbore 
conform expresiei din fisierul primit ca parametru, il 
evalueaza,calculand rezultatul final si in cele din urma sterge 
acest arbore.Functia primeste ca parametru un string in care e 
stocata expresia pentru noul arbore.Parcrge stringul, si citeste 
fiecare element pentru a-l introduce in arbore.Prin urmare, daca 
acesta este un numar, cu ajutorul functiei atoi il convertim in 
integer si il introducem in arbore.De asemenea, pentru acest 
caz,flagul va fi setat pe 1. Daca elemntul citit din fisier este 
operator, fapt pe care-l verificam cu ajutorul functiei 
verificare_operatie, apelam din nu functia introducere_arbore, 
unde parametrii vor fi : flag - setat by default pe 0, 
,NULL(pentru ca aici introducem un operator, respectiv *opd ce 
reprezinta un operand va fi NULL),operatorul respectiv,precum si 
arborele respectiv.Daca elementul citit este o variabila,o 
cautam in vectorul de variabile TVar*, si daca o gasim , 
introducem in arbore valoarea acesteia, in caz ca nu gasim o 
asemenea valoarea, creeam un nou nod in care *opt si *opd vor fi 
nule, astfel acesta fiind ca un semnal de eroare, vom sti ca 
variabila data nu este declarata.Spre final,apelam functia 
efectuare_operatii pentru a calcula valoarea arborelui de 
expresie, dupa care analizam flagurile de eroare eror si eror2 
si in functia de acestea scrim in fisierul output valoarea 
acestui arbore de expresie sau erorile respective. La sfarsit, 
dezalocam memoria ocupata de arbore prin intermediul functiei 
distrugere.

8.Functia distrugere, elibereaza memoria ocupata de arbore 
recursiv.

9.Functia creare citeste din fisierul input variabilele cu 
ajutorul functiei citire_variabile.Dupa care citeste expresiile 
pentru arborii necesari de creeat, si pentru fiecare in parte, 
afla valoarea si o introduce in fisierul output cu ajutorul 
functiei creare_arb. Dupa care sterge vectorul de variabile 
TVar* din memorie cu ajutorul functiei eliberare_vect.

10.Functia eliberare_vect parcurge vectorul de variabile TVar* si 
dezaloca memoria ocupata pentru numele. si valoarea fiecarei 
variabile
