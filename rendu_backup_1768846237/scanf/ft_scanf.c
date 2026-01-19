
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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

int scan_char(FILE *f, va_list ap)
{
	int c;
	char *p;

	c = fgetc(f);
	if (c == EOF)
		return (-1);
	p = va_arg(ap, char *);
	*p = c;
	return (1);
}

int scan_int(FILE *f, va_list ap)
{
	int c;
	int *p;
	char buf[12];
	int i = 0;

	p = va_arg(ap, int *);

	c = fgetc(f);
	if (c == '-' || c == '+') 
	{
		buf[i] = c;
		i++;
	}
	else
		ungetc(c, f);

	while ((c = fgetc(f)) != EOF && isdigit(c))
	{
		buf[i] = c;
		i++;
		if (i >= 11)
			break;
	}
	buf[i] = '\0';
	if (c != EOF && !isdigit(c))
		ungetc(c, f);

	if (i == 0 || (i == 1 && (buf[0] == '-' || buf[0] == '+')))
		return (-1);
	*p = atoi(buf);
	return (1);
}

int scan_string(FILE *f, va_list ap)
{
	int c;
	char *str;
	int i = 0;

	str = va_arg(ap, char *);

	while ((c = fgetc(f)) != EOF && !isspace(c))
	{
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	if (c != EOF && isspace(c))
		ungetc(c, f);
	if (i == 0)
		return (-1);
	return (1);
}

int match_conv(FILE *f, const char **format, va_list ap)
{
	switch (**format)
	{
	case 'c':
		return (scan_char(f, ap));
	case 'd':
		match_space(f);
		return (scan_int(f, ap));
	case 's':
		match_space(f);
		return (scan_string(f, ap));
	case EOF:
		return (-1);
	default:
		return (-1);
	}
}

int match_char(FILE *f, char c)
{
	int re = fgetc(f);
	if (re == EOF)
		return (-1);

	if (re == c)
		return (1);
	else
	{
		ungetc(re, f);
		return(-1);
	}
	// return (1);
}

int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int nconv = 0;

	int c = fgetc(f);
	if (c == EOF)
		return (EOF);
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
		return (EOF);
	return (nconv);
}

int ft_scanf(const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = ft_vfscanf(stdin, format, ap);
	va_end(ap);

	return (ret);
}

// int main(void)
// {
// 	int a;
// 	char s[100];
// 	char c;

// 	ft_scanf("abc%d %s %c", &a, s, &c);
// 	printf("|%d| |%s| |%c|\n", a, s, c);
// }
