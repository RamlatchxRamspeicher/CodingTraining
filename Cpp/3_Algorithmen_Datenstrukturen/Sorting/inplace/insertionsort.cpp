// keine fehler
void insertionsort(double items[], const int size){
    for(int i = 1; i<size; i++){
        int j = i;
        double date = items[i];
        while (j>0 && items[j-1]>date){
            items[j] = items[j-1];
            j--;
        }
        items[j] = date;
    }
}