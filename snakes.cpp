#include<dos.h>
#include<conio.h>
#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include<time.h>
#include<values.h>

//Size of the GREENBLOCK >=3;
#define GREENBLOCK 10
//speed, this make slower the game, >5 slower,<5 faster, >=0;
#define speed 3
//The upper border,>=30 if you want to avoid the text.
#define upper 30
//Space from the border, you can make the battle field more little 1-364
#define ori 100
//>=1
#define maxlen 1000
//>=1
#define vidas 5
//Graphics  resolution Mode 0=320*240 1=640*480 2=800*600 3=1024*768 algo así no estoy muy seguro lo que si se es que sporta hasta 4 y se ve chingon.
#define mode 2
//graphics driver
#define driver "egavga"
//Grown per Greenblock >=1
#define grownpergreen 100
//Starting Grown >=1
#define grownpoints   100
//Cicles before force exit Cicles*speed/1000=seconds >=1
#define gracepoints 200
//Cicles before let the start, avoid speedied efect >=1
#define forcedgrace 100
struct punto{
	int x,y;
};

class player{
	private:
		int dx,dy;
		int x,y;
		int grace;
		int len;
		punto points[maxlen];
		int grown;
		int color[5];
		void drawline();
		void died();
		void lived();
		void initialize(void);
	public:
		int lives;
		int number;
		void move(void);
		void drawmarkers(void);
		void initall(int);
		void nowto(int,int);
};

void draw(void);
void putitem(int);
char cursor(int,int);
void vga16(void);
void askforexit(void);
int  winerscreen(int);
void help(void);
void start(void);
void intro(void);
int play(void);
void finish(void);
void clearbuffer(void);

void main(){
	randomize();
	vga16();
	intro();
	do{
	}while(winerscreen(play()));
	finish();
}


void finish(){
	int x;
	closegraph();
	vga16();
	settextstyle(0,1,1);
	settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
	setcolor(WHITE);
	cleardevice();
	outtextxy(getmaxx()-textheight(""),getmaxy(),"Thanks For your Soul");
	cursor(getmaxx()-textheight(""),getmaxy()-(textwidth("Thanks For your Soul")));
	closegraph();
	exit(1);
}

void intro(void)
{
	 int midx,midy,x,y;
	 player intros;
	 midx = getmaxx() / 2;
	 midy = getmaxy() / 2;
	 setcolor(WHITE);
	 settextjustify(CENTER_TEXT, CENTER_TEXT);
	 do{
			cleardevice();
			settextstyle(0, 0, random(10));
			outtextxy(midx, midy,"Snakes");
			delay(10);
	 }while(!kbhit());
	 getch();
	 cleardevice();
	 settextstyle(0,0,10);
	 outtextxy(midx, midy,"Snakes");
	 x=midx+(textwidth("Snakes")/2);
	 y=midy+(textheight("")/2);
	 intros.initall(-1);
	 settextstyle(0,0,1);
	 settextjustify(RIGHT_TEXT, TOP_TEXT);
	 outtextxy(x,y,"At 8§ Hour. ");
	 do{
		intros.move();
		delay(2);
	 }while(intros.lives>=0);
	 settextstyle(0,0,10);
	 setlinestyle(SOLID_LINE,0,1);
	 for(y=midy-(textheight("")/2);y<midy+(textheight("")/2)+8;y+=2){
		line(midx-(textwidth("Snakes")/2),y,midx+(textwidth("Snakes")/2), y);
		delay(30);
	 }
	 for(y=midy+(textheight("")/2)+9;y>=midy-(textheight("")/2)-1;y-=2){
		line(midx-(textwidth("Snakes")/2),y,midx+(textwidth("Snakes")/2), y);
		delay(30);
	 }
	 settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
	 settextstyle(0,0,1);
	 setcolor(WHITE);
	 outtextxy(getmaxx(),getmaxy(),"By: Arias Levita ");
	 cursor(getmaxx(),getmaxy());

}


int play(){

		settextstyle(0,0,1);
		char lp;
		int x,y,winer;
		player players[2];
		cleardevice();
		draw();
		players[1].initall(1);
		players[0].initall(0);
		players[0].drawmarkers();
		players[1].drawmarkers();
		putitem(1);
		help();
		start();
		do{
			if(kbhit()){
				lp=getch();
				if(lp==0){
					lp=getch();
					switch(lp){
						case 'H':
							players[1].nowto(0,-1);
							break;
						case 'P':
							players[1].nowto(0,1);
							break;
						case 'M':
							players[1].nowto(1,0);
							break;
						case 'K':
							players[1].nowto(-1,0);
							break;
						case ';':
							help();
							break;
					}
				}
				else{
					switch(lp){
						case 'W': case'w':
							players[0].nowto(0,-1);
							break;
						case 'S': case's':
							players[0].nowto(0,1);
							break;
						case 'A': case'a':
							players[0].nowto(-1,0);
							break;
						case 'D': case'd':
							players[0].nowto(1,0);
							break;
						case 27:
							askforexit();
					}
				}
			}
			players[0].move();
			players[1].move();
			delay(speed);

		}while(players[1].lives>=1&&players[0].lives>=1);
		if(players[0].lives)
			return 1;
		if(players[1].lives)
			return 2;
		return 0;
}

void start(){
	setcolor(LIGHTMAGENTA);
	setviewport((getmaxx()/3),1,getmaxx()-(getmaxx()/3),upper,0);
	clearviewport();
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,1, "LET");
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,10,"THE SNAKES");
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,20,"TO KILL      ");

	cursor((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2+10,20);
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2+10,20,"1");
	cursor((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2+20,20);
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2+20,20,"2");
	cursor((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2+30,20);
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2+40,20,"GO!");
	setviewport(1,1,getmaxx(),getmaxy(),0);
}
void vga16(void)
{

	 //Set VGA640x480x16
	 int gdriver =9, gmode=mode, errorcode;
	 gdriver = installuserdriver( driver, 0);
	 initgraph(&gdriver, &gmode, "");
	 /* read result of initialization */
	 errorcode = graphresult();
	 if (errorcode != grOk)  /* an error occurred*/
	 {
			printf("Graphics error: %s\n", grapherrormsg(errorcode));
			printf("Press any key to halt:");
			getch();
			exit(1);             /* return with error code*/
	 }
}


void help(){
	setcolor(LIGHTMAGENTA);
	setviewport((getmaxx()/3),1,getmaxx()-(getmaxx()/3),upper,0);
	clearviewport();
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,1, "1§ SNAKE:A,S,D,W");
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,10,"2§ SNAKE:ARROW-KEYS");
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,20,"EXIT[ESC] HELP?[F1]");
	cursor((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,20);
	setviewport(1,1,getmaxx(),getmaxy(),0);
}


int winerscreen(int winer){
	cleardevice();
	char *crap,lp;
	int x,y,midx,midy;
	settextstyle(0,0,6);
	midx=getmaxx()/2;
	midy=getmaxy()/2;
	if(winer==1)
		setcolor(LIGHTRED);
	if(winer==2)
		setcolor(LIGHTBLUE);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	sprintf(crap,"%d§ SNAKE WIN",winer);
	delay(500);
	for(x=0;x<=6;x++){
		cleardevice();
		settextstyle(0,0,x);
		outtextxy(midx,midy,crap);
		delay(30);
	}
	clearbuffer();
	getch();
	setlinestyle(0,0,1);
	setcolor(BLACK);
	for(y=midy-(textheight("")/2);y<midy+(textheight("")/2);y+=2){
		line(midx-(textwidth(crap)/2),y,midx+(textwidth(crap)/2), y);
		delay(30);
	 }
	 for(y=midy+(textheight("")/2)+9;y>=midy-(textheight("")/2)-1;y-=2){
		line(midx-(textwidth(crap)/2),y,midx+(textwidth(crap)/2), y);
		delay(30);
	 }
	settextstyle(0,0,0);
	setcolor(WHITE);
	settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
	outtextxy(getmaxx(),getmaxy(),"PLAY AGAIN?[Y/N] ");
	lp=cursor(getmaxx(),getmaxy());
	if(lp=='N'||lp=='n'){
		return 0;
	}
	return 1;
}

void askforexit(){
	char lp;
	setcolor(LIGHTMAGENTA);
	setviewport((getmaxx()/3),1,getmaxx()-(getmaxx()/3),upper,0);
	clearviewport();
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,1,"EXIT?[Y/N]");
	lp=cursor((getmaxx()-(getmaxx()/3)-(getmaxx()/3))/2,10);
	if(lp=='Y'||lp=='y'){
		finish();
	}
	setviewport(1,1,getmaxx(),getmaxy(),0);

}


char cursor(int x,int y){
	clearbuffer();
	char car,w,*crap;
	int col;
	col=getcolor();
	do{
	for(w=0;w<=3&&!kbhit();w++){
		switch(w){
			case 0:
				car='\\';
				break;
			case 1:
				car='|';
				break;
			case 2:
				car='/';
				break;
			case 3:
				car='-';
				break;
		}
		delay(40);
		sprintf(crap,"%c",car);
		setcolor(getbkcolor());
		outtextxy(x,y,"Û");
		setcolor(col);
		outtextxy(x,y,crap);
	}
	}while(!kbhit());
	setcolor(getbkcolor());
	outtextxy(x,y,"Û");
	setcolor(col);
	return getch();
}
void putitem(int entry){
		static int x,y;
	if(entry==0){
		setcolor(BLACK);
		setlinestyle(SOLID_LINE,1,3);
		rectangle(x,y,x+GREENBLOCK,y+GREENBLOCK);
		x=rand()%(getmaxx()-GREENBLOCK-ori*2)+ori;
		y=(rand()%(getmaxy()-GREENBLOCK-upper-(ori*2)))+upper+ori;
	}
	else if(entry==1){
		x=rand()%(getmaxx()-GREENBLOCK-ori*2)+ori;
		y=(rand()%(getmaxy()-GREENBLOCK-upper-(ori*2)))+upper+ori;
	}
	setcolor(LIGHTGREEN);
	setlinestyle(DASHED_LINE,1,3);
	rectangle(x,y,x+GREENBLOCK,y+GREENBLOCK);
}


void draw(){
	cleardevice();
	setcolor(YELLOW);
	setlinestyle(DASHED_LINE,1,3);
	rectangle(ori-1,upper+ori-1,getmaxx()-ori+1,getmaxy()-ori+1);
}



void player::drawline(){
	int i=len,j=0;
	while(i>=1){
		for(j=0;j<5&&i>=1;j++){
			putpixel(points[i].x,points[i].y,color[j]);
			i--;
		}
	}
	putpixel(points[i].x,points[i].y,0);
}

void player::died(){
	setlinestyle(DASHED_LINE,1,3);
	if(number!=-1){
		for(int i=3;i<=15;i+=2){
			setcolor(YELLOW);
			circle(x,y,i);
			setcolor(LIGHTRED);
			circle(x,y,i-1);
			setcolor(BLACK);
			circle(x,y,i-1);
			circle(x,y,i);
			delay(3);
		}
		putitem(2);
	}
	else{
		for(int i=3;i<=15;i+=2){
			setcolor(WHITE);
			circle(x,y,i);
			setcolor(LIGHTGRAY);
			circle(x,y,i-1);
			setcolor(BLACK);
			circle(x,y,i-1);
			circle(x,y,i);
			delay(3);
		}
	}
}
void player::lived(){
	setlinestyle(DASHED_LINE,1,3);
	for(int i=3;i<=15;i+=2){
		setcolor(LIGHTBLUE);
		circle(x,y,i);
		setcolor(LIGHTMAGENTA);
		circle(x,y,i-1);
		setcolor(BLACK);
		circle(x,y,i-1);
		circle(x,y,i);
		delay(3);
	}
	putitem(2);
}

void player::drawmarkers(){
	char *crap;

	if(number==0){
		settextjustify(LEFT_TEXT,TOP_TEXT);
		setcolor(LIGHTRED);
		setviewport(1,1,getmaxx()/3,upper,1);
		clearviewport();
		outtextxy(1,5,"1§ SNAKE");
		sprintf(crap,"LIVES:%2d",lives);
		outtextxy(1,15,crap);
	}
	else if(number==1){
		settextjustify(RIGHT_TEXT,TOP_TEXT);
		setcolor(LIGHTBLUE);
		setviewport(getmaxx()-(getmaxx()/3),1,getmaxx(),upper,1);
		clearviewport();
		outtextxy(getmaxx()-((getmaxx()/3)*2),5,"2§ SNAKE");
		sprintf(crap,"%2d:LIVES",lives);
		outtextxy(getmaxx()-((getmaxx()/3)*2),15,crap);
	}

	setviewport(1,1,getmaxx(),getmaxy(),0);

}

void player::initall(int n){
	if(n==1){
		number=1;
		initialize();
		color[0]=DARKGRAY;
		color[1]=LIGHTGRAY;
		color[2]=WHITE;
		color[3]=LIGHTGRAY;
		color[4]=DARKGRAY;

		//color[0]=100;//LIGHTMAGENTA;
		//color[1]=101;//LIGHTBLUE;
		//color[2]=102;//LIGHTCYAN;
		//color[3]=103;//LIGHTMAGENTA;
		//color[4]=104;//LIGHTBLUE;
		lives=vidas;
		y=(getmaxy()-upper-(ori*2))/2+upper+ori;
		x=getmaxx()/2+((getmaxx()-(ori*2))/3);
	}
	else if(n==0){
		number=0;
		initialize();
		color[0]=YELLOW;
		color[1]=YELLOW;
		color[2]=LIGHTRED;
		color[3]=RED;
		color[4]=LIGHTRED;
		lives=vidas;
		y=(getmaxy()-upper-(ori*2))/2+upper+ori;
		x=getmaxx()/2-((getmaxx()-(ori*2))/3);
	}
	else if(n==-1){
		number=-1;
		initialize();
		color[0]=DARKGRAY;
		color[1]=LIGHTGRAY;
		color[2]=WHITE;
		color[3]=LIGHTGRAY;
		color[4]=DARKGRAY;
		lives=0;
		y=(getmaxy()/2)+(textheight("")/2)+2;
		x=(getmaxx()/2)-(textwidth("Snakes")/2);

	}
}

void player::initialize(){
	if(number==-1){
		len=0;
		dx=0;
		dy=0;
		grace=1;
		grown=textwidth("Snakes");
	}
	else{
		x=(rand()%(getmaxx()-(ori*2)))+ori;
		y=(rand()%(getmaxy()-(upper+ori*2)))+upper+ori;
		len=0;
		dx=0;
		dy=0;
		grace=gracepoints;
		grown=grownpoints;
	}
}

void player::move(){
	int i;
	if(grace){
		if(grace==1){
			if(number==-1)
				dx=1;
			else if(number==0)
				dx=1;
			else if(number==1)
				dx=-1;
		}
		grace--;
		setlinestyle(0,0,3);
		setcolor(color[1]);
		rectangle(x-2,y-2,x+2,y+2);
		delay(1);
		setcolor(BLACK);
		rectangle(x-2,y-2,x+2,y+2);
		return;
	}

	x+=dx;
	y+=dy;
	if(x<=ori||x>=getmaxx()-ori||y<=upper+ori||y>=getmaxy()-ori){
		lives--;
		drawmarkers();
		died();
		initialize();
		return;
	}
	else if(getpixel(x,y)){
		if(getpixel(x,y)==LIGHTGREEN){
			grown+=grownpergreen;
			putitem(0);
			return;
		}
		else{
			lives--;
			drawmarkers();
			died();
			initialize();
			return;
		}
	}
	if(grown){
		grown--;
		len++;
		if(len>maxlen){
			lives++;
			drawmarkers();
			lived();
			initialize();
			return;
		}
		points[len].x=x;
		points[len].y=y;
	}
	else{
		for(i=0;i<len;i++){
			points[i]=points[i+1];
		}
		points[len].x=x;
		points[len].y=y;
	}
	drawline();
	return;
}
void player::nowto(int rx,int ry){
	if(grace<forcedgrace){
		grace=0;
		if(!(rx&&dx))
			dx=rx;
		if(!(ry&&dy))
			dy=ry;
	}
}
void clearbuffer(void){
	while(kbhit()){
		getch();
	}
}