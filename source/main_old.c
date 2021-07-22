#include "ft_printf.h"
#include "ft_ssl.h"

/*
** Обнуление структуры.
*/
void	init_ssl(t_ssl *ssl)
{
	ft_memset(ssl, 0, sizeof(t_ssl));
}

/*
** Вывод сообщение об использовании.
*/
void	print_usage(t_ssl *ssl, const char c, const char *message)
{
	ft_putstr_fd(ssl->name_algorithm, 2);
	ft_putstr_fd(message, 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\nusage: ", 2);
	ft_putstr_fd(ssl->name_algorithm, 2);
	ft_putstr_fd(" [-pqr] [-s string] [files ...]\n", 2);
	exit(-1);
}

/*
** Проверяет допустимые флаги. Флаги могут быть pqrs
*/
void	check_valid_flags(t_ssl *ssl, const char *arg)
{
	while (*arg != '\0')
	{
		if (*arg != 'p' && *arg != 'q' && *arg != 'r' && *arg != 's')
			print_usage(ssl, *arg, ": illegal option -- ");
		arg++;
	}
}

void print_array_string(char **arr, size_t len)
{
	ft_printf("array:\n");
    for (size_t i = 0; i < len; i++)
        ft_printf("[%s]\n", arr[i]);
}

/*
** Добавление новой строки в указынный массив строк. Используется для добавление в strings и file_names
*/
void add_string_to_array(char ***array, size_t *count, const char *new_string)
{
    size_t i;
    
    i = 0;
    char **new_arr = (char **)malloc(sizeof(char *) * (*count + 1));
    while (i < *count)
    {
        new_arr[i] = ft_strdup((*array)[i]);
        ft_strdel(&((*array)[i]));
        i++;
    }
    new_arr[i] = ft_strdup(new_string);
    free(*array);
    *array = new_arr;
    (*count)++;
}

/*
** Добавление новой строки в массив строк.
*/
void add_string(t_ssl *ssl, int ac, const char **av, int *i)
{
    (*i)++;
    if (*i == ac)
        print_usage(ssl, 's', ": option requires an argument -- ");
    add_string_to_array(&ssl->strings, &ssl->count_strings, av[*i]);
}

/*
** Происходит прверка флага. Если длина равно 1 (т.е. пришла только '-').
** Значит в аргументах идут имена файлов.
*/
void read_flag(t_ssl *ssl, int ac, const char **av, int *i, int *k)
{
    const char *arg = av[*i];
    if (ft_strlen(arg) == 1)
    {
        *k = 2;
        return ;
    }
    arg++;
    check_valid_flags(ssl, arg);
    while (*arg != '\0')
    {
        if (*arg == 'p')
            ssl->flags[P] = 1;
        else if (*arg == 'q')
            ssl->flags[Q] = 1;
        else if (*arg == 'r')
            ssl->flags[R] = 1;
        else if (*arg == 's')
        {
            ssl->flags[S] = 1;
            add_string(ssl, ac, av, i);
            return ;
        }
        arg++;
    }
}

/*
** Добавляет новое имя файла в массив имен файлов.
*/
void add_file_name(t_ssl *ssl, const char **av, int i)
{
    add_string_to_array(&ssl->file_names, &ssl->count_file_names, av[i]);
}

void print_info(t_ssl *ssl)
{
    print_array_string(ssl->strings, ssl->count_strings);
    print_array_string(ssl->file_names, ssl->count_file_names);
    ft_printf("p = %d\n q = %d\n r = %d\n s = %d\n", ssl->flags[P], ssl->flags[Q], ssl->flags[R], ssl->flags[S]);
}

/*
** Заполнение флагов программы.
*/
void fill_flags(t_ssl *ssl, int ac, const char **av)
{
    int i;
    int k;
    
    i = 1;
    k = 0;
    while (++i < ac)
    {
        if (av[i][0] == '-' && k < 2)
            read_flag(ssl, ac, av, &i, &k);
        else
            k = 2;
        if (k == 2)
            add_file_name(ssl,av, i);
    }
}

/*
** Проверка имени алгоритма.
*/
void check_name_algorithm(t_ssl *ssl, int ac, const char **av)
{
    if (ac < 2)
        sys_err("usage: ft_ssl command [command opts] [command args]\n");
    if (ft_strcmp(av[1], "md5") && ft_strcmp(av[1], "sha256"))
    {
        ft_putstr_fd("ft_ssl: Error: \'", 2);
        ft_putstr_fd(av[1], 2);
        ft_putstr_fd("\' is an invalid command.\n", 2);
        exit(-1);
    }
    ssl->name_algorithm = ft_strdup(av[1]);
}

/*
** Освобождение памяти, выделенные под массивы строк.
*/
void delete_array_string(char ***array, size_t len)
{
    for (size_t i = 0; i < len; i++)
        ft_strdel(&(*array)[i]);
    free(*array);
}

void deinit_ssl(t_ssl *ssl)
{
    free(ssl->name_algorithm);
    delete_array_string(&ssl->strings, ssl->count_strings);
    delete_array_string(&ssl->file_names, ssl->count_file_names);
}


/*
** Возвращает содержимое файла в виде строки.
** Память выделяется динамически.
*/
char *get_data_fd(int fd, size_t *len_message_oct)
{
    char    buff[BUFF + 1];
    ssize_t oct_read;
    char    *data;
    
    data = ft_strnew(0);
    *len_message_oct = 0;
    while ((oct_read = read(fd, buff, BUFF)) > 0)
    {
        buff[oct_read] = '\0';
        char *temp = ft_strnew(*len_message_oct + oct_read);
        ft_memcpy(temp, data, *len_message_oct);
        ft_memcpy(temp + *len_message_oct, buff, oct_read);
        ft_strdel(&data);
        data = temp;
        ft_memset(buff, 0, BUFF);
        *len_message_oct += oct_read;
    }
    return data;
}

void chenge_endian(t_uchar *data, size_t count_octets)
{
    size_t i = 0;
    t_elem elem;
    
    while (i < count_octets)
    {
        elem.i_elem = *(t_uint *)(data + i);
        (data + i)[0] = elem.c_elem[3];
        (data + i)[1] = elem.c_elem[2];
        (data + i)[2] = elem.c_elem[1];
        (data + i)[3] = elem.c_elem[0];
        i += 4;
    }
}

/*
** Подготовка входного сообщения
** Добавляет нулевой бит, выравнивает сообщение до кратности 512 бит.
*/
t_uchar *preparation(const t_uchar *data, size_t *count_octets, t_ssl *ssl)
{
    size_t len = ssl->len_message_oct;
    t_ulong len_message = len * 8 % MAX_LEN;
    t_uchar *temp = (t_uchar *)ft_strnew(len + 1);
    ft_memcpy(temp, data, len);
    temp[len] = 0x80;
    int tail = len % SIZE_MD5;
    size_t count_zeros = 0;
    if (tail < 56)
        count_zeros = 56 - tail;
    else
        count_zeros = SIZE_MD5 - tail + 56;
    *count_octets = len + count_zeros + 8;
    t_uchar *zeros = (t_uchar*)ft_strnew(*count_octets);
    ft_memcpy(zeros, temp, len + 1);
    ft_strdel((char**)&temp);
    if (!ft_strcmp(ssl->name_algorithm, "md5"))
        ft_memcpy(zeros + *count_octets - 8, &len_message, 8);
    else
    {
        chenge_endian(zeros, *count_octets);
        *(t_ulong *)(zeros + *count_octets - 4) = len_message;
    }
    return zeros;
}

/*
** Возвращает хеш в зависимости от алгоритма хеширования.
*/
t_uchar *get_hash(const t_uchar *data, t_ssl *ssl)
{
    t_uchar *hash;
    size_t count_octets;
    t_uchar *new_data = preparation(data, &count_octets, ssl);
    if (!ft_strcmp(ssl->name_algorithm, "md5"))
        hash = alg_md5(new_data, count_octets);
    else
        hash = alg_sha256(new_data, count_octets);
    free(new_data);
    return hash;
}

void print_hash(t_ssl *ssl, const t_uchar *data)
{
    int i = -1;
    int len = 0;
    if (!ft_strcmp(ssl->name_algorithm, "md5"))
        len = 16;
    else
        len = 32;
    while (++i < len)
        ft_printf("%02x", data[i]);
    ft_putendl("");
}

/*
** Взятие информации из входного потока.
*/
void working_stdin(t_ssl *ssl)
{
    char *data = get_data_fd(0, &ssl->len_message_oct);
    t_uchar *hash = get_hash((const t_uchar *)data, ssl);
    if (ssl->flags[P])
        ft_putstr(data);
    print_hash(ssl, hash);
    free(hash);
    ft_strdel(&data);
}

void print_hash_strings_files(t_ssl *ssl, const t_uchar *data, const char *info)
{
    t_uchar *hash = get_hash(data, ssl);
    if (ssl->flags[Q])
        print_hash(ssl, hash);
    else if (ssl->flags[R])
    {
        int len = 0;
        if (!ft_strcmp(ssl->name_algorithm, "md5"))
            len = LEN_HASH_MD5;
        else
            len = LEN_HASH_SHA256;
        for (int i = 0; i < len; i++)
            ft_printf("%02x", hash[i]);
        ft_printf(" %s\n", info);
    }
    else
    {
        ft_printf("%s (%s) = ", ssl->name_algorithm, info);
        print_hash(ssl, hash);
    }
    free(hash);
}

void working_strings(t_ssl *ssl)
{
    size_t i;
    
    i = 0;
    while (i < ssl->count_strings)
    {
        char *info = ft_multi_strdup(3, "\"", ssl->strings[i], "\"");
        ssl->len_message_oct = ft_strlen(ssl->strings[i]);
        print_hash_strings_files(ssl, (const t_uchar*)ssl->strings[i], info);
        ft_strdel(&info);
		i++;
    }
}

/*
** Определение, является ли файл папкой.
** В условиял сказано, что можно использовать функции, если их использование обосновано.
** MD5 не вычисляет хеш папок.
*/
int is_dir(const char *name_file)
{
    struct stat buf;
    lstat(name_file, &buf);
    return S_ISDIR(buf.st_mode);
}

/*
** Вычисление хеш файлов. Папки не обрабатываются.
*/
void working_files(t_ssl *ssl)
{
    int		fd;
    size_t  i;
    
    i = 0;
    while (i < ssl->count_file_names)
    {
        if (is_dir(ssl->file_names[i]))
            ft_printf("%s: %s: Is a directory.\n", ssl->name_algorithm, ssl->file_names[i]);
        else if ((fd = open(ssl->file_names[i], O_RDONLY)) < 0)
            ft_printf("%s: %s: Error open file.\n", ssl->name_algorithm, ssl->file_names[i]);
        else
        {
            char *data = get_data_fd(fd, &ssl->len_message_oct);
            print_hash_strings_files(ssl, (const t_uchar *)data, ssl->file_names[i]);
            ft_strdel(&data);
        }
		i++;
    }
}

void run(t_ssl *ssl)
{
    if (ssl->flags[P] || (!ssl->flags[S] && ssl->count_file_names == 0))
        working_stdin(ssl);
    if (ssl->flags[S])
        working_strings(ssl);
    if (ssl->count_file_names != 0)
        working_files(ssl);
}

int main(int ac, const char * av[]) {
    t_ssl ssl;
    
    init_ssl(&ssl);
    check_name_algorithm(&ssl, ac, av);
    fill_flags(&ssl, ac, av);
    run(&ssl);
    deinit_ssl(&ssl);
    return 0;
}
