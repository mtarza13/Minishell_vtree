#include "expansion.h"
#include <ctype.h>

/**
 * @brief Detects if position points to a valid variable reference
 */
int detect_variable(const char *str, int pos, int *in_quotes)
{
    if (in_quotes[0])
        return 0;
        
    if(!str || pos < 0 || str[pos] != '$')
        return 0;
        
    if(str[pos + 1] == '$')
        return 0;
    
    if(pos > 0 && str[pos - 1] == '\\')
        return 0;

    if(str[pos + 1] == '?')
        return 1;
    
    if(str[pos + 1] == '{')
        return 0;

    if(str[pos + 1] == '\0')
        return 0;
        
    if(isalpha(str[pos + 1]) || str[pos + 1] == '_')
        return 1;
        
    return 0;
}

/**
 * @brief Extracts variable name from string 
 */
char *extract_variable(const char *str, int pos, int *end_pos)
{
    int start, end;
    char *name;
    
    if (!str || str[pos] != '$')
        return NULL;
    
    start = pos + 1;
    
    if (str[start] == '?') {
        if (end_pos)
            *end_pos = start + 1;
        name = malloc(2);
        if (!name)
            return NULL;
        name[0] = '?';
        name[1] = '\0';
        return name;
    }
    
    if (isalpha(str[start]) || str[start] == '_') {
        end = start;
        
        while (str[end] && (isalnum(str[end]) || str[end] == '_'))
            end++;
        
        if (end_pos)
            *end_pos = end;
            
        name = malloc(end - start + 1);
        if (!name)
            return NULL;
        strncpy(name, str + start, end - start);
        name[end - start] = '\0';
        return name;
    }
    
    if (end_pos)
        *end_pos = start;
    return strdup("");
}