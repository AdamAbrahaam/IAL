
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    L->Act = NULL;
    L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/

    L->Act = NULL;                  //Zrusime aktualny prvok

    tElemPtr tmp;                   //Pomocna premenna pre rusenie zoznamu

    while(L->First != NULL) {       //Rusenie celeho zoznamu
        tmp = L->First;
        L->First = L->First->ptr;
        free(tmp);
    }
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/

    //Alokacia noveho prvoku
    tElemPtr newElem = malloc(sizeof(struct tElem));

    if (newElem == NULL) {
        Error();                    //Nepodaril sa malloc
    } else {
        newElem->data = val;        //Vlozi hodnotu do data
        newElem->ptr = L->First;    //Ukazatel na povodni prvy prvok
        L->First = newElem;         //Novy prvok sa stane prvym
    }
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/

    L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/

    if (L->First != NULL) {
        *val = L->First->data;      //Vlozi hodnotu data do val
    } else {
        Error();                    //Zoznam je prazdny
    }
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/

    //Ked zoznam nie je prazdny
    if (L->First != NULL) {

        //Ked prvy prvok je aj aktivny, zrusi aktivitu
        if (L->Act == L->First) {
            L->Act = NULL;
        }

        //Ulozi prvy prvok do pomocnej premennej
        tElemPtr tmp = L->First;

        //Nastavi druhy prvok ako prvy a vymaze povodny prvy
        L->First = L->First->ptr;
        free(tmp);
    }
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/

    if(L->Act != NULL && L->Act->ptr != NULL){
        tElemPtr tmp = L->Act->ptr;         //Ulozi prvok za aktivnym do pomocnej premennej
        L->Act->ptr = L->Act->ptr->ptr;     //Nahradi prvok za aktivnym s 2. prvkom za aktivnym
        free(tmp);                          //Zrusi prvok za aktivnym
    }
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/

    //Zoznam je aktivny
    if (L->Act != NULL) {
        tElemPtr newElem = malloc(sizeof(struct tElem));    //Alokacia noveho prvku

        if (newElem == NULL) {
            Error();                                        //Nepodarila sa alokacia
        } else {
            newElem->data = val;                            //Vlozi hodnotu do noveho prvku
            newElem->ptr = L->Act->ptr;                     //Novy prvok ukazuje na prvok za aktivnym
            L->Act->ptr = newElem;                          //Vlozi novy prvok za aktivnym
        }
    }
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/

    if (L->Act == NULL) {
        Error();                //Zoznam neni aktivny -> Error
    } else {
        *val = L->Act->data;    //Vlozi hodnotu z aktivneho prvku do val
    }
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/

    if (L->Act != NULL) {
        L->Act->data = val;     //Vlozi hodnotu val do aktivneho prvku
    }
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/

    if (L->Act != NULL) {
        if (L->Act->ptr != NULL) {
            L->Act = L->Act->ptr;       //Posunie aktivny prvok ked dalsi existuje
        } else {
            L->Act = NULL;              //Ked dalsi neexistuje stane sa neaktivnym
        }
    }
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/

    return (L->Act != NULL);
}

/* Konec c201.c */
