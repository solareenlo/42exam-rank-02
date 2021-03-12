#include "get_next_line.h"

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup(char *s)
{
	char	*new;
	char	*res;

	res = new = malloc(ft_strlen(s) + 1);
	while (*s)
		*new++ = *s++;
	*new = 0;
	return (res);
}

char	*ft_strchr(char *s, char c)
{
	if (*s == c)
		return (s);
	while (*s++)
		if (*s == c)
			return (s);
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*new;
	char	*res;

	res = new = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	while (*s1)
		*new++ = *s1++;
	while (*s2)
		*new++ = *s2++;
	*new = 0;
	return (res);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*new;
	char	*res;

	res = new = malloc(len + 1);
	while (start--)
		s++;
	while (len-- && *s)
		*new++ = *s++;
	*new = 0;
	return (res);
}

int		get_next_line(char **line)
{
	static char	*s;
	char		buf[1025];
	char		*pos;
	char		*tmp;
	int			ret;

	if (!line || (!s && !(s = ft_strdup(""))))
		return (-1);
	while ((pos = ft_strchr(s, '\n')) == 0)
	{
		if ((ret = read(0, buf, 1024)) == 0)
			break ;
		buf[ret] = 0;
		tmp = s;
		s = ft_strjoin(s, buf);
		free(tmp);
	}
	if (pos)
	{
		*line = ft_substr(s, 0, pos - s);
		tmp = s;
		s = ft_substr(s, pos - s + 1, ft_strlen(s));
		free(tmp);
		return (1);
	}
	*line = ft_strdup(s);
	free(s);
	return (0);
}
