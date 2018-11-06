
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

   L->Act = NULL;
   L->First = NULL;
   L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    tDLElemPtr tmp;
    while (L->First != NULL) {
        tmp = L->First;                     //Ulozi prvy prvok do pomocnej premennej
        L->First = L->First->rptr;          //Nasledujuci prvok sa stane prvym
        free(tmp);                          //Zrusi prvy prvok
    }
    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

    if (newElem == NULL) {
        DLError();                          //Nedostatok pamati
    } else {
        newElem->data = val;                //Vlozi hodnotu do data
        newElem->rptr = L->First;           //Prvy prvok nastavy na nasledujuci
        newElem->lptr = NULL;               //Predchadzajuci prvok neexistuje

        if (L->Last == NULL) {
            L->Last = newElem;              //Ked novy prvok je jediny v zozname tak je aj posledny
        } else {
            L->First->lptr = newElem;       //Ked novy prvok neni jediny v zozname, druhy ukazuje na predosli
        }

        L->First = newElem;                 //Novy prvok sa stane prvym
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

    if (newElem == NULL) {
        DLError();                            //Nedostatok pamati
    } else {
        newElem->data = val;                  //Vlozi hodnotu do data
        newElem->rptr = NULL;                 //Nasledujuci prvok neexistuje
        newElem->lptr = L->Last;              //Predchadzajuci prvok je posledny

        if (L->First == NULL) {
            L->First = newElem;               //Ked je zoznam prazdny, novy prvok je prvy
        } else {
            L->Last->rptr = newElem;          //Ked zoznam neni prazdny, posledny ukazuje na novy
        }

        L->Last = newElem;                    //Novy sa stane poslednym
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->First;          //Prvy sa stane aktualnym
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->Last;           //Posledny sa stane aktualnym
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if (L->First == NULL) {
        DLError();                      //Zoznam je prazdny
    } else {
        *val = L->First->data;          //Vlozi hodnotu prveho prvku do val
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if (L->First == NULL) {
        DLError();                      //Zoznam je prazdny
    } else {
        *val = L->Last->data;           //Vlozi hodnotu posledneho prvku do val
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    if (L->First != NULL) {                     //Zoznam neni prazdny
        tDLElemPtr tmp = L->First;              //Pomocna premenna

        if (L->First == L->Act) {
            L->Act = NULL;                      //Ked je prvy zaroven aj aktivny, tak zrusi aj aktivny
        }

        if (L->First == L->Last) {              //Ked zoznam ma len jeden prvok, zrusi aj posledny aj aktivny
            L->Last = NULL;
            L->First = NULL;
            L->Act = NULL;
        } else {
            L->First = L->First->rptr;          //Ked zoznam ma viac prvkov, prvym sa stane druhy
            L->First->lptr = NULL;              //Novy prvy nema uz predosli prvok
        }

        free(tmp);                              //Zrusi povodny prvy prvok
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    if (L->First != NULL) {                 //Zoznam nie je prazdny
        tDLElemPtr tmp = L->Last;           //Pomocna premenna

        if (L->Last == L->Act) {
            L->Act = NULL;                  //Ked je posledna zaroven aj aktivna, zrusi aktivnu
        }

        if (L->Last == L->First) {          //Ked zoznam ma len jeden prvok, zrusi aj prvy aj aktivny
            L->Last = NULL;
            L->Act = NULL;
            L->First = NULL;
        } else {
            L->Last = L->Last->lptr;        //Ked zoznam ma viac prvkov, poslednym sa stane predposledny
            L->Last->rptr = NULL;           //Novy posledny prvok uz nasledujuci prvok nema
        }

        free(tmp);                          //Zrusi povodny posledny prvok
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

    if (L->Act != NULL && L->Last != L->Act) {          //Zoznam je aktivny a posledny neni aktivny
        tDLElemPtr tmp = L->Act->rptr;                  //Pomocna premenna

        if (L->Act->rptr == L->Last) {                  //Ked prvok za aktivnym je zaroven posledny
            L->Last = L->Act;                           //Poslednym sa stane aktivny
            L->Act->rptr = NULL;                        //Za aktivnym uz ziadny prvok neni
        } else {
            L->Act->rptr = L->Act->rptr->rptr;          //Preskocime jeden prvok
            L->Act->rptr->lptr = L->Act;
        }

        free(tmp);                                      //Zrusime povodny prvok po aktivnom
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

    if (L->Act != NULL && L->First != L->Act) {          //Zoznam je aktivny a prvy neni aktivny
        tDLElemPtr tmp = L->Act->lptr;                   //Pomocna premenna

        if (L->Act->lptr == L->First) {                  //Ked prvok pred aktivnym je zaroven prvy
            L->First = L->Act;                           //Prvy sa stane aktivny
            L->Act->lptr = NULL;                         //Pred aktivnym uz ziadny prvok neni
        } else {
            L->Act->lptr = L->Act->lptr->lptr;          //Preskocime jeden prvok
            L->Act->lptr->rptr = L->Act;
        }

        free(tmp);                                      //Zrusime povodny prvok pred aktivnym
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    if (L->Act != NULL) {
        tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
        if (newElem == NULL) {
            DLError();                           //Nedostatok pamati
        } else {
            newElem->data = val;                 //Vlozi hodnotu do val

            if (L->Act == L->Last) {
                L->Last = newElem;              //Ked posledny je aktivny, poslednym sa stane novy prvok
            } else {
                L->Act->rptr->lptr = newElem;   //Ked posledny nie je aktivny
            }

            newElem->rptr = L->Act->rptr;
            newElem->lptr = L->Act;
            L->Act->rptr = newElem;
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    if (L->Act != NULL) {
        tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
        if (newElem == NULL) {
            DLError();                           //Nedostatok pamati
        } else {
            newElem->data = val;                 //Vlozi hodnotu do val

            if (L->Act == L->First) {
                L->First = newElem;              //Ked prvy je aktivny, prvym sa stane novy prvok
            } else {
                L->Act->lptr->rptr = newElem;    //Ked prvy nie je aktivny
            }

            newElem->lptr = L->Act->lptr;
            newElem->rptr = L->Act;
            L->Act->lptr = newElem;
        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

    if (L->Act == NULL) {
        DLError();                  //Zoznam nie je aktivny
    } else {
        *val = L->Act->data;        //Vlozi data do val
    }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

    if (L->Act != NULL) {       //Zoznam je aktivny
        L->Act->data = val;     //Prepise data aktivneho prvku
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

    if (L->Act != NULL) {           //Zoznam je aktivny
        if (L->Act == L->Last) {
            L->Act = NULL;          //Ked aktivny je zaroven aj posledny, zrusi aktivitu
        } else {
            L->Act = L->Act->rptr;  //Posunie aktivitu na nasledujuci prvok
        }
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

    if (L->Act != NULL) {           //Zoznam je aktivny
        if (L->Act == L->First) {
            L->Act = NULL;          //Ked aktivny je zaroven aj prvy, zrusi aktivitu
        } else {
            L->Act = L->Act->lptr;  //Posunie aktivitu na predosli prvok
        }
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return (L->Act != NULL);
}

/* Konec c206.c*/
