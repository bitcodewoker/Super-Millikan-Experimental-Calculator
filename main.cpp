#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#define Pi 3.1415926        //定义圆周率的取值
#define DENSITY 981         //油滴的密度
#define b (8.22e-3)         //粘滞系数修正常数6.17e-6
#define g 9.8               //重力加速度
#define d 5e-3              //极板间距离
#define p (1.013e5)         //大气压强
#define e 1.60217733e-19         //元电荷
#define ETA (1.83e-5)       //粘滞系数
using namespace std;
//8.49 8.26 8.08 8.93 8.50 8.62 8.54
//4.77 4.86 4.67 4.80 4.71 4.82 4.76
class Data
{
private:
    double voltage;
    double voltage_up;
    double av_time_g;
    double av_time_e;
    double time_g[10];
    double time_e[10];
    double Total_charge;
    double av_charge;
    int NumOfcharges;
    double dongtai_k;
    double dongtai_total_charge;
    int NumOfCharges_dongtai;
    double av_charge_dongtai;
    double dongtai_wucha;
    double jingtai_wucha;
public:
    Data()
        {
            voltage = av_time_g = Total_charge = av_charge = NumOfcharges = 0;
        }
    void input(int I) {
        cout<<"第"<<I<<"组实验数据"<<endl;
        cout<<"请输入所测平衡电压值U/v:"<<endl;
        cin>>voltage;
        cout<<"请输入所测上升电压值U/v:"<<endl;
        cin>>voltage_up;
        cout<<"请输入所测下降时间数据t/s(第1~7次并以空格隔开):"<<endl;
        double time = 0;
        double total_times = 0;
        for(int i = 0;i < 7; i++) {
            cin>>time;
            time_g[i] = time;
            total_times += time;
        }
        av_time_g = total_times/7;
        cout<<"平均下降时间：T = "<<setprecision(1)<<fixed<<av_time_g<<"s"<<endl;

        time = 0;
        total_times = 0;
        cout<<"请输入所测上升时间数据t/s(第1~7次并以空格隔开):"<<endl;
        for(int i = 0;i < 7; i++) {
            cin>>time;
            time_e[i] = time;
            total_times += time;
        }
        av_time_e = total_times/7;
        cout<<"平均上升时间：T = "<<setprecision(1)<<fixed<<av_time_e<<"s"<<endl;
        /*for (int i = 0; i < 7; i++) {
            printf ("%e %e\n", time_g[i], time_e[i]);
        }*/
    }
        
        
    void compute()  {
        Total_charge = (1.43e-14)/( voltage*pow((av_time_g*(1+0.02*sqrt(av_time_g))),1.5));
        NumOfcharges = Total_charge/1.602e-19;
        av_charge = Total_charge/NumOfcharges;
        
    }
    
    void compute(double L) {
        L *= 0.001;
        
        Total_charge = ((18*Pi*d)/(voltage*sqrt(2*DENSITY*g)))*pow((ETA*L)/(av_time_g*(1+(b/(p*(sqrt((9*ETA*L)/(2*DENSITY*g*av_time_g))))))),1.5);
        NumOfcharges = Total_charge/e;
        av_charge = Total_charge/NumOfcharges;

        dongtai_k = ((18*Pi*d)/(voltage_up*sqrt(2*DENSITY*g)))*pow((ETA*L)/((1+(b/(p*(sqrt((9*ETA*L)/(2*DENSITY*g*av_time_g))))))),1.5);
        dongtai_total_charge = (dongtai_k * ((1.0 / av_time_g) + (1.0 / av_time_e)) * sqrt (1.0 / av_time_g));
        printf ("%e %e\n", (1.0 / av_time_g) + (1.0 / av_time_e), sqrt (1.0 / av_time_g));
        NumOfCharges_dongtai = dongtai_total_charge / e;
        av_charge_dongtai = dongtai_total_charge / NumOfCharges_dongtai;
        
        
        for (int i = 0; i < 7; i++) {
            double jingtai_total = ((18*Pi*d)/(voltage*sqrt(2*DENSITY*g)))*pow((ETA*L)/(time_g[i]*(1+(b/(p*(sqrt((9*ETA*L)/(2*DENSITY*g*time_g[i]))))))),1.5);
            int jingtai_num = jingtai_total / e;
            double jingtai_av = jingtai_total / jingtai_num;
            
            double dongtai_now_k = dongtai_k = ((18*Pi*d)/(voltage_up*sqrt(2*DENSITY*g)))*pow((ETA*L)/((1+(b/(p*(sqrt((9*ETA*L)/(2*DENSITY*g*time_g[i]))))))),1.5);
            double dongtai_total = (dongtai_now_k * ((1.0 / time_g[i]) + (1.0 / time_e[i])) * sqrt (1.0 / time_g[i]));
            int dongtai_num = dongtai_total / e;
            double dongtai_av = dongtai_total / dongtai_num;
            printf ("序数:%d\n上升速度%e 下降速度%e\n静态总电荷量%e 动态总电荷量%e\n静态电荷数%d 动态电荷数%d\n静态e %e 动态e %e\n静态误差 %e 动态误差 %e\n\n", i + 1, L / time_e[i], L / time_g[i], jingtai_total, dongtai_total, jingtai_num, dongtai_num, jingtai_av, dongtai_av, (jingtai_av - e) / e, (dongtai_av - e) / e);
            dongtai_wucha = dongtai_wucha + (dongtai_av - e) / e;
            jingtai_wucha = jingtai_wucha + (jingtai_av - e) / e;
        }
        dongtai_wucha /= 7;
        jingtai_wucha /= 7;
    }
    void get() {
        printf("         %e        %02d       %e\n",Total_charge,NumOfcharges,av_charge);
    }
    
    void show() {
        printf("静态：总电荷值：%eC\n电子数：%d\n电子平均电荷值：%eC\n静态误差:%f\n\n",Total_charge,NumOfcharges,av_charge, jingtai_wucha);
        printf("动态：总电荷值：%eC\n电子数：%d\n电子平均电荷值：%eC\n动态误差:%f\n\n",dongtai_total_charge,NumOfCharges_dongtai,av_charge_dongtai, dongtai_wucha);
    }

        
        };
        int main()
        {
            cout<<"FBI Warning"<<endl;
            cout<<"本代码前身来源于CSDN：https://blog.csdn.net/qq_31914923/article/details/72598177"<<endl;
            cout<<"前身代码为静态方法计算器，经修改同时能够计算动态方法数据"<<endl;
            cout<<"由于作者非常懒，真的不想写高精度。。。所以代码结果只做参考，不保证任何误差限度"<<endl;
            cout<<"实践发现：当油滴带十几个e的时候静态和f动态的准确度比较高，当只带几个e的时候会出现n数比准确值小1或者2的情况，估计是因为代码中截断误差"<<endl;
            cout<<"这个误差会导致计算的误差值完全错误，此时建议。。。手算。。。"<<endl;
            cout<<"实践还发现总电荷数保留后的数字较为准确，而计算出的e差距会比较大，目测出锅原因同上"<<endl;
            cout<<"但我就是不改"<<endl;
            cout<<"By BIT_liou"<<endl;
            cout<<"下面开始"<<endl;
            printf ("\n\n\n\n");
            cout<<"Welcome To Robert Millikan's Sweet Lab."<<endl;
            cout<<"———————————————————"<<endl;
            cout<<"请输入你需要计算的数据组数："<<endl;
            int N;
            cin>>N;
            cout<<"请确认油滴匀速上升或下降的距离: \nL = 2mm ? (Y/N)"<<endl;
            char C;
            cin>>C;
            double L;
            if(C!='Y')
            {
                cout<<"请输入油滴匀速上升或下降的距离 L/mm:"<<endl;
                cin>>L;
            }
            cout<<"取电子电荷的公认值：e = 1.602e-19 C(即1.602*10^-19C).\n"<<endl;
            vector<Data> Datas;
            vector<Data>::iterator iter;
            for(int I = 1;I <= N; I++)
            {
                    Data data;
                    data.input(I);
                    if(C!='Y') data.compute(L);
                    else data.compute();
                    data.show();
                    Datas.push_back(data);
            }
             /*cout<<"————————————————————————————"<<endl;
             cout<<"数据处理与实验结果:"<<endl;
             cout<<"油滴编号      q/(e-19C)           n         e/(e-19C)"<<endl;
             int J = 1;
              for(iter = Datas.begin(); iter < Datas.end(); iter++)
                {
                    cout<<"   "<<J;
                    J++;
                    iter->get();
                }
            cout<<"————————————————————————————"<<endl;
            cout<<"Andrews Millikan: Well,It's so cool!"<<endl;*/
            printf("Press any key to continue...");
            getchar();
            getchar();
         
            return 0;
        }
        
        
