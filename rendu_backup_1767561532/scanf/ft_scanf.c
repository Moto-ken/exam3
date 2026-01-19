

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int match_space(FILE *f)
{
	int c;

	while((c = fgetc(f)) != EOF)
	{
		if (!isspace(c))
		{
			ungetc(c, f);
			break;
		}
	}
	if (c == EOF)
		return (-1);
    return (0);
}

int match_char(FILE *f, char c)
{
	int a;

	a = fgetc(f);
	if (c == (char)a)
		return(1);
	else if (a != EOF)
		ungetc(a, f);
	else
		return (0);
    return (0);
}

int scan_char(FILE *f, va_list ap)
{
    char *p = va_arg(ap, char *);
	int c = fgetc(f);
	if (c == EOF)
		return (0);
	if (!p)
	{
		ungetc(c, f);
		return (0);
	}
	*p = (char)c;
    return (1);
}

int ft_atoi(char *str)
{
	int re = 0;
	size_t i = 0;
	int sign = 1;

	if (str[i] == '-' || str[i] =='+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		re = re * 10 + (str[i] - 48);
		i++;
	}
	return (sign * re);
}

int scan_int(FILE *f, va_list ap)
{
	int c;
	int *re;
	char buf[12];
	size_t i = 0;
	int digits = 0;

	re = va_arg(ap, int *);

	c = fgetc(f);
	if (c == '-' || c == '+')
	{
		buf[0] = (char)c;
		i++;
	}
	else if (c != EOF)
		ungetc(c, f);
	else 
		return (0);

	while ((c = fgetc(f)) != EOF && isdigit(c))
	{
		if (i >= 11)
			break;
		buf[i] = (char)c;
		i++;
		digits++;
	}
	buf[i] = '\0';
	if (c != EOF && !isdigit(c))
		ungetc(c, f);
	if (digits == 0)
		return (0);
	*re = ft_atoi(buf);
    return (1);
}

int scan_string(FILE *f, va_list ap)
{
	int c;
	char *p = va_arg(ap, char *);
	size_t i = 0;

	while ((c = fgetc(f)) != EOF && !isspace(c))
	{
		p[i] = c;
		i++;
	}
	p[i] = '\0';
	if (c != EOF && isspace(c))
		ungetc(c, f);
	if (i == 0)
		return (0);
    return (1);
}


int     match_conv(FILE *f, const char **format, va_list ap)
{
    switch (**format)
    {
        case 'c':
            return scan_char(f, ap);
        case 'd':
            match_space(f);
            return scan_int(f, ap);
        case 's':
            match_space(f);
            return scan_string(f, ap);
        case EOF:
            return -1;
        default:
            return -1;
    }
}

int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
        int nconv = 0;

        int c = fgetc(f);
        if (c == EOF)
                return EOF;
        ungetc(c, f);

        while (*format)
        {
                if (*format == '%')
                {
                        format++;
                        if (match_conv(f, &format, ap) != 1)
                                break;
                        else
                                nconv++;
                }
                else if (isspace(*format))
                {
                        if (match_space(f) == -1)
                                break;
                }
                else if (match_char(f, *format) != 1)
                        break;
                format++;
        }

        if (ferror(f))
                return EOF;
        return nconv;
}

int ft_scanf(const char *format, ...)
{
    
    va_list ap;
    va_start(ap, format);
    int ret = ft_vfscanf(stdin, format, ap);
    va_end(ap);
    return ret;
}

#include <stdio.h>

int main(void)
{
	int re;
	char str[100];
	
	re = ft_scanf("%s\n", str);
	printf("%s\n", str);
	return (0);
}

// int main(void)
// {
//     char str[100];
//     int re;

//     re = ft_scanf("%s", str); // ← 改行なし
//     printf("read: %s\n", str);
//     return 0;
// }

// int main(void)
// {
//     char str[100];
//     int re;

//     re = scanf("%s\n", str); // ← 改行なし
//     printf("read: %s\n", str);
//     return 0;
// }
