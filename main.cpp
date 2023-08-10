#include<iostream>
#include<cmath>
#include<vector>
#include<numeric>
#include<algorithm>
using namespace std;


template <typename T>
void vector_show(vector<T> const& arr){
    cout << "[";
    for (auto it = arr.begin(); it != arr.end(); it++) {
        cout << *it <<", ";
    }
    cout << "]";
    cout << endl;
}

template<typename T1, typename T2 ,typename T3>
vector<double>  create_sin_arr(T1 amp,T2 freq,T3 phase,int sample_rate,int time_ms){
    static_assert(is_arithmetic<T1>::value || is_arithmetic<T2>::value ||
                      std::is_arithmetic<T3>::value != 0,
                  "数据类型必须是数字");

    // static_cast 强转换
    // 采样率的单位为 采样点/秒，参数中时间单位为毫秒，故要将毫秒转换为秒(ms/1000 = s)
    int sample_num = static_cast<int>(sample_rate * time_ms) / 1000; 


    if (sample_num <= 1 ){
        cout << "采样点数目过少" << endl;
        return {};
    }

    vector<double> sin_arr(sample_num,0);

    if(freq*2 >= (double)sample_rate){
        cout << "不符合采样定理！" << endl;
        // return arr;
    }

    double delta_t = 1.0 / sample_rate;

    for (int i = 0; i < sample_num;i++){
        double t = i * delta_t;
        sin_arr[i] = amp * sin(2 * M_PI * freq *t + phase*M_PI/180);
    }

    return sin_arr;
}


template<typename T1, typename T2>
vector<double>  create_sin_arr2(T1 amp,T2 freq,int sample_rate,int time_ms){
    
    
    int sample_num = sample_rate * time_ms / 1000; // 采样率的单位为 采样点/秒，参数中时间单位为毫秒，故要将毫秒转换为秒(ms/1000 = s)



    // 1个周期正弦波采样点数
    int sin_cycle = sample_rate / freq;

    if (sample_num <= 1 ){
        cout << "采样点数目过少" << endl;
        return {};
    }

    if ( sin_cycle == 0){
        cout << "周期数为0" << endl;
        return {};
    }

    vector<double> sin_arr(sample_num,0);

    if(freq*2 >= (double)sample_rate){
        cout << "不符合采样定理！" << endl;
        // return arr;
    }


    for (int i = 0; i < sample_num;i++){
        sin_arr[i] = amp * sin(2 * M_PI * (i  % sin_cycle) / sin_cycle);
    }

    return sin_arr;
}




template <typename T>
void MV_cal(vector<T> const& arr){
    double sum = std::accumulate(begin(arr),end(arr),0.0);
    double mean = sum / arr.size();

    double accum = 0.0;

    //  [&]：匿名函数内部所有内容都由引用捕获
    for_each(begin(arr), end(arr),[&](const double d) {
         accum += (d - mean) * (d - mean); 
    });

    double stdev = sqrt(accum/(arr.size()-1));  //标准差

    cout << "总和：" << sum << endl;
    cout << "均值：" << mean << endl;
    cout << "方差：" << accum << endl;
    cout << "标准差：" << stdev << endl;
}


int main(){

    vector<double> x;
    x = create_sin_arr(1,220, 0,4410,2000);
    vector_show(x);
    // MV_cal(x);

    system("pause");
    return 0;
}