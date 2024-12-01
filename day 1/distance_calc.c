#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t ft_difference(value1, value2)
{
	if (value1>value2)
	{
		return (value1 - value2);
	}
	else
	{
		return (value2 - value1);
	}
	printf("IMPOSSIBLE ERROR IN DIFF");
}
void ft_sort(size_t *pairs, size_t len)
{
	size_t *array = pairs;
	size_t tmp = 0;
	
	for (size_t i = 0; i < len; i++)
	{
		for (size_t j = i+1; j < len; j++)
		{
			if (array[j] < array[i])
			{
				tmp = array[i];
				array[i]=array[j];
				array[j] = tmp;
			}
		}
	}
}

int main ()
{
	char buf[14];
	size_t *pairs[2];//list pair
	size_t array1[1001];
	size_t array2[1001];
	pairs[0] = array1;
	pairs[1] = array2;//list pair
	size_t readlen = 14;
	size_t value1 = 0;
	size_t value2 = 0;
	size_t totalval = 0;
	size_t i = 0;
	size_t j = 0;
	

	size_t fd = open("./location_id.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("fd -1\n");
		return 1;
	}
	while (readlen == 14) //place values into size_t **pair
	{
		readlen = read(fd, buf, 14);
		if (readlen == 0 || readlen == -1)
			break;
		value1 = atoi(buf);
		value2 = atoi((buf+7));
		pairs[0][i] = value1;
		pairs[1][i] = value2;
		i++;
	}
	value1 = 0;
	value2 = 0;
	ft_sort(pairs[0], i);
	ft_sort(pairs[1], i);
	while (j < i)//see diff of sorted pair
	{
		value1= pairs[0][j];
		value2= pairs[1][j];
		readlen = ft_difference(value1, value2);
		totalval += readlen;
		printf("line:%3zu,,diff:%4zu,,totalv:%7zu,v1:%zu,v2:%zu\n", j, readlen, totalval, value1, value2);
		value1 = 0;
		value2 = 0;
		j++;
	}
	printf("total value of diff:%zu\n", totalval);
	j = 0;
	totalval = 0;
	size_t duplicates;
	while (j < i)//see similarity score of sorted pair
	{
		value1= pairs[0][j];
		for (size_t k = 0; k < i; k++)
		{
			if (value1 == pairs[1][k])
			{
				duplicates++;
			}
		}
		totalval += (pairs[0][j]*duplicates);
		printf("total value:%8zu    pair[%3zu]:%5zu * dups:%2zu\n", totalval, j, pairs[0][j], duplicates);
		duplicates = 0;
		value1 = 0;
		j++;
	}
	printf("total value of similarity:%zu\n", totalval);
	return 0;
}