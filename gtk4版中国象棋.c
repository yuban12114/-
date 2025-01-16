#include<stdio.h>
#include<stdlib.h>
#include <gtk/gtk.h>
//必要的声明
void move(); 
int ai(int depth, int max_depth);
void rule();
//定义一个链表记录走的历史步数，用于悔棋
typedef struct {
	int last_position[2];
	int now_position[2];
	int beate_chess[2];
	struct past_steps* next;
}past_steps;
//定义状态，选中的棋子的位置，点击棋盘的位置，棋盘以及上面包含的棋子，选中棋子可能的走法，ai给出的走法，当前局势分数（以供ai判断），当前状态（该谁走，也是以供ai判断）
int who, slec[2] = { -1,-1 }, purpose[2], board[9][10][2], may_purpose[18][2], now_step[3] = { 0,0,0 }, sum1,who1=0;
//历史步数的链表头
past_steps* last_p = NULL;
//窗口部件和其下最大的子部件box
GtkWidget* window,*box;
//各棋子在不同地方的分数，以及各棋子自身的分数，以供局势判断
int bing[10][9] = { 0 };//{1, 3, 9, 10, 12, 10, 9, 3, 1,18, 36, 70, 295, 318, 295, 70, 36, 18,15, 28, 60, 273, 280, 273, 60, 28, 15,25, 30, 55, 60, 65, 60, 55, 30, 25,20, 25, 35, 30, 35, 30, 35, 25, 20,15, 0, 30, 0, 8, 0, 30, 0, 15,-1, 0, -3, 0, 100, 0, -3, 0, -1,0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0};
int pao[10][9] = { 0 };//{ 190, 70, 19, 70, 10, 70, 190, 70, 190, 70, 120, 100, 90, 150, 90, 100, 120, 70, 70, 9, 80, 90, 200, 90, 80, 90, 70, 60, 80, 60, 50, 210, 50, 60, 80, 60, 90, 50, 90, 70, 220, 70, 90, 50, 90, 120, 170, 100, 60, 230, 60, 100, 100, 120,10, 30, 10, 30, 120, 30, 10, 30, 10, 30, -20, 30, 20, 70, 90, 30, -20, 30, 30, 10, 30, 30, -10, 30, 30, 10, 30, 20, 20, 20, 20, -10, 20, 20, 20, 20 };
int jiang[10][9] = { 0 };//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9750, 9800, 9750, 0, 0, 0, 0, 0, 0, 9900, 9900, 9900, 0, 0, 0, 0, 0, 0, 10000, 10000, 10000, 0, 0, 0 };
int jv[10][9] = { 0 };//{ 185, 195, 190, 210, 220, 210, 190, 195, 185, 185, 203, 198, 230, 245, 230, 198, 203, 185, 180, 198, 190, 215, 225, 215, 190, 198, 180, 180, 200, 195, 220, 230, 220, 195, 200, 180, 180, 190, 180, 205, 225, 205, 180, 190, 180, 155, 185, 172, 215, 215, 215, 172, 185, 155, 110, 148, 135, 185, 190, 185, 135, 148, 110, 100, 115, 105, 140, 135, 140, 105, 115, 110, 115, 95, 100, 155, 115, 155, 100, 95, 115, 20, 120, 105, 140, 115, 150, 105, 120, 20 };
int ma[10][9] = { 0 };//{ 80, 105, 135, 120, 80, 120, 135, 105, 80, 80, 115, 200, 135, 105, 135, 200, 115, 80, 120, 125, 135, 150, 145, 150, 135, 125, 120, 105, 175, 145, 175, 150, 175, 145, 175, 105, 90, 135, 125, 145, 135, 145, 125, 135, 90, 80, 120, 135, 175, 120, 175, 135, 120, 80, 45, 90, 105, 190, 110, 90, 105, 90, 45,80, 45, 105, 105, 80, 105, 105, 45, 80,20, 45, 80, 80, -10, 80, 80, 45, 20,20, 0, 20, 20, 20, 20, 20, 0, 20 };
int xiangshi[10][9] = { 0 };//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 80, 0, 0, 80, 90, 80, 0, 0, 80, 0, 0, 0, 0, 90, 0, 0, 0, 0, 0, 0, 70, 70, 0, 70, 70, 0, 0 };
int step_score[] = { 989, 439, 210, 226, 65536, 442, 55 };
//判断是否分出胜负
static void judgment() {
	static GtkWidget* image=NULL;
			int p, q;
			int sum = 0;
			if (image) {
				gtk_box_remove(box, image);
				image=0;
			}
			for (p = 0; p < 9; p++) {
				for (q = 0; q < 10; q++) {
					if (board[p][q][0] != 0) {
						switch (board[p][q][1]) {
						case 1:
							if (board[p][q][0] == 1)
								sum += (jv[q][p] + step_score[0]);
							else
								sum -= (jv[9 - q][p] + step_score[0]);
							break;
						case 2:
							if (board[p][q][0] == 1)
								sum += (ma[q][p] + step_score[1]);
							else
								sum -= (ma[9 - q][p] + step_score[1]);
							break;
						case 3:
							if (board[p][q][0] == 1)
								sum += (xiangshi[q][p] + step_score[2]);
							else
								sum -= (xiangshi[9 - q][p] + step_score[2]);
							break;
						case 4:
							if (board[p][q][0] == 1)
								sum += (xiangshi[q][p] + step_score[3]);
							else
								sum -= (xiangshi[9 - q][p] + step_score[3]);
							break;
						case 5:
							if (board[p][q][0] == 1)
								sum += (jiang[q][p] + step_score[4]);
							else
								sum -= (jiang[9 - q][p] + step_score[4]);
							break;
						case 6:
							if (board[p][q][0] == 1)
								sum += (pao[q][p] + step_score[5]);
							else
								sum -= (pao[9 - q][p] + step_score[5]);
							break;
						case 7:
							if (board[p][q][0] == 1)
								sum += (bing[q][p] + step_score[6]);
							else
								sum -= (bing[9 - q][p] + step_score[6]);
							break;
						default:
							break;
						}
					}
				}
			}
		if (sum > 50000) {
			image = gtk_image_new_from_file("./中国象棋文件库/红胜.jpg");
			gtk_widget_set_size_request(image, 500, 500);
			gtk_box_append(GTK_BOX(box), image);
			who = 0;
		}
		else if (sum < -50000) {
			image = gtk_image_new_from_file("./中国象棋文件库/黑胜.jpg");
			gtk_widget_set_size_request(image, 500, 500);
			gtk_box_append(GTK_BOX(box), image);
			who = 0;
		}
}
// 鼠标点击棋盘时时输出点击位置的回调
static void click_cb(GtkGestureClick* gesture, int n_press, double x, double y, GtkWidget* area) {
	purpose[0] = (((int)x) - 50) / 100;
	purpose[1] = ((int)y) / 100;
	if (purpose[1]<10 && purpose[1]>-1 && purpose[0] <9 && purpose[0] >-1 && (board[purpose[0]][purpose[1]][0] == who)) {
		slec[0] = purpose[0];
		slec[1] = purpose[1];
	}
	move();
}
//更新显示的界面
static void display() {
	if (who == 1 || who == -1) {
		GtkWidget* image;
		GtkGesture* click;
		static GtkWidget* grid;
		
		//也可以gtk_widget_unparent(grid);
		gtk_box_remove(box, grid);
		grid = gtk_grid_new();
		gtk_box_append(GTK_BOX(box), grid);

		// 直接从文件创建GtkImage
		//gtk_window_set_child(GTK_WINDOW(window), NULL);
		image = gtk_image_new_from_file("./中国象棋文件库/chinesechessboard.jpg");
		gtk_widget_set_size_request(image, 1000, 1000);
		gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 20, 20);

		int x, y;
		for (y = 0; y < 10; y++) {
			for (x = 0; x < 9; x++) {
				switch (board[x][y][0]) {
				case 0:
					break;
				case 1:
					switch (board[x][y][1]) {
					case 1:
						image = gtk_image_new_from_file("./中国象棋文件库/红车.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 2:
						image = gtk_image_new_from_file("./中国象棋文件库/红马.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 3:
						image = gtk_image_new_from_file("./中国象棋文件库/红相.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 4:
						image = gtk_image_new_from_file("./中国象棋文件库/红士.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 5:
						image = gtk_image_new_from_file("./中国象棋文件库/红帅.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 6:
						image = gtk_image_new_from_file("./中国象棋文件库/红炮.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 7:
						image = gtk_image_new_from_file("./中国象棋文件库/红兵.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					default:
						break;
					}
					break;
				case (-1):
					switch (board[x][y][1]) {
					case 1:
						image = gtk_image_new_from_file("./中国象棋文件库/黑车.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 2:
						image = gtk_image_new_from_file("./中国象棋文件库/黑马.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 3:
						image = gtk_image_new_from_file("./中国象棋文件库/黑象.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 4:
						image = gtk_image_new_from_file("./中国象棋文件库/黑士.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 5:
						image = gtk_image_new_from_file("./中国象棋文件库/黑将.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 6:
						image = gtk_image_new_from_file("./中国象棋文件库/黑炮.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					case 7:
						image = gtk_image_new_from_file("./中国象棋文件库/黑兵.png");
						gtk_widget_set_size_request(image, 100, 100);
						gtk_grid_attach(GTK_GRID(grid), image, x * 2 + 1, y * 2, 2, 2);
						break;
					default:
						break;
					}
				default:
					break;
				}
			}
		}
		if (slec[0] != -1) {
			image = gtk_image_new_from_file("./中国象棋文件库/选中.png");
			gtk_widget_set_size_request(image, 100, 100);
			gtk_grid_attach(GTK_GRID(grid), image, slec[0] * 2 + 1, slec[1] * 2, 2, 2);
		}

		image = gtk_image_new_from_file("./中国象棋文件库/空白.png");
		gtk_widget_set_size_request(image, 1000, 1000);
		gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 20, 20);

		click = gtk_gesture_click_new();
		gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(click), GDK_BUTTON_PRIMARY);
		gtk_widget_add_controller(image, GTK_EVENT_CONTROLLER(click));
		g_signal_connect(click, "pressed", G_CALLBACK(click_cb), image);
		judgment();
	}
}
//定义棋子的移动规则
void rule() {
	if ((slec[1] < 0 || slec[0] < 0 || slec[1] >9 || slec[0] >8) || !board[slec[0]][slec[1]][0]) {
		may_purpose[0][0] = -1;
		return;
	}
	int i = 0, j;
	if (board[slec[0]][slec[1]][1] == 1) {
		for (j = slec[0] + 1; (j < 9) && !board[j][slec[1]][0]; j++) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		if ((j < 9) && (board[slec[0]][slec[1]][0] != board[j][slec[1]][0])) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		for (j = slec[0] - 1; (j > -1) && !board[j][slec[1]][0]; j--) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		if ((j > -1) && (board[slec[0]][slec[1]][0] != board[j][slec[1]][0])) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		for (j = slec[1] + 1; (j < 10) && !board[slec[0]][j][0]; j++) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
		if ((j < 10) && (board[slec[0]][slec[1]][0] != board[slec[0]][j][0])) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
		for (j = slec[1] - 1; (j > -1) && !board[slec[0]][j][0]; j--) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
		if ((j > -1) && (board[slec[0]][slec[1]][0] != board[slec[0]][j][0])) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
	}
	else if (board[slec[0]][slec[1]][1] == 2) {
		j = slec[0] + 2;
		if ((j < 9) && !board[j - 1][slec[1]][0]) {
			if ((slec[1] + 1) < 10 && (board[j][slec[1] + 1][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] + 1;
				i++;
			}
			if ((slec[1] - 1) > -1 && (board[j][slec[1] - 1][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] - 1;
				i++;
			}
		}
		j = slec[0] - 2;
		if ((j > -1) && !board[j + 1][slec[1]][0]) {
			if ((slec[1] + 1) < 10 && (board[j][slec[1] + 1][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] + 1;
				i++;
			}
			if ((slec[1] - 1) > -1 && (board[j][slec[1] - 1][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] - 1;
				i++;
			}
		}
		j = slec[1] + 2;
		if ((j < 10) && !board[slec[0]][j - 1][0]) {
			if ((slec[0] + 1) < 9 && (board[slec[0] + 1][j][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = slec[0] + 1;
				may_purpose[i][1] = j;
				i++;
			}
			if ((slec[0] - 1) > -1 && (board[slec[0] - 1][j][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = slec[0] - 1;
				may_purpose[i][1] = j;
				i++;
			}
		}
		j = slec[1] - 2;
		if ((j > -1) && !board[slec[0]][j + 1][0]) {
			if ((slec[0] + 1) < 9 && (board[slec[0] + 1][j][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = slec[0] + 1;
				may_purpose[i][1] = j;
				i++;
			}
			if ((slec[0] - 1) > -1 && (board[slec[0] - 1][j][0] != board[slec[0]][slec[1]][0])) {
				may_purpose[i][0] = slec[0] - 1;
				may_purpose[i][1] = j;
				i++;
			}
		}
	}
	else if (board[slec[0]][slec[1]][1] == 3) {
		if (board[slec[0]][slec[1]][0] == 1) {
			j = slec[0] + 2;
			if ((slec[1] + 2) < 10 && (j < 9) && !board[j - 1][slec[1] + 1][0]) {
				if (board[j][slec[1] + 2][0] != 1) {
					may_purpose[i][0] = j;
					may_purpose[i][1] = slec[1] + 2;
					i++;
				}
			}
			j = slec[0] - 2;
			if ((slec[1] + 2) < 10 && (j > -1) && !board[j + 1][slec[1] + 1][0]) {
				if (board[j][slec[1] + 2][0] != 1) {
					may_purpose[i][0] = j;
					may_purpose[i][1] = slec[1] + 2;
					i++;
				}
			}
			j = slec[1] - 2;
			if ((slec[0] + 2) < 9 && (j > 4) && !board[slec[0] + 1][j + 1][0]) {
				if (board[slec[0] + 2][j][0] != 1) {
					may_purpose[i][0] = slec[0] + 2;
					may_purpose[i][1] = j;
					i++;
				}
			}
			j = slec[1] - 2;
			if ((slec[0] - 2) > -1 && (j > 4) && !board[slec[0] - 1][j + 1][0]) {
				if (board[slec[0] - 2][j][0] != 1) {
					may_purpose[i][0] = slec[0] - 2;
					may_purpose[i][1] = j;
					i++;
				}
			}
		}
		else if (board[slec[0]][slec[1]][0] == -1) {
			j = slec[0] + 2;
			if ((slec[1] + 2) < 5 && (j < 9) && !board[j - 1][slec[1] + 1][0]) {
				if (board[j][slec[1] + 2][0] != -1) {
					may_purpose[i][0] = j;
					may_purpose[i][1] = slec[1] + 2;
					i++;
				}
			}
			j = slec[0] - 2;
			if ((slec[1] + 2) < 5 && (j > -1) && !board[j + 1][slec[1] + 1][0]) {
				if (board[j][slec[1] + 2][0] != -1) {
					may_purpose[i][0] = j;
					may_purpose[i][1] = slec[1] + 2;
					i++;
				}
			}
			j = slec[1] - 2;
			if ((slec[0] + 2) < 9 && (j > -1) && !board[slec[0] + 1][j + 1][0]) {
				if (board[slec[0] + 2][j][0] != -1) {
					may_purpose[i][0] = slec[0] + 2;
					may_purpose[i][1] = j;
					i++;
				}
			}
			j = slec[1] - 2;
			if ((slec[0] - 2) > -1 && (j > -1) && !board[slec[0] - 1][j + 1][0]) {
				if (board[slec[0] - 2][j][0] != -1) {
					may_purpose[i][0] = slec[0] - 2;
					may_purpose[i][1] = j;
					i++;
				}
			}
		}
	}
	else if (board[slec[0]][slec[1]][1] == 4) {
		if (board[slec[0]][slec[1]][0] == 1) {
			j = slec[0] + 1;
			if ((slec[1] + 1) < 10 && (j < 6) && board[j][slec[1] + 1][0] != 1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] + 1;
				i++;
			}
			j = slec[0] - 1;
			if ((slec[1] + 1) < 10 && (j > 2) && board[j][slec[1] + 1][0] != 1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] + 1;
				i++;
			}
			j = slec[1] - 1;
			if ((slec[0] + 1) < 6 && (j > 6) && board[slec[0] + 1][j][0] != 1) {
				may_purpose[i][0] = slec[0] + 1;
				may_purpose[i][1] = j;
				i++;
			}
			j = slec[1] - 1;
			if ((slec[0] - 1) > 2 && (j > 6) && board[slec[0] - 1][j][0] != 1) {
				may_purpose[i][0] = slec[0] - 1;
				may_purpose[i][1] = j;
				i++;
			}
		}
		else if (board[slec[0]][slec[1]][0] == -1) {
			j = slec[0] + 1;
			if ((slec[1] + 1) < 3 && (j < 6) && board[j][slec[1] + 1][0] != -1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] + 1;
				i++;
			}
			j = slec[0] - 1;
			if ((slec[1] + 1) < 3 && (j > 2) && board[j][slec[1] + 1][0] != -1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1] + 1;
				i++;
			}
			j = slec[1] - 1;
			if ((slec[0] + 1) < 6 && (j > -1) && board[slec[0] + 1][j][0] != -1) {
				may_purpose[i][0] = slec[0] + 1;
				may_purpose[i][1] = j;
				i++;
			}
			j = slec[1] - 1;
			if ((slec[0] - 1) > 2 && (j > -1) && board[slec[0] - 1][j][0] != -1) {
				may_purpose[i][0] = slec[0] - 1;
				may_purpose[i][1] = j;
				i++;
			}
		}
	}
	else if (board[slec[0]][slec[1]][1] == 5) {
		if (board[slec[0]][slec[1]][0] == 1) {
			j = slec[0] + 1;
			if ((j < 6) && board[j][slec[1]][0] != 1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[0] - 1;
			if ((j > 2) && board[j][slec[1]][0] != 1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[1] - 1;
			if ((j > 6) && board[slec[0]][j][0] != 1) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
			j = slec[1] + 1;
			if ((j < 10) && board[slec[0]][j][0] != 1) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
			for (j = slec[1] - 1; (j > -1) && !board[slec[0]][j][0]; j--);
			if ((j > -1) && board[slec[0]][j][1] == 5) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
		}
		else if (board[slec[0]][slec[1]][0] == -1) {
			j = slec[0] + 1;
			if ((j < 6) && board[j][slec[1]][0] != -1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[0] - 1;
			if ((j > 2) && board[j][slec[1]][0] != -1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[1] - 1;
			if ((j > -1) && board[slec[0]][j][0] != -1) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
			j = slec[1] + 1;
			if ((j < 3) && board[slec[0]][j][0] != -1) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
			for (j = slec[1] + 1; (j < 10) && !board[slec[0]][j][0]; j++);
			if ((j < 10) && board[slec[0]][j][1] == 5) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
		}
	}
	else if (board[slec[0]][slec[1]][1] == 6) {
		for (j = slec[0] + 1; (j < 9) && !board[j][slec[1]][0]; j++) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		for (j += 1; (j < 9) && !board[j][slec[1]][0]; j++);
		if ((j < 9) && (board[slec[0]][slec[1]][0] != board[j][slec[1]][0])) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		for (j = slec[0] - 1; (j > -1) && !board[j][slec[1]][0]; j--) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		for (j -= 1; (j > -1) && !board[j][slec[1]][0]; j--);
		if ((j > -1) && (board[slec[0]][slec[1]][0] != board[j][slec[1]][0])) {
			may_purpose[i][0] = j;
			may_purpose[i][1] = slec[1];
			i++;
		}
		for (j = slec[1] + 1; (j < 10) && !board[slec[0]][j][0]; j++) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
		for (j += 1; (j < 10) && !board[slec[0]][j][0]; j++);
		if ((j < 10) && (board[slec[0]][slec[1]][0] != board[slec[0]][j][0])) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
		for (j = slec[1] - 1; (j > -1) && !board[slec[0]][j][0]; j--) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
		for (j -= 1; (j > -1) && !board[slec[0]][j][0]; j--);
		if ((j > -1) && (board[slec[0]][slec[1]][0] != board[slec[0]][j][0])) {
			may_purpose[i][0] = slec[0];
			may_purpose[i][1] = j;
			i++;
		}
	}
	else if (board[slec[0]][slec[1]][1] == 7) {
		if (board[slec[0]][slec[1]][0] == 1) {
			j = slec[0] + 1;
			if (slec[1] < 5 && (j < 10) && board[j][slec[1]][0] != 1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[0] - 1;
			if (slec[1] < 5 && (j > -1) && board[j][slec[1]][0] != 1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[1] - 1;
			if ((j > -1) && board[slec[0]][j][0] != 1) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
		}
		else if (board[slec[0]][slec[1]][0] == -1) {
			j = slec[0] + 1;
			if (slec[1] > 4 && (j < 10) && board[j][slec[1]][0] != -1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[0] - 1;
			if (slec[1] > 4 && (j > -1) && board[j][slec[1]][0] != -1) {
				may_purpose[i][0] = j;
				may_purpose[i][1] = slec[1];
				i++;
			}
			j = slec[1] + 1;
			if ((j < 10) && board[slec[0]][j][0] != -1) {
				may_purpose[i][0] = slec[0];
				may_purpose[i][1] = j;
				i++;
			}
		}
	}
	may_purpose[i][0] = -1;
}
//实现走子并判断是否无棋可走的函数
void move() {
	int i;
	rule();
	for (i = 0; may_purpose[i][0] != -1; i++) {
		if (purpose[0] == may_purpose[i][0] && purpose[1] == may_purpose[i][1]) {
			past_steps* now_p = (past_steps*)malloc(sizeof(past_steps));
			now_p->last_position[0] = slec[0];
			now_p->last_position[1] = slec[1];
			now_p->now_position[0] = purpose[0];
			now_p->now_position[1] = purpose[1];
			now_p->beate_chess[0] = board[purpose[0]][purpose[1]][0];
			now_p->beate_chess[1] = board[purpose[0]][purpose[1]][1];
			now_p->next = last_p;
			last_p = now_p;
			board[purpose[0]][purpose[1]][0] = board[slec[0]][slec[1]][0];
			board[purpose[0]][purpose[1]][1] = board[slec[0]][slec[1]][1];
			board[slec[0]][slec[1]][0] = 0;
			board[slec[0]][slec[1]][1] = 0;
			who *= -1;
			break;
		}
	}
	display();
}
//定义新局
void re_start() {
	int x, y;
	for (x = 0; x < 9; x++) {
		for (y = 0; y < 10; y++) {
			board[x][y][0] = 0;
			board[x][y][1] = 0;
		}
	}
	board[0][0][0] = -1;
	board[0][0][1] = 1;
	board[1][0][0] = -1;
	board[1][0][1] = 2;
	board[2][0][0] = -1;
	board[2][0][1] = 3;
	board[3][0][0] = -1;
	board[3][0][1] = 4;
	board[4][0][0] = -1;
	board[4][0][1] = 5;
	board[5][0][0] = -1;
	board[5][0][1] = 4;
	board[6][0][0] = -1;
	board[6][0][1] = 3;
	board[7][0][0] = -1;
	board[7][0][1] = 2;
	board[8][0][0] = -1;
	board[8][0][1] = 1;
	board[1][2][0] = -1;
	board[1][2][1] = 6;
	board[7][2][0] = -1;
	board[7][2][1] = 6;
	board[0][3][0] = -1;
	board[0][3][1] = 7;
	board[2][3][0] = -1;
	board[2][3][1] = 7;
	board[4][3][0] = -1;
	board[4][3][1] = 7;
	board[6][3][0] = -1;
	board[6][3][1] = 7;
	board[8][3][0] = -1;
	board[8][3][1] = 7;
	board[0][9][0] = 1;
	board[0][9][1] = 1;
	board[1][9][0] = 1;
	board[1][9][1] = 2;
	board[2][9][0] = 1;
	board[2][9][1] = 3;
	board[3][9][0] = 1;
	board[3][9][1] = 4;
	board[4][9][0] = 1;
	board[4][9][1] = 5;
	board[5][9][0] = 1;
	board[5][9][1] = 4;
	board[6][9][0] = 1;
	board[6][9][1] = 3;
	board[7][9][0] = 1;
	board[7][9][1] = 2;
	board[8][9][0] = 1;
	board[8][9][1] = 1;
	board[1][7][0] = 1;
	board[1][7][1] = 6;
	board[7][7][0] = 1;
	board[7][7][1] = 6;
	board[0][6][0] = 1;
	board[0][6][1] = 7;
	board[2][6][0] = 1;
	board[2][6][1] = 7;
	board[4][6][0] = 1;
	board[4][6][1] = 7;
	board[6][6][0] = 1;
	board[6][6][1] = 7;
	board[8][6][0] = 1;
	board[8][6][1] = 7;
	slec[0] = -1;
	who = 1;
	past_steps* i = NULL;
	//g_timeout_add(200, judgment, (who ? 0 : 1));
	while (1) {
		if (!last_p) {
			break;
		}
		i = last_p->next;
		free(last_p);
		last_p = i;
	}
	display();
}
//定义一个悔棋的函数
void huiqi() {
	if (last_p != NULL) {
		board[last_p->last_position[0]][last_p->last_position[1]][0] = board[last_p->now_position[0]][last_p->now_position[1]][0];
		board[last_p->last_position[0]][last_p->last_position[1]][1] = board[last_p->now_position[0]][last_p->now_position[1]][1];
		board[last_p->now_position[0]][last_p->now_position[1]][0] = last_p->beate_chess[0];
		board[last_p->now_position[0]][last_p->now_position[1]][1] = last_p->beate_chess[1];
		who *= -1;
		past_steps* a = last_p->next;
		free(last_p);
		last_p = a;
	}
	display();
}
//遍历自定义的步数之内里的所有走法，如果叶子节点有大于正增长600的走法，那就进行更深层的搜索
int ai(int depth, int max_depth) {
	int i, j, k, best = ((who == 1) ? -100000 : 100000), sum = ((who == 1) ? -100000 : 100000), a[2], may_purpose1[18][2];
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 10; j++) {
			if (board[i][j][0] == who) {
				slec[0] = i;
				slec[1] = j;
				rule();
				for (k = 0; may_purpose[k][0] != -1; k++) {
					may_purpose1[k][0] = may_purpose[k][0];
					may_purpose1[k][1] = may_purpose[k][1];
				}
				may_purpose1[k][0] = -1;
				for (k = 0; may_purpose1[k][0] != -1; k++) {
					if (board[may_purpose1[k][0]][may_purpose1[k][1]][1] == 5) {
						best = who * 99999;
						if (depth == 1) {
							now_step[0] = i;
							now_step[1] = j;
							now_step[2] = k;
						}
						goto count;
					}
					a[0] = board[may_purpose1[k][0]][may_purpose1[k][1]][0];
					a[1] = board[may_purpose1[k][0]][may_purpose1[k][1]][1];
					board[may_purpose1[k][0]][may_purpose1[k][1]][0] = board[i][j][0];
					board[may_purpose1[k][0]][may_purpose1[k][1]][1] = board[i][j][1];
					board[i][j][0] = 0;
					board[i][j][1] = 0;
					who *= -1;
					if (depth == max_depth) {
						int p, q;
						sum = 0;
						for (p = 0; p < 9; p++) {
							for (q = 0; q < 10; q++) {
								if (board[p][q][0] != 0) {
									switch (board[p][q][1]) {
									case 1:
										if (board[p][q][0] == 1)
											sum += (jv[q][p] + step_score[0]);
										else
											sum -= (jv[9 - q][p] + step_score[0]);
										break;
									case 2:
										if (board[p][q][0] == 1)
											sum += (ma[q][p] + step_score[1]);
										else
											sum -= (ma[9 - q][p] + step_score[1]);
										break;
									case 3:
										if (board[p][q][0] == 1)
											sum += (xiangshi[q][p] + step_score[2]);
										else
											sum -= (xiangshi[9 - q][p] + step_score[2]);
										break;
									case 4:
										if (board[p][q][0] == 1)
											sum += (xiangshi[q][p] + step_score[3]);
										else
											sum -= (xiangshi[9 - q][p] + step_score[3]);
										break;
									case 5:
										if (board[p][q][0] == 1)
											sum += (jiang[q][p] + step_score[4]);
										else
											sum -= (jiang[9 - q][p] + step_score[4]);
										break;
									case 6:
										if (board[p][q][0] == 1)
											sum += (pao[q][p] + step_score[5]);
										else
											sum -= (pao[9 - q][p] + step_score[5]);
										break;
									case 7:
										if (board[p][q][0] == 1)
											sum += (bing[q][p] + step_score[6]);
										else
											sum -= (bing[9 - q][p] + step_score[6]);
										break;
									default:
										break;
									}
								}
							}
						}
						if (who == -1 || who == 1) {
							if (max_depth < 200 && ((sum - sum1 > 400) ||sum1 - sum > 400)) {
								int sum2 = sum1;
								sum1 = sum;
								sum = ai(depth + 1, max_depth + 1);
								sum1 = sum2;
							}
						}
					}
					else
						sum = ai(depth + 1, max_depth);
					if ((who == -1 && best < sum) || (who == 1 && best > sum)) {
						best = sum;
						if (depth == 1) {
							now_step[0] = i;
							now_step[1] = j;
							now_step[2] = k;
						}
					}
					who *= -1;
					board[i][j][0] = board[may_purpose1[k][0]][may_purpose1[k][1]][0];
					board[i][j][1] = board[may_purpose1[k][0]][may_purpose1[k][1]][1];
					board[may_purpose1[k][0]][may_purpose1[k][1]][0] = a[0];
					board[may_purpose1[k][0]][may_purpose1[k][1]][1] = a[1];
					slec[0] = i;
					slec[1] = j;
				}
			}
		}
	}
count:
	return best;
}
//局势判断并实现ai走棋
void ai_move(GtkWidget* button, gpointer user_data) {
	int x, y, exp_depth;
	who1 = who;
	sum1 = 0;
	for (x = 0; x < 9; x++) {
		for (y = 0; y < 10; y++) {
			if (board[x][y][0] != 0) {
				switch (board[x][y][1]) {
				case 1:
					if (board[x][y][0] == 1)
						sum1 += (jv[y][x] + step_score[0]);
					else
						sum1 -= (jv[9 - y][x] + step_score[0]);
					break;
				case 2:
					if (board[x][y][0] == 1)
						sum1 += (ma[y][x] + step_score[1]);
					else
						sum1 -= (ma[9 - y][x] + step_score[1]);
					break;
				case 3:
					if (board[x][y][0] == 1)
						sum1 += (xiangshi[y][x] + step_score[2]);
					else
						sum1 -= (xiangshi[9 - y][x] + step_score[2]);
					break;
				case 4:
					if (board[x][y][0] == 1)
						sum1 += (xiangshi[y][x] + step_score[3]);
					else
						sum1 -= (xiangshi[9 - y][x] + step_score[3]);
					break;
				case 5:
					if (board[x][y][0] == 1)
						sum1 += (jiang[y][x] + step_score[4]);
					else
						sum1 -= (jiang[9 - y][x] + step_score[4]);
					break;
				case 6:
					if (board[x][y][0] == 1)
						sum1 += (pao[y][x] + step_score[5]);
					else
						sum1 -= (pao[9 - y][x] + step_score[5]);
					break;
				case 7:
					if (board[x][y][0] == 1)
						sum1 += (bing[y][x] + step_score[6]);
					else
						sum1 -= (bing[9 - y][x] + step_score[6]);
					break;
				default:
					break;
				}
			}
		}
	}
	exp_depth=on_button_clicked(button, user_data);
	ai(1, exp_depth);
	slec[0] = now_step[0];
	slec[1] = now_step[1];
	rule();
	purpose[0] = may_purpose[now_step[2]][0];
	purpose[1] = may_purpose[now_step[2]][1];
	move();
	who1 = 0;
}
//根据文本框的输入，返回ai的深度
static int on_button_clicked(GtkWidget* button, gpointer user_data) {
	GtkWidget* text_view = GTK_WIDGET(user_data);
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	const char* text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
	if (text[0] == 0) {
		return 1;
	}
	int a = (int)text[0] - '0';
	g_free((gchar*)text);
	return a;
}
//信号函数，数据驱动设计，等待回调函数的激活，主界面
static void activate(GtkApplication* app, gpointer user_data) {
	
	GtkWidget* grid2;
	

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "中国象棋");

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_window_set_child(GTK_WINDOW(window), box);

	grid2 = gtk_grid_new();
	gtk_box_append(GTK_BOX(box), grid2);


	GtkWidget* button_re_start = gtk_button_new_with_label("重新开始");
	gtk_grid_attach(GTK_GRID(grid2), button_re_start, 0, 0, 1, 1);
	g_signal_connect(button_re_start, "clicked", G_CALLBACK(re_start), NULL);
	
	GtkWidget* button_huiqi = gtk_button_new_with_label("悔棋");
	gtk_grid_attach(GTK_GRID(grid2), button_huiqi, 0, 1, 1, 1);
	g_signal_connect(button_huiqi, "clicked", G_CALLBACK(huiqi), NULL);
	GtkWidget* button_ai = gtk_button_new_with_label("AI");
	gtk_grid_attach(GTK_GRID(grid2), button_ai, 0, 2, 1, 1);
	GtkWidget* text_view = gtk_text_view_new();
	gtk_widget_set_size_request(text_view, 100, 100);
	gtk_grid_attach(GTK_GRID(grid2), text_view, 0, 3, 1, 1);
	g_signal_connect(button_ai, "clicked", G_CALLBACK(ai_move), text_view);
	display();
	gtk_window_present(GTK_WINDOW(window));
}
//主函数
int main(int argc, char** argv) {
    GtkApplication* app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}