// fehler:
// quicksort ist inplace du idiot -> keine neuen arrays basteln. unnoetig kompliziert


void quicksort(double items[], int left, int right) {
    if (left >= right) return;

    double pivot = items[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (items[j] <= pivot) {
            i++;
            double tmp = items[i]; items[i] = items[j]; items[j] = tmp;
        }
    }
    
    double tmp = items[i+1]; items[i+1] = items[right]; items[right] = tmp;
    int pivot_idx = i + 1;

    quicksort(items, left, pivot_idx - 1);
    quicksort(items, pivot_idx + 1, right);
}


/* 
double* quicksort(double items[], int size, int left=0){
    if(left<size){
        int i = left;
        int j = size-2;
        double pivot = items[size-1];

        while (i<j){
            while (i<j && items[i] <= pivot){i++;}
            while (j>i && items[j] >  pivot){j--;}
            if(items[i]>items[j]){
                double tmp = items[j];
                items[j] = items[i];
                items[i] = tmp;
            }
        }

        if(items[i]> pivot){
            double tmp = items[size-1];
            items[size-1] = items[i];
            items[i] = tmp;
        } else i = size-1;
        double* leftList= new double[i-1]{};
        double* rightList= new double[size-i-1]{};
        for(int idx =0; idx<size; idx++){
            if(idx<i-1) leftList[idx] = items[idx];
            else if (idx==i) continue;
            else rightList[idx-i-1] = items[idx];
        }
        double* sorted_left = quicksort(leftList,i-1,left);
        double* sorted_right = quicksort(rightList,size-i-1,i);
        delete[] leftList, rightList;

        double* result= new double[size]{};

        for(int idx =0; idx<size; idx++){
            if(idx<i-1) result[idx] = sorted_left[idx];
            else if (idx==i) result[idx] = pivot;
            else result[idx] = rightList[idx-i];
        }
        delete[] sorted_left, sorted_right;
        return result;
    } else {
        double* result = new double[size];
        for(int idx = 0; idx<size; idx++) result[idx] = items[idx];
        return result;
    }
} */