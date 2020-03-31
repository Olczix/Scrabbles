#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include "conio2.h"
#include <time.h>
#include <cstdlib>


#define LEGEND_POSITION_X 1
#define LEGEND_POSITION_Y 1
#define AREA_SIZE 15
#define AREA_X 60//Position of the area 
#define AREA_Y 10 //Position of the area
#define CELL_WIDTH 2
#define LETTERS 100 //the amount of all letters
#define FIELDS AREA_SIZE*AREA_SIZE //the amount of fields
#define WORD_LENGTH 7
#define WSx3 3
#define WSx2 2
#define LSx3 3
#define LSx2 2
#define SUM 53901 //the amount of words in my dictionary


void legend()
{
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y+3);
	cputs("AUTHOR: ALEKSANDRA BARSKA s175862");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y+4);
	cputs("DONE: a,b,c=g, d, e=f ");

	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 6);
	cputs("arrows - press to move");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 7);
	cputs("q - EXIT");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 8);
	cputs("n - new game");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 9);
	cputs("enter - choice and end your move");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 10);
	cputs("esc - cancel the current action");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 11);
	cputs("i - start placing a new word");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y +12);
	cputs("backspace - delete a letter");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 13);
	cputs("o - orientation of the word");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 14);
	cputs("w - change the tiles");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 15);
	cputs("1-7 - select the tile to change");
	/*gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 16 );
	cputs("s - save the game");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 17);
	cputs("l - load the game");
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 18);
	cputs("t - learn mode");*/
}

struct area {
	int point_x; //LOCATED by (0,0) in CONSOLE
	int point_y;
	int area_point_x; //LOCATED by (1,1) in AREA
	int area_point_y;
	char value = ' ';
	int bonus_word_3 = 1;
	int bonus_word_2 = 1;
	int bonus_letter_3 = 1;
	int bonus_letter_2 = 1;
	int color=0;
	bool first_word = false;
	bool value_before = false;

} tab[AREA_SIZE + 1][AREA_SIZE + 1];

struct player {

	char name[20];
	int points = 0;


}player1;

struct EnteredWord
{
	char value = ' '; //entered letter
	int backColor = 0; //black as default color
	int point_x=0;
	int point_y=0;
	bool center = false;
	int area_point_x=1;
	int area_point_y=1;
	int TextColor = 7;
	int conect = 0;
};

struct AvaiableLetters
{
	char value = ' ';
	char InUse  ;
	int Color = 0;
	int Checked = -1; //at the beginning, any letter is checked
}avaiable_letters[WORD_LENGTH];

struct words {
	char word[7];
};

void CursorPosition()
{
	textcolor(7);
	int x = wherex(), y = wherey();
	char txt[50];
	sprintf_s(txt, "CURRENT POSITION:  %d ,  %d  ", x,y);
	gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y+1);
	cputs(txt);
	textcolor(7);
	gotoxy(x, y);
}

void PressTheKey()
{
	gotoxy(AREA_X, AREA_Y - 8);
	cputs("PRESS THE KEY: ");
	//clreol();
}

void RemoveOldInfo()
{
	//REMOVING OLD INFORMATION
	gotoxy(AREA_X, AREA_Y - 9);
	clreol();
	gotoxy(AREA_X, AREA_Y - 8);
	clreol();
	gotoxy(AREA_X, AREA_Y - 7);
	clreol();
	gotoxy(AREA_X, AREA_Y - 6);
	clreol();
	gotoxy(AREA_X, AREA_Y - 5);
	clreol();
	gotoxy(AREA_X, AREA_Y - 4);
	clreol();
	CursorPosition();
	PressTheKey();
}

void print_char_code(int zero,int zn)
{
	char txt[32];
	int x = wherex(), y = wherey();

	if (zero==1) sprintf_s(txt, "KEY CODE: 0x00 0x%0x", zn);
	else sprintf_s(txt, "KEY CODE: 0x%02x", zn);
	gotoxy(LEGEND_POSITION_X,LEGEND_POSITION_Y);
	cputs(txt);
	gotoxy(x, y);

}

void ShowArea()
{
	int a = wherex(), b = wherey();
	int k = 65;
	//ABC.. above the area && 123.. on the left side of the area
	for (int i = 0;i < AREA_SIZE*CELL_WIDTH; i = i + 1 * CELL_WIDTH)
	{
		gotoxy(i + AREA_X, AREA_Y - 2);
		putch(k);
		k++;
	}
	for (int j = 0;j < AREA_SIZE;j++)
	{
		gotoxy(AREA_X - 3, j + AREA_Y);

		k = j + 49;
		if (k < 58) putch(k);

		else
		{
			putch('1');
			gotoxy(AREA_X - 2, j + AREA_Y);
			putch(k - 10);
		}
	}

	gotoxy(AREA_X, AREA_Y);
	for (int i = 1;i <= AREA_SIZE;i++) //ROWS
	{
		//BORDER
		if (i == 1)
		{
			gotoxy(wherex() - 1, wherey() - 1);
			putch('*');
			for (int j = 1;j <= AREA_SIZE * 2;j++) putch('-');
			gotoxy(wherex() - 1, wherey());
			putch('*');

			gotoxy(AREA_X - 1, AREA_Y + AREA_SIZE);
			putch('*');
			for (int j = 1;j <= AREA_SIZE * 2;j++) putch('-');
			gotoxy(wherex() - 1, wherey());
			putch('*');
		}
		//SHOWING THE AREA
		for (int j = 1;j <= AREA_SIZE;j++) // COLUMNS
		{
			gotoxy(tab[i][j].point_x - 1, tab[i][j].point_y);
			putch('|');
			textbackground(tab[i][j].color);
			gotoxy(tab[i][j].point_x, tab[i][j].point_y);
			putch(tab[i][j].value);
			textbackground(BLACK);
		}
		putch('|');
	}

	gotoxy(a, b);
}

struct LettersValue {
	char value;
	int points;
}letters_value[27];

int main() 
{
	int zn = 0, x = AREA_X, y = AREA_Y, attr = 0, back = 0, zero = 0;
	bool ModeNewGame = false;
	int decision;
	int WordNumber = 0;


	//GENERATING AREA FIELDS
	for(int i=1;i<=(AREA_SIZE+1);i=i+1)
	{
		for (int j = 1;j <= AREA_SIZE+1;j++)  //COLUMNS
		{
			tab[i][j].point_x = (j*2 + AREA_X - 2);
			tab[i][j].point_y = (i + AREA_Y - 1);
			tab[i][j].area_point_x = i;
			tab[i][j].area_point_y = j;
			
			//2xLS
				if (i % (AREA_SIZE / 4) == 1 && j<AREA_SIZE / 2 &&  i<AREA_SIZE / 2  && j % (AREA_SIZE / 4) == 1 && i!=j ) //2x LS
				{
				tab[i][j].color = 1;
				tab[AREA_SIZE - i + 1][j].color = 1;
				tab[i][AREA_SIZE - j + 1].color = 1;
				tab[AREA_SIZE - i + 1][AREA_SIZE - j + 1].color =1;

				
				tab[i][j].bonus_letter_2 = LSx2;
				tab[AREA_SIZE - i + 1][j].bonus_letter_2 = LSx2;
				tab[i][AREA_SIZE - j + 1].bonus_letter_2 = LSx2;
				tab[AREA_SIZE - i + 1][AREA_SIZE - j + 1].bonus_letter_2 = LSx2;
				}
			
		 
				if (i % ((AREA_SIZE - 1) / 3) == 2 && j % ((AREA_SIZE - 1) / 3) == 2) //3x LS
				{
					tab[i][j].color = 3;
					tab[i][j].bonus_letter_3 = LSx3;

					if ((i == 2 && j == 2) || (j == AREA_SIZE - 1 && i == AREA_SIZE - 1) ||
						(i == AREA_SIZE - 1 && j == 2) || (j == AREA_SIZE - 1 && i == 2)) //AGAIN 2xWS
						{
							tab[i][j].color = 5;
							tab[i][j].bonus_word_2 = WSx2;
						}
				}


				if (i % (AREA_SIZE / 2) == 1  &&  j % (AREA_SIZE / 2) == 1)//3x WS
				{
					tab[i][j].color = 6;
					tab[i][j].bonus_word_3 = WSx3;
				}
			
				//center
				if (i == AREA_SIZE / 2 + 1 && j == AREA_SIZE / 2 + 1) //CENTER OF THE AREA
				{
		
					tab[i][j].value = 'X';
					tab[i][j].color = 8;
					tab[i][j].first_word = true;
					//2x LS
					tab[AREA_SIZE / 2][AREA_SIZE / 2].color = 1;
					tab[AREA_SIZE / 2][AREA_SIZE / 2 + 2].color = 1;
					tab[AREA_SIZE / 2 + 2][AREA_SIZE / 2 + 2].color = 1;
					tab[AREA_SIZE / 2 + 2][AREA_SIZE / 2].color = 1;

					tab[AREA_SIZE / 2][AREA_SIZE / 2].bonus_letter_2 = LSx2;
					tab[AREA_SIZE / 2][AREA_SIZE / 2 + 2].bonus_letter_2 = LSx2;
					tab[AREA_SIZE / 2 + 2][AREA_SIZE / 2 + 2].bonus_letter_2 = LSx2;
					tab[AREA_SIZE / 2 + 2][AREA_SIZE / 2].bonus_letter_2 = LSx2;
				}

				if(i==j && tab[i][j].color==0 ) //2x WS
				{
					tab[i][j].color = 5;
					tab[i][AREA_SIZE + 1 - j].color = 5;
					tab[AREA_SIZE + 1 - i][j].color = 5;

					tab[i][j].bonus_word_2 = WSx2;
					tab[i][AREA_SIZE + 1 - j].bonus_word_2 = WSx2;
					tab[AREA_SIZE + 1 - i][j].bonus_word_2 = WSx2;
				}
			}
		}

	for (int i = 0;i < 27;i++)
	{
		letters_value[i].value = i + 65;
	}

	//getting letters from file
	FILE *plik;
	plik=fopen("let.txt", "r");
	
	char L;
	char *letters = new char[LETTERS];
	char UsedLetters[LETTERS];
	int N, id = 0;
	
	for (int i = 0;i<27;i++)
	{
		fscanf(plik, "%c %d\n", &L, &N);


		if (N >= 5 && letters_value[i].value == L)
				letters_value[i].points = 1;
		
		if(L == 'L' || L=='S' || L=='U')
			letters_value[i].points = 1;

		if (L == 'D' || L == 'G')
			letters_value[i].points = 2;

		if (L == 'B' || L == 'C' || L == 'M' || L == 'P')
			letters_value[i].points = 3;

		if (L == 'F' || L == 'H' || L == 'V' || L == 'W' || L == 'Y' )
			letters_value[i].points = 4;

		if (L == 'K')
			letters_value[i].points = 5;

		if (L == 'J' || L=='Y')
			letters_value[i].points = 8;

		if (L == 'Q' || L=='Z')
			letters_value[i].points = 10;

		for (int j = 0;j<N;j++)
		{
			letters[id] = L;
			UsedLetters[id] = '+'; //at the beginning all letters are available
			id++;
			gotoxy(id, 1);
		}
	}
	fclose(plik);



	//reading the dictionary
	words *arr = new words[SUM];
	FILE *file;
	file = fopen("dictionary.txt", "r");
	for (int i = 0;i <SUM;i++)
	{
		fscanf(file, "%s", &arr[i].word);
		//gotoxy(40, i + 2);
		//cputs(arr[i].word);
	}
	
	fclose(file);


	//generating player's letters
	srand(time(NULL));
	for (int i = 0;i < WORD_LENGTH;i++)
	{
		avaiable_letters[i].InUse = '-';

		int a = rand() % LETTERS;

		if(avaiable_letters[i].InUse =='-')
			for (int j = 0;j < LETTERS;j++)
				if (j == a && UsedLetters[j] == '+')
				{
					avaiable_letters[i].value = letters[j];
					avaiable_letters[i].InUse = '+';

					UsedLetters[j] = '-'; //this letter won't be available now
				}
	}


#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle("Aleksandra Barska 175862");
	legend(); //we call for function to show up the legend
	textbackground(BLACK);

	do 
	{
		textbackground(BLACK); 
		textcolor(7);
		
		PressTheKey();
		CursorPosition();
		PressTheKey();

		int decision_x = wherex(), decision_y = wherey();

		zn = getche();
		print_char_code(zero, zn);


		if(zn == 0) //if player wants to move around the area without pressing 'n' or 'l'
		{ 
			RemoveOldInfo();

			gotoxy(AREA_X, AREA_Y - 6);
			cputs("PRESS 'n' TO START NEW GAME ");
			gotoxy(AREA_X, AREA_Y - 5);
			cputs("PRESS 'l' TO LOAD CURRENT GAME ");
			
		} 
		 		
		/*else if (zn == 't')
		{
			clrscr();
			x = 3;
			y = 3;
			gotoxy(x,y);
			cputs("'HOW TO PLAY' MODE: \n");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 4);
			cputs("POINT FOR EACH LETTER:");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 5);
			cputs("1 point: E, A, I, O, N, R, T, L, S, U");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 6);
			cputs("2 points: D, G");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 7);
			cputs("3 points: B, C, M, P");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 8);
			cputs("4 points: F, H, V, W, Y");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 9);
			cputs("5 points: K");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 10);
			cputs("8 points: J, X");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 11);
			cputs("10 points: Q, Z");
			gotoxy(LEGEND_POSITION_X, LEGEND_POSITION_Y + 13);
			cputs("HOW TO PLAY ??? "); 
		}*/
		
		else if (zn == 'n') //NEW GAME
		{
			WordNumber = 0;
			RemoveOldInfo();
			PressTheKey();
			putch(zn);
			gotoxy(AREA_X, AREA_Y - 7);
			cputs("ARE YOU SURE YOU WANT TO START A NEW GAME?");
			gotoxy(AREA_X, AREA_Y - 6);
			cputs("[Y - Yes / N - No] -> ");
			CursorPosition();
			decision = getche();
			print_char_code(0,decision);

			while (decision != 'Y' && decision != 'y' && decision != 'N' && decision != 'n')
			{
				gotoxy(wherex() - 1, wherey());
				decision = getche();
			}

			
			if (decision == 'Y' || decision == 'y')
			{
				ModeNewGame = true;
				//Clean the area

				for (int i = 1;i <= AREA_SIZE;i++)
					for (int j = 1;j <= AREA_SIZE;j++)
						tab[i][j].value = ' ';
				
				ShowArea();
			}
			RemoveOldInfo();
		}

		else if (zn == 'i'  ) //NEW WORD
		{
			if (ModeNewGame == true)
			{
				RemoveOldInfo();
				int id, counter=0, sum;
				char current_char;
				
				//generating player's letters
				srand(time(NULL));
				for (int i = 0;i < WORD_LENGTH;i++)
				{
					int a = rand() % LETTERS;

					if (avaiable_letters[i].InUse == '-')
						for (int j = 0;j < LETTERS;j++)
							if (j == a && UsedLetters[j] == '+')
							{
								avaiable_letters[i].value = letters[j];
								avaiable_letters[i].InUse = '+';

								UsedLetters[j] = '-'; //this letter won't be available now
							}
				}

					EnteredWord word[WORD_LENGTH];
					gotoxy(AREA_X, AREA_Y - 6);
					current_char = '0';
					id = 0; //letter's number/id
					sum = 0; //the amount of correct letters
					
					cputs("YOUR LETTERS: ");
					for (int i = 0;i < WORD_LENGTH;i++)
					{
						putch(avaiable_letters[i].value);	
						putch(' ');						
					}
					gotoxy(AREA_X+14, AREA_Y - 5);
					for (int i = 0;i < WORD_LENGTH;i++)
					{
						putch(avaiable_letters[i].InUse);
						putch(' ');
					}
					
									
					gotoxy(AREA_X, AREA_Y - 4);
					cputs("NEW WORD: ");
					for (int i = 0;i < WORD_LENGTH;i++)
					{
						putch(' '); //CLEAN THE LINE 
						word[i].value = ' ';
					}
					gotoxy(wherex() - WORD_LENGTH, wherey());
				
					current_char = getch(); 

					//GETTING a NEW WORD FROM PLAYER
					//the letters will be get as long as:
					//1) ENTER or ESC won't be clicked
					//2) the amount of the letters will be lower than WORD_LENGTH (default: 7)
					while (current_char != 0x0d && current_char != 0x1b && id < WORD_LENGTH)
					{
						CursorPosition();
						print_char_code(0, current_char);

						// if a letter was entered
						if ((current_char >= 'A' && current_char <= 'Z') || current_char=='e')
						{
							for (int i = 0;i < WORD_LENGTH;i++) //we check if we have got entered character 
							{
								//if we find this letter and it's avaiable
								if (current_char == avaiable_letters[i].value && avaiable_letters[i].InUse == '+')
								{
									textbackground(GREEN);
									avaiable_letters[i].InUse = '-'; //the letter is used now
									word[id].value = current_char;
									word[id].backColor = 2; //Green 

									i = WORD_LENGTH; //we go out of the loop
								}

								else //if entered letter wasn't correct
								{
									textbackground(RED);
									word[id].value = current_char;
									word[id].backColor = 4; //Red 
								}
							}
							
							putch(word[id].value); //show the character	

							
							id++; //the letter has been entered, so we go for the next one
						}

						//backspace was entered:
						if (current_char == 0x08)
						{
							if (id>1)
							{
								textbackground(BLACK);

								gotoxy(wherex() - 1, wherey());
								putch(' ');
								gotoxy(wherex() - 1, wherey());

								id = id - 1;
								
								for (int i = 0;i < WORD_LENGTH;i++)
									if (word[id].value == avaiable_letters[i].value 
										&& avaiable_letters[i].InUse == '-' 
										&& word[id].backColor==2)
									{
										avaiable_letters[i].InUse = '+';	// we return the letter  
										i = WORD_LENGTH;				//then we go out of the loop
									}
								
								word[id].value = ' ';
								word[id].backColor = 0;
							}

							else if (id==1)
							{
								textbackground(BLACK);
								gotoxy(wherex() - 1, wherey());
								putch(' ');
								gotoxy(wherex() - 1, wherey());
								id = 0;

								for (int i = 0;i < WORD_LENGTH;i++)
									if (word[id].value == avaiable_letters[i].value 
										&& avaiable_letters[i].InUse == '-' 
										&& word[id].backColor == 2)
										{
											avaiable_letters[i].InUse = '+';	
											i = WORD_LENGTH;				
										}

								word[0].value = ' ';
								word[0].backColor = 0;
							}
					
						}

						//ESC was entered:
						if (current_char == 0x1b)
						{
							RemoveOldInfo();
							current_char = '0';
							WordNumber--;
							//zn = 'i';
						}
						int a = wherex(), b = wherey();
						gotoxy(AREA_X + 14, AREA_Y - 5);
						textbackground(BLACK);
						for (int i = 0;i < WORD_LENGTH;i++)
						{
							putch(avaiable_letters[i].InUse);
							putch(' ');
						}
						gotoxy(a, b);
				
						current_char = getch(); //EVERY NEXT LETTER 
					}

					if (current_char == 0x1b)	//entered letter = ESC (just after 'i')
						RemoveOldInfo();
					

					//at the end: value of 'id' equals to the length of the word
					//so we check if all the letters have been entered correctly:
					counter = 0;
					for (int i = 0;i < id;i++)
						if (word[i].backColor == 2)			//if the backgroundcolor of the letter is correct (green),
							counter++;						//so this letter is correct
	
									/* if value of 'counter' equals to the length of the word ('id'),
										it means that all letters have been entered correctly
										and now we can put the word on our area

										otherwise: the procedure of entering new word will be able again
									*/
					

						if (counter == id)
						{
							int si = 0;
							for (int i = 0;i < SUM;i++)
							{
								si = 0;
								for (int j = 0;j < 7;j++)
								{
									if (arr[i].word[j] == word[j].value && arr[i].word[j] >= 'A' && arr[i].word[j] <= 'Z' && word[j].value != ' ')
									{
										si++;
										int a = wherex(), b = wherey();
										gotoxy(40 + j, 1);
										putch(word[j].value);
										gotoxy(a, b);
									}
								}
								if (si == id) //all letters are compatible
								{
									i = SUM; //we go out of the loop
									int a = wherex(), b = wherey();
									gotoxy(40 , 2);
									cputs("WORD FOUND");
									gotoxy(a, b);
									
								}
							}


							if(si==id)
							{
									bool HorizontalModeOn = true;
									WordNumber++;
									int X = 0, Y = 0;
									//we have entered first word, so it have to go through the center of area (X)
									if (WordNumber == 1)
									{
										x = AREA_X + ((AREA_SIZE / 2)*CELL_WIDTH);
										y = AREA_Y + (AREA_SIZE / 2);
										gotoxy(x, y);

										//putting first word on the area

										for (int k = 0;k < id;k++)
											for (int i = 1;i <= AREA_SIZE;i++)
												for (int j = 1;j <= AREA_SIZE;j++)
													if (tab[i][j].point_x == x + k*CELL_WIDTH  && tab[i][j].point_y == y)
													{
														gotoxy(tab[i][j].point_x, tab[i][j].point_y);
														textbackground(tab[i][j].color);
														putch(word[k].value);
														textbackground(BLACK);

														word[k].point_x = x + k*CELL_WIDTH + X;
														word[k].point_y = y + Y;
													}

										//moving the first word
										gotoxy(wherex() + X, wherey() + Y);

										do {
											do {

												if (current_char != 'o')
												{
													zero = 0;
													current_char = getch();
												}

												if (current_char == 'o')
												{

													if (HorizontalModeOn == true && word[0].point_y <= AREA_Y + AREA_SIZE - id) // bede wpisywa³a pionowo
													{
														HorizontalModeOn = false;

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x   && tab[i][j].point_y == word[k].point_y)
																	{
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(tab[i][j].value);
																		textbackground(BLACK);
																	}


														int NewX = word[0].point_x;
														int NewY = word[0].point_y;
														int NewAreaX = word[0].area_point_x;
														int NewAreaY = word[0].area_point_y;

														for (int k = 1;k < id;k++)
														{

															NewAreaY++;
															word[k].area_point_x = NewAreaX;
															word[k].area_point_y = NewAreaY;

															NewY++;
															word[k].point_x = NewX;
															word[k].point_y = NewY;

														}

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																	{
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(word[k].value);
																		textbackground(BLACK);
																	}
														gotoxy(word[0].point_x, word[0].point_y);
													}

													else if (HorizontalModeOn == false && word[0].point_x <= AREA_X + AREA_SIZE*CELL_WIDTH - id*CELL_WIDTH) //bede wpisywa³a poziomo 
													{
														HorizontalModeOn = true;

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																	{
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(tab[i][j].value);
																		textbackground(BLACK);
																	}

														int NewX = word[0].point_x;
														int NewY = word[0].point_y;
														int NewAreaX = word[0].area_point_x;
														int NewAreaY = word[0].area_point_y;

														for (int k = 1;k < id;k++)
														{

															NewAreaY++;
															word[k].area_point_x = NewAreaX;
															word[k].area_point_y = NewAreaY;

															NewX += CELL_WIDTH;
															word[k].point_x = NewX;
															word[k].point_y = NewY;

														}

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																	{
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(word[k].value);
																		textbackground(BLACK);

																	}
														gotoxy(word[0].point_x, word[0].point_y);
													}

													current_char = 0;
												}


												if (current_char == 0 && current_char != 0x1b) //if it is any "arrow" and it's different from ESC
												{
													int EndX = x + id*CELL_WIDTH;
													int EndY = y + id;
													X = 0; Y = 0;
													print_char_code(1, current_char);
													zero = 1;					// if this is the case then we read
													current_char = getch();		// the next code knowing that this will be a special key
													print_char_code(1, current_char);

													if (HorizontalModeOn == true) // ---
													{
														if (current_char == 0x48 && y > AREA_Y) Y--; //UP
														else if (current_char == 0x50 && y < AREA_Y + AREA_SIZE - 1) Y++; //DOWN
														else if (current_char == 0x4b && x >= AREA_X + CELL_WIDTH) X = X - CELL_WIDTH; //LEFT
														else if (current_char == 0x4d && x < AREA_X + AREA_SIZE*CELL_WIDTH - 1 && EndX < AREA_X + AREA_SIZE*CELL_WIDTH - 1) X = X + CELL_WIDTH; //RIGHT
													}

													else if (HorizontalModeOn == false) //   ||
													{
														if (current_char == 0x48 && y > AREA_Y) Y--; //UP
														else if (current_char == 0x50 && y < AREA_Y + AREA_SIZE - 1 && EndY < AREA_Y + AREA_SIZE) Y++; //DOWN
														else if (current_char == 0x4b && x >= AREA_X + CELL_WIDTH) X = X - CELL_WIDTH; //LEFT
														else if (current_char == 0x4d && x < AREA_X + AREA_SIZE*CELL_WIDTH - CELL_WIDTH) X = X + CELL_WIDTH; //RIGHT
													}


													for (int k = 0;k < id;k++)
														for (int i = 1;i <= AREA_SIZE;i++)
															for (int j = 1;j <= AREA_SIZE;j++)
																if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																{
																	gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																	textbackground(tab[i][j].color);
																	putch(tab[i][j].value);
																	textbackground(BLACK);
																}
													ShowArea();
													x = x + X;
													y = y + Y;

													for (int k = 0;k < id;k++)
													{
														word[k].point_x += X;
														word[k].point_y += Y;
													}


													for (int k = 0;k < id;k++)
														for (int i = 1;i <= AREA_SIZE;i++)
															for (int j = 1;j <= AREA_SIZE;j++)
																if (tab[i][j].point_x == word[k].point_x && tab[i][j].point_y == word[k].point_y)
																{
																	gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																	textbackground(tab[i][j].color);
																	putch(word[k].value);
																	textbackground(BLACK);


																	word[k].area_point_x = j;
																	word[k].area_point_y = i;
																}
												}

												gotoxy(word[0].point_x, word[0].point_y);
												CursorPosition();


											} while (current_char != 0x0d && current_char != 0x1b);
											//do this function till the pressed key code it's
											//different from ENTER or ESC


											if (current_char != 0x1b)
												for (int k = 0;k < id;k++)
												{
													if (word[k].point_x == AREA_X + AREA_SIZE / 2 * CELL_WIDTH && word[k].point_y == AREA_Y + AREA_SIZE / 2)
													{
														tab[AREA_SIZE / 2 + 1][AREA_SIZE / 2 + 1].value = word[k].value;
													}
												}
											else {
												tab[AREA_SIZE / 2 + 1][AREA_SIZE / 2 + 1].value = 'X';
												WordNumber--;
											}

										} while (tab[AREA_SIZE / 2 + 1][AREA_SIZE / 2 + 1].value == ' ' && current_char != 0x1b); //First word goes throught the center tile


										//saving the word on the area
										if (current_char != 0x1b) //ESC wasn't clicked
											for (int k = 0;k < id;k++)
												for (int i = 1;i <= AREA_SIZE;i++)
												{
													for (int j = 1;j <= AREA_SIZE;j++)
													{
														if (tab[i][j].point_x == word[k].point_x && tab[i][j].point_y == word[k].point_y)
														{
															tab[i][j].value = word[k].value;
														}

													}
												}


										ShowArea();

									}

									X = 0, Y = 0;
									//we have enter other words and its not ESC
									if (WordNumber != 1 && current_char != 0x1b)
									{
										x = AREA_X;
										y = AREA_Y;
										gotoxy(x, y);

										for (int k = 0;k < id;k++)
											for (int i = 1;i <= AREA_SIZE;i++)
												for (int j = 1;j <= AREA_SIZE;j++)
													if (tab[i][j].point_x == x + k*CELL_WIDTH  && tab[i][j].point_y == y)
													{

														textcolor(GREEN);
														word[k].TextColor = 2;

														gotoxy(tab[i][j].point_x, tab[i][j].point_y);
														textbackground(tab[i][j].color);
														putch(word[k].value);
														textbackground(BLACK);
														textcolor(7);
														word[k].point_x = x + k*CELL_WIDTH + X;
														word[k].point_y = y + Y;

													}


										gotoxy(wherex() + X, wherey() + Y);
										int ok, conection;
										do {
											do {

												if (current_char != 'o')
												{
													zero = 0;
													current_char = getch();
												}

												if (current_char == 'o')
												{

													if (HorizontalModeOn == true && word[0].point_y <= AREA_Y + AREA_SIZE - id) // bede wpisywa³a pionowo
													{
														HorizontalModeOn = false;

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x   && tab[i][j].point_y == word[k].point_y)
																	{
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(tab[i][j].value);
																		textbackground(BLACK);
																	}


														int NewX = word[0].point_x;
														int NewY = word[0].point_y;
														int NewAreaX = word[0].area_point_x;
														int NewAreaY = word[0].area_point_y;

														for (int k = 1;k < id;k++)
														{

															NewAreaY++;
															word[k].area_point_x = NewAreaX;
															word[k].area_point_y = NewAreaY;

															NewY++;
															word[k].point_x = NewX;
															word[k].point_y = NewY;

														}

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																	{
																		textcolor(word[k].TextColor);
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(word[k].value);
																		textcolor(7);
																		textbackground(BLACK);
																	}
														gotoxy(word[0].point_x, word[0].point_y);
													}

													else if (HorizontalModeOn == false && word[0].point_x <= AREA_X + AREA_SIZE*CELL_WIDTH - id*CELL_WIDTH) //bede wpisywa³a poziomo 
													{
														HorizontalModeOn = true;

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																	{
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(tab[i][j].value);
																		textbackground(BLACK);
																	}

														int NewX = word[0].point_x;
														int NewY = word[0].point_y;
														int NewAreaX = word[0].area_point_x;
														int NewAreaY = word[0].area_point_y;

														for (int k = 1;k < id;k++)
														{

															NewAreaY++;
															word[k].area_point_x = NewAreaX;
															word[k].area_point_y = NewAreaY;

															NewX += CELL_WIDTH;
															word[k].point_x = NewX;
															word[k].point_y = NewY;

														}

														for (int k = 1;k < id;k++)
															for (int i = 1;i <= AREA_SIZE;i++)
																for (int j = 1;j <= AREA_SIZE;j++)
																	if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																	{
																		textcolor(word[k].TextColor);
																		gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																		textbackground(tab[i][j].color);
																		putch(word[k].value);
																		textcolor(7);
																		textbackground(BLACK);

																	}
														gotoxy(word[0].point_x, word[0].point_y);
													}

													current_char = 0;
												}

												if (current_char == 0 && current_char != 0x1b) //if it is any "arrow" and it's different from ESC
												{
													int EndX = x + id*CELL_WIDTH;
													int EndY = y + id;
													X = 0; Y = 0;
													print_char_code(1, current_char);
													zero = 1;					// if this is the case then we read
													current_char = getch();		// the next code knowing that this will be a special key
													print_char_code(1, current_char);

													if (HorizontalModeOn == true) // ---
													{
														if (current_char == 0x48 && y > AREA_Y) Y--; //UP
														else if (current_char == 0x50 && y < AREA_Y + AREA_SIZE - 1) Y++; //DOWN
														else if (current_char == 0x4b && x >= AREA_X + CELL_WIDTH) X = X - CELL_WIDTH; //LEFT
														else if (current_char == 0x4d && x < AREA_X + AREA_SIZE*CELL_WIDTH - 1 && EndX < AREA_X + AREA_SIZE*CELL_WIDTH - 1) X = X + CELL_WIDTH; //RIGHT
													}

													else if (HorizontalModeOn == false) //   ||
													{
														if (current_char == 0x48 && y > AREA_Y) Y--; //UP
														else if (current_char == 0x50 && y < AREA_Y + AREA_SIZE - 1 && EndY < AREA_Y + AREA_SIZE) Y++; //DOWN
														else if (current_char == 0x4b && x >= AREA_X + CELL_WIDTH) X = X - CELL_WIDTH; //LEFT
														else if (current_char == 0x4d && x < AREA_X + AREA_SIZE*CELL_WIDTH - CELL_WIDTH) X = X + CELL_WIDTH; //RIGHT
													}


													for (int k = 0;k < id;k++)
														for (int i = 1;i <= AREA_SIZE;i++)
															for (int j = 1;j <= AREA_SIZE;j++)
																if (tab[i][j].point_x == word[k].point_x  && tab[i][j].point_y == word[k].point_y)
																{
																	word[k].TextColor = 7;
																	word[k].conect = 0;
																	gotoxy(tab[i][j].point_x, tab[i][j].point_y);
																	textbackground(tab[i][j].color);
																	putch(tab[i][j].value);
																	textbackground(BLACK);
																}
													ShowArea();
													x = x + X;
													y = y + Y;

													for (int k = 0;k < id;k++)
													{
														word[k].point_x += X;
														word[k].point_y += Y;
														for (int i = 1;i <= AREA_SIZE;i++)
															for (int j = 1;j <= AREA_SIZE;j++)
																if (tab[i][j].point_x == word[k].point_x && tab[i][j].point_y == word[k].point_y)
																{
																	word[k].area_point_x = j;
																	word[k].area_point_y = i;
																}
													}



													for (int k = 0;k < id;k++)
														for (int i = 1;i <= AREA_SIZE;i++)
															for (int j = 1;j <= AREA_SIZE;j++)
																if (i == word[k].area_point_y && j == word[k].area_point_x)
																{

																	if (tab[i][j].value == word[k].value)
																	{
																		textcolor(2);
																		word[k].TextColor = 2;
																		word[k].conect = 1;

																	}
																	else if (tab[i][j].value == ' ')
																	{
																		textcolor(2);
																		word[k].TextColor = 2;
																	}
																	else if (tab[i][j].value != word[k].value)
																	{
																		textcolor(4);
																		word[k].TextColor = 4;
																		word[k].conect = 0;
																	}

																	gotoxy(word[k].point_x, word[k].point_y);
																	textbackground(tab[i][j].color);
																	putch(word[k].value);
																	textbackground(BLACK);
																	textcolor(7);

																}
												}

												gotoxy(word[0].point_x, word[0].point_y);
												CursorPosition();



											} while (current_char != 0x0d && current_char != 0x1b);
											//do this function till the pressed key code it's
											//different from ENTER or ESC


											ok = 0, conection = 0;
											for (int i = 0;i < id;i++)
											{
												if (word[i].TextColor == 2)
													ok++;

												if (word[i].conect == 1)
													conection++;
											}
											if (current_char == 0x1b) //ESC was pressed
											{
												ok = id;
												conection = 1;
											}
										} while (ok != id || conection == 0); //do while all letters wont be green and there will be unless "one letter" conection


										//saving the word on the area
										if (current_char != 0x1b) //ESC wasn't clicked
											for (int k = 0;k < id;k++)
												for (int i = 1;i <= AREA_SIZE;i++)
													for (int j = 1;j <= AREA_SIZE;j++)
														if (tab[i][j].point_x == word[k].point_x && tab[i][j].point_y == word[k].point_y)
															tab[i][j].value = word[k].value;




										ShowArea();

									}

									RemoveOldInfo();
									//generating player's letters
									srand(time(NULL));
									for (int i = 0;i < WORD_LENGTH;i++)
									{
										int a = rand() % LETTERS;

										if (avaiable_letters[i].InUse == '-')
											for (int j = 0;j < LETTERS;j++)
												if (j == a && UsedLetters[j] == '+')
												{
													avaiable_letters[i].value = letters[j];
													avaiable_letters[i].InUse = '+';
													UsedLetters[j] = '-'; //this letter won't be available now
												}
									}
								}
								else //word isn't in dictionary
								{
									RemoveOldInfo();
									gotoxy(AREA_X, AREA_Y - 5);
									cputs("WORD WASN'T FOUND");
								}
						}

					//if word was entered incorrectly
					else
					{
						RemoveOldInfo();
						gotoxy(AREA_X , AREA_Y-6);
						cputs("WRONGLY ENTERED WORD");
						gotoxy(AREA_X , AREA_Y-5);
						cputs("PRESS 'i' AND TRY AGAIN :)");						
					}
			}

			if (ModeNewGame == false) //if 'n' wasn't pressed before 'i'
			{
				RemoveOldInfo();
				
				gotoxy(AREA_X, AREA_Y - 6);
				textcolor(RED);
				cputs("PRESS 'n' - new game OR 'l' - load game ");
				gotoxy(AREA_X, AREA_Y - 5);
				cputs("BEFORE 'i' - enter new word");
				textcolor(BLACK);
			}
			
			gotoxy(decision_x, decision_y);
		}
	
		/*else if (zn == 'l') //LOAD THE GAME
		{
			RemoveOldInfo();

		}*/
		
		/*else if (zn == 's') //SAVE THE GAME STATE
		{
			RemoveOldInfo();

		}*/

		else if (zn == 'w')//CHANGING THE TILES
		{
			//PressTheKey();
			RemoveOldInfo();
			char current_char;
			//current_char = getche();
			
			gotoxy(AREA_X, AREA_Y - 7);
			cputs("YOUR LETTERS: ");
			gotoxy(AREA_X, AREA_Y - 6);
			for (int i = 0;i < WORD_LENGTH;i++)
			{
				avaiable_letters[i].Color = 0; //BACKGROUNDCOLOR
				avaiable_letters[i].InUse = '+';	
				putch(avaiable_letters[i].value);	
				putch(' ');						
												
			}
			gotoxy(AREA_X , AREA_Y - 5);
			for (int i = 0;i < WORD_LENGTH;i++)
			{
				putch(avaiable_letters[i].InUse);
				putch(' ');
			}
			
			do
			{
				
				gotoxy(AREA_X, AREA_Y - 4);
				cputs("SELECT TILES FOR CHANGE: ");

				current_char = getche();
				CursorPosition();
				print_char_code(0,current_char);

				if (current_char != 'w' && current_char != 0x0d)
				{
					int a = current_char;
					a = a - 48 - 1;
					if (avaiable_letters[a].Checked == 1)
					{
						avaiable_letters[a].Color = 0;
						avaiable_letters[a].InUse = '+';
					}
					else
					{
						avaiable_letters[a].Color = 3;
						avaiable_letters[a].InUse = '-';
					}
					avaiable_letters[a].Checked *= -1;


					gotoxy(AREA_X, AREA_Y - 6);
					for (int i = 0;i < WORD_LENGTH;i++)
					{
						textbackground(avaiable_letters[i].Color);
						putch(avaiable_letters[i].value);

						textbackground(0);
						putch(' ');
					}
					gotoxy(AREA_X, AREA_Y - 5);
					for (int i = 0;i < WORD_LENGTH;i++)
					{
						putch(avaiable_letters[i].InUse);
						putch(' ');
					}
				}							

			} while (current_char != 0x77 && current_char!=0x0d);

			

			//new letters for the player
			srand(time(NULL));
			for (int i = 0;i < WORD_LENGTH;i++)
			{
				for (int j = 0;j < LETTERS;j++)
					if (avaiable_letters[i].value == UsedLetters[j] && UsedLetters[j] == '-')
					{
						UsedLetters[j] = '+'; //we give the letetr back 
					}


				int a = rand() % LETTERS;

				if (avaiable_letters[i].InUse == '-')
					for (int j = 0;j < LETTERS;j++)
						if (j == a && UsedLetters[j] == '+')
						{
							avaiable_letters[i].value = letters[j];
							UsedLetters[j] = '-'; //this letter won't be available now
						}
			}

			RemoveOldInfo();
			gotoxy(AREA_X, AREA_Y - 7);
			cputs("YOUR NEW LETTERS: ");
			gotoxy(AREA_X, AREA_Y - 6);
			for (int i = 0;i < WORD_LENGTH;i++)
			{
				avaiable_letters[i].Color = 0; //BACKGROUNDCOLOR
				 //first row is for the letters
				avaiable_letters[i].InUse = '+';	//second row is for computer information (default: space)
				putch(avaiable_letters[i].value);	//if the value is '+' -> the letter is avaiable
				putch(' ');						//if the value is '-' -> it will mean that the letter has been 
												//already used in entered word
			}
			gotoxy(AREA_X, AREA_Y - 5);
			for (int i = 0;i < WORD_LENGTH;i++)
			{
				putch(avaiable_letters[i].InUse);
				putch(' ');
			}


		}

		else if (zn == 'q')
		{
			//CursorPosition();
			textcolor(RED);
			gotoxy(AREA_X, AREA_Y - 7);
			cputs("ARE YOU SURE YOU WANT TO QUIT THE GAME? ");
			gotoxy(AREA_X, AREA_Y - 6);
			cputs("[Y - Yes / N - No] -> ");
			
			CursorPosition();
			textcolor(BLACK);
			
			
			decision = getche();
			print_char_code(0, decision);

			while (decision != 'Y' && decision != 'y' && decision != 'N' && decision != 'n')
			{ //we get the char as long as it will be 'Y' or 'y' or 'N' or 'n'
				gotoxy(wherex() - 1, wherey());
				decision = getche();
			}
			
			if(decision=='n' || decision=='N') //player decided not to end the game
			{
				RemoveOldInfo();
				gotoxy(decision_x, decision_y);
				zn = '0';
			} 
			//otherwise: Program ends and the game progress will be lost	
			else { zn = 'q'; }
		}

	} while (zn != 'q'); //pressing 'q' ends the program
	
	
	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be visible after the program ends
	return 0;
}
