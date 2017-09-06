//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include "ShmDataFrame.h"
#include <iostream>
#include <string>
#include <thread>
#include <memory.h>

using namespace std;

void set_values(ShmDataFrame* shm){
    char buffer[128] = {0};
    for(int i = 0; i < shm->nColumns; i++){
        for(int j = 0; j < shm->nIndexes; j++){
            long _size = shm->column_sizes[i];
            int k = 0;
            for(k = 0; k < _size; k++){
                buffer[k] = '1';
            }
            buffer[k] = '\0';
            shm->set_field(j,i,buffer);
        }
    }
}

void check_values(ShmDataFrame* shm){
    char buffer[128] = {0};
    for(int i = 0; i < 128; i++)
        buffer[i] = '1';
    for(int i = 0; i < shm->nColumns; i++){
        for(int j = 0; j < shm->nIndexes; j++){
            long _size = shm->column_sizes[i];
            void* src = shm->get_field(j,i);
            int re = memcmp((void*)buffer,src,_size);
            if (re != 0){
                cout << "different values" << endl;
            }
        }
    }
}

void check_shm(string ipckey){
    ShmDataFrame df;
    df.connect_shm(ipckey);
    cout << "Columns && Indexs" << endl;
    cout << df.nColumns << '\t' << df.nIndexes << endl;
    cout << "Column Sizes: " << endl;
    for(int i = 0; i < df.nColumns; i++)
        cout << df.column_sizes[i] << '\t';
    cout << endl;
    cout << "Column Offsets: " << endl;
    for(int i = 0; i < df.nColumns; i++)
        cout << df.column_offset[i] << '\t';
    cout << endl;
    cout << "check values" << endl;
    check_values(&df);
}

void create_shm_dataframe(){
    /// create
    string ipckey = "0x0a0a0000";
    long nColumns = 10;
    long nIndexs  = 10000;
    vector<long> column_sizes;
    for(int i = 0; i < nColumns; i++)
        column_sizes.push_back(i+1);
    ShmDataFrame df;
    df.create_shm(ipckey,nColumns,nIndexs,column_sizes);
    set_values(&df);
}

void check_shm_dataframe(){
    string ipckey = "0x0a0a0000";
    std::thread check(check_shm,ipckey);
    check.join();
}

int main(){
    create_shm_dataframe();
    check_shm_dataframe();
    cout << "Test Finished!" << endl;
    return 0;
}