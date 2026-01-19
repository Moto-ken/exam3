
#include "powerset.h"

int *make_list(char **argv, int argc)
{
	if (argc <= 3)
		return (NULL);
	int *ele;
	size_t i = 0;
	size_t j = 2;

	ele = malloc(sizeof(int) * (argc - 2));
	if (!ele)
		return (NULL);
	while (i < argc - 2)
	{
		ele[i] = atoi(argv[j]);
		i++;
		j++;
	}
	return (ele);
}

void dfs(t_ctx *ctx, int depth, int index, int remaining)
{
    if (remaining == 0)
    {
        for (int i = 0; i < depth; i++)
        {
            if (i)
				printf(" ");
            printf("%d", ctx->current[i]);
        }
        printf("\n");
        return;
    }
    if (index == ctx->count)
        return;

    ctx->current[depth] = ctx->nums[index];
	dfs(ctx, depth + 1, index + 1, remaining - ctx->nums[index]);
	dfs(ctx, depth, index + 1, remaining);
}

int main(int argc, char *argv[])
{
	if (argc <= 2)
		return (1);
	
	t_ctx ctx;

	ctx.sum = atoi(argv[1]);
	ctx.nums = make_list(argv, argc);
	if (!ctx.nums)
		return (1);
	ctx.count= argc - 2;
	ctx.current = malloc(sizeof(int) * ctx.count);
	if (!ctx.current)
	{
		free(ctx.nums);
		return (1);
	}
	dfs(&ctx, 0, 0, ctx.sum);
	free(ctx.nums);
	free(ctx.current);
	return (0);
}