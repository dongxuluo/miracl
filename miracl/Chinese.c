#include <stdio.h>
#include "miracl.h"
#include "mirdef.h"

#define MAX 2 //一次同余方程组中方程的个数

int main()
{
	//初始化miracl
	miracl *mip = mirsys(4000, 10);
	mip->IOBASE = 10;

	big x[MAX];
	big a[MAX];
	big m[MAX];
	big Mj[MAX];  //Mj = M / mj
	big Mj1[MAX]; //Mj1 = Mj^Mj^-1(mod mj)

	big g = mirvar(0); //g = gcd(m[i], m[j])
	big one = mirvar(1);
	big M = mirvar(1);	//M = m1 * ... * mk
	big m1 = mirvar(0); //m1 = Mj^-1(mod mj)
	big X = mirvar(0);
	big tmpbig = mirvar(0);

	char tmpstr[1000] = "";

	for (int i = 0; i < MAX; i++)
	{
		x[i] = mirvar(0);
		a[i] = mirvar(0);
		m[i] = mirvar(0);
		Mj[i] = mirvar(0);
		Mj1[i] = mirvar(0);
	}

	FILE *fp = fopen("data.txt", "r");
	for (int i = 0; i < 2 * MAX; i++)
	{
		fscanf(fp, "%s", tmpstr);
		if (i < MAX)
		{
			cinstr(a[i], tmpstr);
			printf("a[%d] = ", i);
			cotnum(a[i], stdout);
		}
		else
		{
			cinstr(m[i - MAX], tmpstr);
			printf("m[%d] = ", i - MAX);
			cotnum(m[i - MAX], stdout);
		}
	}
	fclose(fp);

	for (int i = 0; i < MAX; i++)
	{
		for (int j = i + 1; j < MAX; j++)
		{
			egcd(m[i], m[j], g);

			if (mr_compare(g, one))
			{
				printf("不能直接应用中国剩余定理.\n");
				return 0;
			}
		}
	}

	for (int i = 0; i < MAX; i++)
	{
		multiply(m[i], M, M);
	}

	copy(M, tmpbig);
	for (int i = 0; i < MAX; i++)
	{
		divide(M, m[i], Mj[i]);
		xgcd(Mj[i], m[i], Mj1[i], m1, one);
		copy(tmpbig, M);
	}

	for (int i = 0; i < MAX; i++)
	{
		multiply(Mj[i], Mj1[i], tmpbig);
		multiply(tmpbig, a[i], x[i]);
		add(x[i], X, X);
	}

	powmod(X, one, M, X);

	cotstr(X, tmpstr);
	printf("x = %s(mod ", tmpstr);
	cotstr(M, tmpstr);
	printf("%s)\n", tmpstr);

	return 0;
}