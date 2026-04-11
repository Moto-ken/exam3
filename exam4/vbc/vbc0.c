
#include "asfd.h"

node	*new_node(node n)
{
	node	*ret;

	ret = calloc(1, sizeof(n));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}

void	destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}

void	unexpected(char c)
{
	if (g_error)
		return ;
	g_error = 1;
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int	accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int	expect(char **s, char c)
{
	if (accept(s, c))
		return (1);
	unexpected(**s);
	return (0);
}

node	*parse_value(char **s)
{
	node	*ret;
	node	tmp;
	node	*next;

	if (isdigit(**s))
	{
		tmp.type = VAL;
		tmp.val = **s - '0';
		tmp.l = NULL;
		tmp.r = NULL;
		(*s)++;
		ret = new_node(tmp);
		if (!ret)
			return (NULL);
		if (isdigit(**s) || **s == '(')
		{
			destroy_tree(ret);
			unexpected(**s);
			return (NULL);
		}
		return (ret);
	}
	if (expect(s, '('))
	{
		next = parse_add(s);
		if (!next)
			return (NULL);
		if (!expect(s, ')'))
		{
			destroy_tree(next);
			return (NULL);
		}
		return (next);
	}
	unexpected(**s);
	return (NULL);
}

node	*parse_multi(char **s)
{
	node	*left;
	node	*right;
	node	tmp;
	node	*new;

	left = parse_value(s);
	if (!left)
		return (NULL);
	while (accept(s, '*'))
	{
		right = parse_value(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		tmp.type = MULTI;
		tmp.l = left;
		tmp.r = right;
		new = new_node(tmp);
		if (!new)
		{
			destroy_tree(left);
			destroy_tree(right);
			return (NULL);
		}
		left = new;
	}
	return (left);
}

node	*parse_add(char **s)
{
	node	*left;
	node	*right;
	node	tmp;
	node	*new;

	left = parse_multi(s);
	if (!left)
		return (NULL);
	while (accept(s, '+'))
	{
		right = parse_multi(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		tmp.type = ADD;
		tmp.l = left;
		tmp.r = right;
		new = new_node(tmp);
		if (!new)
		{
			destroy_tree(left);
			destroy_tree(right);
			return (NULL);
		}
		left = new;
	}
	return (left);
}

node	*parse_tree(char *s)
{
	node	*ret;

	ret = parse_add(&s);
	if (!ret)
	{
		return (NULL);
	}
	return (ret);
}

int	eval_tree(node *tree)
{
	switch (tree->type)
	{
	case ADD:
		return (eval_tree(tree->l) + eval_tree(tree->r));
	case MULTI:
		return (eval_tree(tree->l) * eval_tree(tree->r));
	case VAL:
		return (tree->val);
	}
}

int	main(int argc, char **argv)
{
	node	*tree;

	if (argc != 2)
		return (1);
	tree = parse_tree(argv[1]);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
}
