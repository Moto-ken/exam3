

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int match_space(FILE *f)
{
    int c;

	while ((c = fgetc(f)) != EOF)
	{
		if (!isspace(c))
		{
			ungetc(c, f);
			break;
		}
	}
    return (0);
}

int match_char(FILE *f, char c)
{
    int s;

	s = fgetc(f);
	if (c == s)
		return (1);
	else
	{
		ungetc(s, f);
    	return (0);
	}
}

int scan_char(FILE *f, va_list ap)
{
    char *s;
	int c;

	s = va_arg(ap, char *);
	c = fgetc(f);
	if (c == EOF)
		return (0);
	*s = c;
    return (1);
}

int simple_atoi(char *buf)
{
	size_t i = 0;
	int res = 0;
	int sign = 1;

	if (buf[i] == '-' || buf[i] == '+')
	{
		if (buf[i] == '-')
			sign = -1;
		i++;
	}
	while (buf[i])
	{
		res = res * 10 + (buf[i] - 48);
		i++;
	}
	return (sign * res);
}

int scan_int(FILE *f, va_list ap)
{
	int *s;
	int c;
	char buf[12];
	size_t i = 0;
	int flag = 0;
	
	s = va_arg(ap, int *);
	c = fgetc(f);
	if (c == '+' || c == '-')
	{
		buf[0] = c;
		i++;
	}
	ungetc(c, f);
	while ((c = fgetc(f)) != EOF && isdigit(c))
	{
		buf[i] = c;
		i++;
		flag++;
	}
	buf[i] = '\0';
	if (c != EOF && isspace(c))
		ungetc(c, f);
	if (flag == 0)
		return (0);
	*s = simple_atoi(buf);
    return (1);
}

int scan_string(FILE *f, va_list ap)
{
	char *s;
	int c;
	size_t i = 0;

	s =	va_arg(ap, char *);
	while ((c = fgetc(f)) != EOF && !isspace(c))
	{
		s[i] = c;
		i++;
	}
	s[i] = '\0';
	if (c != EOF && isspace(c))
		ungetc(c, f);
	if (i <= 0)
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

int main(void)
{
	char buf[100];

	ft_scanf("%s", buf);
	printf("%s\n", buf);
}
