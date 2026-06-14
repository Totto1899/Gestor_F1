#include "library.h"

void* cargaInicial(const char* nomArch, void* vec, size_t tam, int* capacidad,
                   int* cant_registros, int trozarCamposLongVariable(void*, size_t, const char*)){
    char aux[MAX_LINEA];
    *cant_registros = 0;
    char* p_read = (char*)vec;
    FILE* pf = fopen(nomArch, "rt");
    if(!pf)
        return vec;
    while(fgets(aux, sizeof(aux), pf)){
        if(*cant_registros>=*capacidad){
            *capacidad *= 2;
            void* temp = realloc(vec, (*capacidad)*tam);
            if(!temp){
                fclose(pf);
                return vec;
            }
            vec = temp;
            p_read = (char*)vec + (*cant_registros*tam);
        }
        if(trozarCamposLongVariable(p_read, tam, aux)){
        p_read += tam;
        (*cant_registros)++;
        }
    }
    fclose(pf);
    if(cant_registros>0 && *cant_registros<*capacidad){
        vec = realloc(vec, (*cant_registros)*tam);
        *capacidad = *cant_registros;
    }
    return vec;
}

int volcarABinario(const char* nomArch, const void* vec, size_t tam, size_t ce){
    FILE* pf = fopen(nomArch, "wb");
    if(!pf)
        return ERR_AP;
    size_t escritos = fwrite(vec, tam, ce, pf);
    fclose(pf);
    if(escritos!=ce)
        return ERR_ESCRITURA;
    return TODO_OK;
}

int generarLoteEscuderiasTXT(const char* nomArch){
    FILE* pf = fopen(nomArch, "wt");
    if (!pf)
        return ERR_AP;
    fprintf(pf, "1|RBR|Red Bull Racing|Austria|1\n");
    fprintf(pf, "2|FER|Scuderia Ferrari|Italia|1\n");
    fprintf(pf, "3|MER|Mercedes AMG|Alemania|1\n");
    fprintf(pf, "4|MCL|McLaren|Reino Unido|1\n");
    fprintf(pf, "5|AST|Aston Martin|Reino Unido|1\n");
    fprintf(pf, "6|WIL|Williams Racing|Reino Unido|1\n");
    fprintf(pf, "7|ALP|Alpine|Francia|1\n");
    fprintf(pf, "8|MIN|Minardi|Italia|0\n");
    fclose(pf);
    return TODO_OK;
}

int generarLotePilotosTXT(const char* nomArch) {
    FILE* pf = fopen(nomArch, "wt");
    if (!pf)
        return ERR_AP;
    // Red Bull (Escudería 1)
    fprintf(pf, "101|Max Verstappen|Neerlandesa|1|395|A|875577600\n");   // 1997-09-30
    fprintf(pf, "102|Sergio Perez|Mexicana|1|258|A|633312000\n");        // 1990-01-26
    // Ferrari (Escudería 2)
    fprintf(pf, "103|Charles Leclerc|Monegasca|2|280|A|876960000\n");    // 1997-10-16
    fprintf(pf, "104|Carlos Sainz|Espanola|2|200|A|778377600\n");        // 1994-09-01
    // Mercedes (Escudería 3)
    fprintf(pf, "105|Lewis Hamilton|Britanica|3|220|A|473904000\n");     // 1985-01-07
    fprintf(pf, "106|George Russell|Britanica|3|160|A|887500800\n");     // 1998-02-15
    // McLaren (Escudería 4)
    fprintf(pf, "107|Lando Norris|Britanica|4|195|A|942451200\n");       // 1999-11-13
    fprintf(pf, "108|Oscar Piastri|Australiana|4|150|A|986515200\n");    // 2001-04-06
    // Aston Martin (Escudería 5)
    fprintf(pf, "109|Fernando Alonso|Espanola|5|180|A|365212800\n");     // 1981-07-29
    fprintf(pf, "110|Lance Stroll|Canadiense|5|60|A|909619200\n");       // 1998-10-29
    // Casos especiales para probar filtros de estado (Retirados / Suspendidos)
    fprintf(pf, "111|Kimi Raikkonen|Finlandesa|2|0|R|308966400\n");      // 1979-10-17
    fprintf(pf, "112|Kevin Magnussen|Danesa|6|15|S|718243200\n");        // 1992-10-05
    fclose(pf);
    return TODO_OK;
}

int generarLoteArchivoCarrera(const char* nomArchCar, const char* nomArchPil){
    int puntos_f1[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    int i, j=0, cant_activos = 0;
    int ce_pil = contElementos(nomArchPil, sizeof(tPiloto));
    tPiloto* vecPil = malloc(ce_pil*sizeof(tPiloto));
    if(!vecPil)
        return ERR_MEM;
    FILE* pfPil = fopen(nomArchPil, "rb");
    if(!pfPil){
        free(vecPil);
        return ERR_AP;
    }
    fread(vecPil, sizeof(tPiloto), ce_pil, pfPil);
    fclose(pfPil);

    for(i=0; i<ce_pil; i++)
        if((vecPil+i)->estado=='A')
            cant_activos++;

    FILE* pfCar = fopen(nomArchCar, "wb");
    if(!pfCar){
        free(vecPil);
        return ERR_AP;
    }
    tCarrera car_aux = {0};
    car_aux.id = 1;
    strcpy(car_aux.circuito, "Monza");
    car_aux.fecha = (unsigned long long)time(NULL);
    car_aux.estado = 1;

    car_aux.cant_resultados = cant_activos;
    car_aux.matriz = malloc(cant_activos*sizeof(tResultado));
    if(!car_aux.matriz){
        free(vecPil);
        fclose(pfCar);
        return ERR_MEM;
    }
    for(i=0; i<ce_pil; i++){
        if((vecPil+i)->estado=='A'){
            (car_aux.matriz+j)->id_piloto = (vecPil+i)->id;
            (car_aux.matriz+j)->posicion = j + 1;
            if(j<10)
                (car_aux.matriz+j)->total_puntos = puntos_f1[j];
            else
                (car_aux.matriz+j)->total_puntos = 0;
            j++;
        }
    }

    fwrite(&car_aux, sizeof(tCarrera), 1, pfCar);
    fwrite(car_aux.matriz, sizeof(tResultado), cant_activos, pfCar);
    free(vecPil);
    free(car_aux.matriz);
    fclose(pfCar);
    return TODO_OK;
}

void ssort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*)){
    void* ult = vec + (ce-1)*tam;
    void* men;
    while(vec<ult){
        men = buscar_menor(vec, ce, tam, cmp);
        mswap(men, vec, tam);
        vec +=tam;
        ce--;
    }
}

void mswap(void* a, void* b, size_t tam){
    int i;
    char temp;
    for(i=0; i<tam; i++){
        temp = *(char*)a;
        *(char*)a = *(char*)b;
        *(char*)b = temp;
        a++;
        b++;
    }
}

void* buscar_menor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*)){
    void* men = (void*)vec;
    void* ult = (void*)vec + ce*tam;
    void* ini = (void*)vec;
    while(ini<ult){
        if(cmp(men, ini)>0)
            men = ini;
        ini += tam;
    }
    return men;
}

int buscarEnArchivo(const char* nomArch, const void* clave, void* destino, size_t tam, int cmp(const void*, const void*)){
    int encontrado = 0;
    FILE* pf = fopen(nomArch, "rb");
    if(!pf)
        return ERR_AP;
    fread(destino, tam, 1, pf);
    while(!feof(pf) && !encontrado)
        if(cmp(clave, destino)==0)
            encontrado = 1;
        else
            fread(destino, tam, 1, pf);
    fclose(pf);
    return encontrado;
}

int contElementos(const char* nomArch, size_t tam){
    FILE* pf = fopen(nomArch, "rb");
    if(!pf)
        return ERR_AP;
    fseek(pf, 0, SEEK_END);
    long bytes_totales = ftell(pf);
    fclose(pf);
    return (size_t)(bytes_totales/tam);
}

char menuBase(const char* msj, const char* opc){
    char op;
    do{
        printf("%s",msj);
        scanf(" %c",&op);
        while(getchar() != '\n');
    }while(strchr(opc,op)==NULL);
    return op;
}

int funcionesABM(const char* piloto, const char* escu, const char* carrera){

    char opAccion, opArchivo;
    system("cls");
    opAccion=menuBase(MENUABM,OPCIONES_ABM);
    if(opAccion!='4'){
        system("cls");
        opArchivo=menuBase(MENUABMARCHIVOS,OPCIONES_ABMARCH);
        if(opArchivo==3)
            return 0;
    }
    FILE* pf=fopen((opArchivo=='1'?piloto:(opArchivo=='2'?escu:carrera)),"r+b");
    if(pf==NULL)
        return ERR_AP;

    switch(opAccion){
        case '1':
            opArchivo=='1'?ingresarRegPiloto(pf, sizeof(tPiloto)):ingresarRegEscuderia(pf, sizeof(tEscuderia));
            break;
        case '2':
            opAccion=='1'?bajaRegPiloto(pf,sizeof(tPiloto), ARCH_PILOTOBAJAS):bajaRegEscuderia(pf,sizeof(tEscuderia), ARCH_ESCUDBAJAS);
            break;
        case '3':
            opArchivo=='1'?modifiPiloto(pf, piloto,sizeof(tPiloto)): modifiEscuderia(pf, escu,sizeof(tEscuderia));
            break;
        case '4':
            break;
    }
    fclose(pf);
    return TODO_OK;
}


int bajaRegPiloto(FILE* pf, size_t tam, const char* arc)
{
    tPiloto aux;
    int auxId;
    FILE* pb;

    pb=fopen(arc,"r+b");
    if(pb==NULL)
    {
        pb=fopen(arc,"w+b");
        if(pb==NULL)
            return ERR_AP;
    }

    printf("Ingrese la id del piloto: ");
    scanf("%d", &auxId);

    aux=busquedaPiloto(pf,auxId,tam);
    if(auxId!=-1)
    {
        aux.estado='R';
        fseek(pf,-tam,SEEK_CUR);
        fwrite(&aux,tam,1,pf);

        fseek(pb,0,SEEK_END);
        fwrite(&aux,tam,1,pb);

    }
    fclose(pb);
    return TODO_OK;
}

int bajaRegEscuderia(FILE* pf, size_t tam, const char* arc)
{
    tEscuderia aux;
    int auxId;
    FILE* pb;

    pb=fopen(arc,"r+b");
    if(pb==NULL)
    {
        pb=fopen(arc,"w+b");
        if(pb==NULL)
            return ERR_AP;
    }

    printf("Ingrese la id de la escuderia: ");
    scanf("%d", &auxId);

    aux=busquedaEscuderia(pf,auxId,tam);
    if(auxId!=-1)
    {
        aux.estado=0;
        fseek(pf,-tam,SEEK_CUR);
        fwrite(&aux,tam,1,pf);

        fseek(pb,0,SEEK_END);
        fwrite(&aux,tam,1,pb);

    }
    fclose(pb);
    return TODO_OK;
}


tPiloto busquedaPiloto(FILE *pf, int clave, size_t tam)
{
    int enco=0;
    tPiloto reg;

    rewind(pf);
    fread(&reg,tam,1,pf);
    while(!feof(pf) && !enco)
    {
        if(reg.id==clave)
            enco=1;
        else
            fread(&reg,tam,1,pf);
    }
    if(!enco)
        reg.id=-1;
    return reg;
}
tEscuderia busquedaEscuderia(FILE* pf, int clave, size_t tam)
{
    int enco=0;
    tEscuderia reg;

    rewind(pf);
    fread(&reg,tam,1,pf);
    while(!feof(pf) && !enco)
    {
        if(reg.id==clave)
            enco=1;
        else
            fread(&reg,tam,1,pf);
    }
    if(!enco)
        reg.id=-1;
    return reg;
}

void modifiEscuderia(FILE* pf, const char* arc, size_t tam)
{
    tEscuderia aux;
    int auxId;
    void* valor;
    char op;
    rewind(pf);

    printf("Ingrese la id de la escuderia que quiere modificar: ");
    scanf("%d", &auxId);

    aux=busquedaEscuderia(pf,auxId,tam);

    if(aux.id!=-1)
    {
        op=menuBase(MENU_MODIESCUDERIA,OPCIONES_MENUMODIESCUDERIA);
        fseek(pf,-tam,SEEK_CUR);
        printf("Ingrese el nuevo valor: ");
        switch(op)
        {
        case '1':
            valor=malloc(sizeof(char*));
            fflush(stdin);
            gets((char*)valor);
            strcpy(aux.codigo, (char*)valor);
            break;
        case '2':
            valor=malloc(sizeof(char*));
            fflush(stdin);
            gets((char*)valor);
            strcpy(aux.nombre,(char*)valor);
        case '3':
            valor=malloc(sizeof(char*));
            fflush(stdin);
            gets((char*)valor);
            strcpy(aux.pais,(char*)valor);
            break;
        case '4':
            valor=malloc(sizeof(int));
            scanf("%d", &valor);
            aux.estado=(int)valor;
        }
        fwrite(&aux,tam,1,pf);
        free(valor);
    }
}
void modifiPiloto(FILE* pf, const char* arc, size_t tam)
{
    tPiloto aux;
    int auxId;
    void* valor;
    char op;
    rewind(pf);

    printf("\tIngrese la id del piloto que quiere modificar: ");
    scanf("%d", &auxId);
    aux=busquedaPiloto(pf,auxId,tam);

    if(aux.id!=-1)
    {
        fseek(pf,-tam,SEEK_CUR);
        op=menuBase(MENU_MODIPILOTO,OPCIONES_MENUMODIPILOTO);
        printf("\tIngrese el nuevo valor: ");

        switch(op)
        {
        case '1':
            valor=malloc(sizeof(char*));
            fflush(stdin);
            gets((char*)valor);
            strcpy(aux.nombre,(char*)valor);
            break;
        case '2':
            valor=malloc(sizeof(char*));
            fflush(stdin);
            gets((char*)valor);
            strcpy(aux.nombre,(char*)valor);
            break;
        case '3':
            valor=malloc(sizeof(unsigned int));
            scanf("%u",&valor);
            aux.id_escuderia=(unsigned int)valor;
            break;
        case '4':
            valor=malloc(sizeof(char*));
            fflush(stdin);
            scanf("%c", (char*)valor);
            aux.estado=(char)valor;
            break;
        case '5':
            valor=malloc(sizeof(long long unsigned));
            scanf("%llu", &valor);
            aux.fechaNacimiento=(long long unsigned)valor;
            break;
        case '6':
            valor= malloc(sizeof(unsigned));
            scanf("%d", &valor);
            aux.puntos_acumulados=(unsigned)valor;
            break;
        case '7':
            break;
        }
        fwrite(&aux,tam,1,pf);
        free(valor);
    }
}



void ingresarRegEscuderia(FILE* pf, size_t tam)
{
    unsigned int auxId;
    tEscuderia aux;

    fseek(pf,-tam,SEEK_END);
    fread(&aux,tam,1,pf);
    auxId=aux.id+1;

    aux.id=auxId;
    printf("\n\tIngrese el codigo de escuderia: ");
    fflush(stdin);
    gets(aux.codigo);

    printf("\n\tIngrese el nombre de la escuderia: ");
    fflush(stdin);
    gets(aux.nombre);

    printf("\n\tIngrese el pais de  la escuderia: ");
    fflush(stdin);
    gets(aux.pais);

    do
    {
        printf("\n\tIngrese el codigo de escuderia (1-Activo 0-Inactivo): ");
        scanf("%u", &aux.estado);
    }while(aux.estado!=1 && aux.estado!=0);

    fseek(pf,0,SEEK_END);
    fwrite(&aux,tam,1,pf);
}

void ingresarRegPiloto(FILE* pf, size_t tam)
{
    unsigned int auxId;
    tPiloto aux;

    fseek(pf,-tam,SEEK_END);
    fread(&aux,tam,1,pf);
    auxId=(aux.id)+1;

    aux.id=auxId;

    printf("\n\tIngrese el nombre del piloto: ");
    fflush(stdin);
    gets(aux.nombre);

    printf("\n\tIngrese la nacionalidad del piloto: ");
    fflush(stdin);
    gets(aux.nacionalidad);

    printf("\n\tIngrese la escuderia del piloto: ");
    scanf("%d", &aux.id_escuderia);

    printf("\n\tIngrese los puntos acumulados del piloto: ");
    scanf("%u", &aux.puntos_acumulados);

    do
    {
        printf("\n\tIngrese el estado del piloto(A:Activo, R:Retirado, S:Suspendido): ");
        fflush(stdin);
        scanf("%c", &aux.estado);
        aux.estado=toupper(aux.estado);
    }while(aux.estado!='A'&&aux.estado!='R'&&aux.estado!='S');

    printf("\n\tIngrese la fecha de nacimiento del piloto: ");
    scanf("%llu", &aux.fechaNacimiento);

    fseek(pf,0,SEEK_END);
    fwrite(&aux,tam,1,pf);
}

///FUNCIONALIDADES MINIMAS

int listarPilotos(const char* nomArch, void mostrar(const void*)){
    tPiloto p;
    FILE* pf = fopen(nomArch, "rb");
    if(!pf)
        return ERR_AP;
    fread(&p, sizeof(tPiloto), 1, pf);
    while(!feof(pf)){
        if(p.estado=='A')
            mostrar(&p);
        fread(&p, sizeof(tPiloto), 1, pf);
    }
    fclose(pf);
    return TODO_OK;
}

int listarPilotosPuntos(const char* nomArch, int cmp(const void*, const void*), void mostrar(const void*)){
    tPiloto* vec;
    size_t ce;
    int i;
    FILE* pf = fopen(nomArch, "rb");
    if(!pf)
        return ERR_AP;
    ce = contElementos(nomArch, sizeof(tPiloto));
    vec = malloc(ce*sizeof(tPiloto));
    if(!vec){
        fclose(pf);
        return ERR_MEM;
    }
    fread(vec, sizeof(tPiloto), ce, pf);
    ssort(vec, ce, sizeof(tPiloto), cmp);
    for(i=0; i<ce; i++)
        if((vec+i)->estado=='A')
            mostrar(vec+i);
    free(vec);
    fclose(pf);
    return TODO_OK;
}

int registrarCarrera(const char* nomArchCar, const char* nomArchPil, int cmp(const void*, const void*)){
    int puntos_f1[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    int i, aux, piloto_valido, flag_advertencia = 0;
    tCarrera pc;
    tResultado* pActual;
    tPiloto pil_aux;
    FILE* pfCar = fopen(nomArchCar, "ab");
    if(!pfCar)
        return ERR_AP;

    system("cls");
    printf("Ingrese el id de la carrera: ");
    scanf("%d", &pc.id);
    printf("Ingrese el nombre del circuito: ");
    scanf("%s", pc.circuito);
    pc.estado = 1;
    pc.fecha = (unsigned long long)time(NULL);
    printf("Ingrese la cantidad de corredores: ");
    scanf("%d", &pc.cant_resultados);
    pc.matriz = malloc(pc.cant_resultados * sizeof(tResultado));
    if(!pc.matriz){
        fclose(pfCar);
        return ERR_MEM;
    }
    pActual = pc.matriz;
    for(i=0; i <pc.cant_resultados; i++){
        do{
            printf("\nIngrese el id del piloto %d: ", i+1);
            scanf("%d", &aux);
            piloto_valido = buscarEnArchivo(nomArchPil, &aux, &pil_aux, sizeof(tPiloto), cmp);
            if(piloto_valido == 0)
                printf("El corredor ingresado no esta registrado. Intente nuevamente.\n");
            else if(pil_aux.estado != 'A'){
                printf("El piloto %s con id %d se encuentra inactivo (Estado: %c).\n", pil_aux.nombre, pil_aux.id, pil_aux.estado);
                piloto_valido = 0;
            }
        }while(piloto_valido == 0);

        pActual->id_piloto = aux;
        do{
            printf("Ingrese la posicion del piloto %d en la carrera: ", aux);
            scanf("%d", &(pActual->posicion));
            if(pActual->posicion<0 || pActual->posicion>pc.cant_resultados)
                printf("\nLa posicion ingresada no es valida. Intente nuevamente.");
        }while(pActual->posicion<0 || pActual->posicion>pc.cant_resultados);

        if(pActual->posicion > 0 && pActual->posicion <= 10)
            pActual->total_puntos = puntos_f1[pActual->posicion - 1];
        else
            pActual->total_puntos = 0;
        if(actualizarPuntosPiloto(nomArchPil, pActual->id_piloto, pActual->total_puntos)!=1)
            flag_advertencia = 1;
        pActual++;
    }

    fwrite(&pc, sizeof(tCarrera), 1, pfCar);
    fwrite(pc.matriz, sizeof(tResultado), pc.cant_resultados, pfCar);

    fclose(pfCar);
    free(pc.matriz);

    if(flag_advertencia==1)
        return ERR_ACT;
    return TODO_OK;
}

int actualizarPuntosPiloto(const char* nomArch, size_t id_pil, size_t puntos){
    tPiloto p;
    int flag = 0;
    FILE* pf = fopen(nomArch, "r+b");
    if(!pf)
        return ERR_AP;
    fread(&p, sizeof(tPiloto), 1, pf);
    while(!feof(pf) && flag==0){
        if(p.id==id_pil && p.estado=='A'){
            flag = 1;
            p.puntos_acumulados += puntos;
            fseek(pf, -(long)sizeof(tPiloto), SEEK_CUR);
            fwrite(&p, sizeof(tPiloto), 1, pf);
        }
        else
            fread(&p, sizeof(tPiloto), 1, pf);
    }
    fclose(pf);
    if(flag==1)
        return TODO_OK;
    else
        return ERR_BUSQUEDA;
}

int mostrarPilotoXEscuderia(const char* nomArchPil, const char* nomArchEscu, void mostrar(const void*)){
    tPiloto p;
    tEscuderia e;
    FILE* pfPil = fopen(nomArchPil, "rb");
    if(!pfPil)
        return ERR_AP;
    FILE* pfEscu = fopen(nomArchEscu, "rb");
    if(!pfEscu){
        fclose(pfPil);
        return ERR_AP;
    }
    fread(&e, sizeof(tEscuderia), 1, pfEscu);
    while(!feof(pfEscu)){
        if(e.estado==1){
            printf("\n=== EQUIPO: %s ===\n", e.nombre);
            rewind(pfPil);
            fread(&p, sizeof(tPiloto), 1, pfPil);
            while(!feof(pfPil)){
                if(p.id_escuderia==e.id && p.estado=='A'){
                    printf("\t");
                    mostrar(&p);
                }
                fread(&p, sizeof(tPiloto), 1, pfPil);
            }
        }
        fread(&e, sizeof(tEscuderia), 1, pfEscu);
    }
    fclose(pfPil);
    fclose(pfEscu);
    return TODO_OK;
}

int menuEstadisticas(){
    char opcion;
    size_t cant_pil;
    tEstadisticaPiloto* vec_estadisticas;
    vec_estadisticas = generarEstadisticas(&cant_pil);
    if(!vec_estadisticas){
        printf("\nNo se pudieron cargar las estadísticas.\n");
        system("pause");
        return ERR_MEM;
    }
    do{
        system("cls");
        opcion = menuBase(MENU_ESTADISTICAS, "01234");
        switch(opcion){
            case '1':
                ssort(vec_estadisticas, cant_pil, sizeof(tEstadisticaPiloto), cmpVictorias);
                mostrarTop5(vec_estadisticas, cant_pil);
                break;
            case '2':
                mostrarMejorPromedio(vec_estadisticas, cant_pil);
                break;
            case '3':
                mostrarMejorPosicion(vec_estadisticas, cant_pil);
                break;
            case '4':
                mostrarPeorPosicion(vec_estadisticas, cant_pil);
                break;
            case '0':
                break;
        }
    } while(opcion != '0');
    free(vec_estadisticas);
    return TODO_OK;
}

int cmpVictorias(const void* a, const void* b){
    tEstadisticaPiloto* pA = (tEstadisticaPiloto*)a;
    tEstadisticaPiloto* pB = (tEstadisticaPiloto*)b;
    return pB->victorias - pA->victorias;
}

void mostrarTop5(tEstadisticaPiloto* vec, size_t ce){
    int i;
    tEstadisticaPiloto* pActual;
    size_t limite = (ce<5)?ce:5;
    printf("\n--- TOP 5 PILOTOS CON MAS VICTORIAS ---\n");
    for(i=0; i<limite; i++){
        pActual = vec+i;
        printf("%d. ID Piloto: %-4d | Nombre: %-30s | Victorias: %d\n", i + 1, pActual->id_piloto, pActual->nombre, pActual->victorias);
    }
    system("pause");
}

void mostrarMejorPromedio(tEstadisticaPiloto* vec, size_t cant){
    int i;
    tEstadisticaPiloto* pMejorProm = vec;
    tEstadisticaPiloto* pActual;
    for(i=0; i<cant; i++){
         pActual = vec+i;
        if(pActual->promedio_posicion>0.0)
            if(pMejorProm->promedio_posicion==0.0 || pActual->promedio_posicion<pMejorProm->promedio_posicion)
                pMejorProm = pActual;
    }
    printf("\n---- MEJOR PROMEDIO GENERAL ----\n");
    if(pMejorProm->promedio_posicion>0.0)
            printf("ID Piloto: %d | Nombre: %-30s | Promedio de Posicion: %.2f\n", pMejorProm->id_piloto, pMejorProm->nombre, pMejorProm->promedio_posicion);
    else
        printf("Aun no se registraron carreras.\n");
    system("pause");
}

void mostrarMejorPosicion(tEstadisticaPiloto* vec, size_t cant){
    int i;
    tEstadisticaPiloto* pMejorPos = vec;
    tEstadisticaPiloto* pActual;
    for(i=0; i<cant; i++){
        pActual = vec+i;
        if(pActual->mejor_posicion>0)
            if(pMejorPos->mejor_posicion==0 || pActual->mejor_posicion<pMejorPos->mejor_posicion)
                pMejorPos = pActual;
    }
    printf("\n---- MEJOR POSICION ALCANZADA ----\n");
    if(pMejorPos->mejor_posicion>0)
        printf("ID Piloto: %d | Nombre: %-30s | Posicion: %d\n", pMejorPos->id_piloto, pMejorPos->nombre, pMejorPos->mejor_posicion);
    else
        printf("No hay registros.\n");
    system("pause");
}

void mostrarPeorPosicion(tEstadisticaPiloto* vec, size_t cant){
    int i;
    tEstadisticaPiloto* pPeorPos = vec;
    tEstadisticaPiloto* pActual;
    for(i = 1; i < cant; i++){
        pActual = vec+i;
        if(pActual->peor_posicion>pPeorPos->peor_posicion)
            pPeorPos = pActual;
    }
    printf("\n---- PEOR POSICION ALCANZADA ----\n");
    if(pPeorPos->peor_posicion>0)
        printf("ID Piloto: %d | Nombre: %-30s | Posicion: %d\n", pPeorPos->id_piloto, pPeorPos->nombre, pPeorPos->peor_posicion);
    else
        printf("No hay registros.\n");

    system("pause");
}

tEstadisticaPiloto* generarEstadisticas(size_t* ce){
    FILE* pfCar;
    tCarrera car = {0};
    tResultado res = {0};
    tPiloto p;
    int i = 0, pos, cant_activos = 0;
    *ce = contElementos(ARCH_PILOTO, sizeof(tPiloto));
    if(*ce==0)
        return NULL;
    tEstadisticaPiloto* vec = (tEstadisticaPiloto*)calloc(*ce, sizeof(tEstadisticaPiloto));
    if(!vec)
        return NULL;
    FILE* pfPil = fopen(ARCH_PILOTO, "rb");
    if(!pfPil){
        free(vec);
        return NULL;
    }
    while(fread(&p, sizeof(tPiloto), 1, pfPil)==1){
        if(p.estado=='A'){
            (vec+cant_activos)->id_piloto = p.id;
            strcpy((vec+cant_activos)->nombre, p.nombre);
            (vec+cant_activos)->mejor_posicion = 999;
            (vec+cant_activos)->puntos_acumulados = 0;
            cant_activos++;
        }
    }
    fclose(pfPil);
    *ce = cant_activos;

    pfCar = fopen(ARCH_CARRERA, "rb");
    if(!pfCar){
        return vec;
    }
    while(fread(&car, sizeof(tCarrera), 1, pfCar)==1){
        for(i=0; i<car.cant_resultados; i++){
            fread(&res, sizeof(tResultado), 1, pfCar);
            if(car.estado==1){
                pos = obtenerIndicePiloto(vec, *ce, res.id_piloto);
                if(pos!= -1){
                    (vec+pos)->cant_carreras_corridas++;
                    (vec+pos)->suma_posiciones += res.posicion;
                    (vec+pos)->puntos_acumulados += res.total_puntos;
                    if(res.posicion==1)
                        (vec+pos)->victorias++;
                    if(res.posicion < (vec+pos)->mejor_posicion)
                        (vec+pos)->mejor_posicion = res.posicion;
                    if(res.posicion > (vec+pos)->peor_posicion)
                        (vec+pos)->peor_posicion = res.posicion;
                }
            }
        }
    }
    fclose(pfCar);
    for(i=0; i<*ce; i++){
        if((vec+i)->cant_carreras_corridas>0)
            (vec+i)->promedio_posicion = (float)(vec+i)->suma_posiciones/(vec+i)->cant_carreras_corridas;
        else{
            (vec+i)->promedio_posicion = 0.0;
            (vec+i)->mejor_posicion = 0;
            (vec+i)->peor_posicion = 0;
        }
    }
    return vec;
}

int obtenerIndicePiloto(tEstadisticaPiloto* vec, size_t cant, int id_buscado){
    int i;
    for(i=0; i<cant; i++){
        if((vec+i)->id_piloto==id_buscado)
            return i;
    }
    return -1;
}

void menuExportarATXT(){
    char opcion;
    int estado;
    do{
        system("cls");
        opcion = menuBase(MENU_EXPORTACION, "01234");
        switch(opcion) {
            case '1':
                estado = exportarATXT(ARCH_PILOTO, "pilotos.txt", sizeof(tPiloto), grabarPilotoTXT);
                if(estado == TODO_OK)
                    printf("Pilotos exportados con exito!\n");
                else
                    printf("Error al exportar pilotos.\n");
                system("pause");
                break;
            case '2':
                estado = exportarATXT(ARCH_ESCUD, "escuderias.txt", sizeof(tEscuderia), grabarEscuderiaTXT);
                if(estado == TODO_OK)
                    printf("Escuderias exportadas con exito!\n");
                else
                    printf("Error al exportar escuderias.\n");
                system("pause");
                break;
            case '3':
                estado = exportarCarreraATXT(ARCH_CARRERA, "carreras.txt");
                if(estado==TODO_OK)
                    printf("Carreras exportadas con exito!\n");
                else
                    printf("Error al exportar carreras.\n");
                system("pause");
                break;
            case '4':
                estado = exportarATXT(ARCH_BAJAS, "bajas.txt", sizeof(tPiloto), grabarPilotoTXT);
                if(estado == TODO_OK)
                    printf("Bajas exportadas con exito!\n");
                else
                    printf("Error al exportar bajas.\n");
                system("pause");
                break;
        }
    } while(opcion!='0');
}

int exportarATXT(const char* nomArchBin, const char* nomArchTXT, size_t tam, void grabar(const void*, FILE*)){
    void* dato;
    FILE* pfBin = fopen(nomArchBin, "rb");
    if(!pfBin)
        return ERR_AP;
    FILE* pfTXT = fopen(nomArchTXT, "wt");
    if(!pfTXT){
        fclose(pfBin);
        return ERR_AP;
    }
    dato = malloc(tam);
    if(!dato){
        fclose(pfBin);
        fclose(pfTXT);
        return ERR_MEM;
    }
    fread(dato, tam, 1, pfBin);
    while(!feof(pfBin)){
        grabar(dato, pfTXT);
        fread(dato, tam, 1, pfBin);
    }
    free(dato);
    fclose(pfBin);
    fclose(pfTXT);
    return TODO_OK;
}

void grabarPilotoTXT(const void* registro, FILE* pfTXT){
    tPiloto* pil = (tPiloto*)registro;
    fprintf(pfTXT, "ID: %-4d | Piloto: %-20s | Estado: %c | Puntos: %d\n", pil->id, pil->nombre, pil->estado, pil->puntos_acumulados);
}

void grabarEscuderiaTXT(const void* registro, FILE* pfTXT){
    tEscuderia* esc = (tEscuderia*)registro;
    fprintf(pfTXT, "ID: %-4d | Escuderia: %s\n", esc->id, esc->nombre);
}

int exportarCarreraATXT(const char* nomArchBin, const char* nomArchTXT){
    tCarrera pc;
    tResultado res;
    int i;
    FILE* pfBin = fopen(nomArchBin, "rb");
    if(!pfBin)
        return ERR_AP;

    FILE* pfTXT = fopen(nomArchTXT, "wt");
    if(!pfTXT){
        fclose(pfBin);
        return ERR_AP;
    }
    fread(&pc, sizeof(tCarrera), 1, pfBin);
    while(!feof(pfBin)){
        if(pc.estado==1){
            fprintf(pfTXT, "\n========================================\n");
            fprintf(pfTXT, "CARRERA ID: %d | CIRCUITO: %s\n", pc.id, pc.circuito);
            fprintf(pfTXT, "========================================\n");
            fprintf(pfTXT, "POS | ID PILOTO | PUNTOS\n");
            fprintf(pfTXT, "----------------------------------------\n");
        }
        for(i=0; i < pc.cant_resultados; i++){
            fread(&res, sizeof(tResultado), 1, pfBin);
            if(pc.estado==1)
                fprintf(pfTXT, "%-3d | %-9d | %d\n", res.posicion, res.id_piloto, res.total_puntos);
        }
        fread(&pc, sizeof(tCarrera), 1, pfBin);
    }
    fclose(pfBin);
    fclose(pfTXT);
    return TODO_OK;
}


///PUNTOS ADICIONALES

int simularCarrera(const char* nomArchPil, const char* nomArchCar){
    size_t cant_pil_act;
    tCarrera car = {0};
    int* vec_pos;
    FILE* pf_pil = fopen(nomArchPil, "rb");
    FILE* pf_car;
    if(!pf_pil)
        return ERR_AP;
    cant_pil_act = contPilActivos(pf_pil);
    if(cant_pil_act==0){
        fclose(pf_pil);
        return ERR;
    }
    system("cls");
    printf("La cantidad de pilotos activos es de %d.\nLa carrera sera entre todos ellos.", cant_pil_act);
    vec_pos = asignarPosiciones(pf_pil, cant_pil_act);
    if(!vec_pos){
        fclose(pf_pil);
        return ERR_MEM;
    }

    car.id = validarIdCar(nomArchCar);
    car.cant_resultados = cant_pil_act;
    car.fecha = time(NULL);
    car.estado = 1;
    printf("\nIngrese el nombre del circuito: ");
    while(getchar() != '\n');
    fgets(car.circuito, sizeof(car.circuito), stdin);
    car.circuito[strcspn(car.circuito, "\n")] = 0;

    car.matriz = malloc(cant_pil_act*sizeof(tResultado));
    if(!car.matriz){
        free(vec_pos);
        fclose(pf_pil);
        return ERR_MEM;
    }
    asignarPuntos(&car, vec_pos);
    pf_car = fopen(nomArchCar, "ab");
    if(!pf_car){
        free(car.matriz);
        free(vec_pos);
        fclose(pf_pil);
        return ERR_AP;
    }
    fwrite(&car, sizeof(tCarrera), 1, pf_car);
    fwrite(car.matriz, sizeof(tResultado), car.cant_resultados, pf_car);
    fclose(pf_car);
    fclose(pf_pil);

    mostrarResultados(&car);

    free(car.matriz);
    free(vec_pos);
    printf("\nCarrera simulada y guardada con exito!\n");
    return TODO_OK;
}

size_t contPilActivos(FILE* pf){
    size_t cont = 0;
    tPiloto p;
    rewind(pf);
    fread(&p, sizeof(tPiloto), 1, pf);
    while(!feof(pf)){
        if(p.estado=='A')
            cont++;
        fread(&p, sizeof(tPiloto), 1, pf);
    }
    return cont;
}

int* asignarPosiciones(FILE* pf_pil, size_t cant_pil_act){
    int* vec_pos = malloc(cant_pil_act*sizeof(int));
    int j, temp, i=0;
    if(!vec_pos)
        return NULL;
    tPiloto p;
    rewind(pf_pil);
    fread(&p, sizeof(tPiloto), 1, pf_pil);
    while(!feof(pf_pil)){
        if(p.estado=='A'){
            *(vec_pos+i) = p.id;
            i++;
        }
        fread(&p, sizeof(tPiloto), 1, pf_pil);
    }
    srand((unsigned)time(NULL));
    for(i=cant_pil_act-1; i>0 ;i--){
        j = rand()%(i+1);
        temp = *(vec_pos+i);
        *(vec_pos+i) = *(vec_pos+j);
        *(vec_pos+j) = temp;
    }
    return vec_pos;
}

int validarIdCar(const char* nomArchCar){
    int id_car;
    do{
        printf("\nIngrese el id de la carrera: ");
        scanf("%d", &id_car);
        if(buscarCarreraId(nomArchCar, id_car)==1)
            printf("\nEl id de carrera ingresado ya existe. Intente nuevamente.");
    }while(buscarCarreraId(nomArchCar, id_car)==1);
    return id_car;
}

int buscarCarreraId(const char* nomArchCar, int id_buscado){
    FILE* pf = fopen(nomArchCar, "rb");
    if (!pf)
        return ERR_AP;
    tCarrera car;
    fread(&car, sizeof(tCarrera), 1, pf);
    while (!feof(pf)) {
        if (car.id == id_buscado) {
            fclose(pf);
            return 1;
        }
        fseek(pf, car.cant_resultados * sizeof(tResultado), SEEK_CUR);
        fread(&car, sizeof(tCarrera), 1, pf);
    }
    fclose(pf);
    return 0;
}

void asignarPuntos(tCarrera* c, int* vec_pos){
    int i;
    int puntos_f1[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    tResultado* res = c->matriz;
    for(i=0; i<c->cant_resultados; i++){
        (res+i)->id_piloto = *(vec_pos+i);
        (res+i)->posicion = i + 1;
        if(i<10)
            (res+i)->total_puntos = puntos_f1[i];
        else
            (res+i)->total_puntos = 0;
    }
}

void mostrarResultados(tCarrera* car){
    size_t i;
    tResultado* res = car->matriz;
    printf("\n=== RESULTADOS DEL CIRCUITO: %s ===\n", car->circuito);
    printf(" POS | ID PILOTO | PUNTOS \n");
    printf("--------------------------\n");
    for(i=0; i<car->cant_resultados; i++)
        printf(" %3d | %9d | %6d \n", (res+i)->posicion, (res+i)->id_piloto, (res+i)->total_puntos);
    printf("==========================\n");
}
