#include "library.h"

void crearLote(const char* nomArc)
{
    tPiloto pil[]={{1,"Martinez", "Español", 1,0,'A', 121212},
                    {2,"Gomez", "Argentino", 2,0,'A', 123243},
                    {3, "gonzalez", "Italiano",5,0,'A', 234312}};
    
    FILE* pfPiol=fopen(nomArc, "wb");
    if(!pfPiol)
    {
        printf("error");
    }
    fwrite(pil, sizeof(tPiloto),1,pfPiol);
    fclose(pfPiol);
}
