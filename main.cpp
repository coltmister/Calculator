#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include <string.h>
#include <fstream>
#include <locale.h>

#define CAPACITY 100


typedef struct Lexema
{
	char operatoro;
	int number;
	int error;
	Lexema* next;
}Lexema;

char stack[CAPACITY];
int temp[CAPACITY];
int tos = 0;
const char* operations = "+-/*^()";
const char* numbers = "0123456789";
//Lexema* Parse(char* str);
//void Print(Lexema* head);
//void PrintOPZ(char* input);
//char* Number(char* str);
//int IsNumber(char current);
//int IsOperand(char current);
//void FreeLexema(Lexema* head);
//int Counting(char* item);
//char* OPZ(Lexema* item);
//int DoubleOperators(Lexema* item);


int IsOperand(char current)
{
	for (int i=0;i!=8;i++)
	{
		if (current == operations[i])
			return 1;
	}
	return 0;
}
int IsNumber(char current)
{
	for (int i=0;i!=11;i++)
	{
		if (current == numbers[i])
			return 1;	
	}
	return 0;
}
Lexema* GetLexema(Lexema* current)
{
	if (NULL == current) return NULL;

	return current->next;
}
int GetPriority(char operatoro)
{
	if (operatoro == '^') return 4;
	if (operatoro == '*'||operatoro == '/') return 3;
	if (operatoro == '+'||operatoro == '-') return 2;
	if (operatoro == '('||operatoro == ')') return 1;
}
void FreeLexema(Lexema* head)
{
	if (NULL == head) return;
	Lexema* current = head;
	Lexema* next = head->next;

	while (NULL != current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
char* Number(char* str)
{
	Lexema* head = new Lexema;
	int i=0;
	char tempint[100];

	tempint[i] = str[i];
	i++;

	while (IsNumber(str[i]) == 1)
		{
			tempint[i] = str[i];
			i++;
		}
	return tempint;
		
	
}
Lexema* Parse(char* str)
{
	int length = strlen(str);
	int i = 0;
	Lexema* head = new Lexema;
	head->operatoro = NULL;
	head->number = -1;
	char tempchar = NULL;
	char tempint[100];
	if (str==NULL) {
			printf("Ошибка.Пустая строка\n");
			return NULL;}
	for (i=0;i<length;i++)
	{
		if (IsNumber(str[i])==1)
		{
			strcpy(tempint, Number(str));
			head->number = atoi(tempint);
			return head;
		}
		else if (IsOperand(str[i]) == 1)
		{
			tempchar = str[i];
			head->operatoro = tempchar;
			return head;
		}
		else if (str[i] == ' ') {
			printf("Ошибка. Уберите пробелы\n");
			head->error = 1;
			return head;}
		else {
			printf("Ошибка. Некорректный символ в строке\n");
			head->error = 1;
			return head;}
	}
	return NULL;
}
void Print(Lexema* head)
{
	if (head!=NULL)
	{
		if (head->operatoro!=NULL)
		printf("%c  ", (head->operatoro));
		if (head->number!=-1)
		printf("%d  ", head->number);
	}
}
void pushchar(char i)
{
  if(tos >= 100) {
    printf("Не хватает памяти\n");
    return;
  }
  stack[tos] = i;
  tos++;
}
void pushint(int i)
{
	if(tos >= 100) {
    printf("Не хватает памяти\n");
    return;
  }
  temp[tos] = i;
  tos++;
}
char pop(char* stack)
{
  tos--;
  if(tos < 0)
    return NULL;
  return stack[tos];
}
int popint(int* stack)
{
	tos--;
  if(tos < 0) {
    printf("В стеке ничего нет\n");
    return 0;
  }
  return stack[tos];
}
char* OPZ(Lexema* lexema)
{
	Lexema* item = new Lexema;
	int i = 0;
	int j = 0;
	int m = 0;
	char temp[CAPACITY];
	char result[CAPACITY];
	char operatoro = NULL;

	while (item = lexema) /*Читаем очередной символ.*/
	{

		j=0;
		if (item->number!=-1)
		{
			_itoa(item->number,temp,10);
			for (j = 0;j<=strlen(temp);j++){ /*если символ является числом, добавляем его к выходной строке.*/
				result[i] = temp[j];
				i++;
			}
			i--;
			result[i] = ' ';
			i++;
		}
		else if (item->operatoro != NULL)
		{
			if (item->operatoro == '(')  /*если символ является открывающей скобкой, помещаем его в стек.*/
				pushchar(item->operatoro);
			else if (item->operatoro == ')') /*Если символ является закрывающей скобкой:*/
			{
				operatoro = pop(stack);
				while (operatoro != '(') /*До тех пор, пока верхним элементом стека не станет открывающая скобка,*/
				{
					if(tos < 0) {
					printf("Неправильный порядок скобок\n");
					tos++;
					return NULL;
					}
					result[i] = operatoro; /*выталкиваем элементы из стека в выходную строку.*/
					i++;
					operatoro = pop(stack);
					
				}	
				
			}
			else  
			{
				
				if (tos == 0 || GetPriority(item->operatoro) > GetPriority(stack[tos-1]))/*Если стек пуст, или же приоритет оператора выше - добавляем операторов на вершину стека*/
					pushchar(item->operatoro);
		
				else if (GetPriority(item->operatoro) <= GetPriority(stack[tos-1])) /*приоритет o1 меньше либо равен приоритету оператора, находящегося на вершине стека…*/
					{
						result[i] = pop(stack); /*То добавляем последний оператор из стека в строку с выражением*/
						i++;
						result[i] = ' ';
						i++;
					}
			}

		}
			lexema = lexema->next;
	}
	while (tos != NULL)
	{
		result[i]=pop(stack); /*выталкиваем все символы из стека в выходную строку.*/
		i++;
		result[i] = ' ';
	}
	if (result[i+1]==' ')
		result[i+1] = NULL;
	return result;
}
void PrintOPZ(char* input)
{
	int i=0,m=0;
	while (IsNumber(input[i])==1 || IsOperand(input[i])==1 || input[i] == ' '){
			i++;
			m++;}
	for (i=0 ; i<m ; i++)
	printf("%c",input[i]);
	printf("\n");
}
int Counting(char* item)
{
	int result;
	
	for (int i = 0;i < strlen(item);i++)
	{
		int m = 0;
		if (IsNumber(item[i]) == 1)
		{
			char number[CAPACITY]={0};
			char number1 [CAPACITY] = {0};
			int p=0;
			while (IsNumber(item[i])==1) 
			{
				number[m] = item[i];
				number1[m]=number[m];
				i++;
				m++;
			}
			p = atoi(number1);
			pushint(p);
			
		}
		else if (IsOperand(item[i])==1){
			int a = popint(temp);
			int b = popint(temp);

			if (item[i] =='+'){
				result = b + a;
			    } 
             else if (item[i] =='-'){
				 result = b - a;
				
			     }
             else if (item[i] =='*'){
				 result = b * a;
			 }
             else if (item[i] =='/'){
				 if (a == 0){ printf("Ошибка. Деление на ноль\n");
				 return NULL;}
				 result = b / a; 
			     }
             else if (item[i] =='^'){
				 for (int j = 1;j<item[i+1];j++)
					result = b*b;
				i++;
				i++;
			 }
				pushint(result);
			 }
		else return result;
		}
	
}
int DoubleOperators(Lexema* item)
{
	Lexema* next = NULL;
	while (next = item->next)
	{
		if (item->operatoro!=NULL)
		{
			if (item->operatoro == next->operatoro)
			{
				printf("Ошибка. Повторение операций\n");
				return 1;
			}
			else item = item->next;
		}
		else item = item->next;
	}
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Russian");
	char* str[] = { "134*)136+14*98(", " " ,"12+f+13", "12/0","12+45**4","1+2*2","(333*6)/2",'\0' };
	char* input = NULL;
	char* output = NULL;
	char str1[80];
	Lexema* head = NULL;
	Lexema* current = NULL;
	Lexema* new1 = NULL;
	int i =0;
	start : 
	if (str[i]!=NULL)
	while (new1 = Parse(str[i]))
	{
			if (NULL == current)
			{
				head = new1;
				current = head; 
			}
			else
			{
				current->next = new1;

				current = current->next;
			}
			if (current->operatoro != NULL)
			sprintf(str1,"%c", current->operatoro);
			else sprintf(str1,"%d", current->number);

			str[i] = str[i] + strlen(str1);
			if (current->error == 1)
		{
			i++;
			current = NULL;
			goto start;
		
		}

	}
	if (str[i] == NULL) 
	{
		getchar();
		return 0;
		
	}
	current->next = NULL;
	if (DoubleOperators(head)==1)
	{
		i++;
		current = NULL;
		goto start;
		
	}
	output = OPZ(head);
	if (output == NULL)
	{
		i++;
		goto start;
		
	}
	else
	{
		input = new char[strlen(OPZ(head))];
		strcpy(input,OPZ(head));
		printf("Result:%d\n",Counting(input));
		current = NULL;
		i++;
		goto start;
	}


	getchar();
	return 0;
}
