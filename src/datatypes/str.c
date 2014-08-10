#include "str.h"

str_t *create_str(const char *c_str)
{
    str_t *str = (str_t*)malloc(sizeof(str_t));
    if(c_str){
        str->len = strlen(c_str);
        str->str = (char*)malloc(sizeof(char)*(str->len+1));
        strcpy(str->str, c_str);
    }else{
        str->str = 0;
        str->len = 0;
    }
    return str;
}

void   destroy_str(str_t **str)
{
    if(str){
        free((*str)->str);
        free(*str);
        *str = 0;
    }
}
