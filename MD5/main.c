//
//  main.c
//  MD5
//
//  Created by Михаил Фокин on 13.06.2021.
//

#include "ft_printf.h"
#include "ft_ssl.h"

/*
** Заполнение массив белым шумом.
*/
void fill_arr_k(t_md *md)
{
    t_uint temp[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
    ft_memcpy(md->arr_k, temp, sizeof(t_uint) * SIZE_MD5);
}

/*
** Заполнение массива числами, которые обозначают на сколько будет сбвиг
*/
void fill_arr_s(t_md *md)
{
    t_uint temp[] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
    };
    ft_memcpy(md->arr_s, temp, sizeof(t_uint) * SIZE_MD5);
}

/*
** Инициализация начальными данными.
*/
void init(t_md *md, const t_uchar *data)
{
//    ft_printf("sizeof = %d\n", sizeof(t_md));
    ft_memset(md, 0, sizeof(t_md));
    md->data = (t_uchar*)ft_strdup((const char*)data);
    fill_arr_k(md);
    fill_arr_s(md);
}

/*
** Освобождение памяти.
*/
void deinit(t_md *md)
{
    if (md->data != NULL)
        free(md->data);
}

void print_bits(t_uchar *str, size_t len)
{
    size_t i = 0;
    while (i < len)
    {
        ft_printf("%02x ", *str);
        str++;
        i++;
    }
    ft_putendl("");
}

/*
** Добавляет нули для выравнивания по длины сравнимой с 448 bit (56 oct).
*/
void add_zeros(t_md *md)
{
    size_t len = md->len;
    int tail = len % SIZE_MD5;
    int count_zeros = 0;
    if (tail < 56)
        count_zeros = 56 - tail;
    else
        count_zeros = SIZE_MD5 - tail + 56;
    //ft_printf("md->len = %d\n", md->len);
    //ft_printf("tail = %d\n", tail);
    //ft_printf("count_zeros = %d\n", count_zeros);
    md->count_octets = len + count_zeros + 8; // 8 октетов для хранения длины
    md->count_words = md->count_octets / SIZE_MD5;
    t_uchar *zeros = (t_uchar*)ft_strnew(md->count_octets);
    ft_memcpy(zeros, md->data, len + 1);
    ft_strdel((char**)&md->data);
    md->data = zeros;
    //ft_printf("count_octets %d, count_words %d\n", md->count_octets, md->count_words);
    //print_bits(md->data, md->count_octets);
}

/*
** Выравнивание потока.
*/
void step_one(t_md *md)
{
    md->len = (t_uint)ft_strlen((const char*)md->data);
    md->len_message = md->len * 8 % MAX_LEN;
    //ft_printf("len oct = %d\n", md->len);
    //ft_printf("len_message bit = %d\n", md->len_message);
    t_uchar *temp =(t_uchar*)ft_strjoin((const char*)md->data, "a");
    //temp[md->len - 1] = 0xa;
    temp[md->len] = 0x80;
    ft_strdel((char**)&md->data);
    md->data = temp;
    //print_bits(md->data, ft_strlen((const char *)md->data));
    //ft_printf("%s\n", md->data);
    //ft_printf("%d\n", ft_strlen((const char*)md->data));
    //print_bits((t_uchar *)md->data, 64);
    add_zeros(md);
}

/*
** Добавление длины сообщения (в битах) Последнее слобо будет иметь 512 бит.
** 8 вычитаем для отхода правого края сообщения на 8 октетов
*/
void step_two(t_md *md)
{
    ft_memcpy(md->data + md->count_octets - 8, &md->len_message, 8);
    //print_bits(md->data, md->count_octets);
}

/*
** Инициализация буфера.
*/
void step_three(t_md *md)
{
    md->hash[0] = 0x67452301;   // A
    md->hash[1] = 0xefcdab89;   // B
    md->hash[2] = 0x98badcfe;   // C
    md->hash[3] = 0x10325476;   // D
}

void stage_one(t_uint *vars, int i)
{
    vars[F] = (vars[B] & vars[C]) | ((~vars[B]) & vars[D]);
    vars[G] = i;
}

void stage_two(t_uint *vars, int i)
{
    vars[F] = (vars[D] & vars[B]) | ((~vars[D]) & vars[C]);
    vars[G] = (5 * i + 1) % 16;
}

void stage_three(t_uint *vars, int i)
{
    vars[F] = vars[B] ^ vars[C] ^ vars[D];
    vars[G] = (3 * i + 5) % 16;
}

void stage_four(t_uint *vars, int i)
{
    vars[F] = vars[C] ^ (vars[B] | (~vars[D]));
    vars[G] = (7 * i) % 16;
}

/*
** Основное тело алгоритма. Используются указатели на функцию.
*/
void rounds(t_md *md, const int words)
{
    t_uint *data = (t_uint *)(md->data + words);
    md->vars[A] = md->hash[A];
    md->vars[B] = md->hash[B];
    md->vars[C] = md->hash[C];
    md->vars[D] = md->hash[D];
    md->vars[F] = 0;
    md->vars[G] = 0;
    md->stages[0] = stage_one;
    md->stages[1] = stage_two;
    md->stages[2] = stage_three;
    md->stages[3] = stage_four;
    for (int i = 0; i < SIZE_MD5; i++)
    {
        md->stages[i / LEN_HASH_MD5](md->vars, i);
        md->vars[F] = md->vars[F] + md->vars[A] + md->arr_k[i] + data[md->vars[G]];
        md->vars[A] = md->vars[D];
        md->vars[D] = md->vars[C];
        md->vars[C] = md->vars[B];
        md->vars[B] = md->vars[B] + ((md->vars[F] << md->arr_s[i]) | (md->vars[F] >> (32 - md->arr_s[i])));
    }
    md->hash[A] += md->vars[A];
    md->hash[B] += md->vars[B];
    md->hash[C] += md->vars[C];
    md->hash[D] += md->vars[D];
}

void print_hash(const t_uchar *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
        ft_printf("%02x", data[i]);
    ft_putendl("");
}

/*
** Вычисления в цикле.
*/
void step_four(t_md *md)
{
    int words = 0;
    while (words < md->count_octets)
    {
        rounds(md, words);
        words += SIZE_MD5;
    }
    //print_bits((t_uchar *)md->hash, 16);
    //ft_printf("\n{%d%d%d%d}\n", md->hash[A], md->hash[B], md->hash[C], md->hash[D]);
    //print_hash(md);
}

/*
** Создает строку - hash. Память выделяется динамически.
*/
t_uchar *get_string_hash(t_md *md)
{
    t_uchar *result;
    
    result = (t_uchar*)ft_strnew(LEN_HASH_MD5);
    for (int i = 0; i < LEN_HASH_MD5; i++)
        result[i] = ((t_uchar *)md->hash)[i];
    return (result);
}

t_uchar *alg_md5(const t_uchar *data)
{
    t_uchar *result;
    t_md md;
    
    init(&md, data);
    step_one(&md);
    step_two(&md);
    step_three(&md);
    step_four(&md);
    result = get_string_hash(&md);
    //print_hash(result, 16);
    deinit(&md);
    return (result);
}

// MARK: ft_ssl

void init_ssl(t_ssl *ssl)
{
    ft_memset(ssl, 0, sizeof(t_ssl));
}

void print_used(const char *message)
{
    
}

void read_falg(t_ssl *ssl, const char *agr)
{
    
}

void fill_flags(t_ssl *ssl, int ac, const char **av)
{
    int i;
    
    i = 0;
    while (++i < ac)
    {
        ft_printf("{%s}\n", av[i]);
        if (av[i][0] == '-')
            read_falg(ssl, av[i]);
            
    }
}

int main(int argc, const char * argv[]) {
    t_ssl ssl;
    
    init_ssl(&ssl);
    fill_flags(&ssl, argc, argv);
    print_hash(alg_md5((const t_uchar*)argv[1]), LEN_HASH_MD5);
    return 0;
}

/*
 void rounds(t_md *md, const int words)
 {
     t_uint *data = (t_uint *)(md->data + words);
     md->vars[A] = md->hash[A];
     md->vars[B] = md->hash[B];
     md->vars[C] = md->hash[C];
     md->vars[D] = md->hash[D];
     md->vars[F] = 0;
     md->vars[G] = 0;
     md->stages[0] = stage_one;
     md->stages[1] = stage_two;
     md->stages[2] = stage_three;
     md->stages[3] = stage_four;
     for (int i = 0; i < SIZE_MD5; i++)
     {
         md->stages[i / LEN_HASH_MD5](md->vars, i);
 //        if (i < 16)
 //        {
 //            //stage_one(md->vars, i);
 //            md->stages[0](md->vars, i);
 ////            md->vars[F] = (md->vars[B] & md->vars[C]) | ((~md->vars[B]) & md->vars[D]);
 ////            md->vars[G] = i;
 //        }
 //        else if (i < 32)
 //        {
 //            //stage_two(md->vars, i);
 //            md->stages[1](md->vars, i);
 ////            md->vars[F] = (md->vars[D] & md->vars[B]) | ((~md->vars[D]) & md->vars[C]);
 ////            md->vars[G] = (5 * i + 1) % 16;
 //        }
 //        else if (i < 48)
 //        {
 //            //stage_three(md->vars, i);
 //            md->stages[2](md->vars, i);
 ////            md->vars[F] = md->vars[B] ^ md->vars[C] ^ md->vars[D];
 ////            md->vars[G] = (3 * i + 5) % 16;
 //        }
 //        else if (i < 64)
 //        {
 //            //stage_four(md->vars, i);
 //            md->stages[3](md->vars, i);
 ////            md->vars[F] = md->vars[C] ^ (md->vars[B] | (~md->vars[D]));
 ////            md->vars[G] = (7 * i) % 16;
 //        }
         //ft_printf("i = %d g = %d\n", i, g);
         //g = i / 16;
         md->vars[F] = md->vars[F] + md->vars[A] + md->arr_k[i] + data[md->vars[G]];
         md->vars[A] = md->vars[D];
         md->vars[D] = md->vars[C];
         md->vars[C] = md->vars[B];
         md->vars[B] = md->vars[B] + ((md->vars[F] << md->arr_s[i]) | (md->vars[F] >> (32 - md->arr_s[i])));
         //print_bits((u_char *)data, 4);
         //data++;
     }
     md->hash[A] += md->vars[A];
     md->hash[B] += md->vars[B];
     md->hash[C] += md->vars[C];
     md->hash[D] += md->vars[D];
 }
 */
