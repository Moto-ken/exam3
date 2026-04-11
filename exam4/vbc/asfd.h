
#ifndef ASFD_H
# define ASFD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

typedef struct node
{
	enum
	{
		ADD,
		MULTI,
		VAL
	} type;
	int			val;
	struct node	*l;
	struct node	*r;
}				node;

static int g_error  = 0;

node	*parse_add(char **s);
node	*parse_multi(char **s);
node	*parse_value(char **s);

#endif
