#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct head
{   unsigned int varsta;
    unsigned int len; 
} head;

typedef struct data_structure
{   head *header;
    void *data; 
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


void write_data(void **arr,data_structure *elem, int *len) {    
    // nu o mai comentez ca e similara cu cea de mai sus
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
    
    void *arr = NULL;
    int len = 0;
    int lungime_vector = 0;
    
    // primire comanda
    char buffer[256];
    // rulare pana la exit
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

    } while ( run == 1 );
    
    // free la vectorul generic
    free(arr);




    return 0;
}