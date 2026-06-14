// selfdeclared hello world of sorting
void bubbleSort(double items[], const int size){
    bool unsorted = true;
    while(unsorted){
        unsorted=false;
        for(int i =0; i<size-1; i++){
            if(items[i]>items[i+1]){
                unsorted = true;
                double tmp = items[i+1];
                items[i+1] = items[i];
                items[i] = tmp;
            }
        }
    }
}