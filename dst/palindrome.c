#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int num=0,t=n;
	while(t>0){
		num = num * 10 + t%10;
		t = t /10;
	}
	if(n==num){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
