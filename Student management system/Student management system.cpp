#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct student
{
	int xh;      //學號
	char xm[10]; //姓名
	int xb;      //性別
	int cj_gs;   //高數成績
	int cj_eng;  //英語成績
	int cj_clang;//C語言成績
	int cj_total;//縂成績
	int cj_zh;   //綜合成績
	int cj_lw;   //論文成績
	char dj;     //等級
	int mc_bj;   //班級名次
	int mc_xj;   //校級名次
	char zy[10];     //專業
	int bj = -1;     //班級
	char yj[10];     //研究方向
	char ds[10];     //導師
	struct student* next;//后指针域
	struct student* previous; //前指針域
}graduate, postgraduate;

enum gender {
	m,//0，男性
	f //1，女性
};

student* head_gra = NULL; student* tail_gra = NULL;//本科生头指针与尾指针
student* head_pos = NULL; student* tail_pos = NULL;//研究生头指针与尾指针
int** grade;

void SavetoFile(student* p, FILE* fp);
student* create_linktable(int n, int s);
student* ReadFromFile(FILE* fp);
void menu();
student* insert_func(student* p, int s);
void Insert_Node(student* p, int s);
void Edit_Node(student* p, int s);
void Delete_Node(student* p);
student* Query_Node(student* p, int number);
void Rank_Link(student* p, int s);
int** Analysis_Link(student* p, int s, int** grade);
student* Sort_Link(student* p);

int main()
{
	student* graduate1 = (struct student*)malloc(sizeof(struct student)); student* graduate2 = (struct student*)malloc(sizeof(struct student));
	student* postgraduate1 = (struct student*)malloc(sizeof(struct student)); student* postgraduate2 = (struct student*)malloc(sizeof(struct student));
	FILE* fp1; FILE* fp2;
	errno_t err;
	int n, k = 0;
	int flag = 0;
	while (k != -1)
	{
		printf("請問錄入本科生數據還是研究生數據？\n");
		while (flag != 1)
		{
			flag = 1;
			printf("要錄入本科生數據請按1，研究生數據請按2，結束錄取請按-1：\n");
			scanf_s("%d", &k);
			if (k == 1)
			{
				printf("请问一共有多少本科生？\n");
				scanf_s("%d", &n);
				graduate1 = create_linktable(n, 1);//創建鏈表
				Rank_Link(graduate1, k);//初始化名次
				head_gra = graduate1;

			}
			else if (k == 2)
			{
				printf("请问一共有多少研究生？\n");
				scanf_s("%d", &n);
				postgraduate1 = create_linktable(n, 2);//創建鏈表
				Rank_Link(postgraduate1, k);//初始化名次
				head_pos = postgraduate1;
			}
			else if (k == -1)
			{
				if (head_gra == NULL && head_pos == NULL)
				{
					printf("未輸入任何數據，請重新輸入\n");
				}
				flag = -1;
			}
			else
			{
				printf("請輸入正確的數字！\n");
				flag = -1;
			}
		}
	}

	printf("正在录入数据……\n");
	//錄入過程可寫方程
	if ((err = fopen_s(&fp1, "student.dat", "wb+")) != NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}
	SavetoFile(graduate1, fp1);
	if ((err = fopen_s(&fp2, "student.dat", "wb+")) != NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}
	SavetoFile(postgraduate1, fp2);
	system("pause");
	system("cls");

	printf("正在从文件读取数据到链表，请稍等......\n");
	tail_gra = ReadFromFile(fp1); tail_pos = ReadFromFile(fp2);
	printf("读取成功！共读取%d个本科生，%d個研究生\n", tail_gra->xh, tail_pos->xh);

	menu();

	printf("正在将链表中的数据保存到文件，请稍候……\n");

	if ((err = fopen_s(&fp1, "student.dat", "wb+")) != NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}
	student* tmp1 = head_gra; student* tmp2 = head_pos;
	SavetoFile(tmp1, fp1); SavetoFile(tmp2, fp2);

	printf("保存结束，共保存%d个本科生，%d個研究生\n", tail_gra->xh, tail_pos->xh);
	printf("欢迎再次使用成绩管理键系统，谢谢！\n");
	system("pause");
}
//保存数据到文件
void SavetoFile(student* p, FILE* fp)
{
	errno_t err;
	int k = 0;
	fwrite(p, sizeof(struct student), 1, fp);
	student* tmp = p;
	while (tmp->next)
	{
		tmp = tmp->next;
		k++;
	}
	fclose(fp);
}
//创建新链表并返回头指针
student* create_linktable(int n, int s)
{
	int i, k = 0; int flag;
	char zy[10], bj[10], yj[10], ds[10], xm[10];
	for (i = 0; i < n; i++)
	{
		student* tmp = (struct student*)malloc(sizeof(struct student));
		student* t = (struct student*)malloc(sizeof(struct student));
		if (tmp == NULL)
		{
			printf("分配内存失败！\n");
			exit(0);
		}
		else
		{
			if (s == 1)//本科生錄入
			{
				printf("请录入第%d位学生的姓名、性别（0為男，1為女)、专业、班级代號（成绩为-1则停止录入）：\n", k + 1);
				scanf_s("%s%d%s%s%s", xm, 10, &tmp->xb, zy, 10, &tmp->bj);
				strcpy_s(tmp->xm, 10, xm); strcpy_s(tmp->zy, 10, zy);
				printf("请录入第%d位学生的高數成績、英語成績和C語言成績:\n");
				scanf_s("%d%d%d", &tmp->cj_gs, &tmp->cj_eng, &tmp->cj_clang);
				if (tmp->cj_gs == -1 || tmp->cj_eng == -1 || tmp->cj_clang == -1)
				{
					printf("输入成绩为-1，停止录入！\n");
					break;
				}
				else
				{
					tmp->mc_bj = 1; tmp->mc_xj = 1; tmp->xh = k + 1; tmp->next = NULL;
					if (head_gra == NULL)
					{
						head_gra = tmp; tail_gra = tmp;
						head_gra->previous = NULL;
					}
					else
					{
						while (tail_gra->next)
						{
							tail_gra = tail_gra->next;
						}
						tail_gra->next = tmp;
						t = tail_gra;
						tail_gra = tail_gra->next;
						tail_gra->next = NULL;
						tail_gra->previous = t;
					}
				}
			}
			if (s == 2)//研究生錄入
			{
				printf("请录入第%d位学生的姓名、性别（0為男，1為女)、专业、班级代號、研究方向、导师（成绩为-1则停止录入）：\n", k + 1);
				scanf_s("%s%d%s%s%s%s%s", xm, 10, &tmp->xb, zy, 10, &tmp->bj, yj, 10, ds, 10);
				strcpy_s(tmp->xm, 10, xm); strcpy_s(tmp->zy, 10, zy); strcpy_s(tmp->yj, 10, yj); strcpy_s(tmp->ds, 10, ds);
				printf("请录入第%d位学生的综合成绩、论文成绩:\n");
				scanf_s("%d%d", &tmp->cj_zh, &tmp->cj_lw);
				if (tmp->cj_zh == -1 || tmp->cj_lw == -1)
				{
					printf("输入成绩为-1，停止录入！\n");
					break;
				}
				else
				{
					tmp->mc_bj = 1; tmp->mc_xj = 1; tmp->xh = k + 1; tmp->next = NULL;
					if (head_pos == NULL)
					{
						head_pos = tmp; tail_pos = tmp;
					}
					else
					{
						while (tail_pos->next)
						{
							tail_pos = tail_pos->next;
						}
						tail_pos->next = tmp;
						tail_pos = tail_pos->next; tail_pos->next = NULL;
					}
				}
			}
		}
		k++;//學號的臨時變量
	}

}
//读取文件数据
student* ReadFromFile(FILE* fp)
{
	errno_t err;
	student* tmp = (struct student*)malloc(sizeof(struct student));

	if ((err = fopen_s(&fp, "student.dat", "rb")) != NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}
	fread(tmp, sizeof(struct student), 1, fp);
	while (tmp->next)
	{
		tmp = tmp->next;
	}

	fclose(fp);
	system("pause");
	system("cls");
	return tmp;
}
//主菜单
void menu()
{
	int ch, a, b, c, flag = 0;
	FILE* fp;
	errno_t err;
	student* tmp1 = head_gra;
	student* tmp2 = head_pos;
	while (1)
	{

		printf("             主菜单-本科生成绩管理\n"); printf("   =======================================\n"); printf("   |1-链表维护                 2-计算排名|\n"); printf("   |3-計算排序                 4-查詢|\n"); printf("   |5-統計                0-退出並保存|\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-5）：");//一級菜單
		scanf_s("%d", &ch);
		switch (ch)
		{
		case 1://链表维护
		{
			printf("      二级菜单-链表维护管理\n"); printf("   ===========================\n"); printf("   |    1-批量添加       |\n"); printf("   |    2-修改一个学生信息   |\n"); printf("   |    3-删除一个学生信息   |\n"); printf("   |    4-查询一个学生信息   |\n"); printf("   |    0-返回上级菜单       |\n"); printf("   ===========================\n"); printf("请输入菜单编号（0-4）：");
			scanf_s("%d", &a);
			switch (a)
			{
			case 1://插入一个节点
			{
				int w;
				printf("   ===========================\n"); printf("   |    1-單個添加       |\n"); printf("   |    2-批量添加       |\n"); printf("   ===========================\n");
				scanf_s("%d", &w);
				switch (w)
				{
				case 1://單個添加
				{
					int s;
					printf("要插入本科生還是研究生的信息？");
					printf("1為本科生，2為研究生：");
					scanf_s("%d", &s);
					if (s == 1)
					{
						tmp1->next = insert_func(tmp1, s);
					}
					else tmp2->next = insert_func(tmp2, s);
					system("pause"); system("cls");
				}
				case 2://批量添加
				{
					int s;
					printf("要插入本科生還是研究生的信息？");
					printf("1為本科生，2為研究生：");
					scanf_s("%d", &s);
					if (s == 1) Insert_Node(tmp1, s);
					else Insert_Node(tmp2, s);
					system("pause"); system("cls");
				}
				default:
					break;
				}
				break;
			}
			case 2://查找指定节点并修改节点数据
			{
				int xh, cj, s, o; char xm[10];
				printf("要修改的是本科生還是研究生的信息？\n");
				printf("1為本科生，2為研究生：");
				scanf_s("%d", &s);
				printf("请输入修改学生的学号：");
				scanf_s("%d", &xh);
				//查找指定節點
				if (s == 1)
				{
					tmp1 = Query_Node(tmp1, xh);
					Edit_Node(tmp1, s);
				}
				else
				{
					tmp2 = Query_Node(tmp2, xh);
					Edit_Node(tmp2, s);
				}
				system("pause"); system("cls");
				break;
			}
			case 3://删除指定节点
			{
				student* tmp1 = head_gra;
				student* tmp2 = head_pos;
				int xh, s;
				printf("要修改的是本科生還是研究生的信息？\n");
				printf("1為本科生，2為研究生：");
				scanf_s("%d", &s);
				printf("请输入要删除学生的学号：");
				scanf_s("%d", &xh);
				switch (s)
				{
				case 1:
				{
					tmp1 = Query_Node(tmp1, xh);
					Delete_Node(tmp1);
				}
				case 2:
				{
					tmp2 = Query_Node(tmp2, xh);
					Delete_Node(tmp1);
				}
				default:
					break;
				}
				system("pause"); system("cls");
				break;
			}
			case 4://查詢學號
			{
				student* tmp1 = head_gra;
				int xh, s;
				printf("要修改的是本科生還是研究生的信息？\n");
				printf("1為本科生，2為研究生：");
				scanf_s("%d", &s);
				printf("请输入要查找的学生的学号：");
				scanf_s("%d", &xh);
				switch (s)
				{
				case 1:
				{
					tmp1 = Query_Node(tmp1, xh);
					printf("已找到數據：\n");
					printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績\n");
					printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang);
				}
				case 2:
				{
					tmp2 = Query_Node(tmp2, xh);
					printf("已找到數據：\n");
					printf("学号   姓名      專業    性別   班級   研究方向   導師   綜合成绩   論文成績\n");
					printf("%-5d%-7s%-6s%-5d%-d班   %-7s%-5s%-7d%-7d\n", tmp2->xh, tmp2->xm, tmp2->zy, tmp2->xb, tmp2->bj, tmp2->yj, tmp2->ds, tmp2->cj_zh, tmp2->cj_lw);
				}
				default:
					break;
				}
				system("pause"); system("cls");
				break;
			}
			case 0:
			{
				system("pause"); system("cls");
				break;
			}
			default:
			{
				printf("請輸入正確的指令(0~2)!");
				system("cls");
				break;
			}
			}
			break;
		}
		case 2://计算排名
		{
			int s;
			printf("要計算本科生還是研究生的排名？");
			printf("1為本科生，2為研究生(輸入0來結束計算）：");
			scanf_s("%d", &s);
			switch (s)
			{
			case 1://對本科生排名
			{
				Rank_Link(tmp1, s);
				break;
			}
			case 2://對研究生排名
			{
				Rank_Link(tmp1, s);
				break;
			}
			case 0:
			{
				system("pause"); system("cls");
				break;
			}
			default:
			{
				printf("請輸入正確的指令(0~2)!");
				system("cls");
				break;
			}
			}
		}
		case 3://計算排序
		{
			int s;
			printf("要計算本科生還是研究生的排序？");
			printf("1為本科生，2為研究生(輸入0來結束計算）：");
			scanf_s("%d", &s);
			switch (s)
			{
			case 1://對本科生排名
			{
				tmp1 = Sort_Link(tmp1);
				break;
			}
			case 2://對研究生排名
			{
				tmp1 = Sort_Link(tmp1);
				break;
			}
			case 0:
			{
				system("pause"); system("cls");
				break;
			}
			default:
			{
				printf("請輸入正確的指令(0~2)!");
				system("cls");
				break;
			}
			}
		}
		case 4://查詢
		{
			int s;
			printf("要查詢本科生還是研究生的信息？");
			printf("1為本科生，2為研究生(輸入0來結束計算）：");
			scanf_s("%d", &s);
			switch (s)
			{
			case 1://對本科生排名
			{
				student* p = (struct student*)malloc(sizeof(struct student));
				printf("      二级菜单-本科生排名管理\n"); printf("   ===========================\n"); printf("   |    1-顯示全部學生信息       |\n"); printf("   |    2-顯示某班級學生信息   |\n"); printf("   |    3-查詢學生信息   |\n"); printf("   |    4-查詢某班級不及格學生信息   |\n"); printf("   |    0-返回上级菜单       |\n"); printf("   ===========================\n"); printf("请输入菜单编号（0-4）：");
				scanf_s("%d", &b);
				switch (b)
				{
				case 1://顯示全部學生信息
				{
					int i, j, m, flag = 1, s = -1, k = 0;
					p = head_gra;
					//打印全部數據
					//確定本科生鏈表為非空鏈表
					if (p == NULL)
					{
						printf("目前暫無本科生的信息，請錄入數據後查詢……\n");
						break;
					}
					else
					{
						while (flag != -1)
						{
							printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績   班級名次   校級名次\n");
							while (p && k != 10)
							{
								printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang, tmp1->mc_bj, tmp1->mc_xj);
								p = p->next; k++;
							}
							printf("1.上一頁      2.下一頁      3.首頁     4.最後一頁\n");

							//分頁操作
							while (s == -1)
							{
								student* tmp = p;
								printf("請輸入數字(數字1~4),輸入-1則結束打印數據：");
								scanf_s("%d", &m);
								s = 1;//若正確則退出循環並繼續打印
								switch (m)
								{
								case 1://上一頁
								{

									for (i = 0; i < 10; i++)
									{
										if (tmp == NULL)
										{
											printf("當前已經在首頁了！\n");
											s = -1;
											break;
										}
										else tmp = tmp->previous;
									}
									if (i == 9) p = tmp;
									break;
								}
								case 2://下一頁
								{
									for (i = 0; i < 10; i++)
									{
										if (tmp == NULL)
										{
											printf("當前已經在尾頁了！\n");
											s = -1;
											break;
										}
										else tmp = tmp->next;
									}
									if (i == 9) p = tmp;
									break;
								}
								case 3://首頁
								{
									p = head_gra;//跳轉到鏈表頭節點
									for (i = 0; i < 10; i++)
									{
										if (tmp == NULL)
										{
											printf("當前已經在首頁了！\n");
											s = -1;
											break;
										}
										else tmp = tmp->previous;
										if (i == 9) p = tmp;
									}
								}
								case 4://最後一頁
								{
									p = tail_gra;//跳轉到鏈表尾節點
									j = p->xh % 10; //最後一頁要打印的個數
									for (i = 0; i < j; i++)
									{
										if (tmp == NULL)
										{
											printf("當前已經在尾頁了！\n");
											s = -1;
											break;
										}
										else tmp = tmp->previous;
									}
									if (i == j - 1)p = tmp;
									break;
								}
								case -1:
								{
									flag = -1;//結束打印
									system("pause"); system("cls");
									break;
								}
								default:
								{
									printf("請輸入正確的指令(數字1~4)：\n");
									s = -1;//繼續循環並要求輸入正確的數字
								}
								}
							}

						}
					}
				}
				case 2://顯示某班級學生信息 
				{
					student* temp, * temp_head = (struct student*)malloc(sizeof(struct student));
					temp = NULL; temp_head = NULL;
					int x;
					printf("請輸入班級代號：");
					scanf_s("%d", &x);
					//收集同一個班的信息(可寫成函數）
					while (p != NULL && p->bj == x)
					{
						if (temp == NULL)
						{
							temp = p;
							temp->previous = NULL;
							temp->next = NULL;//初始化temp
							temp_head = temp;//初始化頭節點temp_head
						}
						else
						{
							temp->next = p;
							temp = temp->next;
							temp->previous = p;
							temp->next = NULL;
						}
					}
					printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績   班級名次   校級名次\n");
					while (p)
					{
						printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang, tmp1->mc_bj, tmp1->mc_xj);
						p = p->next;
					}
				}
				case 3://查詢學生信息
				{
					int xh;
					printf("请输入要查找的学生的学号：");
					scanf_s("%d", &xh);
					tmp1 = Query_Node(tmp1, xh);
					printf("已找到數據：\n");
					printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績   班級名次   校級名次\n");
					printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang, tmp1->mc_bj, tmp1->mc_xj);
					break;
				}
				case 4://查詢某個班不及格學生信息
				{
					int d, o, x;
					student* temp, * temp_head, * p = (struct student*)malloc(sizeof(struct student));
					temp_head = NULL; temp = NULL;
					p = tmp1;
					printf("請輸入班級代號：");
					scanf_s("%d", &x);
					while (p != NULL && p->bj == x)
					{
						if (temp_head == NULL)
						{
							temp = p;
							temp->previous = NULL;
							temp->next = NULL;//初始化temp
							temp_head = temp;//初始化頭節點temp_head
						}
						else
						{
							temp->next = p;
							temp = temp->next;
							temp->previous = p;
							temp->next = NULL;
						}
					}
					p = temp_head;
					printf("   =======================================\n"); printf("請選擇想要查找的科目\n"); printf("   |   1-高数成绩      2-英语成绩   |\n"); printf("   |   3-C語言成绩      0-結束查詢   |\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-4）：");
					scanf_s("%d", &o);
					switch (o)
					{
					case 1:
					{
						printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績   班級名次   校級名次\n");
						while (p != NULL && p->cj_gs < 60)
						{
							printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang, tmp1->mc_bj, tmp1->mc_xj);
						}
						break;
					}
					case 2:
					{
						printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績   班級名次   校級名次\n");
						while (p != NULL && p->cj_eng < 60)
						{
							printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang, tmp1->mc_bj, tmp1->mc_xj);
						}
						break;
					}
					case 3:
					{
						printf("学号   姓名      專業    性別   班級   高數成绩   英語成績   C語言成績   班級名次   校級名次\n");
						while (p != NULL && p->cj_clang < 60)
						{
							printf("%-5d%-7s%-6s%-5d%-d班   %-7d%-7d%-9d\n", tmp1->xh, tmp1->xm, tmp1->zy, tmp1->xb, tmp1->bj, tmp1->cj_gs, tmp1->cj_eng, tmp1->cj_clang, tmp1->mc_bj, tmp1->mc_xj);
						}
						break;
					}
					case 0:
					{
						system("pause"); system("cls");
						break;
					}
					default:
					{
						printf("請輸入正確的指令(0~3)!");
						system("cls");
						break;
					}
					}
				}
				default:
				{
					printf("請輸入正確的指令(1~4)!");
					system("cls");
					break;
				}
				}
			}
			case 2://對研究生排名
			{
				student* p = (struct student*)malloc(sizeof(struct student));
				printf("      二级菜单-本科生排名管理\n"); printf("   ===========================\n"); printf("   |    1-顯示全部學生信息       |\n"); printf("   |    2-顯示某班級學生信息   |\n"); printf("   |    3-查詢學生信息   |\n"); printf("   |    4-查詢某班級不及格學生信息   |\n"); printf("   |    0-返回上级菜单       |\n"); printf("   ===========================\n"); printf("请输入菜单编号（0-4）：");
				scanf_s("%d", &b);
				switch (b)
				{
				case 1://顯示全部學生信息
				{
					int i, j, m, flag = 1, s = -1, k = 0;
					p = head_pos;
					//打印全部數據
					//確定研究生為非空鏈表
					if (p == NULL)
					{
						printf("目前暫無研究生的信息，請錄入數據後查詢……\n");
						break;
					}
					else
					{
						while (flag != -1)
						{
							printf("学号   姓名      專業    性別   班級   研究方向   導師   綜合成绩   論文成績   班級名次   學校名次\n");
							while (p && k != 10)
							{
								printf("%-5d%-7s%-6s%-5d%-d班   %-7s%-5s%-7d%-7d%-7d%-7d\n", tmp2->xh, tmp2->xm, tmp2->zy, tmp2->xb, tmp2->bj, tmp2->yj, tmp2->ds, tmp2->cj_zh, tmp2->cj_lw, tmp2->mc_bj, tmp2->mc_xj);
								p = p->next; k++;
							}
							printf("1.上一頁      2.下一頁      3.首頁     4.最後一頁\n");
							printf("請輸入數字(數字1~4),輸入-1則結束打印數據：");
							//分頁操作
							while (s == -1)
							{
								student* tmp = p;
								scanf_s("%d", &m);
								s = 1;//若指令輸入正確則退出循環並繼續打印
								switch (m)
								{
								case 1://上一頁
								{
									for (i = 0; i < 10; i++)
									{
										//判斷該頁是否爲首頁
										if (tmp == NULL)
										{
											printf("當前已經在首頁了！\n");
											s = -1;
											break;
										}
										else tmp = tmp->previous;
									}
									if (i == 9) p = tmp;
									break;
								}
								case 2://下一頁
								{
									for (i = 0; i < 10; i++)
									{
										//判斷該頁是否爲尾頁
										if (tmp == NULL)
										{
											printf("當前已經在尾頁了！\n");
											s = -1;
											break;
										}
										else tmp = tmp->next;
									}
									if (i == 9) p = tmp;
									break;
								}
								case 3://首頁
								{
									p = head_gra;//跳轉到鏈表頭節點
									for (i = 0; i < 10; i++)
									{
										p = p->next;
									}
								}
								case 4://最後一頁
								{
									p = tail_gra;//跳轉到鏈表尾節點
									j = p->xh % 10; //最後一頁要打印的個數
									for (i = 0; i < j; i++)
									{
										p = p->previous;
									}
								}
								default:
								{
									printf("請輸入正確的指令(數字1~4)：\n");
									s = -1;//繼續循環並要求輸入正確的數字
								}
								}
							}

						}
					}
				}
				case 2://顯示某班級學生信息 
				{
					student* temp, * temp_head = (struct student*)malloc(sizeof(struct student));
					temp = NULL; temp_head = NULL;
					int x;
					printf("請輸入班級代號：");
					scanf_s("%d", &x);
					//收集同一個班的信息(可寫成函數）
					while (p != NULL && p->bj == x)
					{
						if (temp_head == NULL)
						{
							temp = p;
							temp->previous = NULL;
							temp->next = NULL;//初始化temp
							temp_head = temp;//初始化頭節點temp_head
						}
						else
						{
							temp->next = p;
							temp = temp->next;
							temp->previous = p;
							temp->next = NULL;
						}
					}
					printf("学号   姓名      專業    性別   班級   研究方向   導師   綜合成绩   論文成績   班級名次   學校名次\n");
					while (p)
					{
						printf("%-5d%-7s%-6s%-5d%-d班   %-7s%-5s%-7d%-7d%-7d%-7d\n", tmp2->xh, tmp2->xm, tmp2->zy, tmp2->xb, tmp2->bj, tmp2->yj, tmp2->ds, tmp2->cj_zh, tmp2->cj_lw, tmp2->mc_bj, tmp2->mc_xj);
						p = p->next;
					}
				}
				case 3://查詢學生信息
				{
					int xh;
					printf("请输入要查找的学生的学号：");
					scanf_s("%d", &xh);
					tmp1 = Query_Node(tmp1, xh);
					printf("已找到數據：\n");
					printf("学号   姓名      專業    性別   班級   研究方向   導師   綜合成绩   論文成績   班級名次   學校名次\n");
					printf("%-5d%-7s%-6s%-5d%-d班   %-7s%-5s%-7d%-7d%-7d%-7d\n", tmp2->xh, tmp2->xm, tmp2->zy, tmp2->xb, tmp2->bj, tmp2->yj, tmp2->ds, tmp2->cj_zh, tmp2->cj_lw, tmp2->mc_bj, tmp2->mc_xj);
					break;
				}
				case 4://查詢某個班不及格學生信息
				{
					int d, o, x;
					student* temp, * temp_head, * p = (struct student*)malloc(sizeof(struct student));
					temp = NULL; temp_head = NULL;
					printf("請輸入班級代號：");
					scanf_s("%d", &x);
					while (p != NULL && p->bj == x)
					{
						if (temp_head == NULL)
						{
							temp = p;
							temp->previous = NULL;
							temp->next = NULL;//初始化temp
							temp_head = temp;//初始化頭節點temp_head
						}
						else
						{
							temp->next = p;
							temp = temp->next;
							temp->previous = p;
							temp->next = NULL;
						}
					}
					p = temp_head;
					printf("   =======================================\n"); printf("請選擇想要查找的科目\n"); printf("   |   1-綜合成绩      2-論文成绩   |\n"); printf("   |   0-結束查詢                |\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-4）：");
					scanf_s("%d", &o);
					switch (o)
					{
					case 1:
					{
						printf("学号   姓名      專業    性別   班級   研究方向   導師   綜合成绩   論文成績   班級名次   學校名次\n");
						while (p != NULL && p->cj_zh < 60)
						{
							printf("%-5d%-7s%-6s%-5d%-d班   %-7s%-5s%-7d%-7d%-7d%-7d\n", tmp2->xh, tmp2->xm, tmp2->zy, tmp2->xb, tmp2->bj, tmp2->yj, tmp2->ds, tmp2->cj_zh, tmp2->cj_lw, tmp2->mc_bj, tmp2->mc_xj);
						}
						break;
					}
					case 2:
					{
						printf("学号   姓名      專業    性別   班級   研究方向   導師   綜合成绩   論文成績   班級名次   學校名次\n");
						while (p != NULL && p->cj_lw < 60)
						{
							printf("%-5d%-7s%-6s%-5d%-d班   %-7s%-5s%-7d%-7d%-7d%-7d\n", tmp2->xh, tmp2->xm, tmp2->zy, tmp2->xb, tmp2->bj, tmp2->yj, tmp2->ds, tmp2->cj_zh, tmp2->cj_lw, tmp2->mc_bj, tmp2->mc_xj);
						}
						break;
					}
					case 0:
					{
						system("pause"); system("cls");
						break;
					}
					default:
					{
						printf("請輸入正確的指令(0~3)!");
						system("cls");
						break;
					}
					}
				}
				default:
				{
					printf("請輸入正確的指令(1~4)!");
					system("cls");
					break;
				}
				}

			}
			case 0:
			{
				system("pause"); system("cls");
				break;
			}
			default:
			{
				printf("請輸入正確的指令(0~2)!");
				system("cls");
				break;
			}
			}
		}
		case 5://統計
		{
			printf("   =======================================\n"); printf("   |1-統計平均成績                  |\n"); printf("   |2-統計某門科目不同等級的學生人數   |\n"); printf("   =======================================\n");
			scanf_s("%d", &c);
			switch (c)
			{
			case 1:
			{
				int total, n, k;
				float average;
				printf("   =======================================\n"); printf("請選擇想要統計的科目\n"); printf("   |   1-高数成绩      2-英语成绩   |\n"); printf("   |   3-C語言成绩      4-綜合成績   |\n"); printf("   |   5-論文成绩      0-結束統計   |\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-5）：");
				scanf_s("%d", &n);
				switch (n)
				{
				case 1://高數
				{
					while (tmp1)
					{
						total += tmp1->cj_gs;
						k++;
					}
					break;
				}
				case 2://英語
				{
					while (tmp1)
					{
						total += tmp1->cj_eng;
						k++;
					}
					break;
				}
				case 3://C語言
				{
					while (tmp1)
					{
						total += tmp1->cj_clang;
						k++;
					}
					break;
				}
				case 4://綜合
				{
					while (tmp2)
					{
						total += tmp2->cj_zh;
						k++;
					}
					break;
				}
				case 5://論文
				{
					while (tmp2)
					{
						total += tmp2->cj_lw;
						k++;
					}
					break;
				}
				case 0://結束
				{
					system("pause"); system("cls");
					break;
				}
				default:
					break;
				}
				average = total / k;
				printf("%.3f", average);
			}
			case 2:
			{
				int n;
				printf("   =======================================\n"); printf("請選擇想要統計的科目\n"); printf("   |   1-高数成绩      2-英语成绩   |\n"); printf("   |   3-C語言成绩      4-綜合成績   |\n"); printf("   |   5-論文成绩      0-結束統計   |\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-5）：");
				scanf_s("%d", &n);
				switch (n)
				{
				case 1:
				{
					printf("等級A人數：%d\n", grade[0][0]);
					printf("等級B人數：%d\n", grade[0][1]);
					printf("等級C人數：%d\n", grade[0][2]);
					printf("等級D人數：%d\n", grade[0][3]);
					printf("等級E人數：%d\n", grade[0][4]);
					break;
				}
				case 2:
				{
					printf("等級A人數：%d\n", grade[1][0]);
					printf("等級B人數：%d\n", grade[1][1]);
					printf("等級C人數：%d\n", grade[1][2]);
					printf("等級D人數：%d\n", grade[1][3]);
					printf("等級E人數：%d\n", grade[1][4]);
					break;
				}
				case 3:
				{
					printf("等級A人數：%d\n", grade[2][0]);
					printf("等級B人數：%d\n", grade[2][1]);
					printf("等級C人數：%d\n", grade[2][2]);
					printf("等級D人數：%d\n", grade[2][3]);
					printf("等級E人數：%d\n", grade[2][4]);
					break;
				}
				case 4:
				{
					printf("等級A人數：%d\n", grade[3][0]);
					printf("等級B人數：%d\n", grade[3][1]);
					printf("等級C人數：%d\n", grade[3][2]);
					printf("等級D人數：%d\n", grade[3][3]);
					printf("等級E人數：%d\n", grade[3][4]);
					break;
				}
				case 5:
				{
					printf("等級A人數：%d\n", grade[4][0]);
					printf("等級B人數：%d\n", grade[4][1]);
					printf("等級C人數：%d\n", grade[4][2]);
					printf("等級D人數：%d\n", grade[4][3]);
					printf("等級E人數：%d\n", grade[4][4]);
					break;
				}
				default:
				{
					system("pause"); system("cls"); break;
				}
				}
			}
			default:
				system("pause"); system("cls"); break;
			}
		}
		case 0:
		{
			flag = -1;
			break;
		}
		default:
		{
			printf("請輸入正確的數字(0~5)");
			system("pause"); system("cls");
			break;
		}
		}
		if (flag == -1) break;
	}
}
//尾插法插入新节点
student* insert_func(student* p, int s)
{
	int t, n = 1, flag = 0;
	char zy[10], bj[10], yj[10], ds[10], xm[10];
	student* temp = (struct student*)malloc(sizeof(struct student));
	while (p)
	{
		p = p->next; n++;
	}
	if (s == 1)
	{
		printf("请录入第%d位学生的姓名、性别（0為男，1為女)、专业、班级代號（成绩为-1则停止录入）：\n", n + 1);
		scanf_s("%s%d%s%s%s", xm, 10, &temp->xb, zy, 10, &temp->bj);
		strcpy_s(temp->xm, 10, xm); strcpy_s(temp->zy, 10, zy);
		printf("请录入第%d位学生的高數成績、英語成績和C語言成績:\n");
		scanf_s("%d%d%d", &temp->cj_gs, &temp->cj_eng, &temp->cj_clang);
		if (temp->cj_gs == -1 || temp->cj_eng == -1 || temp->cj_clang == -1)
		{
			printf("输入成绩为-1，停止录入！\n");
			return NULL;
		}
		else
		{
			temp->xh = n + 1; temp->mc_bj = 1; temp->mc_xj = 1; temp->next = NULL;
			return temp;
		}
	}
	else
	{
		printf("请录入第%d位学生的姓名、性别（0為男，1為女)、专业、班级代號、研究方向、导师（成绩为-1则停止录入）：\n", n + 1);
		scanf_s("%s%d%s%s%s%s%s", xm, 10, &temp->xb, zy, 10, &temp->bj, yj, 10, ds, 10);
		strcpy_s(temp->xm, 10, xm); strcpy_s(temp->zy, 10, zy); strcpy_s(temp->yj, 10, yj); strcpy_s(temp->ds, 10, ds);
		printf("请录入第%d位学生的综合成绩、论文成绩:\n");
		scanf_s("%d%d", &temp->cj_zh, &temp->cj_lw);
		if (temp->cj_zh == -1 || temp->cj_lw == -1)
		{
			printf("输入成绩为-1，停止录入！\n");
			return NULL;
		}
		else
		{
			temp->xh = n + 1; temp->mc_bj = 1; temp->next = NULL;
			return temp;
		}
	}
}
//尾插法依次插入多个节点
void Insert_Node(student* p, int s)
{
	int flag = 0;
	student* temp = (struct student*)malloc(sizeof(struct student));

	while (flag != -1)
	{
		//輸入鏈表爲空
		if (p == NULL)
		{
			int i = 1;
			printf("無數據，將創建新的表\n");
			while (i != -1)
			{
				i = -1;//若正確運行則只運行1次
				p = insert_func(p, s);
				//檢測是否輸入正確數據
				if (p == NULL)
				{
					printf("請輸入非空數據！");
					i = 1;
				}
				else p->previous = NULL;
			}
		}
		else
		{
			temp = p;

			while (p)
			{
				p = p->next;
			}

			p = insert_func(temp, s);

			if (p == NULL) flag = -1;
			else
			{
				temp = p;
				p = p->next;
				p->previous = temp;
			}

		}
	}
}
//修改指定节点
void Edit_Node(student* p, int s)
{
	int n = 0, o;
	int flag = 1;
	int  xb, cj_gs, cj_eng, cj_clang, cj_total, cj_zh, cj_lw;
	char zy[10], bj[10], yj[10], ds[10], xm[10];
	if (p == NULL)
	{
		printf("未录入数据！\n"); exit(0);
	}
	else
	{
		//修改選項
		while (flag != -1)
		{
			if (s == 1)
			{
				printf("请输入要修改的數據：\n");
				printf("   =======================================\n"); printf("   |   1-姓名      2-性别     3-专业     |\n"); printf("   |   4-班级      5-高數成績   6-英語成績   |\n"); printf("   |   7-C語言成績           0-退出   |\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-7）：");
				flag = -1;//輸入正確則循環只運行一次
				scanf_s("%d", &o);
				switch (o)
				{
				case 1://修改姓名
				{
					printf("請輸入姓名：");
					scanf_s("%s", xm, 10);
					strcpy_s(p->xm, 10, xm);
				}
				case 2://修改性別
				{
					printf("請輸入性別：");
					scanf_s("%d", p->xb);
				}
				case 3://修改專業
				{
					printf("請輸入专业：");
					scanf_s("%s", zy, 10);
					strcpy_s(p->zy, 10, zy);
				}
				case 4://修改班級
				{
					printf("請輸入班级：");
					scanf_s("%d", p->bj);
				}
				case 5://修改高數成績
				{
					printf("請輸入高数成绩：");
					scanf_s("%d", p->cj_gs);
				}
				case 6://修改英語成績
				{
					printf("請輸入英语成绩：");
					scanf_s("%d", p->cj_eng);
				}
				case 7://修改C語言成績
				{
					printf("請輸入C语言成绩：");
					scanf_s("%d", p->cj_clang);
				}
				case 0://結束修改
				{
					flag = -1;
				}
				default:
				{
					printf("請輸入正確的數字(0~7)");
					flag = -1;
					system("cls");
				}
				}
			}
			else
			{
				printf("请输入要修改的數據：\n");
				printf("   =======================================\n"); printf("   |   1-姓名      2-性别     3-专业     |\n"); printf("   |   4-班级      5-研究方向   6-導師   |\n"); printf("   |   7-綜合成绩   8-論文成绩   0-退出   |\n"); printf("   =======================================\n"); printf("请输入菜单编号（0-8）：");
				flag = -1;//輸入正確則循環只運行一次
				scanf_s("%d", o);
				switch (o)
				{
				case 1://修改姓名
				{
					printf("請輸入姓名：");
					scanf_s("%s", xm, 10);
					strcpy_s(p->xm, 10, xm);
				}
				case 2://修改性別
				{
					printf("請輸入性別：");
					scanf_s("%d", p->xb);
				}
				case 3://修改專業
				{
					printf("請輸入专业：");
					scanf_s("%s", zy, 10);
					strcpy_s(p->zy, 10, zy);
				}
				case 4://修改班級
				{
					printf("請輸入班级：");
					scanf_s("%d", p->bj);
				}
				case 5://修改研究方向
				{
					printf("請輸入专业：");
					scanf_s("%s", yj, 10);
					strcpy_s(p->yj, 10, yj);
				}
				case 6://修改導師
				{
					printf("請輸入专业：");
					scanf_s("%s", ds, 10);
					strcpy_s(p->ds, 10, ds);
				}
				case 7://修改綜合成績
				{
					printf("請輸入C语言成绩：");
					scanf_s("%d", p->cj_zh);
				}
				case 8://修改論文成績
				{
					printf("請輸入C语言成绩：");
					scanf_s("%d", p->cj_lw);
				}
				case 0://結束修改
				{
					flag = -1;
				}
				default:
				{
					printf("請輸入正確的數字(0~7)");
					flag = -1;
					system("cls");
				}
				}
			}
		}
	}
}
//删除指定节点
void Delete_Node(student* p)
{
	student* tmp1 = head_gra;
	if (p == NULL)
	{
		printf("未录入数据！\n"); exit(0);
	}
	else
	{
		while (tmp1 != p) tmp1 = tmp1->next;
		//所有成績，名次，排名設置爲-1
		tmp1->bj = -1; tmp1->xh = -1; tmp1->xb = -1; tmp1->cj_clang = -1; tmp1->cj_eng = -1; tmp1->cj_gs = -1; tmp1->cj_lw = -1; tmp1->cj_total = -1; tmp1->cj_zh = -1; tmp1->mc_bj = -1; tmp1->mc_xj = -1;
		tmp1 = tmp1->previous;
		tmp1->next = p->next;
	}
}
//查询指定节点并打印查询结果
student* Query_Node(student* p, int number)
{
	while (p != NULL && p->xh != number) p = p->next;
	if (p == NULL)
	{
		printf("未录入数据！\n");
		exit(0);
	}
	else return p;
}
//对链表进行排名
void Rank_Link(student* p, int s)
{
	student* temp, * temp_head = (struct student*)malloc(sizeof(struct student)); student* tmp = p;
	int bj = 1;
	//將p初始化並計算總成績
	while (tmp)
	{
		if (s == 1)
		{
			tmp->mc_bj = 1; tmp->mc_xj = 1; tmp->cj_total = tmp->cj_gs + tmp->cj_eng + tmp->cj_clang;
		}
		else
		{
			tmp->mc_bj = 1; tmp->mc_xj = 1; tmp->cj_total = tmp->cj_lw + tmp->cj_zh;
		}
		tmp = tmp->next;
	}

	if (s == 1)
	{
		//對十個班級學生分別排序
		for (int x = 1; x <= 10; x++)
		{
			temp = NULL; temp_head == NULL;//初始化臨時鏈表
			while (p != NULL && p->bj == x)
			{
				if (temp_head == NULL)
				{
					temp->previous = NULL;
					temp = p;
					temp->next = NULL;//初始化temp
					temp_head = temp;//初始化頭節點temp_head
				}
				else
				{
					temp->next = p;
					temp = temp->next;
					temp->previous = p;
					temp->next = NULL;
				}
			}
			while (temp_head)
			{
				temp = temp_head->next;
				while (temp)
				{
					if (temp_head->cj_total < temp->cj_total)
						temp_head->mc_bj += 1;
					else temp->mc_bj += 1;
					temp = temp->next;
				}
				temp_head = temp_head->next;
			}
			//檢查是否有并列
			while (temp_head)
			{
				temp = temp_head->next;
				if (temp_head->cj_total == temp->cj_total)
				{
					temp_head->mc_bj = temp->mc_bj;
					//將并列學生的後面學生的名次順延一位
					while (temp)
					{
						temp->mc_bj++;
						temp = temp->next;
					}
				}
				temp_head = temp_head->next;
			}
		}
	}
	else
	{
		//對全部學生排序（可構造函數）
		while (p)
		{
			temp = p->next;
			while (temp)
			{
				if (p->cj_total < temp->cj_total)
					p->mc_xj += 1;
				else temp->mc_xj += 1;
				temp = temp->next;
			}
			p = p->next;
		}
	}
}
//计算等级
int** Analysis_Link(student* p, int s, int** grade)
{
	int total = 0;
	//设置C語言等级&统计各等级人数
	while (p)
	{
		if (p->cj_clang >= 90)
		{
			p->dj = 'A'; p = p->next;
			grade[0][0]++;
		}
		else if (p->cj_clang >= 80)
		{
			p->dj = 'B'; p = p->next;
			grade[0][1]++;
		}
		else if (p->cj_clang >= 70)
		{
			p->dj = 'C'; p = p->next;
			grade[0][2]++;
		}
		else if (p->cj_clang >= 60)
		{
			p->dj = 'D'; p = p->next;
			grade[0][3]++;
		}
		else
		{
			p->dj = 'E'; p = p->next;
			grade[0][4]++;
		}
	}
	//设置英語等级&统计各等级人数
	while (p)
	{
		if (p->cj_eng >= 90)
		{
			p->dj = 'A'; p = p->next;
			grade[1][0]++;
		}
		else if (p->cj_eng >= 80)
		{
			p->dj = 'B'; p = p->next;
			grade[1][1]++;
		}
		else if (p->cj_eng >= 70)
		{
			p->dj = 'C'; p = p->next;
			grade[1][2]++;
		}
		else if (p->cj_eng >= 60)
		{
			p->dj = 'D'; p = p->next;
			grade[1][3]++;
		}
		else
		{
			p->dj = 'E'; p = p->next;
			grade[1][4]++;
		}
	}
	//设置高數等级&统计各等级人数
	while (p)
	{
		if (p->cj_gs >= 90)
		{
			p->dj = 'A'; p = p->next;
			grade[2][0]++;
		}
		else if (p->cj_gs >= 80)
		{
			p->dj = 'B'; p = p->next;
			grade[2][1]++;
		}
		else if (p->cj_gs >= 70)
		{
			p->dj = 'C'; p = p->next;
			grade[2][2]++;
		}
		else if (p->cj_gs >= 60)
		{
			p->dj = 'D'; p = p->next;
			grade[2][3]++;
		}
		else
		{
			p->dj = 'E'; p = p->next;
			grade[2][4]++;
		}
	}
	//设置論文等级&统计各等级人数
	while (p)
	{
		if (p->cj_lw >= 90)
		{
			p->dj = 'A'; p = p->next;
			grade[3][0]++;
		}
		else if (p->cj_lw >= 80)
		{
			p->dj = 'B'; p = p->next;
			grade[3][1]++;
		}
		else if (p->cj_lw >= 70)
		{
			p->dj = 'C'; p = p->next;
			grade[3][2]++;
		}
		else if (p->cj_lw >= 60)
		{
			p->dj = 'D'; p = p->next;
			grade[3][3]++;
		}
		else
		{
			p->dj = 'E'; p = p->next;
			grade[3][4]++;
		}
	}
	//设置綜合等级&统计各等级人数
	while (p)
	{
		if (p->cj_zh >= 90)
		{
			p->dj = 'A'; p = p->next;
			grade[4][0]++;
		}
		else if (p->cj_zh >= 80)
		{
			p->dj = 'B'; p = p->next;
			grade[4][1]++;
		}
		else if (p->cj_zh >= 70)
		{
			p->dj = 'C'; p = p->next;
			grade[4][2]++;
		}
		else if (p->cj_zh >= 60)
		{
			p->dj = 'D'; p = p->next;
			grade[4][3]++;
		}
		else
		{
			p->dj = 'E'; p = p->next;
			grade[4][4]++;
		}
	}
	return grade;
}
//冒泡排序法进行班級或校級的学号升序排序或成绩降序排序
student* Sort_Link(student* p)
{
	int ch;
	printf("   =======================================\n"); printf("   |1-顯示班級排名\n"); printf("   |2-顯示全部排名\n"); printf("   |0-退出並保存\n"); printf("   =======================================\n");
	scanf_s("%d", &ch);
	switch (ch)
	{
	case 1:
	{
		/*
		//按學號排序
		student* pb, * pf, * p_head, temp; p_head = p; pf = p;
		if (p->next == NULL) //链表有1个节点
		{
			printf("only one print, needn't order.\n"); exit(0);
		}
		while (pf->next != NULL) {//以pf指向的节点为基准节点
			pb = pf->next;//pb从基准点的下一个节点开始
			while (pb != NULL) {
				if (pf->xh > pb->xh) {
					temp = *pf; *pf = *pb; *pb = temp;//数据域交换
					temp.next = pf->next; pf->next = pb->next; pb->next = temp.next;//指针域交换
				}
				pb = pb->next;
			}
			pf = pf->next;
		}
		return p_head;*/
		student* pb, * pf, * p_head, * temp = NULL;
		student* tmp, * tmp1, * temp_head = (struct student*)malloc(sizeof(struct student));
		tmp = NULL; tmp1 = NULL; temp_head = NULL;
		int x;

		printf("請輸入班級代號：");
		scanf_s("%d", &x);

		while (p != NULL && p->bj == x)
		{
			if (tmp == NULL)
			{
				tmp = p;
				tmp->previous = NULL;
				tmp->next = NULL;//初始化temp
				temp_head = tmp;//初始化頭節點temp_head
			}
			else
			{
				tmp->next = p;
				tmp1 = tmp;
				tmp = tmp->next;
				tmp->previous = tmp1;
				tmp->next = NULL;
			}
		}
		p_head = temp_head; pf = temp_head;
		if (temp_head->next == NULL) //链表有1个节点
		{
			printf("only one print, needn't order.\n");
			exit(0);
		}
		while (pf->next != NULL) {//以pf指向的节点为基准节点
			pb = pf->next;//pb从基准点的下一个节点开始
			while (pb != NULL) {
				if (pf->cj_total < pb->cj_total)
				{
					*temp = *pf; *pf = *pb; *pb = *temp;//数据域交换
				}
				pb = pb->next;
			}
			pf = pf->next;
		}
		return p_head;
	}
	case 2:
	{
		student* pb, * pf, * p_head, temp; p_head = p; pf = p;
		if (p->next == NULL) //链表有1个节点
		{
			printf("only one print, needn't order.\n"); exit(0);
		}
		while (pf->next != NULL) {//以pf指向的节点为基准节点
			pb = pf->next;//pb从基准点的下一个节点开始
			while (pb != NULL) {
				if (pf->cj_total < pb->cj_total)
				{
					temp = *pf; *pf = *pb; *pb = temp;//数据域交换
				}
				pb = pb->next;
			}
			pf = pf->next;
		}
		return p_head;
	}
	default:
	{
		printf("请输入正确的数字！");
		break;
	}
	}
}