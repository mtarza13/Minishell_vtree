/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t ft_strlen(const char *s) {
    size_t len = 0;
    if (!s) return 0;
    while (s[len]) len++;
    return len;
}

char *ft_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = ft_strlen(s);
    char *dup = malloc(len + 1);
    if (!dup) return NULL;
    for (size_t i = 0; i <= len; i++) dup[i] = s[i];
    return dup;
}

char *ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2) {
    if (!s1 && !s2) return NULL;
    if (!s1) return free_s2 ? s2 : ft_strdup(s2);
    if (!s2) return free_s1 ? s1 : ft_strdup(s1);
    
    size_t len1 = ft_strlen(s1), len2 = ft_strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len1; i++) result[i] = s1[i];
    for (size_t i = 0; i < len2; i++) result[len1 + i] = s2[i];
    result[len1 + len2] = '\0';
    
    if (free_s1) free(s1);
    if (free_s2) free(s2);
    return result;
}

int ft_strcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return -1;
    while (*s1 && *s2 && *s1 == *s2) { s1++; s2++; }
    return *s1 - *s2;
}

int ft_strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n && s1[i] && s2[i]; i++)
        if (s1[i] != s2[i]) return s1[i] - s2[i];
    return 0;
}

char *ft_strchr(const char *s, int c) {
    if (!s) return NULL;
    while (*s) { if (*s == c) return (char *)s; s++; }
    return c == '\0' ? (char *)s : NULL;
}

char *ft_strncpy(char *dest, const char *src, size_t n) {
    for (size_t i = 0; i < n; i++)
        dest[i] = (i < ft_strlen(src)) ? src[i] : '\0';
    return dest;
}

void ft_free_array(char **array) {
    if (!array) return;
    for (int i = 0; array[i]; i++) free(array[i]);
    free(array);
}

int ft_isalnum(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int ft_isdigit(int c) { return c >= '0' && c <= '9'; }
int ft_isspace(int c) { return c == ' ' || c == '\t' || c == '\n'; }

int ft_atoi(const char *str) {
    int result = 0, sign = 1, i = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)) i++;
    if (str[i] == '-' || str[i] == '+') { if (str[i] == '-') sign = -1; i++; }
    while (str[i] >= '0' && str[i] <= '9') { result = result * 10 + (str[i] - '0'); i++; }
    return result * sign;
}

char *ft_itoa(int n) {
    char *str; int len = 0; long num = n, temp = num;
    if (num <= 0) len = 1;
    while (temp) { temp /= 10; len++; }
    str = malloc(len + 1);
    if (!str) return NULL;
    str[len] = '\0';
    if (num == 0) str[0] = '0';
    if (num < 0) { str[0] = '-'; num = -num; }
    while (num > 0) { str[--len] = (num % 10) + '0'; num /= 10; }
    return str;
}

int is_valid_identifier(char *str) {
    if (!str || !*str) return 0;
    if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_'))
        return 0;
    for (int i = 1; str[i]; i++)
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || 
              (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
            return 0;
    return 1;
}