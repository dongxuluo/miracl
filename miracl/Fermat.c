#include <stdio.h>
#include "miracl.h"
#include "mirdef.h"

int main()
{
	//初始化miracl
	miracl *mip = mirsys(1000, 10);
	mip->IOBASE = 10;

	int k = 0; //安全参数k
	int exp = 1;
	double p = 0;
	big m = mirvar(0);	 //奇整数m >= 3
	big m_4 = mirvar(0); //m_4 = m - 4
	big m_1 = mirvar(0); //m_1 = m - 1
	big a = mirvar(0);	 //随机整数2 <= a <= m - 2
	big g = mirvar(0);	 //g = gcd(a, m)
	big one = mirvar(1);
	big r = mirvar(0); //r = a ^ (m - 1)(mod m)

	FILE *fp = fopen("odd.txt", "r");
	cinnum(m, fp);
	fclose(fp);

	printf("奇整数 m = ");
	cotnum(m, stdout);

	decr(m, 4, m_4);
	decr(m, 1, m_1);

	printf("安全参数 k = ");
	scanf("%d", &k);

	for (int i = 0; i < k; i++)
	{
		bigrand(m_4, a); //0 <= a <= m - 4
		incr(a, 2, a);	 //2 <= a <= m - 2
		egcd(a, m, g);

		printf("随机整数 a = ");
		cotnum(a, stdout);
		printf("g = gcd(a, m) = ");
		cotnum(g, stdout);

		if (!mr_compare(g, one))
		{
			powmod(a, m_1, m, r);

			printf("r = a^(m - 1)(mod m) = ");
			cotnum(r, stdout);

			if (mr_compare(r, one))
			{
				printf("r = a^(m - 1)(mod m) != 1, m为合数.\n");
				break;
			}
			else
			{
				exp = exp * 2;
				p = 1.0 - 1.0 / exp;
				printf("m 有 p = %.2f%% 的可能性是素数.\n\n", p * 100);
			}
		}
		else
		{
			printf("g = gcd(a, m) != 1, m为合数.\n");
			break;
		}
	}

	return 0;
}