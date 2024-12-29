#include <iostream>
#include <cstring>
using namespace std;
int main ()
{
    char name[10];
    cin>>name;
    char str[] = "Thomas";
    char * pch;
    pch=strstr(name,str);
    if (pch!=nullptr) {
        cout<<"Found"<<endl;
    } else {
		cout<<pch<<endl;
	}

    return 0;
}
