#include<regx52.h>
#define uchar unsigned char
#define uint unsigned int
uchar code table[]="Enter Password:";
sbit lcden=P2^7;//使能端
sbit lcdrs=P2^6;//数据/命令选择端
sbit lcdwr=P2^5;//读写选择器
sbit pwm=P2^0;
sbit jia=P3^1;
sbit jan=P3^0;
uint num;
void delay(uint xms){
	uint i,j;
	for(i=xms;i>0;i--){
		for(j=110;j>0;j--){}
	}
}
void write_com(uchar com){//写指令
	lcdrs=0;
	lcdwr=0;
	lcden=1;
	P0=com;
	delay(5);
	lcden=0;
}
void write_date(uchar date){//写数据
	lcdrs=1;
	lcdwr=0;
	P0=date;
	delay(10);
	lcden=1;
	delay(5);
	lcden=0;
}
void init(){//初始化
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
}

#define KEY_MATRIX_PORT P1
uchar pass[11]={
	'0','1','2','3','4','5','6','7','8','9','*'
};
uchar password[5];
uchar code open[]="12125";
uchar code error[]="Error";
uchar code Door[]="Open!";
uchar key;
uchar pos=0;
uchar flag,jd,count;
uchar key_matrix_ranks_scan(void){
	uchar key_value=0;
	KEY_MATRIX_PORT=0Xf7;
	if(KEY_MATRIX_PORT!=0xf7){
		delay(100);
		switch(KEY_MATRIX_PORT)
		{
			case 0x77:
					key_value=1;
					break;
			case 0xb7:
					key_value=5;
					break;
			case 0xd7:
					key_value=9;
					break;
			case 0xe7:
					key_value=13;
					break;
		}
	}
	while(KEY_MATRIX_PORT!=0xf7);
	KEY_MATRIX_PORT=0xfb;
	if(KEY_MATRIX_PORT!=0xfb){
		delay(100);
		switch(KEY_MATRIX_PORT){
			case 0x7b:
				key_value=2;
				break;
			case 0xbb:
				key_value=6;
				break;
			case 0xdb:
				key_value=10;
				break;
			case 0xeb:
				key_value=14;
				break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfb);
	KEY_MATRIX_PORT=0xfd;
	if (KEY_MATRIX_PORT!=0xfd){
		delay(100);
		switch(KEY_MATRIX_PORT){
			case 0x7d:
					key_value=3;
					break;
			case 0xbd:
					key_value=7;
					break;
			case 0xdd:
					key_value=11;
					break;
			case 0xed:
					key_value=15;
					break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfd);
	KEY_MATRIX_PORT=0xfe;
	if (KEY_MATRIX_PORT!=0xfe){
		delay(100);
		switch(KEY_MATRIX_PORT){
			case 0x7e:
					key_value=4;
					break;
			case 0xbe:
					key_value=8;
					break;
			case 0xde:
					key_value=12;
					break;
			case 0xee:
					key_value=16;
					break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfe);
	return key_value;
}
void Time0(){
  TMOD=0X01;
	TH0=(65536-459)/256;
	TL0=(65536-459)%256;
	EA=1;
	ET0=1;
	TR0=1;

}
void Time0_Int() interrupt 1
{
	TH0=(65536-459)/256;
	TL0=(65536-459)%256;
	if(count<jd){
		pwm=1;
	}else{
		pwm=0;
	}
	count++;
	if(count>=40){
		count=0;
	}
}
void keyscan(uchar flag){
	if(flag==1){
		delay(10);
		jd=2;
	}
	if(flag==0){
		delay(10);
		jd=5;
	}
}
	
void main(){
	init();
	write_com(0x80);
	for(num=0;num<15;num++){
		write_date(table[num]);
		delay(5);
	}
	Time0();
	jd=2;
	count=0;
	while(1){
			key=key_matrix_ranks_scan();
			if(key!=0){
				write_com(0x80+0x40);
				if(pos<5){
					password[pos]=pass[key-1];
					pos++;
					delay(5);
				}
				for(num=0;num<pos;num++){
					write_date(password[num]);
					delay(5);
				}
				if(pass[key-1]=='*'){
					for(num=0;num<5;num++){
						if(open[num]==password[num]){
							flag=1;
							continue;
						}else{
							flag=0;
							break;
						}
					}
					if(flag==0){
						for(num=0;num<5;num++){
							write_date(error[num]);
							delay(5);
						}
					}else{
							for(num=0;num<5;num++){
								write_date(Door[num]);
								delay(5);
						}
							delay(100);
					}
					
				}
				
			}
			keyscan(flag);
	}
}

