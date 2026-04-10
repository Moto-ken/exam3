
#ifndef VBC_H
# define VBC_H

# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

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

node			*parse_add(char **s);
node			*parse_multi(char **s);
node			*parse_value(char **s);

#endif
