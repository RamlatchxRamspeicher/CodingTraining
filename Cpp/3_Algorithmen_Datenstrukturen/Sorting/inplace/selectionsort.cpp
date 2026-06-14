// fehler off by one {idx < maxIdx;}

void selectionsort(double items[],int size){
    
    int maxIdx = size-1;
    int insertIdx = 0;
    while(insertIdx<maxIdx){
        int minPos = insertIdx;
        for(int idx = insertIdx+1; idx <= maxIdx; idx++){
            if(items[idx]<items[minPos])minPos=idx;
        }
        double tmp = items[insertIdx];
        items[insertIdx] = items[minPos];
        items[minPos] = tmp;
        insertIdx++;
    }
}