#include <stdio.h>

//int *pi[8] //array ot 8 ptrss
//-----  study ----> int *pi[] = { 4, 8, 3, 4, 1, 6, 8, 4 }; //array ot ints i ptra --> pi

int is[8];
int *p = 0;

int main()
{
	int i;

	for(i=0; i<8;)
	{
		is[i] = ++i;
	}

	p = is+(char)1;

	//while(1)
	//{
	//	printf("%p => %d\n", p, *p);
	//	p+=1;
	//	if(!*p)
	//		break;
	//}

	while(1)
	{
		printf("%p => %d\n", pi, *pi);
		pi[0]+=1;
		if(!*pi)
			break;
	}

	return 0;
}
