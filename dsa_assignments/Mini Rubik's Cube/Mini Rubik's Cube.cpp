#include<iostream>
#include<ctime>
#include<windows.h>
using namespace std;

//��������-------------------------------------------------------------------------------------
void turn(int i);
//��now0����ִ��ĳ��ת���󸳸�now
void uzip(int i);
//��״̬����i��ѹ����now0����
int zip();
//��now����ѹ��������״̬����
void pret();
//now0����now
void pre();
//��ʼ��������prePos��Ȼ������TurnPos��TurnOri����
void prePos(int);
//��ʼ��ȫ���У�����������λ����Ķ�Ӧ����
void output(int, int);
//���ɲ�������
int input();
//���벢�ж��Ƿ�Ϸ����ɽ�
int search(int i, char mode);
//����״̬����Ϊi��״̬����mode=0����������
int searchall();
//������������״̬
void count();
//ͳ�����ж���״̬

//��������---------------------------------------------------------------------------------------
const int T = 5;
//��״ͼ�����ܶ�
const int MAX_CUBE = 3674160;
//��״̬��
const int MAX_POS = 5040;
//��λ��״̬��
const int MAX_ORI = 729;
//�ܷ���״̬��

//ȫ�ֱ�������------------------------------------------------------------------------------------
unsigned char ht[MAX_CUBE];
//3588K|��Ϊ���֣�
//ht[i]/128���֣�Ϊ��ʾ��Ŀ�����ɵ�״̬�������ʾ�ӻ�ԭ̬���ɵ�״̬
//ht[i]%128%13���֣���״̬���뻹ԭ̬�ľ��룬����ԭ̬����ʾ����Զ����δ������
//ht[i]%128/13���֣���ʾ�ﵽ��״̬ǰ��ת������һ������
int Queue[MAX_CUBE];
//14352K|��������
int now[7 + 7] = { 0,1,2,3,4,5,6,0,0,0,0,0,0,0 }, now0[7 + 7];
//ǰλ��ʾĳλ���ϵĿ�ԭ�����ĸ�����λ��ʾ��ĳ��λ�õĿ�ķ���
//now0��ʾ��ǰ������չ�Ľڵ㣬now��ʾ��ǰ�������ȴ����صĽڵ�
int z, z0;
//now�����ѹ������,now0�����ѹ������
bool PosNumUsed[7];
//prePos�����б�ʾĳ�����Ƿ�ʹ�ù�
unsigned char Possearch[7];
//ö��ĳ������
unsigned char Num_to_Pos[MAX_POS][7];
//34.5K|Num_to_Pos[i]����ĸ�Ԫ�ر�ʾ��i�����е���Ӧ����
short int Pos_to_Num[7][7][7][7][7][7];
//115K|ĳ�������������������ŵ�λ�ã�����
short int TurnPos[MAX_POS][9];
//88.6K|���ؽ�λ��״̬iת��j���λ��״̬
short int TurnOri[MAX_ORI][9];
//12.8K|���ؽ�����״̬iת��j��ķ���״̬
int p1 = 0, p2 = 0;
//ָ�룬�ֱ�ָ����ס���β
int ziptot = 0;
//ͳ�Ƶ�ǰһ��ö�ټ���ȫ����
int main()
{
	//Ϊ�˾�ȷ��ʱ����������ʹ���˵���CPU��Ƶ���ķ���
	LARGE_INTEGER BegainTime;
	LARGE_INTEGER EndTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);//��ȡCPU��Ƶ
	QueryPerformanceCounter(&BegainTime);//�趨��ʱ��ʼ
	pre();//��ʼ��
	QueryPerformanceCounter(&EndTime);//�趨��ʱ����
	cout << "��ʼ��ʱ�䣺" << (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000 << "ms" << endl;
	//�����ʼ��ʱ��
	while (1)
	{
		cout << "�����빤��ģʽ��" << endl << "0.����ģʽ\t1.�����ħ��\t2.�˳�\t[ ]\b\b";
		char mode;
		cin >> mode;//���빤��ģʽ
		cout << endl;
		if (mode == '0')
		{//���ѡ���˲���ģʽ
			char testmode;
			while (1)
			{
				cout << "��ָ��������Ŀ��" << endl << "0.������������״̬\t1.��������ħ���� \
					ʱ\t2.����\t[]\b\b";
				cin >> testmode;//���������Ŀ
				cout << endl;
				if (testmode == '0')
				{//������Ŀ=������������״̬
					searchall();//ִ��������ͳ�ƽ��
					system("pause");//��ͣ�ȴ�
					continue;//����ִ�е����������Ŀ����
				}
				else if (testmode == '1')
				{//������Ŀ=��������ħ����ʱ
					unsigned int num, i;//num��¼���Ը���
					int p20, p10;//��¼��ʱ���ʱ���������
					cout << "���Ը�����"; cin >> num;//������Ը���
					double alltime, maxtime, mintime;//�ֱ��ʾ��������ʱ�䣬�ʱ�䣬���ʱ��
					alltime = 0; maxtime = 0; mintime = 1000;//��ʼ��ʱ��
					int *timetable = NULL;
					while (timetable == NULL)timetable = new int[1000];
					//����ʱ��ͳ������
					int maxtable; maxtable = 0;//���ڼ�¼timetable������Ԫ���Է�����ʾ
					for (i = 0; i < 1000; i++)timetable[i] = 0;//���timetable
					for (i = 0; i<num; i++)
					{
						srand(unsigned(clock()));//��ʼ���������
						QueryPerformanceCounter(&BegainTime);//��¼��ʼʱ��

						search(unsigned int(rand() % MAX_CUBE), mode);//�������ĳ��
						QueryPerformanceCounter(&EndTime);//��¼����ʱ��
						alltime += (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000; //����������ʱ��
						timetable[int((double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000 * T)]++; //����ǰ��ʱֵ����ͳ�Ʊ�
						if ((double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000>maxtime)
						{//������κ�ʱ������ԭ�ʱ��ֵ����¼������������ͬʱˢ���ʱ��ֵ
							p10 = p1;
							p20 = p2;
							maxtime = (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000;
						}
						if ((double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000 < mintime)
						{//ˢ����Сʱ��ֵ
							mintime = (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000;
						}
						if (i % 100 == 99)
						{//����ѽ�ħ�����������ʱ��������%���ҵ�ϵͳƫ��
							cout << "�Ѳ���" << i + 1 << "��\tƽ��" << alltime / (i + 1) << "ms\
								��" << maxtime << "ms(" << p10 << " / " << p20 << ")\t" << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b \
								\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b \
								\b\b\b\b\b";
						}
					}
					//�����������ͳ�ƽ����ʱ��ֲ�
					cout << "�Ѳ��ԣ�" << i << "��ƽ��ÿ��" << alltime / i << "ms ƽ��ÿ \
						�룺" << 1000 * i / alltime << "�������ʱ" << maxtime << "ms" << endl;
					cout << "ʱ��ֲ���" << endl;
					for (i = (int)(mintime*T); i < maxtime*T; i++)
					{
						if (timetable[i] > maxtable)maxtable = timetable[i];
					}
					int j;
					if (maxtime > 10)maxtime = 10;//Ĭ���ʱ�䲻����ms���Է�ֹ��CPU�жϵ��µ�ʱ���������Ч���
					for (i = (int)(mintime*T); i < maxtime*T; i++)
					{
						cout << (double(i)) / T << "ms~" << ((double(i + 1)) / T) << "ms:\t";
						for (j = 1; j < (int)(timetable[i] / (double(maxtable)) * 50); j++)
						{
							cout << '|';
						}
						cout << endl;
					}
					//�����������ͳ�ƽ����ʱ��ֲ�
					delete[] timetable; timetable = NULL;//�ͷ��ڴ�
					cout << endl;
				}
				else
				{//������Ŀ��������ֵ
					break;//����
				}
			}
		}
		else if (mode == '1')
		{//���ѡ���˻�ԭħ��ģʽ
			while (1)
			{
				int f;//���ڱ�֤����״̬�Ϸ��Բ��������ݣ���ԭ��������Ϊ�Ƿ�������ı�ʾ����
				while ((f = input()) == -1);//��ȡ����״̬����֤��Ϸ�
				if (f == -2)break;
				QueryPerformanceCounter(&BegainTime);//��ʼ��ʱ
				search(f, mode);
				QueryPerformanceCounter(&EndTime);//������ʱ
				cout << "�˴���⹲������" << p2 << "��״̬���ܹ���ʱ��" << (double)
					(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000 << "ms";
				//����˴λ�ԭ�����Ϣ
				cout << "�������?(0.Yes\t1.No)\t[ ]\b\b";
				cin >> f;//�Ƿ������⣿
				cout << endl;
				if (f != 0)break;//�����������ֱ�ӷ���ģʽѡ�����
			}
		}
		else break;//���������ģʽ��ֱ�ӵ���ѭ���󽫳���
	}
	return 0;
}

//I\O----------------------------------------------------------------------------------------------------------
int input()
{
	int colour, i;
	int cubecolour[6];//��ɫ��RUFLDB��˳��
	bool checkcolour[6];//�����ɫ�Ƿ����
	for (i = 0; i < 6; i++)checkcolour[i] = false;
	cout << "�밴��������ʾ����ħ��" << endl;
	cout << "�Ƿ�ʹ��Ĭ����ɫ��.��;1.��\t[ ]\b\b";
	cin >> colour;
	if (colour != 0)
	{//�Ƿ���Ҫ���¶�����ɫ���ǣ�
		cout << "�밴RUFLDB���˳��������ħ����ԭ���������ɫ��" << endl;
		cout << "0.��ɫ;1.��ɫ;2.��ɫ;3.��ɫ;4.��ɫ;5.��ɫ;" << endl;
		cout << "R����ɫ��\t[ ]\b\b"; cin >> cubecolour[0];
		cout << "U����ɫ��\t[ ]\b\b"; cin >> cubecolour[1];
		cout << "F����ɫ��\t[ ]\b\b"; cin >> cubecolour[2];
		cout << "L����ɫ��\t[ ]\b\b"; cin >> cubecolour[3];
		cout << "D����ɫ��\t[ ]\b\b"; cin >> cubecolour[4];
		cout << "B����ɫ��\t[ ]\b\b"; cin >> cubecolour[5];
		for (i = 0; i < 6; i++)checkcolour[cubecolour[i]] = true;
		for (i = 0; i < 6; i++)if (checkcolour[i] == false)
		{//�ж������Ƿ�Ϸ�������Ƿ�
			int ret;
			cout << "��������Ƿ��������룿.��������;1.����" << endl;
			cin >> ret;//�����Ƿ����
			if (ret == 0)return -1;//�ǣ�����-1������ִ��input
			else return -2;//�񣺷���-2������ѭ������һ��
		}
	}
	else
	{//�Ƿ���Ҫ���¶�����ɫ����
		for (i = 0; i < 6; i++)cubecolour[i] = i;//��ʼ��Ĭ����ɫ
	}
	char *(c[6]) = { "��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ" };
	int position;
	bool checkPos[7];//���ڼ���Ƿ�ÿһ�鶼������
	for (i = 0; i < 7; i++)checkPos[i] = false;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[5]] << ", " << c[cubecolour[4]] << ", " << c[cubecolour[3]] << "�Ŀ飬����������BDLλ \
		�ã�ͬʱʹ��" << c[cubecolour[4]] << "λ��D����" << endl;
	cout << "�������λ����Ϣ�����룺" << endl; \
		cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[1]] << ", " << c[cubecolour[2]] << ", " << c[cubecolour[3]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 0; checkPos[position] = true;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[1]] << ", " << c[cubecolour[3]] << ", " << c[cubecolour[5]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 1; checkPos[position] = true;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[0]] << ", " << c[cubecolour[1]] << ", " << c[cubecolour[5]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 2; checkPos[position] = true;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[0]] << ", " << c[cubecolour[1]] << ", " << c[cubecolour[2]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 3; checkPos[position] = true;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[2]] << ", " << c[cubecolour[3]] << ", " << c[cubecolour[4]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 4; checkPos[position] = true;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[0]] << ", " << c[cubecolour[4]] << ", " << c[cubecolour[5]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 5; checkPos[position] = true;
	cout << "���ҳ�ͬʱ�� \
		��" << c[cubecolour[0]] << ", " << c[cubecolour[2]] << ", " << c[cubecolour[4]] << "�Ŀ飬����������λ \
		�� : " << endl;
	cout << "�ÿ��λ�ã�.ULF;1.ULB;2.URB;3.URF;4.DLF;5.DRB;6.DRF\t[ ]\b\b";
	cin >> position; now[position] = 6; checkPos[position] = true;
	//λ����Ϣ������ϣ����濪ʼ��������Ƿ�Ϸ�
	for (i = 0; i < 7; i++)if (!checkPos[i])
	{//�����ĳһ����û�б�����
		int ret;
		cout << "��������Ƿ��������룿.��������;1.����" << endl;
		cin >> ret;
		if (ret == 0)return -1;
		else return -2;
	}
	cout << "������з�����Ϣ�����룺" << endl;
	cout << "������ULF����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
		��R; 2.F��B\t[]\b\b";
	cin >> now[7];
	cout << "������ULB����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
		��R; 2.F��B\t[]\b\b";
	cin >> now[8]; now[8] = (3 - now[8]) % 3;
	cout << "������URB����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
			��R; 2.F��B\t[]\b\b";
	cin >> now[9];
	cout << "������URF����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
			��R; 2.F��B\t[]\b\b";
	cin >> now[10]; now[8] = (3 - now[8]) % 3;
	cout << "������DLF����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
			��R; 2.F��B\t[]\b\b";
	cin >> now[11]; now[8] = (3 - now[8]) % 3;
	cout << "������DRB����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
			��R; 2.F��B\t[]\b\b";
	cin >> now[12]; now[8] = (3 - now[8]) % 3;
	cout << "������DRF����" << c[cubecolour[1]] << "��" << c[cubecolour[4]] << "�����ڵ��棺.U��D;1.L \
			��R; 2.F��B\t[]\b\b";
	cin >> now[13];
	//������Ϣ������ϣ������鷽����Ϣ�Ƿ�Ϸ������������ܺͱ�������
	int checkOri = 0;
	for (i = 7; i < 14; i++)
	{
		checkOri += now[i];
	}
	if (checkOri % 3 != 0)
	{//���������Ϣ����Ƿ�
		int ret;
		cout << "��������Ƿ��������룿.��������;1.����" << endl;
		cin >> ret;
		if (ret == 0)return -1;
		else return -2;
	}
	//�����������ȫ���Ϸ�������״̬ѹ�����ֵ����״̬����
	return zip();
}
void output(int i, int cube0)
{//����ⷨ������Ϊ�����һ���������i����ԭ״̬����cube0������ht����ֻ����ĳ��״̬��֮ǰһ��������Ҫ�����������һ������˫������غϵ�״̬�ֱ���Ŀ��״̬�뻹ԭ̬����ת��
	int j;
	
	if (ht[z0] / 128 == 0)
	{//������һ���Ǵӻ�ԭ״̬�������еĽڵ���չ���ͽ�����ʹ�ô�output���̿������һ���Ǵ�Ŀ��ڵ����չ��ͳһ֮�������
		j = z0;
		z0 = z;
		z = j;
		i = (i / 3) * 3 + (2 - i % 3);
	}
	int solve[10];//���ڴӵ�ǰ�ڵ㵽Ŀ��ڵ��ת��������������������෴������Ҫ����ջ�ṹ������
	int p = 0;
	char *(ansT[9]) = { "R","R2","R'","U","U2","U'","F","F2","F'" };
	cout << "��ħ���ⷨ���£�" << endl;
	while (z0 != cube0)
	{//ת��ħ��ֱ������Ŀ��ڵ㣬��ÿ�ν�ת������ѹ��ջ
		solve[p++] = ht[z0] % 128 / 13;
		z0 = TurnPos[z0 / MAX_ORI][((ht[z0] % 128 / 13) / 3) * 3 + 2 -
			(ht[z0] % 128 / 13) % 3] * MAX_ORI + TurnOri[z0%MAX_ORI][((ht[z0] % 128 / 13) / 3) * 3 + 2 -
			(ht[z0] % 128 / 13) % 3];
	}
	for (j = p - 1; j >= 0; j--)cout << ansT[solve[j]] << ' ';//��������ת��
	cout << ansT[i] << ' ';//������һ��������ת������
	while (z != 0)
	{//ת��ħ��ֱ����ԭ����ÿ�����
		cout << ansT[((ht[z] % 128 / 13) / 3) * 3 + 2 - (ht[z] % 128 / 13) % 3] << ' ';
		z = TurnPos[z / MAX_ORI][((ht[z] % 128 / 13) / 3) * 3 + 2 - (ht[z]
			% 128 / 13) % 3] * MAX_ORI + TurnOri[z%MAX_ORI][((ht[z] % 128 / 13) / 3) * 3 + 2 - (ht[z] % 128 / 13) % 3];
	}
	cout << endl;
}

//Ԥ���庯��-----------------------------------------------------------------------------------------------------------------
void pret()
{//now0����now
	for (int i = 0; i < 14; i++)now[i] = now0[i];
}
void turn(int i)
{//��now0����ִ��ĳ��ת���󸳸�now
	if (i == 0)
	{//R
		now[2] = now0[3];
		now[3] = now0[6];
		now[6] = now0[5];
		now[5] = now0[2];
		//����λ�ñ任�����·���任
		now[9] = (now0[10] + 2) % 3;
		now[10] = (now0[13] + 1) % 3;
		now[13] = (now0[12] + 2) % 3;
		now[12] = (now0[9] + 1) % 3;
	}
	else if (i == 1)
	{//R2
		now[2] = now0[6];
		now[6] = now0[2];
		now[3] = now0[5];
		now[5] = now0[3];
		//����λ�ñ任�����·���任
		now[9] = now0[13];
		now[13] = now0[9];
		now[10] = now0[12];
		now[12] = now0[10];
	}
	else if (i == 2)
	{//R'
		now[2] = now0[5];
		now[3] = now0[2];
		now[6] = now0[3];
		now[5] = now0[6];
		//����λ�ñ任�����·���任
		now[9] = (now0[12] + 2) % 3;
		now[10] = (now0[9] + 1) % 3;
		now[13] = (now0[10] + 2) % 3;
		now[12] = (now0[13] + 1) % 3;
	}
	else if (i == 3)
	{//U
		now[0] = now0[3];
		now[1] = now0[0];
		now[2] = now0[1];
		now[3] = now0[2];
		//����λ�ñ任�����·���任
		now[7] = now0[10];
		now[8] = now0[7];
		now[9] = now0[8];
		now[10] = now0[9];
	}
	else if (i == 4)
	{//U2
		now[0] = now0[2];
		now[2] = now0[0];
		now[3] = now0[1];
		now[1] = now0[3];
		//����λ�ñ任�����·���任
		now[7] = now0[9];
		now[9] = now0[7];
		now[10] = now0[8];
		now[8] = now0[10];
	}
	else if (i == 5)
	{//U'
		now[0] = now0[1];
		now[1] = now0[2];
		now[2] = now0[3];
		now[3] = now0[0];
		//����λ�ñ任�����·���任
		now[7] = now0[8];
		now[8] = now0[9];
		now[9] = now0[10];
		now[10] = now0[7];
	}
	else if (i == 6)
	{//F
		now[0] = now0[4];
		now[3] = now0[0];
		now[6] = now0[3];
		now[4] = now0[6];
		//����λ�ñ任�����·���任
		now[7] = (now0[11] + 1) % 3;
		now[10] = (now0[7] + 2) % 3;
		now[13] = (now0[10] + 1) % 3;
		now[11] = (now0[13] + 2) % 3;
	}
	else if (i == 7)
	{//F2
		now[0] = now0[6];
		now[6] = now0[0];
		now[3] = now0[4];
		now[4] = now0[3];
		//����λ�ñ任�����·���任
		now[7] = now0[13];
		now[13] = now0[7];
		now[10] = now0[11];
		now[11] = now0[10];
	}
	else if (i == 8)
	{//F'
		now[0] = now0[3];
		now[3] = now0[6];
		now[6] = now0[4];
		now[4] = now0[0];
		//����λ�ñ任�����·���任
		now[7] = (now0[10] + 1) % 3;
		now[10] = (now0[13] + 2) % 3;
		now[13] = (now0[11] + 1) % 3;
		now[11] = (now0[7] + 2) % 3;
	}
}
int zip()
{//��now����ѹ��������
	int n = 0, m = 0, i;//n:λ�ñ���;m:�������
	n = Pos_to_Num[now[0]][now[1]][now[2]][now[3]][now[4]][now[5]];//λ��״̬�ı���ֱ�Ӷ�ȡ���еĽ��
	for (i = 7; i<13; i++)
	{//����״̬�ı�����������Ʊ任
		m *= 3;
		m += now[i];
	}
	return (n*MAX_ORI + m);
}
void uzip(int i)
{//��i��ѹ����now0����
	int m = i%MAX_ORI, n = i / MAX_ORI;//n:λ�ñ���;m:�������
	now0[13] = 12;//��ʼ��״̬ѹ��ʱδ��ֱ�Ӵ洢�ķ���״̬��
	for (i = 12; i >= 7; i--)
	{//����������ÿһλ������ԭ����״̬��
		now0[i] = m % 3;
		m /= 3;
		now0[13] -= now0[i];
	}
	now0[13] %= 3;//��ԭδ��ֱ�Ӵ洢�ķ���״̬��
	for (i = 6; i >= 0; i--)
	{//λ��״̬�Ļ�ԭֱ�ӵ��ñ��н��
		now0[i] = Num_to_Pos[n][i];
	}
}

//��ʼ��------------------------------------------------------------------------------------------------------------------
void prePos(int i)
{//����λ�����ö�������еĵ�i+1λ��
	if (i == 7)
	{//������е�ǰλ��ö�����ˣ�����ǰ��������λ�����
		for (int j = 0; j<7; j++)Num_to_Pos[ziptot][j] = Possearch[j];
		Pos_to_Num[Possearch[0]][Possearch[1]][Possearch[2]][Possearch[3]][Possearch[4]]
			[Possearch[5]] = ziptot;
		ziptot++;//��Ӧ��λ����+1
	}
	else
	{//�����ǰ����ö�������е�ǰλ
		for (Possearch[i] = 0; Possearch[i]<7; Possearch[i]++)if (!PosNumUsed[Possearch[i]])
		{//��~6�Ե�iλ���г��ԣ����������û�б��õ�
			PosNumUsed[Possearch[i]] = true;//���������ֵ�i+1λ�Ѿ�����
			prePos(i + 1);//ö����һλ��i+2λ��
			PosNumUsed[Possearch[i]] = false;//�����������ѳ�����ȫ
		}//���������һ�γ���
	}
}
void pre()
{
	prePos(0);
	int i, j, k;
	for (i = 0; i<MAX_ORI; i++)
	{//���ڷ���״̬��λ��״̬�٣�Ϊ�˼������������ڼ��㷽��״̬��ʱ�򲻷��ͼ����һ���ֵ�λ��״̬
		uzip(i*MAX_ORI + i);//��ѹ������λ�ñ���
		for (j = 0; j<9; j++)
		{
			pret();//��ʼ����ǰ״̬
			turn(j);//����ĳ��ת��
			k = zip();//�õ�ת�����״̬����
			TurnPos[i][j] = k / MAX_ORI;
			TurnOri[i][j] = k%MAX_ORI;
			//�ֱ𴢴�״̬���ֵķ��򲿷���λ�ò���
		}
	}
	for (i = MAX_ORI; i<MAX_POS; i++)
	{//����λ��״̬
		uzip(i*MAX_ORI);
		for (j = 0; j<9; j++)
		{
			pret();//��ʼ����ǰ״̬
			turn(j);//����ĳ��ת��
			k = zip();//�õ�ת�����״̬����
			TurnPos[i][j] = k / MAX_ORI;
			//���财��״̬���ֵ�λ�ò���
		}
	}
}

//���ϳ�ʼ��----------------------------------------------------------------------------------------------------
int searchall()
{
	LARGE_INTEGER BegainTime;
	LARGE_INTEGER EndTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&BegainTime);
	//�趨��ʱ��ʼ
	int i, k, maxlen = 0;
	memset(ht, 0, MAX_CUBE*sizeof(unsigned char));
	p1 = 0; p2 = 0;
	Queue[0] = 0; ht[0] = 1 + 9 * 13;
	//��ʼ����ϣ�����Ѷ���ͷԪ��
	while (p1 <= p2)
	{//�жϹ��Ѷ���ָ��
		k = ht[Queue[p1]] % 128 % 13;
		z0 = Queue[p1]; p1++;//��ȡz0�ڵ���Ϣ
		for (i = 0; i<9; i++)
		{//���Ը���ת������
			z = TurnPos[z0 / MAX_ORI][i] * MAX_ORI + TurnOri[z0%MAX_ORI][i];//��ħ�����д���Ϊi��ת��
			if (ht[z] != 0); else
			{//������ֵĽڵ��Ѿ�����ǰ�����֣��Ͳ����κβ��������򴢴�ýڵ���Ϣ��������βָ��
				ht[z] = k + 1 + 13 * i + 128 * (ht[z0] / 128);
				p2++;
				Queue[p2] = z;
			}
		}
		if (maxlen<k)
		{//ÿ��ɨ�趼��¼��ǰ����������Զ���룬����и���
			maxlen = k;
			QueryPerformanceCounter(&EndTime);
			cout << "��ǰ������״̬��ĿΪ������չ��/��������" << p1 << '/' << p2 << "��������ʱ \
				�䣺" << (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000 << "ms����ǰ�� \
				�� : " << k - 1 << endl;
		}
	}
	QueryPerformanceCounter(&EndTime);
	cout << "����������ܺ�ʱ��" << (double)(EndTime.QuadPart -
		BegainTime.QuadPart) / Frequency.QuadPart * 1000 << "ms" << endl;
	count();
	return 0;
}
int search(int t, char mode)
{//����״̬����Ϊt�Ķ���״̬
	int i, k;
	if (t == 0) {
		cout << "�ѻ�ԭ" << endl;
		return 0;
	}
	memset(ht, 0, MAX_CUBE*sizeof(unsigned char));
	p1 = 0; p2 = 0;
	Queue[0] = 0; ht[0] = 1 + 9 * 13;
	p2++; Queue[p2] = t; ht[t] = 1 + 9 * 13 + 128;
	//��ʼ����ϣ�����Ѷ���ͷԪ��
	while (p1 <= p2)
	{//�жϹ��Ѷ���
		k = ht[Queue[p1]] % 128 % 13; //��ȡz0״̬
		z0 = Queue[p1]; p1++;
		for (i = 0; i<9; i++)
		{
			z = TurnPos[z0 / MAX_ORI][i] * MAX_ORI + TurnOri[z0%MAX_ORI][i];//��ħ�����д���Ϊi��ת��
			if (((ht[z] % 128 % 13) != 0) && (ht[z] / 128 + ht[z0] / 128 == 1))
			{//������ֵ��µĽڵ��뵱ǰ��չ�Ľڵ�ֱ�����Ŀ��ڵ��뻹ԭ�ڵ㣬���ҵ��ⷨ
				if (mode == '1') {//����ǽ�ħ��ģʽ������ⷨ�����򷵻�ֱ��ͳ��ʱ��
					output(i, t);
				}
				return 0;
			}
			else if (ht[z] != 0); else
			{//���û���ҵ��ⷨ��������һ���µĽڵ㣬�򴢴�ýڵ㣬��������βָ��
				ht[z] = k + 1 + 13 * i + 128 * (ht[z0] / 128);
				p2++;
				Queue[p2] = z;
			}
		}
	}
	return 0;
}
void count()
{
	int maxi = 0;
	int i;
	int len[200];
	for (i = 0; i<200; i++)len[i] = 0;
	for (i = 0; i<MAX_CUBE; i++)
	{//ɨ��ÿһ���ڵ�
		if ((ht[i] % 128 % 13) != 0)
		{//����ýڵ㱻���ʹ�����¼�ýڵ�ĳ�����Ϣ
			maxi = i;
			len[ht[i] % 128 % 13]++;
		}
		else;
	}
	for (i = 1; i<13; i++)
	{//���ͳ�ƽ��
		cout << "��̻�ԭ����ǡΪ" << i - 1 << "���������Ϊ��" << len[i] << "��" << endl;
	}
}