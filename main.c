#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct info
{   unsigned int varsta;
    unsigned int len; 
} info;

typedef struct data_structure
{   info *header;
    void *data; 
} data_structure;

int add_last(void **baza, int *len, data_structure *data)
{   if (*baza == NULL)
        *baza = malloc(sizeof(info) + data->header->len); 
    else
        *baza = realloc(*baza, *len + sizeof(info) + data->header->len);
     
    void *pointer = *baza;
    *baza = *baza + *len;
    
    memcpy(*baza, data->header, sizeof(info));
    (*baza) = ((char *)*baza + 4); 
    size_t size = *(int *)(*baza); 
    (*baza) = ((int *)*baza + 1);
    memcpy(*baza, data->data, size);
    *baza = pointer;

    return 0;
}

// functie delete
int delete_a_user(void **baza, int *len, int index)
{   void *pointer = *baza;
   
    int delete = 0;
    
    int lungime_data = *(int *)(*baza + 4);
    int suma = 0;
    while (delete != index) {
        suma = suma + sizeof(info) + lungime_data;
        *baza = *baza + sizeof(info) + lungime_data;
        lungime_data = *(int *)(*baza + 4);
        delete++;
    }
     
    // modificare date
    int pas = sizeof(info) + lungime_data;
    // adresa de dupa locul ocupat de element
    suma = suma + pas;
    // zona ramasa dupa stergerea elementului
    size_t size = *len - suma;
    // copiere bytes din zona de dupa element in zona elementului 
    memcpy(*baza, *baza + pas, size); // un fel de suprascriere a datelor
    *baza = pointer; // adresa initiala pointer
    *baza = realloc(*baza, *len - pas); // realocare cu dimensiune - element sters
    *len = *len - pas;

    return 0;
}


void add_a_user(void **baza,data_structure *elem, int *len) {    
    printf("Introduceti varsta:\n");
    unsigned int varsta;
    scanf("%d", &varsta);
    elem->header = (info *)malloc(sizeof(info));
    elem->header->varsta = varsta;

    char *nume = malloc(256 * sizeof(char));
    char *parola = malloc(256 * sizeof(char));
    
    printf("Introduceti nume si parola:\n");
    scanf("%s", nume);
    scanf("%s", parola);
    
    printf("Introduceti cnp:\n");
    int64_t cnp;
    scanf("%"SCNd64"", &cnp);
    
    printf("Introduceti vot: [partid x = 0 partid y = 1]\n");
    int8_t vot;
    scanf("%"SCNd8"", &vot);

    size_t size_nume = strlen(nume) + 1;
    size_t size_parola = strlen(parola) + 1;
    size_t size_cnp = sizeof(int64_t);
    size_t size_vot = sizeof(int8_t);
    size_t total_size = size_nume + size_parola + size_vot + size_cnp;
    elem->header->len = (int)total_size;

    elem->data = malloc(total_size);
    memcpy(elem->data, nume, size_nume);
    elem->data = ((char *)elem->data + size_nume);
    memcpy(elem->data, parola, size_parola);
    elem->data = ((char *)elem->data + size_parola);
    memcpy(elem->data, &cnp, size_cnp);
    elem->data = ((int64_t *)elem->data + 1);
    memcpy(elem->data, &vot, size_vot);
    elem->data = ((int8_t *)elem->data + 1);
    elem->data = (char *)elem->data - total_size;
    
    
    if(add_last(baza, len, elem) != 0)
    exit(-1);

    *len = *len + elem->header->len + (int)sizeof(info);

    free(elem->data);
    free(nume);
    free(parola);
    free(elem->header);
    free(elem);
}

// functia de printare
void print(void *baza, int len) {
    if (len == 0)
    return;

    void *pointer = baza;
    int ok = 0; // variabila contor
    int lungime_finala = 0;
    int add = 0;
    
    while (ok == 0) {
    printf("Varsta %d\n", *(int *)(baza));
    

    int lungime_nume = strlen((char *)(baza + 8)) + 1;
    int lungime_parola = strlen((char *)(baza + 8 + lungime_nume)) + 1;
    
    printf("Nume : %s Parola : %s\n", (char *)(baza + 8), (char *)(baza + 8 + lungime_nume)); 
    printf("Cnp :%" PRId64 "\n", *(int64_t *)(baza + 8 + lungime_nume + lungime_parola));
    printf("Vot :%" PRId8 "\n", *(int8_t *)(baza + 16 + lungime_nume + lungime_parola));
    printf("\n");
    
    lungime_finala = 8 + 9 + lungime_nume + lungime_parola;
    add = add + lungime_finala;
    
    if (add < len)
        baza = baza + lungime_finala;
    else
        ok = 1;

    }

    baza = pointer;
}

void print_secret(void *baza, int len) {
    if (len == 0)
    return;

    void *pointer = baza;
    int ok = 0;
    int lungime_finala = 0;
    int add = 0;

    while (ok == 0) {
    printf("Varsta %d\n", *(int *)(baza));
    

    int lungime_nume = strlen((char *)(baza + 8)) + 1;
    int lungime_parola = strlen((char *)(baza + 8 + lungime_nume)) + 1;
    

    printf("Nume : %s Parola : ", (char *)(baza + 8)); 
    for(int i = 0 ; i < lungime_parola - 1 ; i++) {
        printf("*");
    }
    printf("\n");

    printf("Cnp : [hidden]\n");

    printf("Vot : [hidden]\n");
    
    lungime_finala = 8 + 9 + lungime_nume + lungime_parola;
    add = add + lungime_finala;
    
    if (add < len)
        baza = baza + lungime_finala;
    else
        ok = 1;

    }

    baza = pointer;
}


// functie find
void find(void *baza, int len, int index) {
    if (index < 0)
        return;

    void *pointer = baza;
    int find = 0;
    
    int lungime_data = *(int *)(baza + 4);
    while (find != index)
    {
        baza = baza + sizeof(info) + lungime_data;
        lungime_data = *(int *)(baza + 4);
        find++;
    }
    

    printf("Varsta %d\n", *(int *)(baza));


    int lungime_nume = strlen((char *)(baza + 8)) + 1;
    int lungime_parola = strlen((char *)(baza + 8 + lungime_nume)) + 1;
    printf("Nume : %s Parola : %s\n", (char *)(baza + 8), (char *)(baza + 8 + lungime_nume)); 
    printf("Cnp :%" PRId64 "\n", *(int64_t *)(baza + 8 + lungime_nume + lungime_parola));
    printf("Vot :%" PRId8 "\n", *(int8_t *)(baza + 16 + lungime_nume + lungime_parola));
    printf("\n");

    baza = pointer; // returnare adresa initiala
}

void log_in(void *baza, int len, int64_t cnp) {
    if (len == 0)
    return;

    void *pointer = baza;
    int ok = 0;
    int lungime_finala = 0;
    int add = 0;

    int flag = 0;
    
    while (ok == 0) {

    int lungime_nume = strlen((char *)(baza + 8)) + 1;
    int lungime_parola = strlen((char *)(baza + 8 + lungime_nume)) + 1;

    if(cnp == *(int64_t *)(baza + 8 + lungime_nume + lungime_parola)) {
        flag = 1;
        char password[256];
        printf("Please %s, insert password\n", (char *)(baza + 8));
        scanf("%s", password);
        
        if(strcmp(password, (char *)(baza + 8 + lungime_nume)) == 0)
            printf("HEHE\n");
    
    }

    
    lungime_finala = 8 + 9 + lungime_nume + lungime_parola;
    add = add + lungime_finala;
    
    if (add < len)
        baza = baza + lungime_finala;
    else
        ok = 1;

    }

    if(flag == 0) {
        printf("404 not found\n");
    }

    baza = pointer;
}



int main() {
    // 
    void *baza = NULL;
    int len = 0;
    int nr_useri = 0;
    
    // primire comanda
    char buffer[256];
    // rulare pana la exit
    int run = 1;
    

    
    do
    {   // scanare buffer
        scanf("%s", buffer);
        
        if(strcmp(buffer, "add") == 0) {
            data_structure *elem = (data_structure *)malloc(sizeof(data_structure));
            add_a_user(&baza, elem, &len);
            nr_useri++;
        }

        if(strcmp(buffer, "print") == 0) {
            print(baza,len);
        }

        if(strcmp(buffer, "confidential") == 0) {
            // apelam functia de printare
            print_secret(baza,len);
        }

        if(strcmp(buffer, "exit") == 0) {
            // oprim programul din a mai rula
            run = 0;
        }

         if (strcmp(buffer, "log") == 0) {
            // scanam un index si apelam functia de gasire
            int64_t cnp;
            scanf("%"SCNd64"", &cnp);
            log_in(baza, len, cnp);
        }

         if (strcmp(buffer, "delete") == 0) {
            // scanam un index si apelam functia de stergere
            int index;
            scanf("%d", &index);
            // daca nu a mers stergerea iesim din program cu exit status -2
            if (delete_a_user(&baza, &len, index) != 0)
            exit(-2);
            // scadem lungimea vectorului
            nr_useri--;
        }

    } while ( run == 1 );
    
    // free la vectorul generic
    free(baza);




    return 0;
}