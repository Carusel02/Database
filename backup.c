#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// structura tip head (are 8 bytes)
typedef struct head
{   unsigned char type; // (ocupa 4 bytes si se foloseste de primul)
    unsigned int len; // (ocupa si foloseste 4 bytes)
} head;

// structura tip data_structure
typedef struct data_structure
{   head *header; // (structura de 8 bytes)
    void *data; // (structura de dimensiune variabila)
} data_structure;

// adaugare
int add_last(void **arr, int *len, data_structure *data)
{   // alocare vector ARR fix cu cat avem nevoie
    if (*arr == NULL)
        *arr = malloc(sizeof(head) + data->header->len); 
    else
        *arr = realloc(*arr, *len + sizeof(head) + data->header->len);
     
    // salvare adresa inceput
    void *pointer = *arr;
    // adaugare spre final
    *arr = *arr + *len;
    // copiere header
    memcpy(*arr, data->header, sizeof(head));
    (*arr) = ((char *)*arr + 4); // trecere catre zona len
    size_t size = *(int *)(*arr); // extragere len
    (*arr) = ((int *)*arr + 1); // ajungere zona data
    memcpy(*arr, data->data, size); // copiere bytes din data
    *arr = pointer; // adresa de start

    return 0;
}

// functie delete
int delete_at(void **arr, int *len, int index)
{   // salvare adresa initiala
    void *pointer = *arr;
    // index element de sters
    int delete = 0;
    
    // parcurgere pana la index
    int lungime_data = *(int *)(*arr + 4);
    int suma = 0;
    while (delete != index) {
        // aflam cat din lungimea vectorului am parcurs
        suma = suma + sizeof(head) + lungime_data;
        // trecem la urmatorul element
        *arr = *arr + sizeof(head) + lungime_data;
        // aflam cat are zona data a urmatorului element
        lungime_data = *(int *)(*arr + 4);
        // incrementam indexul delete
        delete++;
    }
     
    // modificare date
    int pas = sizeof(head) + lungime_data;
    // adresa de dupa locul ocupat de element
    suma = suma + pas;
    // zona ramasa dupa stergerea elementului
    size_t size = *len - suma;
    // copiere bytes din zona de dupa element in zona elementului 
    memcpy(*arr, *arr + pas, size); // un fel de suprascriere a datelor
    *arr = pointer; // adresa initiala pointer
    *arr = realloc(*arr, *len - pas); // realocare cu dimensiune - element sters
    *len = *len - pas;

    return 0;
}

// functie inserare la un index
int add_at(void **arr, int *len, data_structure *data, int index) {
    // iesim daca indexul e negativ
    if (index < 0)
    {
        return 0;
    }
    
    // dimensiune structura
    size_t structura = sizeof(head) + data->header->len;
    // realocam cu + dimensiunea structurii ( facem loc )
    *arr = realloc(*arr, *len + structura);
    // stocam adresa vector ( ne ajuta pentru printare )
    void *pointer = *arr;
    // index add ( similar cu cel de la delete )
    int add = 0;
    
    // parcurgere pana la index ( la fel ca la delete )
    int lungime_data = *(int *)(*arr + 4);
    int suma = 0;
    while (add != index)
    {
        suma = suma + sizeof(head) + lungime_data;
        *arr = *arr + sizeof(head) + lungime_data;
        lungime_data = *(int *)(*arr + 4);
        add++;
    }
    
    // copiem bytes in adresa arr + dim structura de la adresa arr
    memcpy(*arr + (int)structura, *arr, *len - suma);
    // abia acum putem sa bagam structura 
    // bagam headul
    memcpy(*arr,data->header,sizeof(head));
    *arr = *arr + sizeof(head);
    // bagam si zona data
    memcpy(*arr,data->data,data->header->len);
    
    // arr recapata adresa initiala
    *arr = pointer; 
    return 0;
}

// functie care citeste si apeleaza functia add_at
void write_element(void *arr,data_structure *elem, int *len, int index) {
    int type; // citire tip
    scanf("%d", &type);
    elem->header = (head *)malloc(sizeof(head)); // alocare head
    // salvare tip 
    if (type == 1)
    elem->header->type = '1';
    if (type == 2)
    elem->header->type = '2';
    if (type == 3)
    elem->header->type = '3';
    // alocare dinamica pe citire siruri
    char *sir = malloc(256 * sizeof(char));
    char *sir2 = malloc(256 * sizeof(char));
    
    // stocare pe cazuri
    if (type == 1)
    {   scanf("%s", sir);
        int8_t bancnota1;
        int8_t bancnota2;
        scanf("%" SCNd8 " %" SCNd8 "", &bancnota1, &bancnota2);
        scanf("%s", sir2);


        // salvare size siruri + inca 1 byte pentru caracterul NULL
        size_t size1 = strlen(sir) + 1;
        size_t size2 = strlen(sir2) + 1;
        size_t bancnota1_size = sizeof(int8_t);
        size_t bancnota2_size = sizeof(int8_t);
        size_t total_size = size1 + size2 + bancnota1_size + bancnota2_size;
        elem->header->len = (int)total_size;
        
        // alocare zona data
        elem->data = malloc(total_size);
        // copiere sir
        memcpy(elem->data, sir, size1);
        // trecere zona urmatoare
        elem->data = ((char *)elem->data + size1);
        // copiere bancnota 1
        memcpy(elem->data, &bancnota1, bancnota1_size);
        // trecem zona urmatoare
        elem->data = ((int8_t *)elem->data + 1);
        // copiere bancnota 2
        memcpy(elem->data, &bancnota2, bancnota2_size);
        // trecem zona urmatoare
        elem->data = ((int8_t *)elem->data + 1);
        // copiere sir 2
        memcpy(elem->data, sir2, size2);
        
        // ajungem la final si scadem size ul total sa ajungem la adresa de start 
        elem->data = ((char *)elem->data + size2);
        elem->data = (char *)elem->data - total_size;
    }
    
    // procedeul e analog si aici
    if (type == 2)
    {   scanf("%s", sir);
        int16_t bancnota1;
        int32_t bancnota2;
        scanf("%" SCNd16 " %" SCNd32 "", &bancnota1, &bancnota2);
        scanf("%s", sir2); 

        size_t size1 = strlen(sir) + 1;
        size_t size2 = strlen(sir2) + 1;
        size_t bancnota1_size = sizeof(int16_t);
        size_t bancnota2_size = sizeof(int32_t);
        size_t total_size = size1 + size2 + bancnota1_size + bancnota2_size;
        elem->header->len = (int)total_size;

        elem->data = malloc(total_size);
        memcpy(elem->data, sir, size1);
        elem->data = ((char *)elem->data + size1);
        memcpy(elem->data, &bancnota1, bancnota1_size);
        elem->data = ((int16_t *)elem->data + 1);
        memcpy(elem->data, &bancnota2, bancnota2_size);
        elem->data = ((int32_t *)elem->data + 1);
        memcpy(elem->data, sir2, size2);
        elem->data = ((char *)elem->data + size2);
        elem->data = (char *)elem->data - total_size;
    }

    if (type == 3)
    {   scanf("%s", sir); 
        int32_t bancnota1;
        int32_t bancnota2;
        scanf("%"SCNd32" %"SCNd32"", &bancnota1, &bancnota2);
        scanf("%s", sir2);

        size_t size1 = strlen(sir) + 1;
        size_t size2 = strlen(sir2) + 1;
        size_t bani = sizeof(int);
        size_t total_size = size1 + size2 + 2 * bani;
        elem->header->len = (int)total_size;

        elem->data = malloc(total_size);
        memcpy(elem->data, sir, size1);
        elem->data = ((char *)elem->data + size1);
        memcpy(elem->data, &bancnota1, bani);
        elem->data = ((int32_t *)elem->data + 1);
        memcpy(elem->data, &bancnota2, bani);
        elem->data = ((int32_t *)elem->data + 1);
        memcpy(elem->data, sir2, size2);
        elem->data = ((char *)elem->data + size2);
        elem->data = (char *)elem->data - total_size;
    }
    
    // adaugare la un index
    if(add_at(arr, len, elem, index) != 0)
    exit(-1);
    
    // incrementare lungime vector
    *len = *len + elem->header->len + (int)sizeof(head);
    
    // eliberare pe rand memorie alocata
    free(elem->data);
    free(sir);
    free(sir2);
    free(elem->header);
    free(elem);
}

/* functie de citire si stocare data , e la fel ca write_element
doar ca apeleaza functia de inserare la final */
void write_data(void **arr,data_structure *elem, int *len) {    
    // nu o mai comentez ca similara cu cea de mai sus
    int type;
    scanf("%d", &type);
    elem->header = (head *)malloc(sizeof(head));
    if (type == 1)
    elem->header->type = '1';
    if (type == 2)
    elem->header->type = '2';
    if (type == 3)
    elem->header->type = '3';

    char *sir = malloc(256 * sizeof(char));
    char *sir2 = malloc(256 * sizeof(char));

    if (type == 1)
    {   scanf("%s", sir);
        int8_t bancnota1;
        int8_t bancnota2;
        scanf("%" SCNd8 " %" SCNd8 "", &bancnota1, &bancnota2);
        scanf("%s", sir2);

        size_t size1 = strlen(sir) + 1;
        size_t size2 = strlen(sir2) + 1;
        size_t bancnota1_size = sizeof(int8_t);
        size_t bancnota2_size = sizeof(int8_t);
        size_t total_size = size1 + size2 + bancnota1_size + bancnota2_size;
        elem->header->len = (int)total_size;

        elem->data = malloc(total_size);
        memcpy(elem->data, sir, size1);
        elem->data = ((char *)elem->data + size1);
        memcpy(elem->data, &bancnota1, bancnota1_size);
        elem->data = ((int8_t *)elem->data + 1);
        memcpy(elem->data, &bancnota2, bancnota2_size);
        elem->data = ((int8_t *)elem->data + 1);
        memcpy(elem->data, sir2, size2);
        elem->data = ((char *)elem->data + size2);
        elem->data = (char *)elem->data - total_size;
    }

    if (type == 2)
    {   scanf("%s", sir);
        int16_t bancnota1;
        int32_t bancnota2;
        scanf("%" SCNd16 " %" SCNd32 "", &bancnota1, &bancnota2);
        scanf("%s", sir2); 

        size_t size1 = strlen(sir) + 1;
        size_t size2 = strlen(sir2) + 1;
        size_t bancnota1_size = sizeof(int16_t);
        size_t bancnota2_size = sizeof(int32_t);
        size_t total_size = size1 + size2 + bancnota1_size + bancnota2_size;
        elem->header->len = (int)total_size;

        elem->data = malloc(total_size);
        memcpy(elem->data, sir, size1);
        elem->data = ((char *)elem->data + size1);
        memcpy(elem->data, &bancnota1, bancnota1_size);
        elem->data = ((int16_t *)elem->data + 1);
        memcpy(elem->data, &bancnota2, bancnota2_size);
        elem->data = ((int32_t *)elem->data + 1);
        memcpy(elem->data, sir2, size2);
        elem->data = ((char *)elem->data + size2);
        elem->data = (char *)elem->data - total_size;
    }

    if (type == 3)
    {   scanf("%s", sir); 
        int bancnota1;
        int bancnota2;
        scanf("%d%d", &bancnota1, &bancnota2);
        scanf("%s", sir2);

        size_t size1 = strlen(sir) + 1;
        size_t size2 = strlen(sir2) + 1;
        size_t bani = sizeof(int);
        size_t total_size = size1 + size2 + 2 * bani;
        elem->header->len = (int)total_size;

        elem->data = malloc(total_size);
        memcpy(elem->data, sir, size1);
        elem->data = ((char *)elem->data + size1);
        memcpy(elem->data, &bancnota1, bani);
        elem->data = ((int32_t *)elem->data + 1);
        memcpy(elem->data, &bancnota2, bani);
        elem->data = ((int32_t *)elem->data + 1);
        memcpy(elem->data, sir2, size2);
        elem->data = ((char *)elem->data + size2);
        elem->data = (char *)elem->data - total_size;
    }
    
    if(add_last(arr, len, elem) != 0)
    exit(-1);

    *len = *len + elem->header->len + (int)sizeof(head);

    free(elem->data);
    free(sir);
    free(sir2);
    free(elem->header);
    free(elem);
}

// functia de printare
void print(void *arr, int len) {
    // daca vectorul are lungimea 0 nu mai afisam nimic
    if (len == 0)
    return;

    void *pointer = arr;
    int ok = 0; // variabila contor
    int lungime_finala = 0;
    int add = 0;
    
    // extragem tipul si dupa afisam informatia pana cand ajungem la len
    while (ok == 0) {
    printf("Tipul %c\n", *(char *)(arr));
    char tip = *(unsigned char *)(arr);
    
    // afisare pe cazuri
    if (tip == '1') {
        int lungime = strlen((char *)(arr + 8)) + 1;
        int lungime2 = strlen((char *)(arr + 8 + lungime + 2)) + 1;
        // facem cast la tipul de informatie pe care vrem sa il citim, dupa sarim peste x pasi
        
        // aici e arr + sizeof(head) = 8 + lungime la sir1 + 2 bytes ( 2 bancnote de 1 byte)
        printf("%s pentru %s\n", (char *)(arr + 8), (char *)(arr + 8 + lungime + 2)); 
        // prima bancnota se afla la arr + sizeof(head) = 8 + lungime primul sir
        printf("%" PRId8 "\n", *(int8_t *)(arr + 8 + lungime));
        // a doua se afla dupa prima + 1 byte
        printf("%" PRId8 "\n", *(int8_t *)(arr + 9 + lungime));
        printf("\n");
        
        // crestem add cu dimensiunea unui struct de element
        lungime_finala = 8 + 2 + lungime + lungime2;
        add = add + lungime_finala;
    }
    
    // analog si pentru celelalte 2 tipuri
    if (tip == '2') {
        int lungime = strlen((char *)(arr + 8)) + 1;
        int lungime2 = strlen((char *)(arr + 8 + lungime + 6)) + 1;
        printf("%s pentru %s\n", (char *)(arr + 8), (char *)(arr + 8 + lungime + 6));
        printf("%" PRId16 "\n", *(int16_t *)(arr + 8 + lungime));
        printf("%" PRId32 "\n", *(int32_t *)(arr + 10 + lungime));
        printf("\n");

        lungime_finala = 8 + 6 + lungime + lungime2;
        add = add + lungime_finala;
    }

    if (tip == '3') {
        int lungime = strlen((char *)(arr + 8)) + 1;
        int lungime2 = strlen((char *)(arr + 8 + lungime + 8)) + 1;
        printf("%s pentru %s\n", (char *)(arr + 8), (char *)(arr + 8 + lungime + 8));
        printf("%"PRId32"\n", *(int32_t *)(arr + 8 + lungime));
        printf("%"PRId32"\n", *(int32_t *)(arr + 12 + lungime));
        printf("\n");

        lungime_finala = 8 + 8 + lungime + lungime2;
        add = add + lungime_finala;
    }
    
    // verificare daca am ajuns pana la finalul vectorului
    if (add < len)
        arr = arr + lungime_finala;
    else
        ok = 1; // iesire daca este cazul

    }

    arr = pointer; // arr ia adresa intiala pentru alta posibila afisare
}

// functie find
void find(void *data_block, int len, int index) {
    if (index < 0)
        return;
    // salvare adresa initiala
    void *pointer = data_block;
    int find = 0;
    
    // parcurgere vector ( similar cu functiile de mai sus )
    int lungime_data = *(int *)(data_block + 4);
    while (find != index)
    {
        data_block = data_block + sizeof(head) + lungime_data;
        lungime_data = *(int *)(data_block + 4);
        find++;
    }
    
    // dupa ce am gasit indexul afisam ( similar cu functia print )
    printf("Tipul %c\n", *(char *)(data_block));
    char tip = *(unsigned char *)(data_block);

    if (tip == '1') {
        int lungime = strlen((char *)(data_block + 8)) + 1;
        printf("%s pentru %s\n", (char *)(data_block + 8), (char *)(data_block + 8 + lungime + 2));
        printf("%" PRId8 "\n", *(int8_t *)(data_block + 8 + lungime));
        printf("%" PRId8 "\n", *(int8_t *)(data_block + 9 + lungime));
        printf("\n");
    }

    if (tip == '2') {
        int lungime = strlen((char *)(data_block + 8)) + 1;
        printf("%s pentru %s\n", (char *)(data_block + 8), (char *)(data_block + 8 + lungime + 6));
        printf("%" PRId16 "\n", *(int16_t *)(data_block + 8 + lungime));
        printf("%" PRId32 "\n", *(int32_t *)(data_block + 10 + lungime));
        printf("\n");
    }

    if (tip == '3') {
        int lungime = strlen((char *)(data_block + 8)) + 1;
        printf("%s pentru %s\n", (char *)(data_block + 8), (char *)(data_block + 8 + lungime + 8));
        printf("%"PRId32"\n", *(int32_t *)(data_block + 8 + lungime));
        printf("%"PRId32"\n", *(int32_t *)(data_block + 12 + lungime));
        printf("\n");
    }

    data_block = pointer; // returnare adresa initiala
}

int main() {
    // valori de start
    void *arr = NULL;
    int len = 0;
    int lungime_vector = 0;
    
    // valori rulare program
    char buffer[256];
    int run = 1;

    do
    {   // scanare buffer
        scanf("%s", buffer);
        
        // comanda insert care creeaza un element, il aloca + scrie in el
        if(strcmp(buffer, "insert") == 0) {
            data_structure *elem = (data_structure *)malloc(sizeof(data_structure));
             // dupa ce scrie aloca in vectorul generic apeland o subfunctie din subprogram
            write_data(&arr, elem, &len);
            // crestem lungimea vectorului ( nu ca nr de bytes, ca nr de elem)
            lungime_vector++;
        }

        if(strcmp(buffer, "print") == 0) {
            // apelam functia de printare
            print(arr,len);
        }

        if(strcmp(buffer, "exit") == 0) {
            // oprim programul din a mai rula
            run = 0;
        }

         if (strcmp(buffer, "find") == 0) {
            // scanam un index si apelam functia de gasire
            int index;
            scanf("%d", &index);
            find(arr, len, index);
        }

         if (strcmp(buffer, "delete_at") == 0) {
            // scanam un index si apelam functia de stergere
            int index;
            scanf("%d", &index);
            // daca nu a mers stergerea iesim din program cu exit status -2
            if (delete_at(&arr, &len, index) != 0)
            exit(-2);
            // scadem lungimea vectorului
            lungime_vector--;
        }

        if (strcmp(buffer, "insert_at") == 0) {
            // scanam index
            int index;
            scanf("%d", &index);
            // daca e mai mare decat nr de elem, il adaugam la final
            if(index > lungime_vector)
            index = lungime_vector;
            // alocam o structura si o inseram apeland functia de write_elem
            data_structure *elem = (data_structure *)malloc(sizeof(data_structure));
            // apeleaza in spate functia de add_at dupa citire si stocare date
            write_element(&arr, elem, &len, index);
            // crestem lungimea la vector
            lungime_vector++;
         }

    } while ( run == 1 );
    
    // free la vectorul generic
    free(arr);
    
    return 0;
}