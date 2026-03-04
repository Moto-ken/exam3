
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

size_t replace_and_write(char *work, size_t work_len, char *needle, size_t needle_len)
{
	size_t i = 0;
	while (i + needle_len <= work_len)
	{
		char *found = memmem(work + i, work_len - i, needle, needle_len);
		if (!found)
			break;

		size_t pos = found - work;
		if (pos > i)
			write(1, work + i, pos - i);
		size_t j = 0;
		while (j < needle_len)
		{
			write(1, "*", 1);
			j++;
		}
		i = pos + needle_len;
	}
	return (i);
}

int main(int argc, char *argv[])
{

	if (argc != 2 || !argv[1] || argv[1][0] == '\0')
		return (1);
	char *needle = argv[1];
	size_t needle_len = strlen(needle);
	char buf[5];
	char remain[needle_len];
	size_t remain_len = 0;
	ssize_t n;

	while ((n = read(0, buf, sizeof(buf))) > 0)
	{
		size_t work_len = remain_len + n;
		char *work = malloc(work_len);
		if (!work)
		{
			perror("Error");
			return (1);
		}
		if (remain_len > 0)
			memmove(work, remain, remain_len);
		memmove(work + remain_len , buf, n);

		size_t i = replace_and_write(work, work_len, needle, needle_len);
		remain_len = work_len - i;
		if (remain_len > 0)
			memmove(remain, work + work_len - remain_len, remain_len);
		free(work);
	}
	if (n < 0)
	{
		perror("Error");
		return (1);
	}
	// if (remain_len > 0)
	// 	write(1, remain, remain_len);
	if (remain_len > 0)
	{
		size_t last_i = replace_and_write(remain, remain_len, needle, needle_len);
		if (last_i < remain_len)
			write(1, remain + last_i, remain_len - last_i);
	}
	return (0);
}
