#include <unistd.h>
#include <stdarg.h>
#define BUF	16
#define PAD	16

const char	*digits	=	"0123456789abcdef";
const char	*zero	=	"0000000000000000";
const char	*space	=	"                ";

typedef struct	s_data {
	char			*fmt, *ptr;
	char			c, sign;
	int				ret, zeroflag;
	int				width, prec;
	int				len, reallen, prelen;
	unsigned int	val;
	char			buf[BUF];
}				t_data;

int	ft_isdigit(char c)
{
	return ('0' <= c && c <= '9');
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

size_t	ft_strnlen(char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (s[len] && len < maxlen)
		len++;
	return (len);
}

void	ft_itoa_base(t_data *d, int base)
{
	if (d->prec >= 0) d->zeroflag = 0;
	d->ptr = d->buf + BUF;
	if (d->val || d->prec)
	{
		*(--d->ptr) = digits[d->val % base];
		d->val /= base;
		while (d->val)
		{
			*(--d->ptr) = digits[d->val % base];
			d->val /= base;
		}
	}
	d->len = d->buf + BUF - d->ptr;
}

int	ft_get_flag(t_data *d, va_list ap)
{
	while (1)
	{
		d->c = *d->fmt++;
		if (d->c == 0)
			return (0);
		else if (d->c == '.')
		{
			d->prec = 0;
			d->c = *d->fmt++;
			while (ft_isdigit(d->c))
			{
				d->prec = 10 * d->prec + (d->c - '0');
				d->c = *d->fmt++;
			}
			d->fmt--;
		}
		else if (ft_isdigit(d->c))
		{
			if (d->c == '0')
				d->zeroflag = 1;
			else
			{
				d->width = d->c - '0';
				d->c = *d->fmt++;
				while (ft_isdigit(d->c))
				{
					d->width = 10 * d->width + (d->c - '0');
					d->c = *d->fmt++;
				}
				d->fmt--;
			}
		}
		else if (d->c == 'd')
		{
			d->val = va_arg(ap, int);
			if ((int)d->val < 0)
			{
				d->val *= -1;
				d->sign = '-';
			}
			ft_itoa_base(d, 10);
			return (1);
		}
		else if (d->c == 'x')
		{
			d->val = va_arg(ap, unsigned int);
			d->sign = 0;
			ft_itoa_base(d, 16);
			return (1);
		}
		else if (d->c == 's')
		{
			d->ptr = va_arg(ap, char *);
			if (d->ptr == NULL)
				d->ptr = "(null)";
			if (d->prec >= 0)
				d->len = ft_strnlen(d->ptr, d->prec);
			else
				d->len = ft_strlen(d->ptr);
			d->prec = d->sign = 0;
			return (1);
		}
	}
	return (1);
}

void	ft_pad(int len, const char *s)
{
	while (len > 0)
	{
		int n;

		n = (len >= PAD) ? PAD : len;
		write(1, s, n);
		len -= n;
	}
}

void	ft_put_after_percent(t_data *d)
{
	d->reallen = d->prec > d->len ? d->prec : d->len;
	if (d->sign) d->reallen++;
	d->prelen = d->width > d->reallen ? d->width : d->reallen;
	if (!d->zeroflag)
		ft_pad(d->width - d->reallen, space);
	if (d->sign)
		write(1, &d->sign, 1);
	if (d->zeroflag)
		ft_pad(d->width - d->reallen, zero);
	ft_pad(d->prec - d->len, zero);
	write(1, d->ptr, d->len);
	d->ret += d->prelen;
}

int	ft_printf(const char *fmt, ... )
{
	t_data	d = {0};
	va_list	ap;

	d.fmt = (char *)fmt;
	va_start(ap, fmt);
	while (1)
	{
		d.ptr= d.fmt;
		while ((d.c = *d.fmt) && d.c != '%')
			d.fmt++;
		write(1, d.ptr, d.fmt - d.ptr);
		d.ret += d.fmt - d.ptr;
		if (d.c == 0)
			break ;
		d.fmt++;
		d.width = d.zeroflag = d.sign = 0;
		d.prec = -1;
		if (!ft_get_flag(&d, ap))
			break ;
		ft_put_after_percent(&d);
	}
	va_end(ap);
	return (d.ret);
}
