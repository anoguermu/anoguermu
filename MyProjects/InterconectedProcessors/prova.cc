#include <iostream>
using namespace std;
void rec(int num, int & numf){
    if(num !=0){
        int dig=num%10;
        if(dig%2!=0) numf=numf*10 + dig;
        rec(num/10, numf);

        if(dig%2==0)numf=numf*10+dig;
    }

}

int main(){
    int num;
    int numf=0;
    cin>>num;
    rec(num,numf);

    cout<<numf<<endl;
}
