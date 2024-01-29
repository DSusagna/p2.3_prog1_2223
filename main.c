/***********************************************************************
*
* @Propòsit: Aquest programa està dissenyat per proporcionar un diagnòstic als 
* 				usuaris en funció dels seus símptomes. El programa llegeix un 
* 				fitxer que conté informació sobre diverses malalties i els seus 
* 				símptomes. A continuació, permet a l'usuari introduir els seus 
* 				símptomes i genera una llista de possibles malalties que puguin 
* 				tenir. El programa també pot actualitzar la rellevància dels 
* 				símptomes a la base de dades de malalties llegint des d'un fitxer 
* 				binari. El programa és interactiu, amb una interfície de menú que 
* 				guia l'usuari a través de les seves diferents opcions.
* 
* @Autor:                      David Susagna Holgado
* @Data de creació:              29/04/2023
* @Data de l'última modificació: 02/05/2023
*
***********************************************************************/
// Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"


// Constants
#define MAX_LINE 		60		// Nombre màxim de caràcters d'una línia.
#define	USER_REQUEST 	1000	// Nombre màxim de caràcters dels símptomes introduïts per l'usuari.

// Tipus propis

typedef struct {
	char disease[MAX_CHAR];
	int n_symptoms;
	LinkedList symptoms; 
	int total_relevance;
} Disease;


typedef struct {
	char disease[MAX_CHAR];
	char symptom[MAX_CHAR];
	int relevance;
} FileDisease;

typedef struct {
	char user_symptoms_name[MAX_CHAR];
} UserSymptoms;



// Procediments i funcions


/***********************************************************************
*
* @Finalitat: 	Copiar una cadena de caràcters 'origen' en una nova cadena 'destino'
*        		començant des del índex 'i' i detenint-se quan es troba el caràcter 'end'.
* 
* @Paràmetres: 	origen - cadena de caràcters que es vol copiar.
* 		      	destino - cadena de caràcters on es copiarà la cadena origen.
*          		i - índex on es començarà la còpia de la cadena origen.
*          		end - caràcter que indica el final de la còpia.
* 
* @Retorn: Índex on s'ha trobat el caràcter 'end' a la cadena origen.
* 
***********************************************************************/
int cpyString(char origen[], char destino[], int i, char end){

	int n = 0;

	n = 0;
	while(end != origen[i]){
		destino[n] = origen[i];
		i++;
		n++;
	}
	destino[n] = '\0';
	return(i);
}

/***********************************************************************
*
* @Finalitat: Obrir un fitxer amb el format especificat i retornar el punter al fitxer obert.
* 
* @Paràmetres: format - cadena de caràcters que especifica el format del fitxer a obrir.
* 
* @Retorn: Retorna el punter al fitxer obert o NULL si no s'ha pogut obrir el fitxer.
* 
***********************************************************************/
FILE * openFile(char format[MAX_CHAR]){

	char file_name[MAX_CHAR];
	FILE *file = NULL;

	scanf("%s",file_name);
	file = fopen(file_name,format);
	
	return(file);
}

/***********************************************************************
*
* @Finalitat: Tancar el fitxer obert i assignar NULL al punter al fitxer per evitar problemes de referència.
* 
* @Paràmetres: file - punter al punter al fitxer que es vol tancar.
* 
* @Retorn: La funció no retorna cap valor.
* 
***********************************************************************/
void closeFile(FILE **file){
	fclose(*file);
	*file = NULL;
}	

/***********************************************************************
*
* @Finalitat: Mostrar un menú d'opcions a l'usuari i assignar l'opció seleccionada al punter passat com a paràmetre.
* 
* @Paràmetres: opcio - punter a un enter on es guardarà l'opció seleccionada per l'usuari.
* 
* @Retorn: La funció no retorna cap valor.
* 
***********************************************************************/
int menu(){
	
	char user[MAX_CHAR];
	int aux = 0;
	int opcio = 0;

	printf("\n1. Show diseases | 2. Load research advances | 3. Help me! | 4. Quit \n");
	printf("\nEnter option: ");
	scanf("%s", user);

	aux = atoi(user);

	switch (aux) {
        case 1:
            opcio = 1;
            break;
        case 2:
            opcio = 2;
            break;
        case 3:
            opcio = 3;
            break;
        case 4:
            opcio = 4;
            break;
        default:
            // Opción inválida
            printf("\nERROR: Wrong option\n");
            opcio = 0;
            break;
    }
    return(opcio);
}

/***********************************************************************
*
* @Finalitat: 	Llegir una línia de text d'un fitxer i guardar-la en una cadena de caràcters, eliminant el caràcter de nova línia.
* 
* @Paràmetres: 	name - cadena de caràcters on es guardarà la línia llegida del fitxer.
* 				doc - punter al fitxer on es llegirà la línia.
* 
* @Retorn: La funció no retorna cap valor.
* 
***********************************************************************/

void getString(char name[], FILE *doc){

	fgets(name, MAX_LINE, doc);
	name[strlen(name)-1] = '\0';		// Eliminar '\n'.

}

/***********************************************************************
*
* @Finalitat: Obtenir un element del tipus Element que conté un símptoma i la seva rellevància a partir d'una cadena de caràcters.
* 
* @Parametres: aux: cadena de caràcters que conté el símptoma i la seva rellevància en un format determinat.
* 
* @Retorn: Retorna un element del tipus Element que conté el símptoma i la seva rellevància.
* 
***********************************************************************/

Element getStringPlus(char aux[]){

	int p = 0;
	char aux_Symptoms[MAX_LINE]; 
	Element s_aux;

	

			
	// Llegir nom del símptoma
	p = 0;
	p = cpyString(aux, aux_Symptoms, 0, '(');
	aux_Symptoms[strlen(aux_Symptoms)-1] = '\0';  	// Eliminar ' '.
	strcpy(s_aux.symptom,aux_Symptoms);
	p++;

	p = cpyString(aux, aux_Symptoms, p, '\0');
	aux_Symptoms[strlen(aux_Symptoms)-1] = '\0';  	// Eliminar ')'.
	s_aux.relevance = atoi(aux_Symptoms);


	return(s_aux);
}

/***********************************************************************
*
* @Finalitat: 	Imprimir la informació de les malalties i els seus símptomes.
* 
* @Paràmetres: 	d - punter a l'estructura de dades de malalties, que conté la informació a imprimir.
* 				num_disease - nombre de malalties a imprimir.
* 
* @Retorn: 	La funció no retorna cap valor.
* 
***********************************************************************/

void printInfo(Disease * d, int num_disease){

    int k = 0, i = 0;
    Element aux_symptoms;

    for(k = 0; k < num_disease; k++){

        printf("%s ", d[k].disease);
        printf("(");

        LINKEDLIST_goToHead (&d[k].symptoms);

        for(i = 0; i < d[k].n_symptoms; i++){
	        if (!LINKEDLIST_isEmpty (d[k].symptoms)){

		        if(i != d[k].n_symptoms-1){
		        	aux_symptoms = LINKEDLIST_get (&d[k].symptoms);
	                printf("%s#%d | ", aux_symptoms.symptom, aux_symptoms.relevance);
	                LINKEDLIST_next (&d[k].symptoms);
	            }
	            else{
	            	aux_symptoms = LINKEDLIST_get (&d[k].symptoms);
	                printf("%s#%d", aux_symptoms.symptom, aux_symptoms.relevance);
	            }
	        }
        }
        printf(")\n");
    }
}



/***********************************************************************
*
* @Finalitat: Llegir informació de malalties i els seus símptomes d'un fitxer i emmagatzemar-ho en una estructura de dades.
* 
* @Parametres:	d: punter a un array de malalties on es guardaran les malalties i els seus símptomes llegits del fitxer.
* 				doc: punter al fitxer on es troba la informació de les malalties i els seus símptomes.
* 				num_disease: nombre de malalties a llegir del fitxer.
* 
* @Retorn: 	La funció no retorna cap valor.
* 
***********************************************************************/

void getDiseases(Disease *d, FILE *doc, int num_disease){
    int k = 0, i = 0;
    char aux_char[MAX_LINE];
    Element s_aux;


    fscanf(doc, "%d", &k);

    for(k=0; k<num_disease; k++){

        // Llegir nom de la malaltia
        getString(d[k].disease, doc);

        // Llegir num Symptoms
        getString(aux_char, doc);
        d[k].n_symptoms = atoi(aux_char);

        // Crear lista de síntomas para la enfermedad actual
        d[k].symptoms = LINKEDLIST_create();

        // Guardar Symptoms
        for(i = 0; i < d[k].n_symptoms; i++){
            // Llegir la línia del símptoma
            getString(aux_char, doc);
            s_aux = getStringPlus(aux_char);

            LINKEDLIST_add(&d[k].symptoms, s_aux);
            
        }
    }
}

/***********************************************************************
*
* @Finalitat: Llegir el número de registres de la informació de malalties i els seus símptomes que hi ha en un fitxer binari.
* 
* @Parametres:	bin: punter al fitxer binari on es troba la informació de les malalties i els seus símptomes.
* 
* @Retorn: Retorna el nombre de registres que hi ha en el fitxer binari.
* 
***********************************************************************/

int numFileDisease(FILE * bin){
	int i = 0;
	FileDisease aux;

	while(!feof(bin)){
		fread(&aux, sizeof(FileDisease), 1, bin);
		i++;
	}
	fseek(bin, 0, SEEK_SET);
	return(i-1);
}




/***********************************************************************
*
* @Finalitat: Actualitza la informació de les malalties del vector de malalties "d" 
* 				amb la informació del vector de malalties en format binari "fd",
* 				llegit d'un fitxer binari, i informa de les estadístiques d'actualització
* 				de simptomes i malalties ignorades.
* 
* @Parametres:	FileDisease *fd: vector de malalties en format binari llegit d'un fitxer
* 				int num_filef_disease: nombre de malalties en el vector binari
*				Disease *d: vector de malalties a actualitzar
*				int num_disease: nombre de malalties en el vector "d"
* 				FILE *bin: fitxer binari a llegir
* 
* @Retorn: La funció no retorna cap valor, simplement informa per pantalla
* 				sobre l'estadístiques de la comparació de les malalties del vector "d"
* 				amb les malalties del vector binari "fd"
* 
* ***********************************************************************/

void compareBin(FileDisease *fd,int num_filef_disease,Disease * d,int num_disease,FILE * bin){

	int i = 0, k = 0,j = 0;
	int ignored = 0, found = 0;
	int existeix = 0, symptoms_upgrade = 0, symptoms_added = 0;
	Element s_aux;

	


	for (i = 0; i < num_filef_disease; i++){
		fread(&fd[i], sizeof(FileDisease), 1, bin);
		
	}
	
	for (i = 0; i < num_filef_disease; i++){
		found = 0;
		for (k = 0; k < num_disease; k++){
			if(0 == strcmp(d[k].disease,fd[i].disease)){
				found = 1;
				existeix = 0;
				LINKEDLIST_goToHead (&d[k].symptoms);

				for(j = 0 ; j <d[k].n_symptoms; j++){

					s_aux = LINKEDLIST_get(&d[k].symptoms);
					LINKEDLIST_remove (&d[k].symptoms);

					if (0 == strcmp(fd[i].symptom, s_aux.symptom)){

						s_aux.relevance = fd[i].relevance;
						existeix = 1;
						symptoms_upgrade++;

					}

					LINKEDLIST_add(&d[k].symptoms, s_aux);			
				}

				
				if (0 == existeix){

					d[k].n_symptoms++;

					strcpy(s_aux.symptom, fd[i].symptom);
					s_aux.relevance = fd[i].relevance;

					LINKEDLIST_add(&d[k].symptoms, s_aux);
					symptoms_added++;
				}
			}
		}
		if (0 == found){
			ignored++;
		}
	}
	printf("\n%d diseases ignored\n",ignored);
	printf("%d symptoms updated\n",symptoms_upgrade);
	printf("%d symptoms added\n",symptoms_added);

}	

/***********************************************************************
*
* @Finalitat: Comptar el nombre de símptomes introduïts per l'usuari. Aquesta funció compta el nombre de "+" que apareixen en la cadena d'entrada i li suma 1.
* 
* @Parametres: La funció rep com a paràmetre la cadena de caràcters "user" que conté els símptomes introduïts per l'usuari.
* 
* @Retorn: Retorna un enter que indica el nombre de símptomes introduïts per l'usuari.
* 
***********************************************************************/

int numUserSymptoms(char user[]){
	// Contar el num_user_symptoms
	int i = 0;
	int num_user_symptoms = 1;


	
	while('\0' != user[i]){

		if ('+' == user[i]){
			num_user_symptoms++;
		}
		i++;
	}

	return(num_user_symptoms);
}



/***********************************************************************
*
* @Finalitat: Buscar les malalties més probables a partir dels símptomes introduïts per l'usuari
* 
* @Parametres:	num_user_symptoms: enter que indica el número de símptomes introduïts per l'usuari
* 				disease_num: enter que indica el número de malalties en el sistema
* 				user_symptoms: array de UserSymptoms que conté els símptomes introduïts per l'usuari
* 				d: array de Disease que conté les malalties del sistema
* 
* @Retorn: enter que indica si s'han trobat malalties probables o no (0 o 1)
* 
***********************************************************************/

int findProbable(int num_user_symptoms,int disease_num,UserSymptoms user_symptoms[],Disease d[]){

	int k = 0,i = 0, j = 0, found = 0, already_found = 0;
	int num_simptomes_txt = 0;
	Element s_aux;
	int ok = 0;


	for(k = 0; k < disease_num; k++){
		num_simptomes_txt = num_simptomes_txt + d[k].n_symptoms;

		LINKEDLIST_goToHead (&d[k].symptoms);

		for(i = 0; i < d[k].n_symptoms; i++){

			s_aux = LINKEDLIST_get(&d[k].symptoms);
			//LINKEDLIST_remove (&d[k].symptoms);
			LINKEDLIST_next (&d[k].symptoms);

			for(j = 0; j < num_user_symptoms; j++){
				if (0 == strcmp(s_aux.symptom, user_symptoms[j].user_symptoms_name)){
					d[k].total_relevance += s_aux.relevance;
					already_found = 1;
				}
				else{
					found++;
				}
			}
		}
	}

	if ((found == (num_simptomes_txt*num_user_symptoms)) && (0 == already_found)){
		ok = 0;
	}
	else{
		ok = 1;
	}

	return(ok);
}


/***********************************************************************
 * 
 * @Finalitat: Ordena les malalties de manera descendent segons la seva relevància total.
 * 
 * @Parametres:		disease_num el nombre de malalties
 * 					disease un array de malalties
 * 
 * @Retorn: 	La funció no retorna cap valor.
 * 
***********************************************************************/



void ordenarProbable(int disease_num, Disease disease[]){

	int i = 0,j = 0, ord = 0;
	Disease aux;
	
	while(!ord){
		ord=1;
		for(j = 0; j < disease_num-1; j++){

			if(disease[j].total_relevance < disease[j+1].total_relevance){

				aux = disease[j];
				disease[j] = disease[j+1];
				disease[j+1] = aux;
				ord = 0;
			}
		}
		i++;
	}	
}

/***********************************************************************
 * 
 * @Finalitat: La funció imprimeix les malalties ordenades per la seva rellevància 
 * 				total, mostrant només aquelles amb una rellevància major que zero.
 * 
 * @Parametres: disease_num Nombre de malalties a comprovar
 * 				disease Array de les malalties amb les seves dades
 * 
 * @Retorn: 	La funció no retorna cap valor.
 * 
***********************************************************************/

void printProbable(int disease_num, Disease disease[]){

	int k = 0;

	ordenarProbable(disease_num,disease);

	for(k = 0; k < disease_num; k++){
		if (0 < disease[k].total_relevance){
			printf("Total relevance %d: %s\n",disease[k].total_relevance, disease[k].disease);
			disease[k].total_relevance = 0;
		} 
	}
}



int main(){
	
	// Variables
	FILE *doc = NULL;						// Punter a fitxer pel document de text.
	FILE *bin = NULL;						// Punter a fitxer pel document de binari.
	Disease* d;
	FileDisease * fd;
	UserSymptoms *user_symptoms;
	int num_disease = 0;
	int opcio = 0;
	int num_filef_disease = 0;
	int num_user_symptoms = 0, found = 0;
	char user[USER_REQUEST];
	char buffer;
	int k = 0,j = 0, i = 0;


	printf("Enter diseases file name: ");
	doc = openFile("r");

	if (NULL == doc){
		// Output 1. S'introdueix el nom d'un fitxer que no existeix. 
    	printf("\nERROR: Diseases file not found \n");
	}
	else{

		// Guarda document TXT a l'estructura
		
		//Recollir num_disease
		fscanf(doc,"%d",&num_disease);

		// Crear memòria de la mida Diseases
		d = (Disease*)malloc(sizeof(Disease) * num_disease);
		if (NULL != d){

			getDiseases(d,doc,num_disease);



			printf("Welcome to myDoctor! \n");
			do{
				opcio = menu();


				switch(opcio){

					// Opció 1: Show diseases 
					case 1:
						printf("\nRegistered diseases: \n\n");
						printInfo(d, num_disease);
						break;

					// Opció 2: Load research advances
					case 2:
						printf("Enter research file name: ");
						// Obrir fitxer binari
						bin = openFile("rb");
						if(NULL == bin){
							// Error amb el fitxer binari
							printf("\nERROR: Research file not found \n");
						}
						else{
							num_filef_disease = numFileDisease(bin);
							fd = (FileDisease*) malloc(sizeof(FileDisease) * num_filef_disease);
							compareBin(fd,num_filef_disease,d,num_disease,bin);
							free(fd);
							fd = NULL;
							fclose(bin);
							bin = NULL;
						}
						
						break;
					// Opció 3: Help me! 
					case 3:

						printf("Enter your symptoms: ");
						scanf("%c",&buffer);
						fgets(user, USER_REQUEST, stdin);
						user[strlen(user)-1] = '\0';  	// Eliminar '\n'.

						// Contar el num_user_symptoms
						num_user_symptoms = numUserSymptoms(user);
						
						user_symptoms = (UserSymptoms*)malloc(sizeof(UserSymptoms) * num_user_symptoms);

						if(NULL != user_symptoms){

							j = 0;
							for (i = 0; i<num_user_symptoms; i++){
								if(i != num_user_symptoms-1){
									j = cpyString(user, user_symptoms[i].user_symptoms_name, j, '+');
									j++;
								}
								else{
									j = cpyString(user, user_symptoms[i].user_symptoms_name, j, '\0');
								}
							}

							found = findProbable(num_user_symptoms,num_disease,user_symptoms,d);

							if(0 == found){
								printf("\nNo diseases found!\n");
								num_user_symptoms = 0;
							}
							else{
								printf("\nMost probable diseases: \n");
								printProbable(num_disease,d);
								num_user_symptoms = 0;
							}

							free(user_symptoms);
							user_symptoms = NULL;
						}

						break;
					
				}

			} while(4 != opcio);
			

			// Alliberar memòria Diseases
			for (k = 0; k < num_disease; k++) {
			    LINKEDLIST_destroy(&d[k].symptoms); 
		    }
		    free(d);
		    d = NULL;

			printf("\nStay healthy!");
			closeFile(&doc);
		}
	}
	return(0);
}