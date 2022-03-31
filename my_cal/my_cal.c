extern char _my_getchar();
extern void _my_putchar(char ch);
void my_cal(){
	int shuchu = 0;
	char s[100];
	int l =0;
	int i =0;
	unsigned int num = 0;
while(1){
char c = _my_getchar();	
if(c == '\n') break;
if(c=='0'&&shuchu==0) continue;
shuchu = 1;
s[l++]=c;
}
if(shuchu==0)
_my_putchar('0');
else {
	for( i=0;i<l;i++){
		num *= 10;
		num += s[i]-'0';
	}
	char out[100];
	int ll =0;
	do{
		int tmp = num%2;
		out[ll++] = tmp + '0';
		num/=2;
	}while (num != 0);
	for( i = ll-1;i>=0;i--){
		_my_putchar(out[i]);
	}
}

}
