#include "chess.h"

static char	*ft_fill(const char *s1, const char *s2, char *f, int i)
{
	int	z;

	z = 0;
	while (s1[z])
	{
		f[z] = s1[z];
		z++;
	}
	z = 0;
	while (s2[z])
	{
		f[i + z] = s2[z];
		z++;
	}
	f[i + z] = 0;
	return (f);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		c;
	int		z;
	char	*f;

	if (!s1 || !s2)
		return (NULL);
	i = strlen(s1);
	c = strlen(s2);
	z = 0;
	f = malloc(sizeof(char) * (c + i + 1));
	if (!f)
		return (NULL);
	f = ft_fill(s1, s2, f, i);
	return (f);
}