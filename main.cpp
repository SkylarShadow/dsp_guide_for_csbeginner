#include<iostream>
#include<cmath>
#include<vector>
#include<numeric>
#include<algorithm>
#include<complex>
using namespace std;

/**
 * @description: ���vector�����ݣ��Զ��ŷָ���������python�л�ͼ
 * @return {*}
 */
template <typename T>
void vector_show(vector<T> const& arr){
    cout << "[";
    for (auto it = arr.begin(); it != arr.end(); it++) {
        cout << *it <<", ";
    }
    cout << "]";
    cout << endl;
}



/**
 * @description: �������Ҳ�
 * @return {*}
 */
template<typename T1, typename T2 ,typename T3>
vector<double>  create_sin_arr(T1 amp,T2 freq,T3 phase,int sample_rate,int time_ms){
    static_assert(is_arithmetic<T1>::value || is_arithmetic<T2>::value ||
                      std::is_arithmetic<T3>::value != 0,
                  "�������ͱ���������");

    // static_cast ǿת��
    // �����ʵĵ�λΪ ������/�룬������ʱ�䵥λΪ���룬��Ҫ������ת��Ϊ��(ms/1000 = s)
    int sample_num = static_cast<int>(sample_rate * time_ms) / 1000; 


    if (sample_num <= 1 ){
        cout << "��������Ŀ����" << endl;
        return {};
    }

    vector<double> sin_arr(sample_num,0);

    if(freq*2 >= (double)sample_rate){
        cout << "�����ϲ�������" << endl;
        // return arr;
    }

    double delta_t = 1.0 / sample_rate;

    for (int i = 0; i < sample_num;i++){
        double t = i * delta_t;
        sin_arr[i] = amp * sin(2 * M_PI * freq *t + phase*M_PI/180);
    }

    return sin_arr;
}


/**
 * @description: �������Ҳ���ÿ�����ڵĵ����ֵ����ͬ��
 * @param {T1} amp
 * @param {T2} freq
 * @param {int} sample_rate
 * @param {int} time_ms
 * @return {*}
 */
template<typename T1, typename T2>
vector<double>  create_sin_arr2(T1 amp,T2 freq,int sample_rate,int time_ms){
    
    
    int sample_num = sample_rate * time_ms / 1000; // �����ʵĵ�λΪ ������/�룬������ʱ�䵥λΪ���룬��Ҫ������ת��Ϊ��(ms/1000 = s)



    // 1���������Ҳ���������
    int sin_cycle = sample_rate / freq;

    if (sample_num <= 1 ){
        cout << "��������Ŀ����" << endl;
        return {};
    }

    if ( sin_cycle == 0){
        cout << "������Ϊ0" << endl;
        return {};
    }

    vector<double> sin_arr(sample_num,0);

    if(freq*2 >= (double)sample_rate){
        cout << "�����ϲ�������" << endl;
        // return arr;
    }


    for (int i = 0; i < sample_num;i++){
        sin_arr[i] = amp * sin(2 * M_PI * (i  % sin_cycle) / sin_cycle);
    }

    return sin_arr;
}



/**
 * @description: �����ֵ�ͷ����
 * @return {*}
 */
template <typename T>
void MV_cal(vector<T> const& arr){
    double sum = std::accumulate(begin(arr),end(arr),0.0);
    double mean = sum / arr.size();

    double accum = 0.0;

    //  [&]�����������ڲ��������ݶ������ò���
    for_each(begin(arr), end(arr),[&](const double d) {
         accum += (d - mean) * (d - mean); 
    });

    double stdev = sqrt(accum/(arr.size()-1));  //��׼��

    cout << "�ܺͣ�" << sum << endl;
    cout << "��ֵ��" << mean << endl;
    cout << "���" << accum/(arr.size() - 1)  << endl;
    cout << "��׼�" << stdev << endl;
}

/**
 * @description: ÿһ��ѭ������һ�ξ�ֵ�ͷ���Դ�ģ����������ʱ(���ݿ��Գ�������)�ľ�ֵ����ģ�͵ļ���
 * @param {vector<T>} const
 * @return {*}
 */
template <typename T>
void MV_cal_running(vector<T> const& arr){


    double sum = 0, mean, variance = 0, stdev = 0, sum_sqrt = 0;
    for (int i = 0; i < arr.size();i++){
        

        sum = arr[i] + sum;
        mean = sum / (i + 1);
        sum_sqrt = sum_sqrt + arr[i] * arr[i];

        variance = (sum_sqrt - sum * sum / (i + 1)) / i;
        stdev = sqrt(variance);
        cout << "�ܺͣ�" << sum << endl;
        cout << "��ֵ��" << mean << endl;
        cout << "���" << variance << endl;
        cout << "��׼�" << stdev << endl<<endl;
    }

}


/**
 * @description: ����ֱ��ͼ���ƺ����ڴ�й©���⣩
 * @param {vector<T>} const
 * @param {double} acc ���vector����ΪС������vector�е���*acc��ȡ��
 * @return {*}
 */
template <typename T>
vector<int> binned_hist_cal(vector<T> const& arr,double acc){
    T maxValue = *max_element(arr.begin(),arr.end());
    double size = maxValue * acc;

    while(size <= 0){
        size = size * 10;
        acc = acc * 10;
    }

    vector<int> hist(static_cast<int>(size)+1, 0);
    for_each(arr.begin(),arr.end(), [&](const double d) {
        int bin_num = static_cast<int>(d * acc);
        hist[bin_num] ++;
    });
    
    return hist;
}

template <typename T1,typename T2>
vector<double> convolution_input_side(vector<T1> X,vector<T2> H){
    vector<double> Y(X.size()+H.size(), 0);
    for(int i = 0; i < X.size(); i++){ 
        for(int j = 0; j < H.size(); j++){
            Y[i + j] += X[i] * H[j];
        }
    }
    return Y;
}

template <typename T1,typename T2>
vector<double> convolution_output_side(vector<T1> X,vector<T2> H){
    vector<double> Y(X.size()+H.size(), 0);
    for(int i = 0; i < Y.size(); i++){ 
        for(int j = 0; j < H.size(); j++){ 
            if(i - j < 0) 
                continue;
            if(i - j >= X.size() ) 
                continue;
            Y[i] += X[i - j] * H[j];
        }
    }
    return Y;
}


/**
 * @description: 
 * @param {vector<complex<double>>} arr
 * @param {int} N ȡ���������ǰN��������
 * @return {*}
 */
vector<complex<double>> DFT(vector<complex<double>> arr,int N){
    vector<complex<double>> ret(N);
    complex<double> temp{0, 0};
    for (int k = 0; k < N;k++){
        ret[k].imag(0);
        ret[k].real(0);
        for (int i = 0; i < N;i++){
            temp.real(cos(2*M_PI*k*i/N));
            temp.imag(-sin(2*M_PI*k*i/N));
            ret[k] += arr[i] * temp;
            // ret[k].real(ret[k].real() + arr[i].real() * temp.real()-arr[i].imag()*temp.imag());
            // ret[k].imag(ret[k].imag() + arr[i].real() * temp.imag()+arr[i].imag()*temp.real());
        }
    }
    return ret;
}

vector<complex<double>> IDFT(vector<complex<double>> arr,int N){
    vector<complex<double>> ret(N);
    complex<double> temp{0, 0};
    for (int k = 0; k < N;k++){
        ret[k].imag(0);
        ret[k].real(0);
        for (int i = 0; i < N;i++){
            temp.real(cos(2*M_PI*k*i/N));
            temp.imag(sin(2*M_PI*k*i/N));
            ret[k] += arr[i] * temp;
        }
        ret[k].real(ret[k].real()/N);
        ret[k].imag(ret[k].imag()/N);
    }
    return ret;
}

int main(){
    vector<double> x,x_part;
    x = create_sin_arr(1,220, 0,4410,2000);

    // vector<double>::const_iterator begin = x.begin() ; 
    // vector<double>::const_iterator end = x.begin() + 1000;
    // x_part.assign(begin, end);

    // vector_show(x);
    // MV_cal(x);

    /********************exp0.�������Ҳ�����*************************/
    // x = create_sin_arr(1,220, 0,4400,2000);
    // vector_show(x);
    /********************exp0.�������Ҳ�����*************************/


    /********************exp1.�����ֵ������*************************/
    // std::vector<int> ids = []() {std::vector<int> v; v.resize(100); for (int i = 0; i < 100; ++i) v[i] = i; return v; }();
    // MV_cal_running(ids);
    // MV_cal(ids);
    // vector_show(ids);
    /********************exp1.�����ֵ������*************************/


    /********************exp2.����ֱ��ͼ*************************/
    // x = create_sin_arr(1,220, 0,4410,2000);
    // vector<int> hist = binned_hist_cal(x, 100);
    // vector_show(hist);
    /********************exp2.����ֱ��ͼ*************************/



    /********************exp3.���*************************/
    // vector<double> X={0, -1, -1.3, 2, 1.4, 1.4, 0.6, 0, -0.6};
    // vector<double> H={1, -0.5, -0.3, -0.2};
    // vector<double> Y;
    // Y = convolution_input_side(X, H);
    // vector_show(Y);
    // Y = convolution_output_side(X, H);
    // vector_show(Y);
    /********************exp3.���*************************/

    /********************exp4.dft*************************/
    
    vector<complex<double>> arr(10),res1(10),res2(10);
    for (int i = 0; i < 10;i++){
        arr[i].real(i);
        arr[i].imag(0);
    }
    cout << endl << "���ɴ�0��9�ĸ������飺";
    vector_show(arr);
    res1 = DFT(arr,10);
    cout << endl << "dft��";
    vector_show(res1);
    res2 = IDFT(res1,10);
    cout << endl << "idft��";
    vector_show(res2);
    /********************exp4.dft*************************/
    system("pause");
    return 0;
}