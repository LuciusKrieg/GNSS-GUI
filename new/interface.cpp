#include "interface.h"

Interface::Interface(std::string s){
    filename = s;
    cols = 0;
    rows = 0;
    heap = 0;
    get_size();

    std::fstream file;
    file.open(filename.c_str());
    for(int n = 0; n < heap; n++){
        getline(file, s);
    }
    Calibration_Data = new double*[rows];
    for(int k = 0; k < rows; k++){
        getline(file, s);
        std::istringstream i;
        i.str(devider(s));
        Calibration_Data[k] = new double[cols];
        for (int j = 0; j < cols; j++){
            getline(i, s, ' ');
            Calibration_Data[k][j] = atof(s.c_str());
        }
    }
    std::cout<<"Interface constructor was correctly called!\n";
}

Interface::Interface(){
    std::cout<<"Interface constructor was correctly called!\n";
}

power::power(std::string s) : Interface(s){
    L = new double**[4];
    //this->out(this->Calibration_Data);
    for(int i = 1; i != 4; i++){
        L[i] = new double*[3];
        for(int j = 0; j != 3; j++){
            L[i][j] = new double[rows];
            for(int k = 0; k < rows; k++){
                L[i][j][k] = Calibration_Data[k][8+j+(i-1)];
            }
        }
    }

    int start = 0;
    int stop = 0;
    S = new int*[101];
    D = new double*[101];
    for(int i = 0; stop < rows-1; i++){
        D[i] = new double[6];
        S[i] = new int[2];
        start = (int)Calibration_Data[0+start+1][17]-1;
        stop = (int)Calibration_Data[0+start+1][18]-1;
        S[i][0] = start;
        S[i][1] = stop;
        for(int j = 0; j < 3; j++){
            D[i][j] = Calibration_Data[0+start+1][19+j];
            D[i][3+j] = Calibration_Data[0+start+1][24+j];
            //cout<<D[i][j]<<" "<<D[i][j+3]<<"\n";
        }
        start = stop;
    }
}

spectrum::spectrum(std::string s) : Interface(s){
    //this->out(this->Calibration_Data);
    cols--;
    rows--;
    Power = new double*[9];
    for(int i = 0; i < 9; i++){
        Power[i] = new double[rows];
    }

    std::cout<<rows<<"\n";
    std::cout<<cols<<"\n";
    RBW = 510000;
    F = new double[rows];
    for(int i = 0; i < rows; i++){
        F[i] = Calibration_Data[i+1][0];
        //cout<<F[i-1]<<endl;
    }

    T = new double[cols];
    for(int i = 0; i < cols; i++){
        T[i] = Calibration_Data[0][i+1];
        //cout<<T[i-1]<<endl;
    }
    P = new double*[rows];
    for(int i = 0; i < rows; i++){
        P[i] = new double[cols];
        for(int j = 0; j < cols; j++){
            P[i][j] = Calibration_Data[i+1][j+1];
        }
    }

}

sun::sun(std::string s) : Interface(s){
    L = new double*[4];
    //this->out(this->Calibration_Data);
    for(int i = 1; i != 4; i++){
        L[i] = new double[rows];
        for(int j = 0; j < rows; j++){
            L[i][j] = Calibration_Data[j][10+i];
        }
    }
    G = new double*[4];
    for(int i = 1; i != 4; i++){
        G[i] = new double[rows];
        for(int j = 0; j < rows; j++){
            G[i][j] = Calibration_Data[j][7+i];
        }
        //cout<<G[][]<<endl;
    }
    std::cout<<"sun ended\n";
}



Interface::~Interface(){

    for(int i = 0; i < rows; i++){
        //delete []Calibration_Data[i];
    }
    //delete []Calibration_Data;
    std::cout<<" \"Interface\" Destructed\n";
}

power::~power(){
    /*
    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            delete []L[i][j];
        }
        delete []L[i];
    }
    //delete []L;*/
    std::cout<<" \"power\" Destructed\n";
}

spectrum::~spectrum(){
    /*
    delete []F;
    delete []T;
    for(int i = 0; i < rows; i++){
        delete []P[i];
    }
    //delete []P;*/
    std::cout<<" \"spectrum\" Destructed\n";

}

sun::~sun(){
    /*
    for(int i = 0; i < 4; i++){
        delete []L[i];
    }
    delete []L;*/
    std::cout<<" \"sun\" Destructed\n";
}



double* spectrum::calculate(double f1, double f2){

    double* A = new double[cols];
    int pos1 = find_pos(F, f1);
    int pos2 = find_pos(F, f2);
    std::cout<<"pos1: "<<f1<<'\t'<<pos1<<std::endl;
    std::cout<<"pos2: "<<f2<<'\t'<<pos2<<std::endl;
    A[0]=0;
    if(pos1 == pos2 || pos1<0 || pos2 < 0){
        std::cout<<"ERROR in calculate"<<std::endl;
        for(int i = 0; i<cols; i++){
            A = P[pos1];
        }
    }
    else{
        for(int i = 0; i<cols; i++){
            A[i] = 0;
            int j = pos1;
            while(j<=pos2){
                A[i] = 0.5*(0.001*pow(10,(P[j][i]-10*log10(RBW))/10)+0.001*pow(10,(P[j+1][i]-10*log10(RBW))/10))*(F[j+1]-F[j]) + A[i];
                j++;
            }
            A[i] = 10*log10(A[i]*1000);
        }
    }
    return A;
}

void spectrum::file_writting(double** C, std::string s){
    std::ofstream wfile(s.c_str());
    wfile<<"\t";
    for(int k = 0;k<3;k++){
        wfile<<"\t L"<<k+1<<":\t\t";
    }
    wfile<<"\n";
    for(int i = 0; i<cols; i++){
        wfile<<T[i] <<std::setprecision(10)<<","<<"\t";
        for(int j = 0; j < 3; j++){
            wfile  << C[j][i] <<", ";
        }
        wfile<<"\n";
    }

    wfile.close();
}

void spectrum::file_writting(double** C, double* t, std::string s){
    std::ofstream wfile(s.c_str());
    for(int i = 0; i<cols; i++){
        wfile << std::setprecision(10) << t[i];
        for(int j = 0; j < 9; j++){
            wfile << ",\t" << C[j][i];
        }
        wfile<<"\n";
    }
    wfile.close();
}

void spectrum::writting(std::string s){
    std::ofstream wfile(s.c_str());
    wfile<<"\t";
    wfile<<"\n";
    for(int i = 0; i<rows; i++){
        wfile<<F[i] <<std::setprecision(10)<<","<<"\t";
        for(int j = 0; j < cols; j++){
            wfile  << P[i][j] <<'\t'<<", ";
        }
        wfile<<"\n";
    }

    wfile.close();
}

void Interface::out(double** M){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            std::cout<<'['<<M[i][j]<<"]";
        }
        std::cout<<i<<std::endl;
    }
}

void Interface::out(double* M){
    int length = _msize(M) / sizeof(double); // 42!
    for(int j = 0; j < length; j++){
        std::cout<<'['<<M[j]<<"]\n";
    }
}

int Interface::find_pos(double* A, double x){
    int i = 0;
    if((A[(int)(rows/2)])<x){
        while(x>(A[(int)(rows/2)+i])){
            i = i + 1;
            //cout<<info->Frequency[(info->lines/2)+i]<<endl;
        }
        return (rows/2)+i;
    }
    else if((A[(int)(rows/2)])>x){
        while(x<A[(int)(rows/2)-i]){
            i = i + 1;
        }
        return (rows/2)-i;
    }
    else return (rows)/2;
}

std::string Interface::devider(std::string s){
    bool space = false;
    char* res = new char[s.length()];
    int k = 0;
    for(size_t i=0; i<s.length(); i++)
    {
        if(s[i]!= ' ' && s[i]!= '\t'){
            res[k] = s[i];
            space = true;
            k++;
        }
        else if(space){
            res[k] = ' ';
            space = false;
            k++;
        }
    }
    res[k] = '\0';
    s = (std::string)res;
    return s;
}

void Interface::get_size(){
    std::fstream file;
    std::string temp, t;
    std::cout<<filename<<std::endl;
    file.open(filename.c_str());
    heap = 0;
    cols = 0;
    rows = 0;
    getline(file, t); rows++;
    while(getline(file, temp)){
        rows++;
        if(temp.length()<3){
            rows++;
            if((getline(file, temp) && temp.length() < 3) || file.eof()){
                heap = rows;
                getline(file, t);
                rows++;
            }
        }
    }
    rows = rows - heap;
    temp = devider(t);
    std::istringstream i;
    i.str(temp);
    /*
    for (string line; getline(i, line, ' ');){
        cols++;
    }*/
    size_t start {temp.find_first_not_of(' ')};
    std::cout<<"Start: "<<start<<'\n';
    while (start != std::string::npos) // проходим, пока в строке не окажется других символов, кроме separators
    {
        size_t end = temp.find_first_of(' ', start + 1); // находим, где кончается слово

        if (end == std::string::npos){ // если НЕ найден ни один из символов-разделителей
            end = temp.length();
        }// устанавливаем переменную end на конец текста
        cols++; // добавляем в вектор слово
        //std::cout<<"Cols: "<<cols<<"\n";
        start = temp.find_first_not_of(' ', end + 1); // находим начальный индекс следующего слова и переустанавливаем start
    }
    file.close();
    std::cout<<"Rows: "<<rows<<std::endl;
    std::cout<<"Heap: "<<heap<<std::endl;
    std::cout<<"Cols: "<<cols<<std::endl;
}
/*
int find_pos(double* M, double f){
    int i = 0;
    if((M[(int)(1002/2)])<f){
        while(f>(M[(int)(1002/2)+i])){
            i = i + 1;
            //cout<<info->Frequency[(info->lines/2)+i]<<endl;
        }
        return (1002/2)+i;
    }
    else if((M[(int)(1002/2)])>f){
        while(f<M[(int)(1002/2)-i]){
            i = i + 1;
        }
        return (1002/2)-i;
    }
    else return (1002)/2;
}
*/
