#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int ft_readline(char *buf, int fd)
{
	int i = 0;
	char tmp[2];
	int readlen = 0;
	tmp[1] = '\0';
	while (i < 1024)
	{
		readlen = read(fd, tmp, 1);
		if (tmp[0] == '\n' || tmp[0] == '\0')
			break;
		buf[i] = tmp[0];
		// printf("\nread[%d] buf[%s] tmp[%s]",i,buf,tmp);
		i++;
	}
	return (i);
}

int ft_difference(int value1, int value2, int *increase)
{
	if (value1 > value2)
	{
		*increase = 1;
		return (value1 - value2);
	}
	else if (value1 < value2)
	{
		*increase = -1;
		return (value2 - value1);
	}
	return 0;
}

void ft_zero_out(char *str, int len)
{
	int i = 0;
	while (i < len)
	{
		str[i] = '\0';
		i++;
	} 
}

int ft_intarray(char *str, int report[1024])
{
	int i = 0;
	int j = 0;
	char buf[13];
	
	ft_zero_out(buf, 13);
	while (str[i])
	{
		for (int k = 0;str[i] >= '0' && str[i] <= '9'; k++)
		{
			buf[k] = str[i];
			i++;
		}
		report[j] = atoi(buf);
		ft_zero_out(buf, 13);
		while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
			i++;
		j++;
	}
	return j;
}

int ft_check_success(int report[1024], int len)
{
	int i = 0;
	int diff = 0;
	int oldstate = 1;
	int state = 0;
	
	ft_difference(report[0], report[1], &state);
	oldstate = state;
	while (i < len-1)
	{
		diff = ft_difference(report[i], report[i+1], &state);
		if ((diff > 3 || diff < 1))
		{
			// printf("n1:%d,n2:%d,diff:%d ", report[i],report[i+1],diff);
			return 0;
		}
		if ((state > oldstate || state < oldstate))
		{
			// printf("-+ diff s:%d,os:%d ",state, oldstate);
			return 0;
		}
		oldstate = state;
		i++;
	}
	return 1;
}

int ft_check_wdamp(int report[1024], int len, int failhere)
{
	int i = 0;
	int diff = 0;
	int oldstate = 1;
	int state = 0;
	int failpos = 0;
	int fails = 0;
	
	if (failhere == 0)
		i++;
	if (failhere == 1)
		ft_difference(report[0], report[2], &state);
	else
		ft_difference(report[0], report[1], &state);
	oldstate = state;
	while (i < len-1)
	{
		if (failhere > 0 && (i+1 == failhere || i == failhere))
		{
			if (i+1 == failhere)
				diff = ft_difference(report[i], report[i+2], &state);
			if (i == failhere)
				diff = ft_difference(report[i-1], report[i+1], &state);
			oldstate = state;
		}
		else
			diff = ft_difference(report[i], report[i+1], &state);
		if ((diff > 3 || diff < 1))
		{
			if (failpos == 0)
				failpos = i;
			printf("n1:%d,n2:%d,diff:%d\n", report[i],report[i+1],diff);
			fails++;
		}
		if ((state > oldstate || state < oldstate))
		{
			if (failpos == 0)
				failpos = i;
			printf("-+ diff s:%d,os:%d \n",state, oldstate);
			if (fails == 0)
				fails++;
		}
		oldstate = state;
		i++;
	}
	if (fails > 0)
	{
		if (fails == 1 && failhere == -1)
		{
			return ft_check_wdamp(report, len, failpos);
		}
		return 0;
	}
	return 1;
}

int main()
{
	char str[1024];
	int reports[1024];
	int i = 0;
	int readlen = 1;
	int success = 0;
	int success_wdamp = 0;
	int len = 0;
	int tmp;
	int fd = open("report_levels.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("FD ERROR -1\n");
		return 1;
	}
	ft_zero_out(str, 1024);
	while (readlen != 0)
	{
		readlen = ft_readline(str,fd);
		if (str[0] == '\0')
				break;
		printf("line[%d{len:%d}]: %s ", i, readlen, str);
		len = ft_intarray(str, reports);
		tmp = ft_check_success(reports, len);
		success += tmp;
		tmp = ft_check_wdamp(reports, len, -1);
	 	if (tmp == 1)
		{
			printf("-Success\n");
		}
		else
			printf("-Fail\n");
		success_wdamp += tmp;
		ft_zero_out(str, readlen);
		i++;
	}
	printf("Amounts of success: %d.\n", success);
	printf("Amounts of success with dampener: %d.\n", success_wdamp);
	close(fd);
	return 0;
}